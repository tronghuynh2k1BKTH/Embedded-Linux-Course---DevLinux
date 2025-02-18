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
