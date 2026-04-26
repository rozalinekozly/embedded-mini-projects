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
    MIN_ARGS = 4
};

/* management struct for a wd instance */
typedef struct
{
    char** m_client_argv;
    scheduler_ty* m_sch;
    unsigned int m_fail_cnt;
    unsigned int m_frequency_check;
    mode_ty m_current_mode;
    pid_t m_partner_pid;
} wd_management_ty;

/*loaders function*/
typedef void (*load_tasks_func_ty)(wd_management_ty*);

static volatile unsigned int g_missed_signals_cnt = 0;
static volatile int g_should_exit = 0;

/*method to switch between modes*/
static void SwitchModeIMP(wd_management_ty* info, mode_ty new_mode);
/*loaders to init FSM */
static void LoadInitTasksIMP(wd_management_ty* info);
static void LoadRoutineTasksIMP(wd_management_ty* info);
static void LoadReviveTasksIMP(wd_management_ty* info);
static void LoadErrorTasksIMP(wd_management_ty* info);
static void LoadExitTasksIMP(wd_management_ty* info);
/*task of adding task to scheduler */
/*wrapper but might delete later*/
static void AddTaskIMP(wd_management_ty* info, scheduler_op_ty tsk_func);
/*method to set signal handlers*/
static void RegisterSignalHandlersIMP(void);
/*signal handlers functions*/
static void ResetCounterSigHandler(int sig);
static void SetExitFlagSigHandler(int sig);
/*tasks*/

static sch_op_status_ty SendHeartbeatTSK(void* info);
static sch_op_status_ty IncrementCounterTSK(void* info);
static sch_op_status_ty CheckCounterTSK(void* info);
static sch_op_status_ty CheckExitFlagTSK(void* info);
static sch_op_status_ty ReviveTSK(void* info);
static sch_op_status_ty ExitTSK(void* info);
static sch_op_status_ty StopSchedulerTSK(void* info);
/*dummy funciton*/
static void TaskCleanupIMP(void* unused);

/*transition table between modes*/
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
	scheduler_ty* sch = NULL;
	uid_ty task_id = {0};
	wd_management_ty wd_management = {0};

    /*validate argc*/
    EXIT_IF_BAD(3 <= argc, 1, "Invalid arguments");
    
    /*set wd management instance fields */
    wd_management.m_frequency_check = atoi(argv[1]);
    EXIT_IF_BAD(wd_management.m_frequency_check > 0, 1, "invaliud arguments");
    wd_management.m_client_cmd = argv + CLIENT_ARGS_IDX;
    wd_management.m_partner_pid = getppid();
    wd_management.m_fail_cnt =  argv + FAIL_CNT_IDX;

    /*register signals to their handlers */
    RegisterSignals();
    /*SchedulerCreate*/
    /*if failed exit*/
    /*set m_sch*/
    /*SwitchModeIMP MODE_INIT*/
    /*SchedulerRun*/
    /*SchedulerDestroy*/
    /*return 0*/
}

static void SwitchModeIMP(wd_management_ty* info, mode_ty new_mode)
{
    /*assert info*/
    /*assert new_mode < NUM_MODES*/
    /*SchedulerClear*/
    /*update m_current_mode*/
    /*call transition_table[new_mode](info)*/
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

static void LoadInitTasksIMP(wd_management_ty* info)
{
    /*AddTaskIMP RaiseSemaphoreTSK*/
}

static void LoadRoutineTasksIMP(wd_management_ty* info)
{
    /*AddTaskIMP SendHeartbeatTSK*/
    /*AddTaskIMP IncrementCounterTSK*/
    /*AddTaskIMP CheckCounterTSK*/
    /*AddTaskIMP CheckExitFlagTSK*/
}

static void LoadReviveTasksIMP(wd_management_ty* info)
{
    /*AddTaskIMP ReviveTSK*/
}

static void LoadErrorTasksIMP(wd_management_ty* info)
{
    /*AddTaskIMP ExitTSK*/
}

static void LoadExitTasksIMP(wd_management_ty* info)
{
    /*AddTaskIMP StopSchedulerTSK*/
}

static void AddTaskIMP(wd_management_ty* info, scheduler_op_ty tsk_func)
{
    /*assert info*/
    /*assert tsk_func*/
    /*SchedulerAddTask with interval tsk_func info TaskCleanupIMP*/
    /*if failed exit*/
}

static void ResetCounterSigHandler(int sig)
{
    /*atomic store g_missed_signals_cnt = 0*/
}

static void SetExitFlagSigHandler(int sig)
{
    /*atomic store g_should_exit = 1*/
}

static void RegisterSignalHandlersIMP(void)
{
    /*sigemptyset*/
    /*sigaction SIGUSR1 -> ResetCounterSigHandler*/
    /*sigaction SIGUSR2 -> SetExitFlagSigHandler*/
}

static void TaskCleanupIMP(void* unused)
{
    /*UNUSED*/
}
