/*
	this code writes my implementation of a design pattern Observer / Broadcast 
	pattern via POSIX sync mechansims
	which are : mutex, semaphores, condition variabels.
*/

/* quick explanation of the design pattern */
/*
	(wikipidia) 
	- it's a design pattern, which an object (subject/ event source/ event stream)
	maintains a list of it's dependencies (observers) and automatically notify them 
	of any state changes.
	- one to many dependincies 
*/

/*
	problems to be solved:
	1- no race consition: either observbers / broadcaster enter the critical
	section (message)
	2- notification: observers must know a new message has sent 
	3- all observers can access to same shared message "togather" (no synching
	 here)
	4- avoid duplication: avoid same observer to read same message more than once
	5- avoid busy waiting
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#include "utilis.h"
/*----------------------------------------------------------------------------*/
#define OBSERVERS_NUM	3

enum
{
	MSG_NUM = 5
};
/*----------------------------------------------------------------------------*/
typedef struct
{
    int id;				/*observer id (to print)*/
    int recent_version;		/*marks recent version the observer has read*/
    int msg;				/*recent version msg (for prinitng)*/
    pthread_t observer_thrd;	/*handle for observer's thread*/
} observer_ty;

typedef struct
{
	int msg;					/* critical resource = actual messgae*/
	int msg_version;			/*utilied to make sure observers do not read 
						 		same message more than once */
	sem_t observed_msg_count;	/*counts number of observers that has observed 
							  	msg with msg_version*/
	pthread_mutex_t lock;		/*lock for msg field*/
	pthread_cond_t new_msg_cond;/*cond var to identify a new message*/
}msg_ty;
/*----------------------------------------------------------------------------*/
static msg_ty g_message;
/*-------------------------forward declarations-------------------------------*/
void* ObserverThread(void* arg_);
void* BroadcasterThread(void* arg_);

int Broadcast(void);
void Observer(observer_ty* observer_);

void InitMessage(void);
void DestroyMessage(void);
/*----------------------------------------------------------------------------*/
int main(void)
{
	/*declare on observers array intilaized to {0}*/
	/*declare on brodcaster, intialized to {0}*/
	
	/*create/init message's instance fields'*/
	/*call InitMessage*/
	
	/*create observers threads*/
	/*traverse on observers and for each instance call pthread_create*/
	/*if creation failed*/
		/*exit*/
		
	/*create a broadcaster thread via pthread_create*/
	/*if failed*/
		/*exit*/
		
	/*join brodcaster's thread*/
	/*if failed*/
		/*exit*/
		
	/*join observers (via loop) threads */
	/*if failed*/
		/*exit*/
		
	/*cleanup*/
		/*call DestroyMessage*/
		
	/*return 0*/
}
/*----------------------------------------------------------------------------*/
void InitMessage()
{
	int is_failed = 0;
	/*init message (shared resource) fields*/
	g_message.msg = 0;
	g_message.msg_version = 0;
	/*init semaphore*/
	is_failed = sem_init(&g_message.observed_msg_count, 0 ,0);
	/*if failed*/
		/*exit*/
	EXIT_IF_BAD(0 != is_failed, 1, "failed init semaphore");
	/*init mutex lock (macro)*/
	g_message.lock = PTHREAD_MUTEX_INITIALIZERL
	/*init condition variable (also there's a macro)*/
	g_message.new_msg_cond = PTHREAD_COND_INITIALIZER;
}
/*----------------------------------------------------------------------------*/
void DestroyMessage()
{
	/*dsetroy semaphore*/
	sem_destroy(&g_message.observed_msg_count);
	/*destroy mutex*/
	pthread_mutex_destroy(&g_message.lock);
	/*destroy cond var*/
	pthread_cond_destroy(&g_message.new_msg_cond);
}
/*----------------------------------------------------------------------------*/
void* ObserverThread(void* arg_)
{
	int i = 0;
	/*cast arg_ to observer_ty* type (maybe make this a wrapper function )*/
	observer_ty* observer = (observer_ty*)arg_;
	/*iterate from 0 to MSG_NUM times*/
	for(i = 0 ; i < MSG_NUM ; i++)
	{	
		/*lock mutex*/
		pthread_mutex_lock(&g_message.lock);
			/*cond_wait*/
			pthread_cond_wait(&g_message.new_msg_cond, &g_message.lock);
		/*unlock mutex*/
		pthread_mutex_unlock(&g_message.lock)
		/*if observers recent_version equals message's version = no new one '*/
		if(observer->recent_version == g_message.msg_version)
		{
			/*continue */
			continue;
		}	
		/*otherwise*/
			/*update observer's fields */
			observer->recent_version = g_message.msg_version;
			observer->msg = g_message.msg;
			Observer(observer);
		/*increment semaphore*/
		sem_post(&g_message.observed_msg_count);
	}
	return NULL;
}
/*----------------------------------------------------------------------------*/
void* BroadcasterThread(void* arg_)
{
	int i = 0;
	int j = 0;
	int local_msg = -1;

	/*iterate from 0 to MSG_NUM*/
	for(i = 0 ; i < MSG_NUM ; i++)
	{
		/*get new message*/
			/*call Broadcast() and store it's return value in local variable*/
			local_msg = Broadcast();
		/*lock mutex of shared resource*/
		pthread_mutex_lock(&g_message.lock);
			/*update message (copy local msg var to it)*/
			g_message.msg = local_msg;
			/*increment message instance's version*/
			++g_message.msg_version;
			/*notify all observers*/
			pthread_cond_broadcast(&g_message.new_msg_cond);
		/*unlock mutex*/
		pthread_mutex_unlock(&g_message.lock);
		/*wait until all observers signaled that they finished reading this 
		message (gave off their semaphore) call wait OBSERVE_NUM times*/
			/*loop from 0 to OBSERVERS_NUM*/
		for(j = 0 ; j < OBSERVERS_NUM ; j++)
		{
				/*wait*/
				sem_wait(&g_message.observed_msg_count);
		}
	}
	
	return NULL;
}
/*----------------------------------------------------------------------------*/
int Broadcast()
{
	/*declare on a static variable (the generator)*/
	static int ret = 0;
	/*increment generator by 1*/
	++ret;
	/*return generator*/
	return ret;
}
/*----------------------------------------------------------------------------*/
void Observer(observer_ty* observer_)
{
	/*print observer's id and it's message */
	printf("observer %d, received message: %d\n", observer_->id, observer_->msg);
}
/*----------------------------------------------------------------------------*/
