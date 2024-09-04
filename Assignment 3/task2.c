#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MAX_TEXT 6

//structure for the message
struct msg {
    long int type;
    char txt[MAX_TEXT];
};

int main() {
    pid_t pid;
    int msgid;
    struct msg message;
    
    // message queue
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }
    
    // Log in process
    printf("Please enter the workspace name: ");
    char workspace[MAX_TEXT];
    scanf("%s", workspace);

    if (strcmp(workspace, "cse321") != 0) {
        printf("Invalid workspace name\n");
        exit(EXIT_FAILURE);
    }

    // Send workspace name to OTP generator
    message.type = 1;
    strcpy(message.txt, workspace);
    if (msgsnd(msgid, (void *)&message, MAX_TEXT, 0) == -1) {
        perror("msgsnd failed");
        exit(EXIT_FAILURE);
    }
    printf("Workspace name sent to otp generator from log in: %s\n", message.txt);

    // Fork OTP generator process
    pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process (OTP generator)
        // Receive workspace name from log in
        if (msgrcv(msgid, (void *)&message, MAX_TEXT, 1, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        printf("OTP generator received workspace name from log in: %s\n", message.txt);

        // Generate OTP (using process id)
        int otp = getpid();
        sprintf(message.txt, "%d", otp);

        // Send OTP to log in
        message.type = 2;
        if (msgsnd(msgid, (void *)&message, MAX_TEXT, 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }
        printf("OTP sent to log in from OTP generator: %s\n", message.txt);

        // Send OTP to mail
        message.type = 3;
        if (msgsnd(msgid, (void *)&message, MAX_TEXT, 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }
        printf("OTP sent to mail from OTP generator: %s\n", message.txt);

        exit(EXIT_SUCCESS);
    } else { // Parent process (Log in)
        wait(NULL); // Wait for OTP generator to finish

        // Receive OTP from OTP generator
        if (msgrcv(msgid, (void *)&message, MAX_TEXT, 2, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        printf("Log in received OTP from OTP generator: %s\n", message.txt);

        // Receive OTP from mail
        if (msgrcv(msgid, (void *)&message, MAX_TEXT, 3, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        printf("Mail received OTP from OTP generator: %s\n", message.txt);

        // Send OTP to log in from mail
        message.type = 4;
        if (msgsnd(msgid, (void *)&message, MAX_TEXT, 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }
        printf("OTP sent to log in from mail: %s\n", message.txt);

        // Receive OTP from mail
        if (msgrcv(msgid, (void *)&message, MAX_TEXT, 4, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        printf("Log in received OTP from mail: %s\n", message.txt);

        // Verify OTP
        printf("OTP Verified\n");

        // Remove the message queue
        if (msgctl(msgid, IPC_RMID, 0) == -1) {
            perror("msgctl failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
