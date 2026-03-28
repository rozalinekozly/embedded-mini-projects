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
	3- all observers read message simultainsly
	4- avoid duplication: avoid same observer to read same message more than once
	5- avoid busy waiting
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define OBSERVERS_NUM	3
#define MSG_NUM			5

/* observer's struct */
typedef struct
{
    int id;			/*observer id (to print)*/
    int recent_version;		/*marks recent version the observer has read*/
    pthread_t observer_thrd;	/*handle for observer's thread*/
} observer_ty;

typedef struct
{
	int msg = 0;	/* critical resource*/
	int msg_version = 0;	/*utilied to make sure observers do not read 
							same message more than once */
	int is_observed = 0;	/*this flag identifies to broadcaster that 
							all observers has read the message and ite
							can write a new messgae*/
}msg_ty;

