#include <stdio.h>		/*printf()*/
#include <pthread.h>	/**/
#include <semaphore.h>	/**/
/*-----------------------------------------------------------------------------*/
static slist_ty* g_messages = NULL;
static pthread_mutex_t g_lock;
/*make name of semaphore to indicate what we count */
static sem_t g_items; 
/*-----------------------------------------------------------------------------*/
static void* ProducerThreadIMP(void* arg_);
static void* ConsumerThreadIMP(void* arg_);
static int* ProduceIMP(void);
static void ConsumeIMP(int* message_);
static void AddMessageIMP(int* message_);
static int* ReadMessageIMP(void);
/*----------------------------------------------------------------------------*/
int main()
{
	/*asserts*/
	
	/*create slist*/
	/*if creation failed*/
		/*exit*/
	
	/*init mutex*/
	/*if failed*/
		/*exit*/
		
	/*init semaphore to PRODUCTS_NUM*/
	/*if failed*/
		/*exit*/
		
	/*create PRODUCERS_NUM threads */
		/*exit if failed*/
		
	/*create CONSUMERS_NUM threads */
		/*exit if failed*/
		
	/*join PRODUCERS_NUM threads */
		/*exit if failed*/
		
	/*join CONSUMERS_NUM threads */
		/*exit if failed*/
		
	/*destroy mutex*/
	/*destroy semaphore*/
	/*destroy slist*/
}
/*----------------------------------------------------------------------------*/
static void* ProducerThreadIMP(void* arg_)
{
	/*loop PRODUCTS_NUM times*/
	
		/*call ProduceIMP() and save int* message*/
		/*assert message != NULL*/
		
		/*sem_wait(g_items) - block if list is full (count == 0)*/
		
		/*lock mutex*/
			/*SlistInsertBefore - insert message into g_messages*/
			/*assert insertion succeeded*/
		/*unlock mutex*/
}
/*----------------------------------------------------------------------------*/
static void* ConsumerThreadIMP(void* arg_)
{
	/*loop PRODUCTS_NUM times*/
	
		/*lock mutex*/
			/*get begin iter*/
			/*assert begin != end (list not empty)*/
			/*get data from begin iter - save as int* message*/
			/*SListRemove begin iter*/
		/*unlock mutex*/
		
		/*sem_post(g_items) - signal one slot freed, wake a producer*/
		
		/*call ConsumeIMP(message)*/
}
/*----------------------------------------------------------------------------*/
static int* ProduceIMP(void)
{
	/*allocate int* message*/
	/*if allocation failed*/
		/*return NULL*/
	
	/*fill message with value (static counter++)*/
	/*return message*/
}
/*----------------------------------------------------------------------------*/
static void ConsumeIMP(int* message_)
{
	/*assert message_ != NULL*/
	
	/*print message value*/
	/*free message*/
}
/*----------------------------------------------------------------------------*/
static void AddMessageIMP(int* message_)
{
	/*assert message_ != NULL*/
	
	/*sem_wait(g_items)  block if list is full*/
	
	/*lock mutex*/
		/*SlistInsertBefore  insert at end of g_messages*/
		/*assert insertion succeeded*/
	/*unlock mutex*/
}
/*----------------------------------------------------------------------------*/
static int* ReadMessageIMP(void)
{
	/*declare int* message = NULL*/
	
	/*lock mutex*/
		/*get begin iter*/
		/*assert begin != end*/
		/*get data from begin - save as message*/
		/*SListRemove begin*/
	/*unlock mutex*/
	
	/*sem_post(g_items)  signal slot freed*/
	
	/*return message*/
}
