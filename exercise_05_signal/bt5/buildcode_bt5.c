#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <termios.h>

// Signal handler for SIGINT
void sigint_handler(int signo) {
    printf("SIGINT received. But no Exit.\n");
}

// Signal handler for SIGTERM
void sigterm_handler(int signo) {
    printf("SIGTERM received. Exiting.\n");
    exit(0);
}

int main() {
    // Set up signal handling
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigterm_handler);

    printf("Program started. my PID: %d. Press Ctrl+C to send SIGINT, Ctrl+\\ to send SIGTERM.\n", getpid());

    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        // Use select to wait for signal or keyboard input
        int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, NULL);

        if (ret == -1) {
            perror("select");
            exit(1);
        } else if (ret > 0) {
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                // Read data from keyboard
                char c[100];
                scanf("%s", c);
                printf("You entered: %s\n", c);
            }
        }
    }
    return 0;
}
