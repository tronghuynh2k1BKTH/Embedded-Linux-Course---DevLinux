#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int timer_count = 0; // Timer count variable

void handle_alarm(int sig) {
    timer_count++;  
    printf("Timer: %d seconds\n", timer_count);

    if (timer_count >= 15) {
        printf("Timer reached 15 seconds. Exiting program.\n");
        exit(0); // End the program
    }

    alarm(1); // Reset the alarm after 1 second
}

int main() {
    // Register signal handler for SIGALRM
    signal(SIGALRM, handle_alarm);

    printf("Starting timer...\n");
    
    alarm(1); // Trigger SIGALRM after 1 second

    // Keep the program running continuously
    while (1) {
        pause(); // Wait for SIGALRM signal
    }

    return 0;
}

