#define _POSIX_C_SOURCE 199506L
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include "utils.h"
#include "scheduler.h"
#include "uid.h"

#define WD_SEM_NAME "/wd_sem"
#define UNUSED(x) (void)x

static pid_t g_client_pid = 0;

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
    INTERVAL_IDX = 1,
    FAIL_CNT_IDX = 2,
    CLIENT_ARGS_IDX = 3,
    MIN_ARGS = 4,
    SEM_TIMEOUT_SEC = 5
};

typedef struct
{
    char** m_client_argv;
    scheduler_ty* m_sch;
    unsigned int m_fail_cnt;
    unsigned int m_interval;
    mode_ty m_current_mode;
    sem_t* m_sem;
} tsk_info_ty;

static volatile unsigned int g_missed_signals_cnt = 0;
static volatile int g_should_exit = 0;

static sch_op_status_ty RaiseSemaphoreTSK(void* info);
static sch_op_status_ty SendHeartbeatTSK(void* info);
static sch_op_status_ty IncrementCounterTSK(void* info);
static sch_op_status_ty CheckCounterTSK(void* info);
static sch_op_status_ty CheckExitFlagTSK(void* info);
static sch_op_status_ty ReviveTSK(void* info);
static sch_op_status_ty ExitTSK(void* info);
static sch_op_status_ty StopSchedulerTSK(void* info);
static void SwitchModeIMP(tsk_info_ty* info, mode_ty new_mode);
static void LoadInitTasksIMP(tsk_info_ty* info);
static void LoadRoutineTasksIMP(tsk_info_ty* info);
static void LoadReviveTasksIMP(tsk_info_ty* info);
static void LoadErrorTasksIMP(tsk_info_ty* info);
static void LoadExitTasksIMP(tsk_info_ty* info);
static void AddTaskIMP(tsk_info_ty* info, scheduler_op_ty tsk_func);
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

int main(int argc, char* argv[])
{
    /*validate argc*/
    /*g_client_pid = getppid()*/
    /*parse interval*/
    /*validate interval*/
    /*parse fail_cnt*/
    /*validate fail_cnt*/
    /*set m_client_argv = argv + CLIENT_ARGS_IDX*/
    /*RegisterSignalHandlersIMP*/
    /*SchedulerCreate*/
    /*if failed exit*/
    /*set m_sch*/
    /*SwitchModeIMP MODE_INIT*/
    /*SchedulerRun*/
    /*SchedulerDestroy*/
    /*return 0*/
}

static void SwitchModeIMP(tsk_info_ty* info, mode_ty new_mode)
{
    /*assert info*/
    /*assert new_mode < NUM_MODES*/
    /*SchedulerClear*/
    /*update m_current_mode*/
    /*call transition_table[new_mode](info)*/
}

static sch_op_status_ty RaiseSemaphoreTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    /*sem_open WD_SEM_NAME*/
    /*if failed*/
        /*SwitchModeIMP MODE_ERROR*/
        /*return SCH_NOT_REPEAT*/
    /*sem_post*/
    /*if failed*/
        /*sem_close*/
        /*SwitchModeIMP MODE_ERROR*/
        /*return SCH_NOT_REPEAT*/
    /*sem_close*/
    /*SwitchModeIMP MODE_ROUTINE*/
    /*return SCH_NOT_REPEAT*/
}

static sch_op_status_ty SendHeartbeatTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    /*kill(g_client_pid, SIGUSR1)*/
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

static sch_op_status_ty ReviveTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    /*waitpid g_client_pid WNOHANG*/
    /*fork*/
    /*if failed*/
        /*SwitchModeIMP MODE_ERROR*/
        /*return SCH_NOT_REPEAT*/
    /*if child*/
        /*execvp client_argv*/
        /*exit 1*/
    /*update g_client_pid = pid*/
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
    /*SchedulerStop*/
    /*return SCH_NOT_REPEAT*/
}

static void LoadInitTasksIMP(tsk_info_ty* info)
{
    /*AddTaskIMP RaiseSemaphoreTSK*/
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
    /*AddTaskIMP ReviveTSK*/
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
    /*SchedulerAddTask with interval tsk_func info TaskCleanupIMP*/
    /*if failed exit*/
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
