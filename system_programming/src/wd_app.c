#define _POSIX_C_SOURCE 199506L
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <sys/wait.h>
/*-----------------------------------------------------------------------------*/
#include "utils.h"
#include "scheduler.h"
/*-----------------------------------------------------------------------------*/
#define UNUSED(x) (void)x
/*-----------------------------------------------------------------------------*/
enum
{
    INTERVAL_IDX = 1,
    FAIL_CNT_IDX = 2,
    CLIENT_ARGS_IDX = 3,
    MIN_ARGS = 4
};
/*-----------------------------------------------------------------------------*/
typedef enum
{
    ROUTINE_TASKS = 0,
    REVIVE_TASKS,
    ERROR_TASKS,
    EXIT_TASKS,
    NUM_TASKS
} tasks_type_ty;
/*-----------------------------------------------------------------------------*/
/* management struct for a wd instance */
typedef struct
{
    char** m_client_argv;
    scheduler_ty* m_sch;
    unsigned int m_max_fail_cnt;
    unsigned int m_frequency_check;
    tasks_type_ty m_current_mode;
    pid_t m_partner_pid;
} wd_management_ty;
/*-----------------------------------------------------------------------------*/
/*loaders function*/
typedef void (*load_tasks_func_ty)(wd_management_ty*);
/*-----------------------------------------------------------------------------*/
static volatile unsigned int g_missed_signals_cnt = 0;
static volatile int g_should_exit = 0;
/*-----------------------------------------------------------------------------*/
/*method to switch between modes*/
static void SwitchModeIMP(wd_management_ty* info, tasks_type_ty new_mode);
/*-----------------------------------------------------------------------------*/
/*loaders of relevant tasks according to system's current state (not previous so not FSM)*/
static void LoadRoutineTasksIMP(wd_management_ty* info);
static void LoadReviveTasksIMP(wd_management_ty* info);
static void LoadErrorTasksIMP(wd_management_ty* info);
static void LoadExitTasksIMP(wd_management_ty* info);
/*-----------------------------------------------------------------------------*/
/*----tasks----*/
/*routine tasks*/
static sch_op_status_ty SendHeartbesatTSK(void* info);
static sch_op_status_ty IncrementCounterTSK(void* info);
static sch_op_status_ty CheckCounterTSK(void* info);
static sch_op_status_ty CheckExitFlagTSK(void* info);
/*revive task*/
static sch_op_status_ty ReviveTSK(void* info);
/*exit task*/
static sch_op_status_ty ExitTSK(void* info);
/*error task*/
static sch_op_status_ty ErrorTSK(void* info);
/*-----------------------------------------------------------------------------*/
/*method to set signal handlers*/
static void RegisterSignalHandlersIMP(void);
/*signal handlers functions*/
static void ResetCounterSH(int sig);
static void SetExitFlagSH(int sig);
/*-----------------------------------------------------------------------------*/
/*dummy funciton*/
static void TaskCleanupIMP(void* unused);
/*-----------------------------------------------------------------------------*/
/*transition table between modes*/
static load_tasks_func_ty transition_table[NUM_TASKS] =
{
    LoadRoutineTasksIMP,
    LoadReviveTasksIMP,
    LoadErrorTasksIMP,
    LoadExitTasksIMP
};
/*-----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	/*declare on scheduler instance*/
	/*declare on wd management struct instance*/
	
	/*asserts*/
    /*validate argc*/
    
    /*set wd management instance fields */
    	/*set this instance's partner as it's parent (client)*/

    /*register signals to their handlers (not in scheduler to prevent missing sig)*/

    /*create scheduler */
    /*handle failure*/

    /*switch mode to ROUTINE_TASKS*/
    
    /*SchedulerRun*/
	/*handle return value */
	
	/*we got here if client asked to stop wd app/ scheduler is empty*/
    /*switch mode to EXIT_TASKS*/

    /*return 0*/
    return 0;
}
/*-----------------------------------------------------------------------------*/
static void SwitchModeIMP(wd_management_ty* info_, tasks_type_ty new_mode_)
{
    /*assert info*/
    /*assert new_mode < NUM_TASKS*/
    
    /*clear scheduler*/
    
    /*assign current mode to be new mode*/
    /*call transition_table to load relevant tasks */
}
/*-------------------------TASKS IMPLEMENTATONS -------------------------------*/
static sch_op_status_ty SendHeartbeatTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    
    /*send SIGUSR1 to partner*/
    /*if failed*/
        /*if errno is ESRCH(The target process does not exist)*/
            /*SwitchModeIMP REVIVE_TASKS*/
            /*return SCH_NOT_REPEAT*/
            
        /*otherwise*/
        	/*SwitchModeIMP ERROR_TASKS*/
        /*return SCH_NOT_REPEAT*/
        
    /*return SCH_REPEAT*/
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty IncrementCounterTSK(void* info_)
{
    /*atomic increment g_missed_signals_cnt*/
    /*return SCH_REPEAT*/
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty CheckCounterTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    /*if counter >= fail_cnt*/
        /*SwitchModeIMP REVIVE_TASKS*/
        /*return SCH_NOT_REPEAT*/
    /*return SCH_REPEAT*/
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty CheckExitFlagTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    /*if g_should_exit*/
        /*SwitchModeIMP EXIT_TASKS*/
        /*return SCH_NOT_REPEAT*/
    /*return SCH_REPEAT*/
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty ReviveTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
	/*execvp client_argv*/
	/*handle failure*/
	/*return not repeat (supposed to not get here)*/
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty ExitTSK(void* info_)
{
    /*cast param*/
    /*assert param*/
    /*stop scheduler*/
    /*return SCH_NOT_REPEAT*/
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty ErrorTSK(void* info)
{
	/*cast param*/
	/*assert param*/
	/*stop scheduler*/
	/*return SCH_NOT_REPEAT*/
}
/*------------------- SIGNAL HANDLERS ----------------------------------------*/
static void ResetCounterSH(int sig)
{
    /*atomic store g_missed_signals_cnt = 0*/
}
/*-----------------------------------------------------------------------------*/
static void SetExitFlagSH(int sig)
{
    /*atomic store g_should_exit = 1*/
}
/*--------------------------AUX FUNCTIONS - -----------------------------------*/
/*---------------------------LOADERS-------------------------------------------*/
static void LoadRoutineTasksIMP(wd_management_ty* info)
{
    /*add task SendHeartbeatTSK*/
    /*add task IncrementCounterTSK*/
    /*add task CheckCounterTSK*/
    /*add task CheckExitFlagTSK*/
}
/*-----------------------------------------------------------------------------*/
static void LoadReviveTasksIMP(wd_management_ty* info)
{
    /*add task ReviveTSK*/
}
/*-----------------------------------------------------------------------------*/
static void LoadErrorTasksIMP(wd_management_ty* info)
{
    /*add task ErrorTSK*/
}
/*-----------------------------------------------------------------------------*/
static void LoadExitTasksIMP(wd_management_ty* info)
{
    /*add task ExitTSK*/
}
/*-----------------------------------------------------------------------------*/
static void RegisterSignalHandlersIMP(void)
{
    /*sigaction SIGUSR1 to be handled via ResetCounterSH*/
    /*sigaction SIGUSR2 to be handled via SetExitFlagSH*/
}
/*-----------------------------------------------------------------------------*/
static void TaskCleanupIMP(void* unused)
{
    /*UNUSED*/
}
