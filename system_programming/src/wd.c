

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
	INIT_TASKS = 0,
    ROUTINE_TASKS,
    REVIVE_TASKS,
    ERROR_TASKS,
    EXIT_TASKS,
    NUM_TASKS
} tasks_type_ty;
/*-----------------------------------------------------------------------------*/
typedef struct
{
	char** m_client_argv; /*in order to send it to to child*/
	sem_t* has_wd_launched;
	pid_t partner;
	
}wd_guard_management_ty;
/*-----------------------------------------------------------------------------*/
/*method to switch between modes*/
static void SwitchModeIMP(wd_guard_management_ty* info, tasks_type_ty new_mode);
/*-----------------------------------------------------------------------------*/
/*loaders of relevant tasks according to system's current state (not previous so not FSM)*/
static void LoadInitTasksIMP(wd_guard_management_ty* info);
static void LoadRoutineTasksIMP(wd_guard_management_ty* info);
static void LoadReviveTasksIMP(wd_guard_management_ty* info);
static void LoadErrorTasksIMP(wd_guard_management_ty* info);
static void LoadExitTasksIMP(wd_guard_management_ty* info);
/*-----------------------------------------------------------------------------*/
/*transition table between modes*/
static load_tasks_func_ty transition_table[NUM_TASKS] =
{
	LoadInitTasksIMP,
    LoadRoutineTasksIMP,
    LoadReviveTasksIMP,
    LoadErrorTasksIMP,
    LoadExitTasksIMP
};
/*-----------------------------------------------------------------------------*/
static void* WdThrdIMP(void* thrd_args_ty);
/*-----------------------------------------------------------------------------*/
/*----tasks----*/
/*init tasks*/
static sch_op_status_ty LaunchedSuccessfullyTSK(void* info);
static sch_op_status_ty RegisterSignalHandlersTSK(void* info);
static sch_op_status_ty SpawnWdAppTSK(void* info);
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
int MakeMeImmortal(size_t cmd_len, const char** cmd, int how_often, int fail_cnt)
{
	/*declare on a thread */
	/*declare on an unamed semaphore (IPC between client and thread)*/
	/*declare on int var to track status*/
	/*declare on a thread arguments struct*/
	
	/*asserts*/
	
	/*set thread arguments struct*/
	/*create thread and assign it to WdThread funtion and store return value 
	  in status*/
		/*if return status is failed */
			/*return failed*/
		/*otherwise*/
			/*wait till you recieve the semaphore (timed waiting) store status*/
			/*close semaphore*/
			/*if status is have not recieved sem*/
				/*return failed*/
			/*otherwise*/
				/*return success*/
}
/*-----------------------------------------------------------------------------*/
void DoNotResuscitate(void)
{

}
/*-----------------------------------------------------------------------------*/
static void* WdThrdIMP(void* thrd_args_ty)
{
	/*asserts*/
	
	/*set signals handlers*/
	/*set wd thread's management struct*/
	
	/*create scheduler */
    /*handle failure*/

    /*switch mode to INIT_TASKS*/
	
	/*run scheduler*/
	/*handle return value */
	
    /*switch mode to EXIT_TASKS*/

    /*return 0*/
}
/*-----------------TASKS IMPLEMENTATIONS--------------------------------------*/
static sch_op_status_ty LaunchedSuccessfullyTSK(void* info)
{
	/*check a global flag (?) or structs field? */
	/*if flag is raised*/
		/*set thread's mode to be ROUTINE*/
		/*return SCH_NOT_REPEAT*/
	/*return SCH_REPEATE*/
	
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty RegisterSignalHandlersTSK(void* info)
{
	/*asserts*/
    /*sigaction SIGUSR1 to be handled via ResetCounterSH*/
    /*sigaction SIGUSR2 to be handled via SetExitFlagSH*/
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty SpawnWdAppTSK(void* info)
{
	/*asserts*/
	/*fork a process*/
		/*if failed*/
			/*set thread's mode to ERROR*/
		/*otherwise*/
		/*set in management struct, partner"s pid*/
		/*if child*/
			/*exec wd_app store it's status*/
			/*exit (exec did not worked)*/
		/*otherwise (parent the wd thread)*/
			/*wait till u recive a heartbeat from child (timed)*/
			/*raise semaphore that he's alive*/
		/*return SCH_NOT_REPEAT*/
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty SendHeartbesatTSK(void* info)
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
static sch_op_status_ty IncrementCounterTSK(void* info)
{
    /*atomic increment g_missed_signals_cnt*/
    /*return SCH_REPEAT*/
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty CheckCounterTSK(void* info)
{
	/*cast param*/
    /*assert param*/
    /*if counter >= fail_cnt*/
        /*SwitchModeIMP REVIVE_TASKS*/
        /*return SCH_NOT_REPEAT*/
    /*return SCH_REPEAT*/
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty CheckExitFlagTSK(void* info)
{
    /*cast param*/
    /*assert param*/
    /*if g_should_exit*/
        /*SwitchModeIMP EXIT_TASKS*/
        /*return SCH_NOT_REPEAT*/
    /*return SCH_REPEAT*/
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty ReviveTSK(void* info)
{
    /*cast param*/
    /*assert param*/
	/*execvp client_argv*/
	/*handle failure*/
	/*return not repeat (supposed to not get here)*/
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty ExitTSK(void* info)
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
/*---------------------------SETTERS -----------------------------------------*/
static void LoadInitTasksIMP(wd_guard_management_ty* info)
{
	/*add task LaunchedSuccessfullyTSK*/
	/*add task RegisterSignalHandlersTSK*/
	/*add SpawnWdAppTSK*/
}
/*-----------------------------------------------------------------------------*/
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
/*DUMMY FUNCTION*/
/*-----------------------------------------------------------------------------*/
static void TaskCleanupIMP(void* unused)
{
    /*UNUSED*/
}
