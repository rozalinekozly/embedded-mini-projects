#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
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

#define WD_APP      "./wd_app"
#define WD_SEM_NAME "/wd_sem"
#define UNUSED(x)   (void)x

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
    SEM_TIMEOUT_SEC = 5,
    WD_APP_IDX      = 0,
    INTERVAL_IDX    = 1,
    FAIL_CNT_IDX    = 2,
    CLIENT_ARGS_IDX = 3
};

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
static sch_op_status_ty WaitForWdAppTSK(void* info);
static sch_op_status_ty SendHeartbeatTSK(void* info);
static sch_op_status_ty IncrementCounterTSK(void* info);
static sch_op_status_ty CheckCounterTSK(void* info);
static sch_op_status_ty CheckExitFlagTSK(void* info);
static sch_op_status_ty ReviveWdAppTSK(void* info);
static sch_op_status_ty ExitTSK(void* info);
static sch_op_status_ty StopSchedulerTSK(void* info);
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
    /*assert args*/
    /*create named semaphore initialized to 0*/
    /*if failed*/
        /*return FAIL*/
    /*set timeout for sem_timedwait*/
    /*if failed*/
        /*sem_close*/
        /*sem_unlink*/
        /*return FAIL*/
    /*init wd_guard_info*/
    /*pthread_create WdGuardThreadIMP*/
    /*if failed*/
        /*sem_close*/
        /*sem_unlink*/
        /*return FAIL*/
    /*sem_timedwait*/
    /*if failed*/
        /*pthread_cancel*/
        /*pthread_join*/
        /*sem_close*/
        /*sem_unlink*/
        /*return FAIL*/
    /*sem_close*/
    /*sem_unlink*/
    /*store thread handle in g_wd_guard_thread*/
    /*return SUCCESS*/
}

void DoNotResuscitate(void)
{
    /*assert thread exists*/
    /*set g_should_exit = 1 atomically*/
    /*pthread_join*/
    /*reset globals*/
}

static void* WdGuardThreadIMP(void* wd_info_)
{
    /*cast param*/
    /*AllocWdArgvIMP*/
    /*if failed*/
        /*sem_post*/
        /*return NULL*/
    /*RegisterSignalHandlersIMP*/
    /*SchedulerCreate*/
    /*if failed*/
        /*FreeWdArgvIMP*/
        /*sem_post*/
        /*return NULL*/
    /*SpawnWdAppIMP*/
    /*if failed*/
        /*SchedulerDestroy*/
        /*FreeWdArgvIMP*/
        /*sem_post*/
        /*return NULL*/
    /*init tsk_info fields*/
    /*SwitchModeIMP MODE_ROUTINE*/
    /*SchedulerRun*/
    /*SchedulerDestroy*/
    /*FreeWdArgvIMP*/
    /*return NULL*/
}

static void SwitchModeIMP(tsk_info_ty* info, mode_ty new_mode)
{
    /*assert info*/
    /*assert new_mode < NUM_MODES*/
    /*SchedulerClear*/
    /*update m_current_mode*/
    /*call transition_table[new_mode](info)*/
}

static sch_op_status_ty WaitForWdAppTSK(void* info_)
{
    /*cast param*/
    /*assert sem exists*/
    /*set timeout*/
    /*sem_timedwait*/
    /*if success*/
        /*sem_post to unblock MMI*/
        /*set m_sem = NULL*/
        /*SwitchModeIMP MODE_ROUTINE*/
    /*if timeout*/
        /*sem_post to unblock MMI*/
        /*set m_sem = NULL*/
        /*SwitchModeIMP MODE_ERROR*/
    /*return SCH_NOT_REPEAT*/
}

static sch_op_status_ty SendHeartbeatTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    /*kill(m_wd_app_pid, SIGUSR1)*/
    /*if failed*/
        /*if ESRCH*/
            /*SwitchModeIMP MODE_REVIVE*/
            /*return SCH_NOT_REPEAT*/
        /*SwitchModeIMP MODE_ERROR*/
        /*return SCH_NOT_REPEAT*/
    /*return SCH_REPEAT*/
}

static sch_op_status_ty IncrementCounterTSK(void* info_)
{
    /*atomic increment g_missed_signals_cnt*/
    /*return SCH_REPEAT*/
}

static sch_op_status_ty CheckCounterTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    /*if counter >= fail_cnt*/
        /*SwitchModeIMP MODE_REVIVE*/
        /*return SCH_NOT_REPEAT*/
    /*return SCH_REPEAT*/
}

static sch_op_status_ty CheckExitFlagTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    /*if g_should_exit*/
        /*SwitchModeIMP MODE_EXIT*/
        /*return SCH_NOT_REPEAT*/
    /*return SCH_REPEAT*/
}

static sch_op_status_ty ReviveWdAppTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    /*waitpid m_wd_app_pid*/
    /*SpawnWdAppIMP*/
    /*if failed*/
        /*SwitchModeIMP MODE_ERROR*/
        /*return SCH_NOT_REPEAT*/
    /*update m_wd_app_pid*/
    /*reset g_missed_signals_cnt = 0 atomically*/
    /*SwitchModeIMP MODE_ROUTINE*/
    /*return SCH_NOT_REPEAT*/
}

static sch_op_status_ty ExitTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    /*SchedulerStop*/
    /*return SCH_NOT_REPEAT*/
}

static sch_op_status_ty StopSchedulerTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    /*kill(m_wd_app_pid, SIGUSR2)*/
    /*waitpid m_wd_app_pid*/
    /*SchedulerStop*/
    /*return SCH_NOT_REPEAT*/
}

static void LoadInitTasksIMP(tsk_info_ty* info)
{
    /*AddTaskIMP WaitForWdAppTSK*/
}

static void LoadRoutineTasksIMP(tsk_info_ty* info)
{
    /*AddTaskIMP SendHeartbeatTSK*/
    /*AddTaskIMP IncrementCounterTSK*/
    /*AddTaskIMP CheckCounterTSK*/
    /*AddTaskIMP CheckExitFlagTSK*/
}

static void LoadReviveTasksIMP(tsk_info_ty* info)
{
    /*AddTaskIMP ReviveWdAppTSK*/
}

static void LoadErrorTasksIMP(tsk_info_ty* info)
{
    /*AddTaskIMP ExitTSK*/
}

static void LoadExitTasksIMP(tsk_info_ty* info)
{
    /*AddTaskIMP StopSchedulerTSK*/
}

static void AddTaskIMP(tsk_info_ty* info, scheduler_op_ty tsk_func)
{
    /*assert info*/
    /*assert tsk_func*/
    /*SchedulerAddTask with interval, tsk_func, info, TaskCleanupIMP*/
    /*if failed*/
        /*exit*/
}

static char** AllocWdArgvIMP(wd_guard_info_ty* wd_info)
{
    /*assert wd_info*/
    /*malloc argv array of size client_argv_len + CLIENT_ARGS_IDX + 1*/
    /*if failed return NULL*/
    /*malloc interval_str*/
    /*if failed*/
        /*free wd_argv*/
        /*return NULL*/
    /*malloc fail_cnt_str*/
    /*if failed*/
        /*free interval_str*/
        /*free wd_argv*/
        /*return NULL*/
    /*set wd_argv[0] = WD_APP*/
    /*sprintf interval into interval_str*/
    /*set wd_argv[1] = interval_str*/
    /*sprintf fail_cnt into fail_cnt_str*/
    /*set wd_argv[2] = fail_cnt_str*/
    /*copy client_argv into wd_argv[3...]*/
    /*set last entry to NULL*/
    /*return wd_argv*/
}

static void FreeWdArgvIMP(char** wd_argv)
{
    /*if NULL return*/
    /*free wd_argv[INTERVAL_IDX]*/
    /*free wd_argv[FAIL_CNT_IDX]*/
    /*free wd_argv*/
}

static pid_t SpawnWdAppIMP(char** wd_argv)
{
    /*fork*/
    /*if failed return -1*/
    /*if child*/
        /*execvp wd_argv*/
        /*exit 1*/
    /*return pid*/
}

static void ResetCounterSH(int sig)
{
    /*atomic store g_missed_signals_cnt = 0*/
}

static void SetExitFlagSH(int sig)
{
    /*atomic store g_should_exit = 1*/
}

static void RegisterSignalHandlersIMP(void)
{
    /*sigemptyset*/
    /*sigaction SIGUSR1 -> ResetCounterSH*/
    /*sigaction SIGUSR2 -> SetExitFlagSH*/
}

static void TaskCleanupIMP(void* unused)
{
    /*UNUSED*/
}
