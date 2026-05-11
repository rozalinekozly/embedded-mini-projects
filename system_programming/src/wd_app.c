/*review version*/
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

enum
{
    INTERVAL_IDX    = 1,
    FAIL_CNT_IDX    = 2,
    CLIENT_ARGS_IDX = 3,
    MIN_ARGS        = 4
};

typedef struct
{
    char**        m_client_argv;
    scheduler_ty* m_sch;
    unsigned int  m_fail_cnt;
    unsigned int  m_interval;
    pid_t         m_client_pid;
} tsk_info_ty;

static volatile unsigned int g_missed_cnt  = 0;
static volatile int          g_should_exit = 0;

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
    scheduler_ty* sch      = NULL;
    tsk_info_ty   tsk_info = {0};

    EXIT_IF_BAD(MIN_ARGS <= argc, 1, "invalid arguments");

    tsk_info.m_interval = (unsigned int)atoi(argv[INTERVAL_IDX]);
    EXIT_IF_BAD(0 < tsk_info.m_interval, 1, "invalid interval");

    tsk_info.m_fail_cnt = (unsigned int)atoi(argv[FAIL_CNT_IDX]);
    EXIT_IF_BAD(0 < tsk_info.m_fail_cnt, 1, "invalid fail count");

    tsk_info.m_client_argv = argv + CLIENT_ARGS_IDX;
    tsk_info.m_client_pid  = getppid();

    RegisterSignalHandlersIMP();

    kill(getppid(), SIGUSR1);

    sch = SchedulerCreate();
    EXIT_IF_BAD(NULL != sch, 1, "failed to create scheduler");

    tsk_info.m_sch = sch;

    SchedulerAddTask(sch, tsk_info.m_interval, SendHeartbeatTSK,    &tsk_info, TaskCleanupIMP, NULL);
    SchedulerAddTask(sch, tsk_info.m_interval, IncrementCounterTSK, &tsk_info, TaskCleanupIMP, NULL);
    SchedulerAddTask(sch, tsk_info.m_interval, CheckCounterTSK,     &tsk_info, TaskCleanupIMP, NULL);
    SchedulerAddTask(sch, tsk_info.m_interval, CheckExitFlagTSK,    &tsk_info, TaskCleanupIMP, NULL);

    SchedulerRun(sch);
    SchedulerDestroy(sch);

    return 0;
}

static sch_op_status_ty SendHeartbeatTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);

    kill(info->m_client_pid, SIGUSR1);

    return SCH_REPEAT;
}

static sch_op_status_ty IncrementCounterTSK(void* info_)
{
    __atomic_add_fetch(&g_missed_cnt, 1, __ATOMIC_SEQ_CST);
    UNUSED(info_);
    return SCH_REPEAT;
}

static sch_op_status_ty CheckCounterTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);

    if (__atomic_load_n(&g_missed_cnt, __ATOMIC_SEQ_CST) >= info->m_fail_cnt)
    {
        __atomic_store_n(&g_missed_cnt, 0, __ATOMIC_SEQ_CST);
        SchedulerAddTask(info->m_sch, 0, ReviveClientTSK, info, TaskCleanupIMP, NULL);
    }

    return SCH_REPEAT;
}

static sch_op_status_ty CheckExitFlagTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    assert(info_);

    if (__atomic_load_n(&g_should_exit, __ATOMIC_SEQ_CST))
    {
        SchedulerStop(info->m_sch);
        return SCH_NOT_REPEAT;
    }

    return SCH_REPEAT;
}

static sch_op_status_ty ReviveClientTSK(void* info_)
{
    tsk_info_ty* info = (tsk_info_ty*)info_;
    sigset_t mask = {0};

    assert(info_);

    kill(getppid(), SIGUSR2);
    kill(info->m_client_pid, SIGKILL);
    waitpid(info->m_client_pid, NULL, 0);

    sigemptyset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    execvp(info->m_client_argv[0], info->m_client_argv);

    SchedulerStop(info->m_sch);
    return SCH_NOT_REPEAT;
}

static void ResetCounterSH(int sig)
{
    __atomic_store_n(&g_missed_cnt, 0, __ATOMIC_SEQ_CST);
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