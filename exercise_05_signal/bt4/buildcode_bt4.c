#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signo)
{
    if (signo == SIGTSTP)
    {
        printf("SIGTSTP ignored\n");
    }
}

int main()
{
    signal(SIGTSTP, sig_handler);

    while (1)
    {
        printf("running!\n");
        sleep(1);
    }
    return 0;
}
