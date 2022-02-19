#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>

#define THREADS 8
#define MAX 10000000000
#define PTHREAD_ERROR -1

int count = 0;

typedef struct 
{
    int pthreadNum;
}PthreadData;

pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

void *increment(void *inputdata)
{
    PthreadData* data = (PthreadData*)inputdata;
    int tmp = 0;
    for (int i = 0; i < MAX/THREADS; i++)
    {
        pthread_mutex_lock(&MUTEX);
        tmp = count;
        tmp++;
        count = tmp;
        pthread_mutex_unlock(&MUTEX);
    }
    printf("thread %d complited!\n", data->pthreadNum);
    return 0;
}

int main()
{
    pthread_t *thread = malloc(sizeof(pthread_t) * THREADS);
    PthreadData *data = malloc(sizeof(PthreadData) * THREADS);
    int status;

    for (int i = 0; i < THREADS; i++)
    {
        data[i].pthreadNum = i;
        status = pthread_create(&thread[i], NULL, increment, &data[i]);
        if (status != 0)
        {
            printf("pthread error, exit!");
            exit(PTHREAD_ERROR);
        }
    }
    for (int i = 0; i < THREADS; i++)
    {
        status = pthread_join(thread[i], NULL);
        if (status != 0)
        {
            printf("error pthread join");
            exit(PTHREAD_ERROR);
        }
    }
    printf("count = %d\n", count);
    return 0;
}