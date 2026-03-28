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

void Broadcast(int* msg_);
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

}
/*----------------------------------------------------------------------------*/
void* BroadcasterThread(void* arg_)
{

}
/*----------------------------------------------------------------------------*/
void Broadcast(int* msg_)
{

}
/*----------------------------------------------------------------------------*/
void Observer(observer_ty* observer_)
{

}
/*----------------------------------------------------------------------------*/
