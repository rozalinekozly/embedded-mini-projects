/*mutex locks template */
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock;
int shared_resource = 0;

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

int main()
{
    pthread_t t1, t2;

    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, Routine, NULL);
    pthread_create(&t2, NULL, Routine, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);

    printf("Final value: %d\n", shared_resource);

    return 0;
}
