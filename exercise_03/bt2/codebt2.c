#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> // Include this header for wait function

int main()
{
    pid_t pid = fork(); // Create child process

    if (pid < 0)
    { // Check for fork error
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {                                        // Child process
        char *env_var = getenv("EXEC_MODE"); // Get the value of the environment variable

        if (env_var == NULL)
        {
            printf("The environment variable EXEC_MODE is not set.\n");
            return 1;
        }

        int mode = atoi(env_var); // Convert to integer
        if (mode == 1)
        {
            execlp("ls", "ls", "-l", NULL);
        }
        else if (mode == 2)
        {
            execlp("date", "date", NULL);
        }
        else
        {
            printf("Invalid value for EXEC_MODE (only 1 or 2 are accepted).\n");
            return 1;
        }

        perror("exec failed"); // If exec fails
    }
    else
    {               // Parent process
        wait(NULL); // Wait for the child process to finish
        printf("Parent process finished.\n");
    }

    return 0;
}

// What happens after exec is called?
// • When execlp() or execvp() is called, the old program is completely replaced
//   by the new program (ls or date).
// • The child process does not return to execute the next line of code in
//   the C program, because exec has changed the entire content of the process.
// • If exec fails (e.g., calling a non-existent program), the child process
//   will print an error and continue executing.

// export EXEC_MODE=1
// ./codebt2 # Run the command ls -l
// export EXEC_MODE=2
// ./codebt2 # Run the command date
// export EXEC_MODE=3
// ./codebt2 # Print an error because the value of EXEC_MODE is invalid