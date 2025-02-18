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

int main(){
    signal(SIGTSTP, sig_handler);

    while(1)
    {
        printf("running!\n");
        sleep(1);
    }
    return 0;
}


// Câu hỏi: Điều gì xảy ra nếu không xử lý tín hiệu SIGTSTP và người dùng nhấn Ctrl+Z?

// -> chương trình sẽ bị tạm dừng (suspended).
// Tín hiệu SIGTSTP là tín hiệu được gửi đến một tiến trình khi người dùng nhấn tổ hợp phím Ctrl+Z trong terminal.
// Mặc định, khi nhận được tín hiệu này, tiến trình sẽ bị tạm dừng và có thể được tiếp tục bằng cách sử dụng lệnh
// fg (foreground) hoặc bg (background) trong terminal.

// Chương trình sẽ không bị tạm dừng mà thay vào đó sẽ in ra thông báo và tiếp tục chạy.

// Nếu bạn không có hàm xử lý tín hiệu SIGTSTP, chương trình sẽ bị tạm dừng khi người dùng nhấn Ctrl+Z.



