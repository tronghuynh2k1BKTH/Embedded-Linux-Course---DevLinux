#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int sigint_count = 0; // Biến đếm số lần nhận SIGINT

void handle_sigint(int sig) {
    sigint_count++;  
    printf("SIGINT đã nhận %d lần \n", sigint_count);

    if (sigint_count >= 3) {
        printf("Thoát tiến trình sau khi nhận signal interrupt 3 lần.\n");
        exit(0); // Kết thúc chương trình
    }
}

int main() {
    // Đăng ký xử lý tín hiệu SIGINT bằng signal()
    signal(SIGINT, handle_sigint);

    printf("Nhấn Ctrl+C để gửi SIGINT \n");

    // Vòng lặp vô hạn để giữ chương trình chạy
    while (1) {
        sleep(1); // Chờ tín hiệu
    }

    return 0;
}

// Trong Linux, việc nhấn tổ hợp phím Ctrl + C sẽ gửi tín hiệu SIGINT (Signal Interrupt)
// đến tiến trình đang chạy. Tín hiệu này sẽ yêu cầu tiến trình dừng hoạt động ngay lập tức.
// Đây là cách nhanh chóng và hiệu quả để kết thúc một tiến trình từ dòng lệnh mà không cần
// phải đóng toàn bộ terminal.

// Nếu một chương trình không đăng ký một trình xử lý tùy chỉnh cho tín hiệu SIGINT
// (ví dụ, không có hàm cụ thể để xử lý tín hiệu này), hệ điều hành sẽ sử dụng hành
// động mặc định cho tín hiệu đó. Với SIGINT, hành động mặc định là kết thúc tiến trình hiện tại.


-----
// Câu hỏi: Nếu bỏ qua tín hiệu SIGINT, chuyện gì sẽ xảy ra khi nhấn Ctrl+C?

// Để có thể bỏ qua tín hiệu SIGINT có thể đăng kí tín hiệu bỏ qua tín hiệu như sau
// signal(SIGINT, SIG_IGN);

// Khi đó khi nhấn Ctrl C thì chương trình sẽ không bị kết thúc mà vẫn tiếp tục chạy.
