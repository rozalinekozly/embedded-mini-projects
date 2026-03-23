/*ex2: multi producers and multi consumers, shared resource is slist sync mech is mutex*/
/*mutex locks template */
#include <pthread.h>
#include <stdio.h>

#include "utils.h"
#include "slist.h"

static void* ProducerThreadIMP(void* arg_);
static void* ConsumerThreadIMP(void* arg_);
static int* ProduceIMP(void);
static void ConsumeIMP(int* message_);
static void AddMessageIMP(int* message_);
static int* ReadMessageIMP(void);

enum
{
	PRODUCERS_NUM = 5,
	CONSUMERS_NUM = 5
};

pthread_mutex_t lock;

slist_ty* messages = NULL;

int main()
{
    pthread_t producers[PRODUCERS_NUM];
    pthread_t consumers[CONSUMERS_NUM];
    int i = 0;
    
    messages = SListCreate();
    EXIT_IF_BAD(NULL != messages, 1, "failed to create a linkedlist");
    
	EXIT_IF_BAD(0 == pthread_mutex_init(&lock, NULL), 1, "failed to initialize mutex");

	for(i = 0 ; i < PRODUCERS_NUM ; i++)
	{
		while(0 != pthread_create(&producers[i], NULL, ProducerThreadIMP, (void*)i));
	}

	for(i = 0 ; i < CONSUMERS_NUM ; i++)
	{
		while(0 != pthread_create(&consumers[i], NULL, ConsumerThreadIMP, (void*)i));
	}

	for(i = 0 ; i < PRODUCERS_NUM ; i++)
	{
    	EXIT_IF_BAD(0 == pthread_join(producers[i], NULL), 1, "failed to join producer");
	}

	for(i = 0 ; i < CONSUMERS_NUM ; i++)
	{
    	EXIT_IF_BAD(0 == pthread_join(consumers[i], NULL), 1, "failed to join consumer");
	}

    EXIT_IF_BAD(0 == pthread_mutex_destroy(&lock), 1, "failed to destroy mutex");
    
    return 0;
}

void* Routine(void* arg)
{
    int i;

    for(i = 0; i < 100000; ++i)
    {
        pthread_mutex_lock(&lock);

        shared_resource++;

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}


