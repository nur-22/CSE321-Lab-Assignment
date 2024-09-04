#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t mutex;
int sum1, sum2, sum3;
char result[20];

// ASCII sum of characters in a string
int ascii_sum(char *name) {
    int sum = 0;
    for (int i = 0; name[i] != '\0'; ++i) {
        sum += name[i];
    }
    return sum;
}

// Thread function to calculate the ASCII sum
void *thread_function(void *name) {
    char *user_name = (char *)name;
    int sum = ascii_sum(user_name);

    pthread_mutex_lock(&mutex);
    if (sum1 == 0) {
        sum1 = sum;
    } else if (sum2 == 0) {
        sum2 = sum;
    } else {
        sum3 = sum;
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

// Thread function to print the result
void *result_thread(void *arg) {
    pthread_mutex_lock(&mutex);
    if (sum1 == sum2 && sum2 == sum3) {
        strcpy(result, "Youreka");
    } else if (sum1 == sum2 || sum2 == sum3 || sum1 == sum3) {
        strcpy(result, "Miracle");
    } else {
        strcpy(result, "Hasta la vista");
    }
    printf("%s\n", result);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[4];
    char names[3][20];

    printf("Enter the names of three users:\n");
    for (int i = 0; i < 3; ++i) {
        printf("User %d: ", i + 1);
        scanf("%s", names[i]);
    }

    pthread_mutex_init(&mutex, NULL);

    // Create threads for calculating ASCII sum
    for (int i = 0; i < 3; ++i) {
        pthread_create(&threads[i], NULL, thread_function, (void *)names[i]);
    }

    // Create result thread
    pthread_create(&threads[3], NULL, result_thread, NULL);

    // Join threads
    for (int i = 0; i < 4; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
