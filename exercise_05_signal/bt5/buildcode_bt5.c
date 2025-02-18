#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <termios.h>

// Hàm xử lý tín hiệu SIGINT
void sigint_handler(int signo) {
    printf("SIGINT received. But no Exit.\n");
}


// Hàm xử lý tín hiệu SIGTERM
void sigterm_handler(int signo) {
    printf("SIGTERM received. Exiting.\n");
    exit(0);
}

int main() {
    // Thiết lập xử lý tín hiệu
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigterm_handler);

    printf("Program started. my PID: %d. Press Ctrl+C to send SIGINT, Ctrl+\\ to send SIGTERM.\n", getpid());

    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        // Sử dụng select để chờ tín hiệu hoặc dữ liệu nhập từ bàn phím
        int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, NULL);

        if (ret == -1) {
            perror("select");
            exit(1);
        } else if (ret > 0) {
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                // Đọc dữ liệu từ bàn phím
                char c[100];
                scanf("%s", c);
                printf("You entered: %s\n", c);
            }
        }
    }
    return 0;
}