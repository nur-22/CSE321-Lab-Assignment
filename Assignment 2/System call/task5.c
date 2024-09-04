#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("1. Parent process ID: %d\n", getpid());

    pid_t child_pid = fork();

    if (child_pid < 0) {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (child_pid == 0) {
        // Child process
        printf("2. Child process ID: %d\n", getpid());
	// Grandchild process
        for (int i = 0; i < 3; i++) {
            pid_t grandchild_pid = fork();
            if (grandchild_pid < 0) {
                // Fork failed
                fprintf(stderr, "Fork failed\n");
                return 1;
            } else if (grandchild_pid == 0) {
                printf("3. Grandchild process ID: %d\n", getpid());
                return 0;
            }
        }
    } else {
        // Parent process
        wait(NULL);
    }

    return 0;
}
