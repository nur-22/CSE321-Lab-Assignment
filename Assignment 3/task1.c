#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_SIZE sizeof(struct shared)
#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

struct shared {
    char sel[100];
    int b;
};

int main() {
    int shmid, *shm_ptr;
    key_t key = IPC_PRIVATE;
    struct shared *shm_struct;

    // Removing any existing shared memory segment
    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid != -1) {
        shmctl(shmid, IPC_RMID, NULL);
    }

    // Creating shared memory
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Attaching shared memory
    if ((shm_ptr = shmat(shmid, NULL, 0)) == (int *) -1) {
        perror("shmat");
        exit(1);
    }

    shm_struct = (struct shared *)shm_ptr;

    // Creating pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process (opr)
        close(pipefd[PIPE_WRITE_END]);

        char buffer[100];
        read(pipefd[PIPE_READ_END], buffer, sizeof(buffer));
        printf("Your selection: %s\n", buffer);

        // Read current balance from shared memory
        int current_balance = shm_struct->b;

        if (strcmp(buffer, "a") == 0) {
            printf("Enter amount to be added:\n");
            int amount;
            scanf("%d", &amount);
            if (amount > 0) {
                current_balance += amount;
                shm_struct->b = current_balance;
                printf("Balance added successfully\nUpdated balance after addition:\n%d\n", current_balance);
            } else {
                printf("Adding failed, Invalid amount\n");
            }
        } else if (strcmp(buffer, "w") == 0) {
            printf("Enter amount to be withdrawn:\n");
            int amount;
            scanf("%d", &amount);
            if (amount > 0 && amount <= current_balance) {
                current_balance -= amount;
                shm_struct->b = current_balance;
                printf("Balance withdrawn successfully\nUpdated balance after withdrawal:\n%d\n", current_balance);
            } else {
                printf("Withdrawal failed, Invalid amount\n");
            }
        } else if (strcmp(buffer, "c") == 0) {
            printf("Your current balance is:\n%d\n", current_balance);
        } else {
            printf("Invalid selection\n");
        }

        close(pipefd[PIPE_READ_END]);
        write(pipefd[PIPE_WRITE_END], "Thank you for using\n", strlen("Thank you for using\n") + 1);
        close(pipefd[PIPE_WRITE_END]);
        exit(EXIT_SUCCESS);
    } else { // Parent process (home)
        close(pipefd[PIPE_READ_END]);

        printf("Provide Your Input From Given Options:\n1. Type a to Add Money\n2. Type w to Withdraw Money\n3. Type c to Check Balance\n");
        char selection[100];
        scanf("%s", selection);
        printf("Your selection: %s\n", selection);

        strcpy(shm_struct->sel, selection);
        shm_struct->b = 1000;

        write(pipefd[PIPE_WRITE_END], selection, strlen(selection) + 1);
        close(pipefd[PIPE_WRITE_END]);

        wait(NULL); // Wait for child process to finish

        char buffer[100];
        read(pipefd[PIPE_READ_END], buffer, sizeof(buffer));
        printf("%s", buffer);

        close(pipefd[PIPE_READ_END]);

        printf("Execution completed.\n");

        // Detach and remove shared memory
        shmdt(shm_ptr);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
