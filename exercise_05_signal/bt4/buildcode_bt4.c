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

// Question: What happens if the SIGTSTP signal is not handled and the user presses Ctrl+Z?

// -> The program will be suspended.
// The SIGTSTP signal is sent to a process when the user presses the Ctrl+Z key combination in the terminal.
// By default, when this signal is received, the process will be suspended and can be resumed using the
// fg (foreground) or bg (background) command in the terminal.

// The program will not be suspended but will instead print a message and continue running.

// If you do not have a handler for the SIGTSTP signal, the program will be suspended when the user presses Ctrl+Z.
