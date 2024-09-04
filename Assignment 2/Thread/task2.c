#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define NUM_INTEGERS_PER_THREAD 5

pthread_mutex_t mutex;

void *thread_function(void *thread_num) {
    int thread_id = *(int *)thread_num;
    int start_integer = (thread_id * NUM_INTEGERS_PER_THREAD) + 1;
    int end_integer = start_integer + NUM_INTEGERS_PER_THREAD - 1;

    for (int i = start_integer; i <= end_integer; ++i) {
        printf("Thread %d prints %d\n", thread_id, i);
    }
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, (void *)&thread_ids[i]);
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
