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
/*replace with enum*/
#define MSG_NUM			5
/*----------------------------------------------------------------------------*/
typedef struct
{
    int id;			/*observer id (to print)*/
    int recent_version;		/*marks recent version the observer has read*/
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
	/*init message (shared resource) fields*/
	/*init semaphore*/
	/*if failed*/
		/*exit*/
	/*init mutex lock (macro)*/
	/*init condition variable (also there's a macro)*/
}
/*----------------------------------------------------------------------------*/
void DestroyMessage()
{
	/*dsetroy semaphore*/
	/*destroy mutex*/
	/*destroy cond var*/
}
/*----------------------------------------------------------------------------*/
void* ObserverThread(void* arg_)
{
	/*cast arg_ to observer_ty* type (maybe make this a wrapper function )*/
	/*iterate from 0 to MSG_NUM times*/
		/*lock mutex*/
			/*cond_wait*/
		/*unlock mutex*/
		/*if observers recent_version equals message's version = no new one '*/
				/*continue */
		/*otherwise*/
			/*decrement semaphore*/
			/*update observer's fields */
		/*increment semaphore*/
}
/*----------------------------------------------------------------------------*/
void* BroadcasterThread(void* arg_)
{
	/*iterate from 0 to MSG_NUM*/
		/*get new message*/
			/*call Broadcast() and store it's return value in local variable*/
		/*lock mutex of shared resource*/
			/*update message (copy local msg var to it)*/
			/*increment message instance's version*/
			/*notify all observers*/
		/*unlock mutex*/
		/*wait until all observers signaled that they finished reading this 
		message (gave off their semaphore) call wait OBSERVE_NUM times*/
			/*loop from 0 to OBSERVERS_NUM*/
				/*wait*/
	
}
/*----------------------------------------------------------------------------*/
int Broadcast()
{
	/*declare on a static variable (the generator)*/
	/*increment generator by 1*/
	/*return generator*/
}
/*----------------------------------------------------------------------------*/
void Observer(observer_ty* observer_)
{
	/*print observer's id and it's message */
}
/*----------------------------------------------------------------------------*/
