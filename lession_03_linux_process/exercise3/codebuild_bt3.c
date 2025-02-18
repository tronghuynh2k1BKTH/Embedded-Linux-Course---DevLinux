#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// Signal handler function in the child process
void handle_signal(int sig) {
    if (sig == SIGUSR1) {
        printf("Child process received SIGUSR1 signal!\n");
    }
}

int main() {
    pid_t pid = fork(); // Create a child process

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) { // Child process code
        signal(SIGUSR1, handle_signal); // Define behavior when receiving SIGUSR1
        printf("Child process waiting for SIGUSR1...\n");
        pause(); // Wait for signal from parent process
    } 
    else { // Parent process code
        sleep(2); // Wait 2 seconds before sending the signal
        printf("Parent process sending SIGUSR1 to child...\n");
        kill(pid, SIGUSR1); // Send SIGUSR1 signal to child process
        wait(NULL); // Wait for child process to finish
    }

    return 0;
}

// gcc codebuild_bt3.c -o codebuild_bt3
// ./codebuild_bt3
// Output:
// Parent process sending SIGUSR1 to child...
// Child process received SIGUSR1 signal!