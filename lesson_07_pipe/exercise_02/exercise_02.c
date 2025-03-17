#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
    int pipe1[2], pipe2[2]; // pipe1: Parent -> Child1, pipe2: Child1 -> Child2
    pid_t pid1, pid2;
    char message[] = "Hello from Parent!";
    char buffer[BUFFER_SIZE];

    // Create 2 pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    // Fork first child process
    pid1 = fork();
    if (pid1 < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid1 == 0) { // First child process
        close(pipe1[1]); // Close write end of pipe1
        read(pipe1[0], buffer, BUFFER_SIZE); // Receive data from parent
        close(pipe1[0]); // Close read end of pipe1

        strcat(buffer, " - Modified by Child 1"); // Append message

        close(pipe2[0]); // Close read end of pipe2
        write(pipe2[1], buffer, strlen(buffer) + 1); // Send data to child 2
        close(pipe2[1]); // Close write end of pipe2

        exit(0);
    }

    // Fork second child process
    pid2 = fork();
    if (pid2 < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid2 == 0) { // Second child process
        close(pipe2[1]); // Close write end of pipe2
        read(pipe2[0], buffer, BUFFER_SIZE); // Receive data from child 1
        close(pipe2[0]); // Close read end of pipe2

        printf("Child 2 received: %s\n", buffer); // Print message

        exit(0);
    }

    // Parent process
    close(pipe1[0]); // Close read end of pipe1
    write(pipe1[1], message, strlen(message) + 1); // Send data to child 1
    close(pipe1[1]); // Close write end of pipe1

    // Wait for child processes to complete
    wait(NULL);
    wait(NULL);

    return 0;
}