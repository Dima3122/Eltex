#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define THREADS 8
#define COUNT_SHOPS 5
#define COUNT_BUYERS 3
#define COUNT_SHOPS 5
#define PTHREAD_ERROR -1

int shops[COUNT_SHOPS];

typedef struct
{
    int ID;
    int Left_to_buy;
    int TimeToSleep;
} Buyer;

typedef struct
{
    int how_to_replenish_the_store;
    int TimeToSleep;
} Supplier;

pthread_mutex_t MUTEX[COUNT_SHOPS] = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t UPDATE_SHOP = PTHREAD_MUTEX_INITIALIZER;

void *buy(void *pthreadData)
{
    Buyer *buyer = (Buyer *)pthreadData;
    return 0;
}

void *bring_goods_to_the_store(void *pthreadData)
{
    Supplier *supplier = (Supplier *)pthreadData;
    while (1)
    {
        for (int i = 0; i < COUNT_SHOPS; i++)
        {
            if (shops[i] == 0)
            {
                pthread_mutex_lock(&UPDATE_SHOP);
                shops[i]+= supplier->how_to_replenish_the_store;
                pthread_mutex_unlock(&UPDATE_SHOP);
                sleep(supplier->TimeToSleep); 
            }   
        }
    }
    return 0;
}

int main()
{
    Buyer buyer[COUNT_BUYERS];
    pthread_t buyerThreads[COUNT_BUYERS];

    Supplier supplier;
    pthread_t supplierThread;
    supplier.how_to_replenish_the_store = 1000;
    supplier.TimeToSleep = 2;
    
    int status = pthread_create(&supplierThread, NULL, bring_goods_to_the_store, &supplier);
    if (status != 0)
    {
        printf("pthread error");
        exit(PTHREAD_ERROR);
    }
    for (int i = 0; i < COUNT_BUYERS; i++)
    {
        buyer->ID = i;
        buyer->Left_to_buy = 10000;
        buyer->TimeToSleep = 3;
        status = pthread_create(&buyerThreads[i], NULL, buy, &buyer[i]);
        if (status != 0)
        {
            printf("pthread error");
            exit(PTHREAD_ERROR);
        }
    }
    return 0;
}