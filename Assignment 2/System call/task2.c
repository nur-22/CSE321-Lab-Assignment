#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid < 0) {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (child_pid == 0) {
        // Child process
        pid_t grandchild_pid = fork();

        if (grandchild_pid < 0) {
            // Fork failed
            fprintf(stderr, "Fork failed\n");
            return 1;
        } else if (grandchild_pid == 0) {
            // Grandchild process
            printf("I am grandchild\n");
        } else {
            // Child process waits for grandchild
            wait(NULL);
            printf("I am child\n");
        }
    } else {
        // Parent process waits for child
        wait(NULL);
        printf("I am parent\n");
    }

    return 0;
}
