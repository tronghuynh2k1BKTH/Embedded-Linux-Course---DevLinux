#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
    int pipefd[2]; // Pipe file descriptors
    pid_t pid;
    char message[] = "Hello, this is a test message!";
    char buffer[BUFFER_SIZE];

    // Create pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    // Fork child process
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) { // Child process
        close(pipefd[1]); // Close write end

        read(pipefd[0], buffer, BUFFER_SIZE); // Read data from pipe
        close(pipefd[0]); // Close read end

        int length = strlen(buffer); // Count characters
        printf("Child Process: Received string: \"%s\"\n", buffer);
        printf("Child Process: Character count = %d\n", length);

        exit(0);
    } else { // Parent process
        close(pipefd[0]); // Close read end

        write(pipefd[1], message, strlen(message) + 1); // Send data
        close(pipefd[1]); // Close write end

        wait(NULL); // Wait for child process to finish
    }

    return 0;
}