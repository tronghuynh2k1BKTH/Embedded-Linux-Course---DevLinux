#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 100

int main() {
    int pipefd[2]; // pipefd[0] - read, pipefd[1] - write
    pid_t pid;
    char message[] = "Hello from Parent!";
    char buffer[BUFFER_SIZE];

    // Create pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    // Fork child process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0) { // Parent process
        close(pipefd[0]); // Close read end
        write(pipefd[1], message, strlen(message) + 1); // Send message
        close(pipefd[1]); // Close write end
    } else { // Child process
        close(pipefd[1]); // Close write end
        read(pipefd[0], buffer, BUFFER_SIZE); // Read data from pipe
        printf("Child received: %s\n", buffer); // Print to screen
        close(pipefd[0]); // Close read end
    }

    return 0;
}