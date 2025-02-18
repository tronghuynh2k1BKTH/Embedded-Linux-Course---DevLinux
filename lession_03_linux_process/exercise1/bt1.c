#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork(); // Create a child process

    if (pid < 0) { // Check for error in process creation
        perror("Fork failed");
        return 1;
    } else if (pid == 0) { // Code for the child process
        printf("Child Process: PID = %d\n", getpid());
    } else { // Code for the parent process
        printf("Parent Process: Child PID = %d\n", getpid());
    }

    return 0;
}

// Both branches if (pid == 0) and else are executed because fork()
// creates a copy of the current process, meaning after calling fork(),
// there are two processes running the same code but with different pid values:
//  • Parent process: fork() returns the PID of the child process (a positive integer).
//  • Child process: fork() returns 0.