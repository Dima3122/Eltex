#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define COUNT_SHOPS 5
#define COUNT_CUSTOMERS 3

typedef struct
{
    int Id;
    int TimeOfSleep;
    int LeftToBuy;
} Customer;

typedef struct
{
    int TimeOfSleep;
    int Capacity;
} Supplier;

int shops[COUNT_SHOPS];
pthread_mutex_t MUTEX[COUNT_SHOPS] = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t COMPLITED_CUSTOMERS_MUTEX = PTHREAD_MUTEX_INITIALIZER;
int COUNT_COMPLITED_CUSTOMERS = 0;

void *buy(void *pthreadData)
{
    Customer *customer = (Customer *)pthreadData;
    int i = 0;
    while (1)
    {
        int previousLeftToBuy = customer->LeftToBuy;
        if (pthread_mutex_trylock(&MUTEX[i]) == 0)
        {
            if (customer->LeftToBuy >= shops[i])
            {
                customer->LeftToBuy -= shops[i];
                shops[i] = 0;
            }
            else
            {
                shops[i] -= customer->LeftToBuy;
                customer->LeftToBuy = 0;
            }
            pthread_mutex_unlock(&MUTEX[i]);
            if (customer->LeftToBuy == 0)
            {
                pthread_mutex_lock(&COMPLITED_CUSTOMERS_MUTEX);
                COUNT_COMPLITED_CUSTOMERS++;
                pthread_mutex_unlock(&COMPLITED_CUSTOMERS_MUTEX);
                
                printf("Customer %d = %d\n", customer->Id, customer->LeftToBuy);
                pthread_exit(0);
            }
            if (previousLeftToBuy != customer->LeftToBuy)
            {
                printf("Customer %d = %d\n", customer->Id, customer->LeftToBuy);
                sleep(customer->TimeOfSleep);
            }
        }
        i == COUNT_SHOPS - 1 ? i = 0 : i++;
    }
}

void *deliver(void *pthreadData)
{
    Supplier *supplier = (Supplier *)pthreadData;
    int i = 0;
    while (1)
    {
        if (pthread_mutex_trylock(&MUTEX[i]) == 0)
        {
            shops[i] += supplier->Capacity;
            pthread_mutex_unlock(&MUTEX[i]);
            if (COUNT_COMPLITED_CUSTOMERS == COUNT_CUSTOMERS)
            {
                pthread_exit(0);
            }
            sleep(supplier->TimeOfSleep);
        }
        i == COUNT_SHOPS - 1 ? i = 0 : i++;
    }
}

int main()
{
    Customer customers[COUNT_CUSTOMERS];
    Supplier supplier;
    pthread_t customerThreads[COUNT_CUSTOMERS],supplierThread;
    int i, status;

    for (i = 0; i < COUNT_CUSTOMERS; i++)
    {
        customers[i].Id = i;
        customers[i].TimeOfSleep = 3;
        customers[i].LeftToBuy = 10000;
    }
    supplier.TimeOfSleep = 2;
    supplier.Capacity = 1000;

    for (i = 0; i < COUNT_CUSTOMERS; i++)
    {
        status = pthread_create(&customerThreads[i], NULL, buy, &customers[i]);
        if (status != 0)
        {
            perror("pthread_create");
            exit(-1);
        }
    }
    status = pthread_create(&supplierThread, NULL, deliver, &supplier);
    if (status != 0)
    {
        perror("pthread_create");
        exit(-1);
    }

    for (i = 0; i < COUNT_CUSTOMERS; i++)
    {
        status = pthread_join(customerThreads[i], NULL);
        if (status != 0)
        {
            perror("pthread_join");
            exit(-1);
        }
    }
    status = pthread_join(supplierThread, NULL);
    if (status != 0)
    {
        perror("pthread_join");
        exit(-1);
    }
    return 0;
}