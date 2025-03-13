#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int count = 0;

void handle_sigusr1(int sig)
{
    if (sig == SIGUSR1)
    {
        count++;
        printf("Received signal from parent, %d times\n", count);
        if (count >= 5)
        {
            exit(0);
        }
    }
}

int main()
{
    signal(SIGUSR1, handle_sigusr1);

    pid_t pid_child = fork();
    if (pid_child == 0)
    {
        printf("I am child, PID: %d\n", getpid());
        while (1)
        {
            pause();
        }
    }
    else if (pid_child > 0)
    {
        printf("I am parent, PID: %d\n", getpid());
        int parent_count = 0;
        // Send signal to child every 2 seconds
        while (parent_count < 5)
        {
            sleep(2);
            kill(pid_child, SIGUSR1);
            parent_count++;
        }
        printf("Parent sent 5 signals, exiting...\n");
        exit(0);
    }
    else if (pid_child < 0)
    {
        printf("Error\n");
        exit(1);
    }
    return 0;
}

// gcc -o buildcode_bt3 buildcode_bt3.c
// ./buildcode_bt3
