#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include "utils.h"
#include "wd.h"
#include "scheduler.h"
#include "uid.h"

#define WD_APP "./wd_app"
#define UNUSED(x) (void)(x)

typedef enum
{
    MODE_INIT = 0,
    MODE_ROUTINE,
    MODE_REVIVE,
    MODE_ERROR,
    MODE_EXIT,
    NUM_MODES
} mode_ty;

enum
{
    MAX_DIGITS = 20,
    WD_APP_IDX = 0,
    INTERVAL_IDX = 1,
    FAIL_CNT_IDX = 2,
    CLIENT_ARGS_IDX = 3
};

typedef struct
{
    char** m_wd_argv;
    scheduler_ty* m_sch;
    unsigned int m_fail_cnt;
    unsigned int m_interval;
    pid_t m_wd_pid;
    sem_t* m_sem;
    mode_ty m_current_mode;
} tsk_info_ty;

typedef struct
{
    char** m_client_argv;
    size_t m_client_argv_len;
    unsigned int m_interval;
    unsigned int m_fail_cnt;
    sem_t* m_sem;
} wd_guard_info_ty;

static pthread_t g_wd_guard_thread = 0;
static volatile unsigned int g_missed_signals_cnt = 0;
static volatile int g_should_exit = 0;

static void* WdGuardThreadIMP(void* wd_info);
static void SwitchModeIMP(tsk_info_ty* info, mode_ty new_mode);
static void LoadInitTasksIMP(tsk_info_ty* info);

static void LoadRoutineTasksIMP(tsk_info_ty* info);
static void LoadReviveTasksIMP(tsk_info_ty* info);
static void LoadErrorTasksIMP(tsk_info_ty* info);
static void LoadExitTasksIMP(tsk_info_ty* info);

static void AddTaskIMP(tsk_info_ty* info, scheduler_op_ty tsk_func);

static char** AllocWdArgvIMP(wd_guard_info_ty* wd_info);
static void FreeWdArgvIMP(char** wd_argv);
static pid_t SpawnWdAppIMP(char** wd_argv);
static void RegisterSignalHandlersIMP(void);
static void TaskCleanupIMP(void* unused);

static void ResetCounterSH(int sig);
static void SetExitFlagSH(int sig);

static sch_op_status_ty WaitForWdAppTSK(void* info);
static sch_op_status_ty SendHeartbeatTSK(void* info);
static sch_op_status_ty IncrementCounterTSK(void* info);
static sch_op_status_ty CheckCounterTSK(void* info);
static sch_op_status_ty CheckExitFlagTSK(void* info);
static sch_op_status_ty ReviveWdAppTSK(void* info);
static sch_op_status_ty ExitTSK(void* info);
static sch_op_status_ty StopSchedulerTSK(void* info);

typedef void (*load_tasks_func_ty)(tsk_info_ty*);
static load_tasks_func_ty transition_table[NUM_MODES] =
{
    LoadInitTasksIMP,
    LoadRoutineTasksIMP,
    LoadReviveTasksIMP,
    LoadErrorTasksIMP,
    LoadExitTasksIMP
};

int MakeMeImmortal(size_t cmd_len, const char** cmd, int how_often, int fail_cnt)
{
    sem_t sem = {0};
    wd_guard_info_ty info = {0};
    int status = 0;

    assert(NULL != cmd);
    assert(0 < how_often);
    assert(0 < fail_cnt);
    assert(0 < cmd_len);

    if (-1 == sem_init(&sem, 0, 0)) return FAIL;

    {
        sigset_t mask = {0};
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigaddset(&mask, SIGUSR2);
        pthread_sigmask(SIG_BLOCK, &mask, NULL);
    }

    info.m_client_argv = (char**)cmd;
    info.m_client_argv_len = cmd_len;
    info.m_interval = (unsigned int)how_often;
    info.m_fail_cnt = (unsigned int)fail_cnt;
    info.m_sem = &sem;

    if (0 != pthread_create(&g_wd_guard_thread, NULL, WdGuardThreadIMP, &info))
    {
        sem_destroy(&sem);
        return FAIL;
    }
    /* Wait for the semaphore */
    do 
    {
        status = sem_wait(&sem);
    } while (-1 == status && EINTR == errno);
    sem_destroy(&sem);

    if (-1 == status)
    {
        pthread_cancel(g_wd_guard_thread);
        pthread_join(g_wd_guard_thread, NULL);
        g_wd_guard_thread = 0;
        return FAIL;
    }

    return SUCCESS;
}

void DoNotResuscitate(void)
{
    assert(0 != g_wd_guard_thread);

    __atomic_store_n(&g_should_exit, 1, __ATOMIC_SEQ_CST);
    pthread_join(g_wd_guard_thread, NULL);

    {
        sigset_t mask = {0};
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigaddset(&mask, SIGUSR2);
        pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
    }

    g_wd_guard_thread = 0;
    g_should_exit = 0;
    g_missed_signals_cnt = 0;
}

static void* WdGuardThreadIMP(void* wd_info_)
{
    wd_guard_info_ty* wd_info = (wd_guard_info_ty*)wd_info_;
    scheduler_ty* sch = NULL;
    char** wd_argv = NULL;
    tsk_info_ty tsk_info = {0};
    pid_t pid = 0;

    assert(wd_info_);
/* Block all signals */
    sigset_t all = {0};
    sigset_t usr = {0};
    sigfillset(&all);
    pthread_sigmask(SIG_BLOCK, &all, NULL);
    sigemptyset(&usr);
    /* Unblock only SIGUSR1 and SIGUSR2, which are used for wd_app :wd_guard_thread communication */
    sigaddset(&usr, SIGUSR1);
    sigaddset(&usr, SIGUSR2);
    pthread_sigmask(SIG_UNBLOCK, &usr, NULL);

    RegisterSignalHandlersIMP();

    wd_argv = AllocWdArgvIMP(wd_info);
    if (NULL == wd_argv)
    {
         sem_post(wd_info->m_sem); 
         return NULL; 
    }

    sch = SchedulerCreate();
    if (NULL == sch)
    { 
        FreeWdArgvIMP(wd_argv); 
        sem_post(wd_info->m_sem); 
        return NULL; 
    }

    pid = SpawnWdAppIMP(wd_argv);
    if (-1 == pid)
    {
        SchedulerDestroy(sch);
        FreeWdArgvIMP(wd_argv);
        sem_post(wd_info->m_sem);
        return NULL;
    }

    tsk_info.m_wd_argv = wd_argv;
    tsk_info.m_sch = sch;
    tsk_info.m_fail_cnt = wd_info->m_fail_cnt;
    tsk_info.m_interval = wd_info->m_interval;
    tsk_info.m_wd_pid = pid;
    tsk_info.m_sem = wd_info->m_sem;

    SwitchModeIMP(&tsk_info, MODE_INIT);

    SchedulerRun(sch);

    SchedulerDestroy(sch);
    FreeWdArgvIMP(wd_argv);

    return NULL;
}

static void SwitchModeIMP(tsk_info_ty* info, mode_ty new_mode)
{
    assert(info);
    assert(new_mode < NUM_MODES);

    SchedulerClear(info->m_sch);
    info->m_current_mode = new_mode;
    transition_table[new_mode](info);
}

static sch_op_status_ty WaitForWdAppTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);
    assert(info->m_sem);

    /* wd app launched successfully*/
    if (0 == __atomic_load_n(&g_missed_signals_cnt, __ATOMIC_SEQ_CST))
    {
        sem_post(info->m_sem);
        info->m_sem = NULL;
        SwitchModeIMP(info, MODE_ROUTINE);
        return SCH_NOT_REPEAT;
    }

    __atomic_add_fetch(&g_missed_signals_cnt, 1, __ATOMIC_SEQ_CST);

    if (__atomic_load_n(&g_missed_signals_cnt, __ATOMIC_SEQ_CST) >= info->m_fail_cnt)
    {
        sem_post(info->m_sem);
        info->m_sem = NULL;
        SwitchModeIMP(info, MODE_ERROR);
        return SCH_NOT_REPEAT;
    }

    return SCH_REPEAT;
}

static sch_op_status_ty SendHeartbeatTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);

    if (0 != kill(info->m_wd_pid, SIGUSR1))
    {
        if (ESRCH == errno)
        {
            SwitchModeIMP(info, MODE_REVIVE);
            return SCH_NOT_REPEAT;
        }
        SwitchModeIMP(info, MODE_ERROR);
        return SCH_NOT_REPEAT;
    }

    return SCH_REPEAT;
}

static sch_op_status_ty IncrementCounterTSK(void* info_)
{
    __atomic_add_fetch(&g_missed_signals_cnt, 1, __ATOMIC_SEQ_CST);
    UNUSED(info_);
    return SCH_REPEAT;
}

static sch_op_status_ty CheckCounterTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);

    if (__atomic_load_n(&g_missed_signals_cnt, __ATOMIC_SEQ_CST) >= info->m_fail_cnt)
    {
        __atomic_store_n(&g_missed_signals_cnt, 0, __ATOMIC_SEQ_CST);
        SwitchModeIMP(info, MODE_REVIVE);
        return SCH_NOT_REPEAT;
    }

    return SCH_REPEAT;
}

static sch_op_status_ty CheckExitFlagTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);

    if (__atomic_load_n(&g_should_exit, __ATOMIC_SEQ_CST))
    {
        SwitchModeIMP(info, MODE_EXIT);
        return SCH_NOT_REPEAT;
    }

    return SCH_REPEAT;
}

static sch_op_status_ty ReviveWdAppTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    pid_t new_pid = 0;
    assert(info_);

    kill(info->m_wd_pid, SIGKILL);
    waitpid(info->m_wd_pid, NULL, 0);

    new_pid = SpawnWdAppIMP(info->m_wd_argv);
    if (-1 == new_pid)
    {
        SwitchModeIMP(info, MODE_ERROR);
        return SCH_NOT_REPEAT;
    }

    info->m_wd_pid = new_pid;
    __atomic_store_n(&g_missed_signals_cnt, 0, __ATOMIC_SEQ_CST);

    SwitchModeIMP(info, MODE_ROUTINE);

    return SCH_NOT_REPEAT;
}

static sch_op_status_ty ExitTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);
    SchedulerStop(info->m_sch);
    return SCH_NOT_REPEAT;
}

static sch_op_status_ty StopSchedulerTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);
    kill(info->m_wd_pid, SIGUSR2);
    waitpid(info->m_wd_pid, NULL, 0);
    SchedulerStop(info->m_sch);
    return SCH_NOT_REPEAT;
}

static void LoadInitTasksIMP(tsk_info_ty* info)
{
    __atomic_store_n(&g_missed_signals_cnt, 0, __ATOMIC_SEQ_CST);
    AddTaskIMP(info, WaitForWdAppTSK);
}

static void LoadRoutineTasksIMP(tsk_info_ty* info)
{
    __atomic_store_n(&g_missed_signals_cnt, 0, __ATOMIC_SEQ_CST);
    AddTaskIMP(info, SendHeartbeatTSK);
    AddTaskIMP(info, IncrementCounterTSK);
    AddTaskIMP(info, CheckCounterTSK);
    AddTaskIMP(info, CheckExitFlagTSK);
}

static void LoadReviveTasksIMP(tsk_info_ty* info)
{
    AddTaskIMP(info, ReviveWdAppTSK);
}

static void LoadErrorTasksIMP(tsk_info_ty* info)
{
    AddTaskIMP(info, ExitTSK);
}

static void LoadExitTasksIMP(tsk_info_ty* info)
{
    AddTaskIMP(info, StopSchedulerTSK);
}

static void AddTaskIMP(tsk_info_ty* info, scheduler_op_ty tsk_func)
{
    uid_ty uid = {0};
    assert(info);
    assert(tsk_func);

    uid = SchedulerAddTask(info->m_sch, info->m_interval,
                           tsk_func, info, TaskCleanupIMP, NULL);
    if (IsMatchId(invalid_uid_g, uid))
    {
        exit(1);
    }
}

static char** AllocWdArgvIMP(wd_guard_info_ty* wd_info)
{
    char** wd_argv = NULL;
    char* interval_str = NULL;
    char* fail_cnt_str = NULL;
    size_t i = 0;

    assert(wd_info);

    wd_argv = (char**)malloc(sizeof(char*) *
              (wd_info->m_client_argv_len + CLIENT_ARGS_IDX + 1));
    if (NULL == wd_argv) 
        return NULL;

    interval_str = (char*)malloc(MAX_DIGITS);
    if (NULL == interval_str) 
    {
         free(wd_argv); 
         return NULL; 
    }

    fail_cnt_str = (char*)malloc(MAX_DIGITS);
    if (NULL == fail_cnt_str)
    {
         free(interval_str); 
         free(wd_argv); 
         return NULL; 
    }

    wd_argv[WD_APP_IDX] = WD_APP;
    sprintf(interval_str, "%u", wd_info->m_interval);
    wd_argv[INTERVAL_IDX] = interval_str;
    sprintf(fail_cnt_str, "%u", wd_info->m_fail_cnt);
    wd_argv[FAIL_CNT_IDX] = fail_cnt_str;

    /* copy client args to wd_argv */
    for (i = 0; i < wd_info->m_client_argv_len; ++i)
        wd_argv[CLIENT_ARGS_IDX + i] = wd_info->m_client_argv[i];

    wd_argv[CLIENT_ARGS_IDX + wd_info->m_client_argv_len] = NULL;

    return wd_argv;
}

static void FreeWdArgvIMP(char** wd_argv)
{
    if (NULL == wd_argv) return;
    free(wd_argv[INTERVAL_IDX]);
    free(wd_argv[FAIL_CNT_IDX]);
    free(wd_argv);
}

static pid_t SpawnWdAppIMP(char** wd_argv)
{
    pid_t pid = fork();
    if (-1 == pid) return -1;

    if (0 == pid)
    {
        execvp(wd_argv[WD_APP_IDX], wd_argv);
        exit(1);
    }

    return pid;
}

static void ResetCounterSH(int sig)
{
    __atomic_store_n(&g_missed_signals_cnt, 0, __ATOMIC_SEQ_CST);
    UNUSED(sig);
}

static void SetExitFlagSH(int sig)
{
    __atomic_store_n(&g_should_exit, 1, __ATOMIC_SEQ_CST);
    UNUSED(sig);
}

static void RegisterSignalHandlersIMP(void)
{
    struct sigaction sa = {0};
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = ResetCounterSH;
    sigaction(SIGUSR1, &sa, NULL);
    sa.sa_handler = SetExitFlagSH;
    sigaction(SIGUSR2, &sa, NULL);
}

static void TaskCleanupIMP(void* unused)
{
    UNUSED(unused);
}