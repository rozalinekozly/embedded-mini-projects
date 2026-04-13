/*
	watch dog app version 2: user input <wd_app_exe> <frequency_check> <client_app>
*/
/*-----------------------------------------------------------------------------*/
#define _POSIX_SOURCE
#include <signal.h>		/* kill */
#include <unistd.h>		/* getppid, execvp*/
#include <stdlib.h>		/* atoi*/
#include <assert.h>		/* assert  */
/*-----------------------------------------------------------------------------*/
#include "scheduler.h"
#include "utils.h"
/*-----------------------------------------------------------------------------*/
typedef struct
{
	pid_t pid;
	char** client_cmd;
	int frequency_check;
}client_ty;
/*-----------------------------------------------------------------------------*/
#define UNUSED(x)	(void)x
/*-----------------------------------------------------------------------------*/
/*forward declarations*/
static sch_op_status_ty ReviveIfNotAliveIMP(void*);
static void RevivalCleanupIMP(void*);
/*-----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	scheduler_ty* sch = NULL;
	uid_ty task_id = {0};
	client_ty client = {0};
	
	/*asserts*/
	/*if argc < 3 */
		/*exit*/
	EXIT_IF_BAD(3 <= argc, 1, "Invalid arguments");
	
	/*init client's fields*/
		/*set frequency_check as argv[1] (convert to int)*/
		client.frequency_check = atoi(argv[1]);
		TODO: validate return value
		/*set client_cmd as argv + 2)*/
		client.client_cmd = argv + 2;
		/*set clent pid as parent pid via ppid*/
		client.pid = getppid();
	
	/*create a scheduler*/
	sch = SchedulerCreate();
	/*handle failure*/
	EXIT_IF_BAD(NULL != sch, 1, "scheduler creation failed");
	/*add task to scheduler with frequency_check as an interval, op_func as ReviveIfNotAliveIMP,
	op_param &client instance, clean_func = dummy cleanup func, clean_param is null*/
	task_id = SchedulerAddTask(sch, client.frequency_check, ReviveIfNotAliveIMP,
								&client, RevivalCleanupIMP, NULL);
	/*handle failure*/
	EXIT_IF_BAD(!IsMatchId(invalid_uid_g, task_id), 1, "failed to add task");
	
	/*run scheduler*/
	SchedulerRun(sch);
	
	/*cleanup*/
	/*destroy scheduler*/
	SchedulerDestroy(sch);
	return 0;
}
/*-----------------------------------------------------------------------------*/
static sch_op_status_ty ReviveIfNotAliveIMP(void* param_)
{
	client_ty* client = NULL;
	/*assertions*/
	assert(NULL != param_);
	/*cast param to client_ty*/
	client = (client_ty*)param_;
	
	/*send client app instance a signal via kill 0*/
	/*if fails (client is dead)*/
	if (-1 == kill(client->pid, 0))
	{
		/*execvp(client_cmd[0], client_cmd)*/
		execvp(client->client_cmd[0], client->client_cmd);
		/*execvp failed*/
			/*return SCH_NOT_REPEAT*/
			return SCH_NOT_REPEAT;
	}
	/*client alive */
		/*return SCH_REPEAT*/
		return SCH_REPEAT;
}
/*-----------------------------------------------------------------------------*/
static void RevivalCleanupIMP(void* param_)
{
	/*do nothing*/
	UNUSED(param_);
}
/*-----------------------------------------------------------------------------*/
