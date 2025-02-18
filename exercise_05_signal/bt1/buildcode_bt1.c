#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int sigint_count = 0; // Variable to count the number of SIGINT received

void handle_sigint(int sig) {
    sigint_count++;
    printf("SIGINT received %d times\n", sigint_count);

    if (sigint_count >= 3) {
        printf("Exiting process after receiving SIGINT 3 times.\n");
        exit(0); // Terminate the program
    }
}

int main() {
    // Register the SIGINT signal handler using signal()
    signal(SIGINT, handle_sigint);

    printf("Press Ctrl+C to send SIGINT\n");

    // Infinite loop to keep the program running
    while (1) {
        sleep(1); // Wait for signal
    }

    return 0;
}

// In Linux, pressing the Ctrl+C key combination sends the SIGINT (Signal Interrupt)
// signal to the running process. This signal requests the process to stop immediately.
// This is a quick and effective way to terminate a process from the command line without
// having to close the entire terminal.

// If a program does not register a custom handler for the SIGINT signal
// (e.g., does not have a specific function to handle this signal), the operating system
// will use the default action for that signal. For SIGINT, the default action is to
// terminate the current process.

-----
// Question: What happens if the SIGINT signal is ignored?

// To ignore the SIGINT signal, you can register to ignore the signal as follows:
// signal(SIGINT, SIG_IGN);

// In this case, when Ctrl+C is pressed, the program will not be terminated and will continue running.
