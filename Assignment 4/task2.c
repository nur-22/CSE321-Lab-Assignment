#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define MaxCrops 5 // Maximum crops a Farmer can produce or a ShopOwner can take
#define warehouseSize 5 // Size of the warehouse
#define RUN_TIME 5

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
char crops[warehouseSize] = {'R', 'W', 'P', 'S', 'M'}; // Indicating room for different crops
char warehouse[warehouseSize] = {'N', 'N', 'N', 'N', 'N'}; // Initially all the rooms are empty
pthread_mutex_t mutex;
int terminate = 0;

void *Farmer(void *farmer_number)
{
    int fnum = *((int *)farmer_number);
    char crop = crops[fnum - 1];

    while (!terminate)
    {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        // Critical Section
        warehouse[in] = crop;
        printf("Farmer %d: Insert crops %c at %d\n", fnum, crop, in);
        in = (in + 1) % warehouseSize;
        printf("Warehouse: ");
        for (int i = 0; i < warehouseSize; i++)
        {
            printf("%c", warehouse[i]);
        }
        printf("\n");
        // End Critical Section
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }
}

void *ShopOwner(void *shop_owner_number)
{
    int snum = *((int *)shop_owner_number);

    while (!terminate)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        // Critical Section
        char crop = warehouse[out];
        printf("Shop owner %d: Remove crops %c from %d\n", snum, crop, out);
        warehouse[out] = 'N';
        out = (out + 1) % warehouseSize;
        printf("Warehouse: ");
        for (int i = 0; i < warehouseSize; i++)
        {
            printf("%c", warehouse[i]);
        }
        printf("\n");
        // End Critical Section
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(1);
    }
}

void exit_after_time(int signum)
{
    printf("\nProgram execution time reached. Exiting...\n");
    terminate = 1;
}

int main()
{
    signal(SIGALRM, exit_after_time);
    alarm(RUN_TIME);

    pthread_t farmer_threads[5], shop_owner_threads[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, warehouseSize);
    sem_init(&full, 0, 0);

    int farmer_numbers[5] = {1, 2, 3, 4, 5};
    int shop_owner_numbers[5] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&farmer_threads[i], NULL, Farmer, (void *)&farmer_numbers[i]);
        pthread_create(&shop_owner_threads[i], NULL, ShopOwner, (void *)&shop_owner_numbers[i]);
    }
    while (!terminate)
    {
        sleep(1);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
