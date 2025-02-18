#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // Create a child process

    if (pid == 0) {  // Child process
        printf("Child process running...\n");
        sleep(2);  // Simulate child process running
        int exit_code = 42;  // Set custom exit code
        printf("Child process exiting with code %d\n", exit_code);
        exit(exit_code);  // Exit with code 42
    } 
    else if (pid > 0) {  // Parent process
        int status;
        printf("Parent waiting for child to exit...\n");
        wait(&status);  // Wait for child process to finish

        // Check if the child process exited normally
        if (WIFEXITED(status)) {
            printf("Child exited normally with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child did not exit normally.\n");
        }
        
        printf("Parent process exiting.\n");
        exit(2);
    } 
    else {  // fork() failed
        perror("fork failed");
        return 1;
    }

    return 0;
}

// Purpose of using exit() with exit code
// When a process terminates, it can return an exit code (exit status) to inform
// the parent process or the operating system about the result of its execution. This is important because:
// - It helps the parent process know the execution result of the child process.
// - It allows the parent process to handle special situations based on the child's exit code.
// - It allows the operating system to know the result of the child process and take necessary actions
//   (e.g., freeing resources, logging, etc.).
