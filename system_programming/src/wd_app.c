TODO: add comment of used function next to each header
TODO: rename helpers that switch modes to ToMode<something>
TODO: add seperators to make code readable
#define _POSIX_C_SOURCE 200112L
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <sys/wait.h>
#include "utils.h"
#include "scheduler.h"
#include "uid.h"

#define UNUSED(x) (void)x

enum
{
    INTERVAL_IDX    = 1,
    FAIL_CNT_IDX    = 2,
    CLIENT_ARGS_IDX = 3,
    MIN_ARGS        = 4
};

typedef struct
{
    char** m_client_argv;
    scheduler_ty* m_sch;
    unsigned int m_fail_cnt;
    unsigned int m_interval;
    pid_t m_client_pid;
} tsk_info_ty;

static volatile unsigned int g_missed_signals_cnt = 0;
static volatile int g_should_exit = 0;

static sch_op_status_ty SendHeartbeatTSK(void* info);
static sch_op_status_ty IncrementCounterTSK(void* info);
static sch_op_status_ty CheckCounterTSK(void* info);
static sch_op_status_ty CheckExitFlagTSK(void* info);
static sch_op_status_ty ReviveClientTSK(void* info);
static void ResetCounterSH(int sig);
static void SetExitFlagSH(int sig);
static void RegisterSignalHandlersIMP(void);
static void TaskCleanupIMP(void* unused);

int main(int argc, char* argv[])
{
    scheduler_ty* sch = NULL;
    tsk_info_ty tsk_info = {0};

    /*validate argc*/
    EXIT_IF_BAD(MIN_ARGS <= argc, 1, "invalid arguments");

    /*parse interval*/
    tsk_info.m_interval = (unsigned int)atoi(argv[INTERVAL_IDX]);
    /*handle invalid interval*/
    EXIT_IF_BAD(0 < tsk_info.m_interval, 1, "invalid interval");

    /*parse fail count*/
    tsk_info.m_fail_cnt = (unsigned int)atoi(argv[FAIL_CNT_IDX]);
    /*handle invalid fail count*/
    EXIT_IF_BAD(0 < tsk_info.m_fail_cnt, 1, "invalid fail count");

    /*set client argv*/
    tsk_info.m_client_argv = argv + CLIENT_ARGS_IDX;
    /*set client pid as parent pid*/
    tsk_info.m_client_pid  = getppid();

    /*register signal handlers*/
    RegisterSignalHandlersIMP();

    /*send SIGUSR1 to parent to signal wd_app is ready and reset wd_thrd counter*/
    kill(getppid(), SIGUSR1);

    /*create scheduler*/
    sch = SchedulerCreate();
    /*handle failure*/
    EXIT_IF_BAD(NULL != sch, 1, "failed to create scheduler");

    /*set scheduler in tsk_info*/
    tsk_info.m_sch = sch;

    /*add heartbeat task*/
    SchedulerAddTask(sch, tsk_info.m_interval, SendHeartbeatTSK,    &tsk_info, TaskCleanupIMP, NULL);
    /*add counter increment task*/
    SchedulerAddTask(sch, tsk_info.m_interval, IncrementCounterTSK, &tsk_info, TaskCleanupIMP, NULL);
    /*add counter check task*/
    SchedulerAddTask(sch, tsk_info.m_interval, CheckCounterTSK,     &tsk_info, TaskCleanupIMP, NULL);
    /*add exit flag check task*/
    SchedulerAddTask(sch, tsk_info.m_interval, CheckExitFlagTSK,    &tsk_info, TaskCleanupIMP, NULL);

    /*run scheduler*/
    SchedulerRun(sch);

    /*destroy scheduler*/
    SchedulerDestroy(sch);

    return 0;
}

static sch_op_status_ty SendHeartbeatTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);

    /*collect zombie if client died*/
    /*if client exited*/
    if (waitpid(info->m_client_pid, NULL, WNOHANG) > 0)
    {
        /*schedule revival*/
        SchedulerAddTask(info->m_sch, 0, ReviveClientTSK, info, TaskCleanupIMP, NULL);
        return SCH_REPEAT;
    }

    /*send heartbeat to client*/
    kill(info->m_client_pid, SIGUSR1);

    return SCH_REPEAT;
}

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

static sch_op_status_ty CheckExitFlagTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);

    /*if DNR requested clean shutdown*/
    if (__atomic_load_n(&g_should_exit, __ATOMIC_SEQ_CST))
    {
        /*stop scheduler*/
        SchedulerStop(info->m_sch);
        return SCH_NOT_REPEAT;
    }

    return SCH_REPEAT;
}

static sch_op_status_ty ReviveClientTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);

    /*collect zombie*/
    waitpid(info->m_client_pid, NULL, WNOHANG);

    /*replace wd_app with client via execvp - new client calls MMI creating new wd_thrd and wd_app*/
    execvp(info->m_client_argv[0], info->m_client_argv);

    /*execvp failed - stop scheduler*/
    SchedulerStop(info->m_sch);
    return SCH_NOT_REPEAT;
}

static void ResetCounterSH(int sig)
{
    /*wd_thrd sent SIGUSR1 - reset missed signals counter*/
    __atomic_store_n(&g_missed_signals_cnt, 0, __ATOMIC_SEQ_CST);
    UNUSED(sig);
}

static void SetExitFlagSH(int sig)
{
    /*DNR sent SIGUSR2 - set exit flag*/
    __atomic_store_n(&g_should_exit, 1, __ATOMIC_SEQ_CST);
    UNUSED(sig);
}

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

static void TaskCleanupIMP(void* unused)
{
    UNUSED(unused);
}
