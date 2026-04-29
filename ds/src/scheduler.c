/*-----------------------------------------------------------------------------
submitter : Rozaline Kozly
reviewer : Itay
version : 1
date : 11 Jan 2026
------------------------------------------------------------------------------*/
#include <stdlib.h>	/* malloc(), free() */
#include <assert.h>	/* assert() */
#include <unistd.h>	/* sleep() */
#include <time.h>	/* time_t, time(), difftime() */

#include "scheduler.h"
#include "pqueue.h"
#include "task.h"
#include "uid.h"

#define UNUSED(var) ((void)(var))

/*-----------------------------------------------------------------------------*/
struct scheduler
{
	pq_ty* pq;
	sch_run_status_ty run_status;
	uid_ty current_task_uid;
	int is_running;
	int skip_reschedule;
};
/*---------------------------aux functions -------------------------------------*/
static void HandleFailure(scheduler_ty* sch);
static int TaskCmp(const void* task1, const void* task2, void* param);
static int WrapperIsMatchId( void* data1, void* data2);
/*-----------------------------------------------------------------------------*/
scheduler_ty* SchedulerCreate(void)
{
	scheduler_ty* sch = (scheduler_ty*)malloc(sizeof(*sch));
	if (NULL == sch)
	{
		return NULL;
	}
	
	sch->pq = PQCreate(TaskCmp, NULL); 
	if (NULL == sch->pq)
	{
		free(sch);
		sch = NULL;
		return NULL;
	}
	
	sch->is_running = 0;
	sch->current_task_uid = invalid_uid_g;
	sch->skip_reschedule = 0;
	sch->run_status = SCH_SUCCESS;
	
	return sch;
}
/*-----------------------------------------------------------------------------*/
void SchedulerDestroy(scheduler_ty* sch)
{
	if (NULL == sch)
	{
		return;
	}
	
	SchedulerClear(sch);
	PQDestroy(sch->pq);
	free(sch);
	sch = NULL;
}
/*-----------------------------------------------------------------------------*/
uid_ty SchedulerAddTask(scheduler_ty* sch, unsigned int interval, 
                        scheduler_op_ty op_func, void* op_param,
                        scheduler_cleanup_ty clean_func, void* clean_param)
{
	task_ty* tsk = NULL;
	
	assert(NULL != sch);
	assert(NULL != op_func);
	assert(NULL != clean_func);
	
	tsk = TaskCreate((task_op_ty)op_func, op_param, 
					(task_cleanup_ty)clean_func, clean_param, interval);
	if (NULL == tsk)
	{
		return invalid_uid_g;
	}
	
	if (PQ_FAIL == PQEnqueue(sch->pq, tsk))
	{
		TaskDestroy(tsk);
		tsk = NULL;
		return invalid_uid_g;
	}
	
	return TaskGetUID(tsk);
}
/*-----------------------------------------------------------------------------*/
int SchedulerRemoveTask(scheduler_ty* sch, uid_ty task_id)
{
	task_ty* tsk = NULL;
	
	assert(NULL != sch);
	assert(!IsMatchId(task_id, invalid_uid_g));
	
	/* removing current task from within itself */
	if (sch->is_running && IsMatchId(sch->current_task_uid, task_id))
	{
		sch->skip_reschedule = 1;
		return 0;
	}
	
	tsk = PQRemove(sch->pq, WrapperIsMatchId, &task_id);
	if (NULL == tsk)
	{
		return 1;
	}
	
	TaskDestroy(tsk);
	
	return 0;
}
/*-----------------------------------------------------------------------------*/
size_t SchedulerSize(const scheduler_ty* sch)
{
	assert(NULL != sch);
	
	return PQCount(sch->pq) + sch->is_running;
}
/*-----------------------------------------------------------------------------*/
int SchedulerIsEmpty(const scheduler_ty* sch)
{
	assert(NULL != sch);
	
	return PQIsEmpty(sch->pq) && !sch->is_running;
}
/*-----------------------------------------------------------------------------*/
sch_run_status_ty SchedulerRun(scheduler_ty* sch)
{
	task_ty* curr_task = NULL;
	sch_op_status_ty op_status = SCH_REPEAT;
	time_t curr_time = 0;
	time_t time_to_run = 0;
	unsigned int sleep_time = 0;
	
	assert(NULL != sch);
	
	sch->is_running = 1;
	sch->run_status = SCH_SUCCESS;
	
	while (sch->is_running && !PQIsEmpty(sch->pq))
	{
		curr_time = time(NULL);
		if ((time_t)-1 == curr_time)
		{
			HandleFailure(sch);
			return sch->run_status;
		}
		
		curr_task = PQDequeue(sch->pq);
		sch->current_task_uid = TaskGetUID(curr_task);
		
		time_to_run = TaskGetTimeToRun(curr_task);
		if (time_to_run > curr_time)
		{
			sleep_time = (unsigned int)difftime(time_to_run, curr_time);
			while (sleep_time)
			{
				sleep_time = sleep(sleep_time);
			}
		}
		
		op_status = (sch_op_status_ty)TaskExecute(curr_task);
		TaskUpdateTimeToRun(curr_task);
		
		/* only reschedule if interval > 0 */
		if (SCH_REPEAT == op_status && !sch->skip_reschedule && 
				TaskGetTimeToRun(curr_task) >= time_to_run)
		{
			if (PQ_FAIL == PQEnqueue(sch->pq, curr_task))
			{
				TaskDestroy(curr_task);
				HandleFailure(sch);
				return sch->run_status;
			}	
		}
		else
		{
			TaskDestroy(curr_task);
		}
		
		sch->current_task_uid = invalid_uid_g;
		sch->skip_reschedule = 0;
	}
	
	sch->is_running = 0;
	return sch->run_status;
}
/*-----------------------------------------------------------------------------*/
void SchedulerStop(scheduler_ty* sch)
{
	assert(NULL != sch);
	
	sch->is_running = 0;
	sch->run_status = SCH_STOP;
}
/*-----------------------------------------------------------------------------*/
void SchedulerClear(scheduler_ty* sch)
{
	assert(NULL != sch);
	
	while (!PQIsEmpty(sch->pq))
	{
		TaskDestroy(PQDequeue(sch->pq));
	}
	
	if (sch->is_running)
	{
		sch->skip_reschedule = 1;
		/*sch->is_running = 0;*/
	}
}
/*-----------------------------------------------------------------------------*/
static void HandleFailure(scheduler_ty* sch)
{
	sch->run_status = SCH_FAIL;
	sch->is_running = 0;
	sch->current_task_uid = invalid_uid_g;
}
/*-----------------------------------------------------------------------------*/
static int TaskCmp(const void* task1, const void* task2, void* param)
{
	UNUSED(param);
	
	return (int)difftime(TaskGetTimeToRun((task_ty*)task1),  
						TaskGetTimeToRun((task_ty*)task2));
}
/*----------------------------------------------------------------------------*/
static int WrapperIsMatchId( void* data1, void* data2)
{
	return IsMatchId(TaskGetUID((task_ty*)data1), *(uid_ty*)data2);
}
/*-----------------------------------------------------------------------------*/

