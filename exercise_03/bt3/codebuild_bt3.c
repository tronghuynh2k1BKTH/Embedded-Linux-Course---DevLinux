#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// Hàm xử lý tín hiệu trong tiến trình con
void handle_signal(int sig) {
    if (sig == SIGUSR1) {
        printf("Child process received SIGUSR1 signal!\n");
    }
}

int main() {
    pid_t pid = fork(); // Tạo tiến trình con

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) { // Code của tiến trình con
        signal(SIGUSR1, handle_signal); // Định nghĩa hành vi khi nhận SIGUSR1
        printf("Child process waiting for SIGUSR1...\n");
        pause(); // Chờ tín hiệu từ tiến trình cha
    } 
    else { // Code của tiến trình cha
        sleep(2); // Đợi 2 giây trước khi gửi tín hiệu
        printf("Parent process sending SIGUSR1 to child...\n");
        kill(pid, SIGUSR1); // Gửi tín hiệu SIGUSR1 đến tiến trình con
        wait(NULL); // Đợi tiến trình con kết thúc
    }

    return 0;
}

// gcc codebuild_bt3.c -o codebuild_bt3
// ./codebuild_bt3
// Kết quả:
// Parent process sending SIGUSR1 to child...
// Child process received SIGUSR1 signal!