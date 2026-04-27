#define _POSIX_C_SOURCE 200112L
#include <sys/types.h>  /*pid_t */ 
#include <unistd.h>     /*fork, execvp*/
#include <sys/wait.h>   /*waitpid*/
#include <assert.h>     /*assert*/  
#include <signal.h>     /*SIGUSR1, SIGUSR2, SIGKILL, sigaction, struct sigaction */  
#include <stdlib.h>     /*exit, atoi, malloc, free*/
#include <pthread.h>    /*pthread_create, pthread_join, pthread_t*/
#include <semaphore.h>  /*sem_t, sem_init, sem_destroy, sem_post, sem_wait, sem_timedwait*/
#include <time.h>       /*clock_gettime, struct timespec, CLOCK_REALTIME*/
#include <errno.h>      /*errno*/
#include <stdio.h>      /*sprintf*/ 
/*---------------------------------------------------------------------------------------------------------------------------- */
#include "utils.h"      /*EXIT_IF_BAD*/
#include "scheduler.h"  /*scheduler_ty, SchedulerCreate, SchedulerAddTask, SchedulerRun, SchedulerStop, SchedulerDestroy*/
#include "wd.h"         /*MakeMeImmortal, DoNotResuscitate*/
/*----------------------------------------------------------------------------------------------------------------------------*/
#define WD_PATH "./wd_app"
#define UNUSED(x) (void)x
/*----------------------------------------------------------------------------------------------------------------------------*/
enum
{
    /*to parse input*/
    WD_APP_IDX      = 0,
    INTERVAL_IDX    = 1,
    FAIL_CNT_IDX    = 2,
    CLIENT_ARGS_IDX = 3,
    /*constants*/
    MAX_DIGITS      = 20,
    SEM_TIMEOUT_SEC = 5
};
/*----------------------------------------------------------------------------------------------------------------------------*/
/*struct to encapsulate arguments to pass/initialize wd process's (child) arguments */
typedef struct
{
    char** m_client_argv;
    size_t m_client_argv_len;   
    unsigned int m_interval;
    unsigned int m_fail_cnt;
    sem_t* m_sem;
} thrd_args_ty;
/*----------------------------------------------------------------------------------------------------------------------------*/
/*management struct to track wd thread information */
typedef struct
{
    char** m_wd_argv;
    scheduler_ty* m_sch;
    unsigned int m_fail_cnt;
    unsigned int m_interval;
    pid_t m_wd_pid;
} tsk_info_ty;
/*-----------------------------------------------GLOBALS---------------------------------------------------------------*/
/*thread handle to pass to DoNotResuscitate */
static pthread_t g_wd_thrd = 0;
/*flag to indicate when to exit (accessibility of signal handlers)*/
static volatile int g_should_exit = 0;
/*flag to indicate number of missed signals (accessibility of signal handlers)*/
static volatile unsigned int g_missed_signals_cnt = 0;
/*-----------------------------------FORWARD DECLARATIONS--------------------------------------------------------------*/
/*SIGNAL HANDLERS*/
static void ResetCounterSH(int sig);
static void SetExitFlagSH(int sig);
/*THREAD FUNCTION*/
static void* WdThrdIMP(void* args);
/*SCHEDULER TASKS*/
static sch_op_status_ty SendHeartbeatTSK(void* info);
static sch_op_status_ty IncrementCounterTSK(void* info);
static sch_op_status_ty CheckCounterTSK(void* info);
static sch_op_status_ty CheckExitFlagTSK(void* info);
static sch_op_status_ty ReviveWdAppTSK(void* info);
/*HELPERS */
/*helper to construct wd_app arguments*/
static char** AllocWdArgvIMP(thrd_args_ty* args);
/*helper to free wd_app arguments*/
static void FreeWdArgvIMP(char** wd_argv);
/*signals setter */
static void RegisterSignalHandlersIMP(void);
/*dummy cleanup, could be supported by scheduler.h */
static void TaskCleanupIMP(void* unused);
/*----------------------------------------------------------------------------------------------------------------------------*/
int MakeMeImmortal(size_t cmd_len, const char** cmd, int how_often, int fail_cnt)
{
    sem_t sem = {0};
    struct timespec ts = {0}; /*to wait for semaphore in timed spec*/
    thrd_args_ty args = {0};
    int sem_status = 0;

    /*assert args*/
    assert(NULL != cmd);
    assert(0 < how_often);
    assert(0 < fail_cnt);
    assert(0 < cmd_len);

    /*create unnamed semaphore initialized to 0*/
    /*handle failure*/
    if (-1 == sem_init(&sem, 0, 0)) return FAIL;

    /*get current time for timeout*/
    /*handle failure*/
    if (-1 == clock_gettime(CLOCK_REALTIME, &ts))
    {
        sem_destroy(&sem);
        return FAIL;
    }

    /*set timeout*/
    ts.tv_sec += SEM_TIMEOUT_SEC;

    /*init thread args*/
    args.m_client_argv = (char**)cmd;
    args.m_client_argv_len = cmd_len;
    args.m_interval = how_often;
    args.m_fail_cnt = fail_cnt;
    args.m_sem = &sem;

    /*create wd_thrd*/
    /*if failed*/
    if (0 != pthread_create(&g_wd_thrd, NULL, WdThrdIMP, &args))
    {
        /*cleanup and return FAIL*/
        sem_destroy(&sem);
        return FAIL;
    }

    /*wait for wd_app to signal ready*/
    sem_status = sem_timedwait(&sem, &ts);

    /*destroy semaphore*/
    sem_destroy(&sem);

    /*if timed out*/
    if (-1 == sem_status)
    {
        /*set system's state to exit*/
       __atomic_store_n(&g_should_exit, 1, __ATOMIC_SEQ_CST);
        pthread_join(g_wd_thrd, NULL);
        g_wd_thrd = 0;
        return FAIL;
    }

    return SUCCESS;
}
/*----------------------------------------------------------------------------------------------------------------------------*/

void DoNotResuscitate(void)
{
    /*assert thread exists*/
    assert(0 != g_wd_thrd);

    /*set exit flag atomically*/
    __atomic_store_n(&g_should_exit, 1, __ATOMIC_SEQ_CST);

    /*wait for wd_thrd to finish cleanup*/
    pthread_join(g_wd_thrd, NULL);

    /*reset all globals*/
    g_wd_thrd = 0;
    g_should_exit = 0;
    g_missed_signals_cnt = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------*/

static void* WdThrdIMP(void* args_)
{
    thrd_args_ty* args = (thrd_args_ty*)args_;
    scheduler_ty* sch  = NULL;
    char** wd_argv     = NULL;
    tsk_info_ty tsk_info = {0};
    pid_t pid = 0;

    assert(args_);

    /*register signal handlers*/
    RegisterSignalHandlersIMP();

    /*alloc wd_app argv*/
    wd_argv = AllocWdArgvIMP(args);
    /*if failed notify MMI and return*/
    if (NULL == wd_argv)
    {
        sem_post(args->m_sem);
        return NULL;
    }

    /*fork wd_app*/
    pid = fork();
    /*if failed*/
    if (-1 == pid)
    {
        /*cleanup and notify MMI*/
        FreeWdArgvIMP(wd_argv);
        sem_post(args->m_sem);
        return NULL;
    }

    /*if child*/
    if (0 == pid)
    {
        /*become wd_app*/
        execvp(wd_argv[0], wd_argv);
        /*execvp failed*/
        exit(1);
    }

    /*notify MakeMeImmortal that wd_app is spawned*/
    sem_post(args->m_sem);

    /*create scheduler*/
    sch = SchedulerCreate();
    /*if failed cleanup and return*/
    if (NULL == sch)
    {
        FreeWdArgvIMP(wd_argv);
        return NULL;
    }

    /*init tsk_info fields*/
    tsk_info.m_wd_argv  = wd_argv;
    tsk_info.m_sch      = sch;
    tsk_info.m_fail_cnt = args->m_fail_cnt;
    tsk_info.m_interval = args->m_interval;
    tsk_info.m_wd_pid   = pid;

    /*add heartbeat task*/
    SchedulerAddTask(sch, args->m_interval, SendHeartbeatTSK,    &tsk_info, TaskCleanupIMP, NULL);
    /*add counter increment task*/
    SchedulerAddTask(sch, args->m_interval, IncrementCounterTSK, &tsk_info, TaskCleanupIMP, NULL);
    /*add counter check task*/
    SchedulerAddTask(sch, args->m_interval, CheckCounterTSK,     &tsk_info, TaskCleanupIMP, NULL);
    /*add exit flag check task*/
    SchedulerAddTask(sch, args->m_interval, CheckExitFlagTSK,    &tsk_info, TaskCleanupIMP, NULL);

    /*run scheduler - blocks until EXIT mode*/
    SchedulerRun(sch);

    /*cleanup*/
    SchedulerDestroy(sch);
    FreeWdArgvIMP(wd_argv);

    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------*/

static sch_op_status_ty SendHeartbeatTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);

    /*collect zombie and check if wd_app exited*/
    /*if wd_app exited*/
    if (waitpid(info->m_wd_pid, NULL, WNOHANG) > 0)
    {
        /*schedule revival*/
        SchedulerAddTask(info->m_sch, 0, ReviveWdAppTSK, info, TaskCleanupIMP, NULL);
        return SCH_REPEAT;
    }

    /*send heartbeat to wd_app*/
    kill(info->m_wd_pid, SIGUSR1);

    return SCH_REPEAT;
}
/*----------------------------------------------------------------------------------------------------------------------------*/

static sch_op_status_ty IncrementCounterTSK(void* info_)
{
    /*atomic increment missed signals counter*/
    __atomic_add_fetch(&g_missed_signals_cnt, 1, __ATOMIC_SEQ_CST);
    UNUSED(info_);
    return SCH_REPEAT;
}

static sch_op_status_ty CheckCounterTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);

    /*if counter reached limit*/
    if (__atomic_load_n(&g_missed_signals_cnt, __ATOMIC_SEQ_CST) >= info->m_fail_cnt)
    {
        /*reset counter - death detection handled by SendHeartbeatTSK*/
        __atomic_store_n(&g_missed_signals_cnt, 0, __ATOMIC_SEQ_CST);
    }

    return SCH_REPEAT;
}
/*----------------------------------------------------------------------------------------------------------------------------*/

static sch_op_status_ty CheckExitFlagTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);

    /*if DNR requested clean shutdown*/
    if (__atomic_load_n(&g_should_exit, __ATOMIC_SEQ_CST))
    {
        /*send SIGUSR2 to wd_app for clean shutdown*/
        kill(info->m_wd_pid, SIGUSR2);
        /*wait for wd_app to exit*/
        waitpid(info->m_wd_pid, NULL, 0);
        /*stop scheduler*/
        SchedulerStop(info->m_sch);
        return SCH_NOT_REPEAT;
    }

    return SCH_REPEAT;
}
/*----------------------------------------------------------------------------------------------------------------------------*/

static sch_op_status_ty ReviveWdAppTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    pid_t new_pid = 0;
    assert(info_);

    /*kill old wd_app if still alive*/
    if (0 == kill(info->m_wd_pid, 0))
    {
        kill(info->m_wd_pid, SIGKILL);
    }
    /*collect zombie*/
    waitpid(info->m_wd_pid, NULL, 0);

    /*fork new wd_app*/
    new_pid = fork();
    /*if failed*/
    if (-1 == new_pid)
    {
        /*stop scheduler*/
        SchedulerStop(info->m_sch);
        return SCH_NOT_REPEAT;
    }

    /*if child*/
    if (0 == new_pid)
    {
        /*become wd_app*/
        execvp(info->m_wd_argv[0], info->m_wd_argv);
        exit(1);
    }

    /*update wd_pid in tsk_info and global*/
    info->m_wd_pid = new_pid;

    /*reset counter*/
    __atomic_store_n(&g_missed_signals_cnt, 0, __ATOMIC_SEQ_CST);

    /*re-add all tasks*/
    SchedulerAddTask(info->m_sch, info->m_interval, SendHeartbeatTSK,    info, TaskCleanupIMP, NULL);
    SchedulerAddTask(info->m_sch, info->m_interval, IncrementCounterTSK, info, TaskCleanupIMP, NULL);
    SchedulerAddTask(info->m_sch, info->m_interval, CheckCounterTSK,     info, TaskCleanupIMP, NULL);
    SchedulerAddTask(info->m_sch, info->m_interval, CheckExitFlagTSK,    info, TaskCleanupIMP, NULL);

    return SCH_NOT_REPEAT;
}
/*----------------------------------------------------------------------------------------------------------------------------*/

static char** AllocWdArgvIMP(thrd_args_ty* args)
{
    char** wd_argv     = NULL;
    char* interval_str = NULL;
    char* fail_cnt_str = NULL;
    size_t i = 0;

    /*malloc wd_argv array*/
    wd_argv = (char**)malloc(sizeof(char*) * (args->m_client_argv_len + CLIENT_ARGS_IDX + 1));
    /*if failed return NULL*/
    if (NULL == wd_argv) return NULL;

    /*malloc interval string*/
    interval_str = (char*)malloc(MAX_DIGITS);
    /*if failed*/
    if (NULL == interval_str)
    {
        free(wd_argv);
        return NULL;
    }

    /*malloc fail_cnt string*/
    fail_cnt_str = (char*)malloc(MAX_DIGITS);
    /*if failed*/
    if (NULL == fail_cnt_str)
    {
        free(interval_str);
        free(wd_argv);
        return NULL;
    }

    /*set wd_app executable*/
    wd_argv[WD_APP_IDX] = WD_PATH;

    /*convert interval to string and set*/
    sprintf(interval_str, "%u", args->m_interval);
    wd_argv[INTERVAL_IDX] = interval_str;

    /*convert fail_cnt to string and set*/
    sprintf(fail_cnt_str, "%u", args->m_fail_cnt);
    wd_argv[FAIL_CNT_IDX] = fail_cnt_str;

    /*copy client argv*/
    for (i = 0; i < args->m_client_argv_len; i++)
        wd_argv[CLIENT_ARGS_IDX + i] = args->m_client_argv[i];

    /*set NULL terminator for execvp*/
    wd_argv[CLIENT_ARGS_IDX + args->m_client_argv_len] = NULL;

    return wd_argv;
}
/*----------------------------------------------------------------------------------------------------------------------------*/

static void FreeWdArgvIMP(char** wd_argv)
{
    /*if NULL return*/
    if (NULL == wd_argv) return;

    /*free malloced strings*/
    free(wd_argv[INTERVAL_IDX]);
    free(wd_argv[FAIL_CNT_IDX]);

    /*free argv array*/
    free(wd_argv);
}
/*----------------------------------------------------------------------------------------------------------------------------*/
static void ResetCounterSH(int sig)
{
    /*wd_app sent SIGUSR1 - reset missed signals counter*/
    __atomic_store_n(&g_missed_signals_cnt, 0, __ATOMIC_SEQ_CST);
    UNUSED(sig);
}
/*----------------------------------------------------------------------------------------------------------------------------*/

static void SetExitFlagSH(int sig)
{
    /*DNR sent SIGUSR2 - set exit flag*/
    __atomic_store_n(&g_should_exit, 1, __ATOMIC_SEQ_CST);
    UNUSED(sig);
}
/*----------------------------------------------------------------------------------------------------------------------------*/

static void RegisterSignalHandlersIMP(void)
{
    struct sigaction sa = {0};

    /*init signal mask*/
    sigemptyset(&sa.sa_mask);

    /*set SIGUSR1 handler to reset counter*/
    sa.sa_handler = ResetCounterSH;
    sigaction(SIGUSR1, &sa, NULL);

    /*set SIGUSR2 handler to set exit flag*/
    sa.sa_handler = SetExitFlagSH;
    sigaction(SIGUSR2, &sa, NULL);
}
/*----------------------------------------------------------------------------------------------------------------------------*/

static void TaskCleanupIMP(void* unused)
{
    UNUSED(unused);
}
