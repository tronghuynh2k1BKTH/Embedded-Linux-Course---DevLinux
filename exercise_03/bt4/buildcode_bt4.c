#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // Tạo tiến trình con

    if (pid == 0) {  // Tiến trình con
        printf("Child process running...\n");
        sleep(2);  // Giả lập tiến trình con chạy
        int exit_code = 42;  // Đặt mã thoát tùy chọn
        printf("Child process exiting with code %d\n", exit_code);
        exit(exit_code);  // Thoát với mã 42
    } 
    else if (pid > 0) {  // Tiến trình cha
        int status;
        printf("Parent waiting for child to exit...\n");
        wait(&status);  // Chờ tiến trình con kết thúc

        // Kiểm tra nếu tiến trình con kết thúc bình thường
        if (WIFEXITED(status)) {
            printf("Child exited normally with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child did not exit normally.\n");
        }
        
        printf("Parent process exiting.\n");
        exit(2);
    } 
    else {  // fork() thất bại
        perror("fork failed");
        return 1;
    }

    return 0;
}


// Mục đích của việc sử dụng exit() với mã thoát
// Khi một tiến trình kết thúc, nó có thể trả về mã thoát (exit status) để thông báo
// cho tiến trình cha hoặc hệ điều hành biết kết quả của quá trình thực thi. Việc này rất quan trọng vì:
// - Giúp tiến trình cha biết kết quả thực thi của tiến trình con.
// - Cho phép tiến trình cha xử lý các tình huống đặc biệt dựa trên mã thoát của tiến trình con.
// - Cho phép hệ điều hành biết kết quả của tiến trình con và thực hiện các hành động cần thiết
//     (ví dụ: giải phóng tài nguyên, ghi log, v.v.).