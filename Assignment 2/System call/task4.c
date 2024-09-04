#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <numbers>\n", argv[0]);
        return 1;
    }

    int n = argc - 1;
    int numbers[n];
    for (int i = 0; i < n; i++) {
        numbers[i] = atoi(argv[i + 1]);
    }

    // Creating a child process
    pid_t child_pid = fork();

    if (child_pid < 0) {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (child_pid == 0) {
        bubbleSort(numbers, n);
        printf("Sorted array in descending order: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", numbers[i]);
        }
        printf("\n");
    } else {
        // Parent process
        wait(NULL); // Wait for child process to finish

        printf("Odd/Even status for each number in the sorted array:\n");
        for (int i = 0; i < n; i++) {
            if (numbers[i] % 2 == 0) {
                printf("%d is even\n", numbers[i]);
            } else {
                printf("%d is odd\n", numbers[i]);
            }
        }
    }

    return 0;
}
