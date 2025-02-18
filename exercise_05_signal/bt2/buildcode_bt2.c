#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int timer_count = 0; // Biến đếm thời gian

void handle_alarm(int sig) {
    timer_count++;  
    printf("Timer: %d seconds\n", timer_count);

    if (timer_count >= 15) {
        printf("Timer reached 10 seconds. Exiting program.\n");
        exit(0); // Kết thúc chương trình
    }

    alarm(1); // Đặt lại báo động sau 1 giây
}

int main() {
    // Đăng ký xử lý tín hiệu SIGALRM
    signal(SIGALRM, handle_alarm);

    printf("Starting timer...\n");
    
    alarm(1); // Kích hoạt SIGALRM sau 1 giây

    // Giữ chương trình chạy liên tục
    while (1) {
        pause(); // Chờ tín hiệu SIGALRM
    }

    return 0;
}


// Câu hỏi: Điều gì xảy ra nếu không gọi lại alarm(1) trong hàm xử lý?

// -> hệ thống sẽ treo và nằm trong while (1) vì không có điều kiện tự kết thúc
// chương trình, trừ khi có những tín hiệu khác gửi đến chương trình.