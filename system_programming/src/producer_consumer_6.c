#include <pthread.h>   /* pthread_t, pthread_create, pthread_join, pthread_mutex_t,
                         pthread_cond_t and their related functions */
#include <semaphore.h> /*sem_t, sem_init, sem_wait, sem_post, sem_destroy */
#include <stdio.h>     /* printf */

#include "utils.h"	  /*EXIT_IF_BAD*/
/*----------------------------------------------------------------------------*/
#define CONSUMERS_NUM	3

enum
{
	MSG_NUM = 5
};
/*----------------------------------------------------------------------------*/
typedef struct
{
    int id;					/*consumer id (to print)*/
    int recent_version;		/*marks recent version the consumer has read*/
    int msg;				/*recent version msg (for prinitng)*/
    pthread_t consumer_thrd;	/*handle for consumer's thread*/
} consumer_ty;

typedef struct
{
	volatile int msg;					/* critical resource = actual messgae*/
	volatile int msg_version;			/*utilied to make sure consumers do not read 
						 		same message more than once */
	sem_t observed_msg_count;	/*counts number of consumers that has consumed 
							  	msg with msg_version*/
	pthread_mutex_t lock;		/*lock for msg field*/
	pthread_cond_t new_msg_cond;/*cond var to identify a new message*/
}msg_ty;
/*----------------------------------------------------------------------------*/
static msg_ty g_message;
/*-------------------------forward declarations-------------------------------*/
void* ConsumerThreadIMP(void* arg_);
void* ProducerThreadIMP(void* arg_);

int ProduceMessageIMP(void);
void ConsumeMessageIMP(consumer_ty* consumer_);

void InitMessage(void);
void DestroyMessage(void);
/*----------------------------------------------------------------------------*/
int main(void)
{
	int i = 0;
	int is_failed = 0;
	
	/*declare on consumers array intilaized to {0}*/
	consumer_ty consumers[CONSUMERS_NUM] = {0};
	/*declare on producer, intialized to {0}*/
	pthread_t producer = {0};
	
	/*create/init message's instance fields'*/
	/*call InitMessage*/
	InitMessage();
	
	/*create consumers threads*/
	for(i = 0; i < CONSUMERS_NUM; ++i)
	{
		consumers[i].id = i;
		/*traverse on consumers and for each instance call pthread_create*/
		is_failed = pthread_create(&consumers[i].consumer_thrd, NULL, ConsumerThreadIMP,
									&consumers[i]);
		/*if creation failed*/
			/*exit*/
		EXIT_IF_BAD(0 == is_failed, 1 , "failed at creating a consumer");
	}
		/*create a producer thread via pthread_create*/
		is_failed = pthread_create(&producer, NULL, ProducerThreadIMP, NULL);
		/*if failed*/
			/*exit*/
		EXIT_IF_BAD(0 == is_failed, 1, "failed to create a producer's thread");
			
		/*join producer's thread*/
		is_failed = pthread_join(producer, NULL);
		/*if failed*/
			/*exit*/
		EXIT_IF_BAD(0 == is_failed,1, "failed to join producer thread");
			
		/*join consumers (via loop) threads */
		for(i = 0 ; i < CONSUMERS_NUM ; i++)
		{
			/*if failed*/
			is_failed = pthread_join(consumers[i].consumer_thrd, NULL);
				/*exit*/
			EXIT_IF_BAD(0 == is_failed, 1, "failed to join a consumer");
		}
		
	/*cleanup*/
		/*call DestroyMessage*/
		DestroyMessage();
		
	/*return 0*/
	return 0;
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
	EXIT_IF_BAD(0 == is_failed, 1, "failed init semaphore");
	/*init mutex lock*/
	is_failed = pthread_mutex_init(&g_message.lock, NULL);
	EXIT_IF_BAD(0 == is_failed, 1, "failed init mutex");
	/*init condition variable*/
	is_failed = pthread_cond_init(&g_message.new_msg_cond, NULL);
	EXIT_IF_BAD(0 == is_failed, 1, "failed init condition variable");
}
/*----------------------------------------------------------------------------*/
void DestroyMessage()
{
	/*destroy semaphore*/
	sem_destroy(&g_message.observed_msg_count);
	/*destroy mutex*/
	pthread_mutex_destroy(&g_message.lock);
	/*destroy cond var*/
	pthread_cond_destroy(&g_message.new_msg_cond);
}
/*----------------------------------------------------------------------------*/
void* ConsumerThreadIMP(void* arg_)
{
	int i = 0;
	/*cast arg_ to consumer_ty* type*/
	consumer_ty* consumer = (consumer_ty*)arg_;
	/*iterate from 0 to MSG_NUM times*/
	for(i = 0 ; i < MSG_NUM ; i++)
	{	
		/*lock mutex*/
		pthread_mutex_lock(&g_message.lock);
			/*cond_wait*/
			pthread_cond_wait(&g_message.new_msg_cond, &g_message.lock);
		/*unlock mutex*/
		pthread_mutex_unlock(&g_message.lock);
		/*if consumer's recent_version equals message's version = no new one*/
		if(consumer->recent_version == g_message.msg_version)
		{
			/*continue */
			continue;
		}	
		/*otherwise*/
			/*update consumer's fields */
			consumer->recent_version = g_message.msg_version;
			consumer->msg = g_message.msg;
			ConsumeMessageIMP(consumer);
		/*increment semaphore*/
		sem_post(&g_message.observed_msg_count);
	}
	return NULL;
}
/*----------------------------------------------------------------------------*/
void* ProducerThreadIMP(void* arg_)
{
	int i = 0;
	int j = 0;
	int local_msg = -1;

	/*iterate from 0 to MSG_NUM*/
	for(i = 0 ; i < MSG_NUM ; i++)
	{
		/*get new message*/
			/*call ProduceMessageIMP() and store it's return value in local variable*/
			local_msg = ProduceMessageIMP();
		/*lock mutex of shared resource*/
		pthread_mutex_lock(&g_message.lock);
			/*update message (copy local msg var to it)*/
			g_message.msg = local_msg;
			/*increment message instance's version*/
			++g_message.msg_version;
			/*notify all consumers*/
			pthread_cond_broadcast(&g_message.new_msg_cond);
		/*unlock mutex*/
		pthread_mutex_unlock(&g_message.lock);
		/*wait until all consumers signaled that they finished reading this 
		message (gave off their semaphore) call wait CONSUMERS_NUM times*/
			/*loop from 0 to CONSUMERS_NUM*/
		for(j = 0 ; j < CONSUMERS_NUM ; j++)
		{
				/*wait*/
				sem_wait(&g_message.observed_msg_count);
		}
	}
	
	return NULL;
}
/*----------------------------------------------------------------------------*/
int ProduceMessageIMP()
{
	/*declare on a static variable (the generator)*/
	static int ret = 0;
	/*increment generator by 1*/
	++ret;
	/*return generator*/
	return ret;
}
/*----------------------------------------------------------------------------*/
void ConsumeMessageIMP(consumer_ty* consumer_)
{
	/*print consumer's id and its message */
	printf("consumer %d, received message: %d\n", consumer_->id, consumer_->msg);
}
/*----------------------------------------------------------------------------*/
