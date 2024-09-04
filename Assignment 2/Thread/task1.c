#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

pthread_mutex_t mutex;
int current_thread = 1;

void *thread_function(void *thread_num) {
    int thread_id = *(int *)thread_num;
    while(1) {
        pthread_mutex_lock(&mutex);
        if (current_thread != thread_id) {
            pthread_mutex_unlock(&mutex);
            continue;
        }
        printf("thread-%d running\n", thread_id);
        pthread_mutex_unlock(&mutex);
       
        usleep(500000);
        
        pthread_mutex_lock(&mutex);
        printf("thread-%d closed\n", thread_id);
        current_thread = (current_thread % NUM_THREADS) + 1;
        pthread_mutex_unlock(&mutex);
        break;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_function, (void *)&thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
