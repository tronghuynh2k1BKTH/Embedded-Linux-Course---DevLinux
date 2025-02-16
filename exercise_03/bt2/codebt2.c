#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> // Include this header for wait function

int main() {
    pid_t pid = fork(); // Tạo process con

    if (pid < 0) { // Kiểm tra lỗi fork
        perror("fork failed");
        return 1;
    }

    if (pid == 0) { // Tiến trình con
        char *env_var = getenv("EXEC_MODE"); // Lấy giá trị biến môi trường

        if (env_var == NULL) {
            printf("Biến môi trường EXEC_MODE chưa được thiết lập.\n");
            return 1;
        }

        int mode = atoi(env_var); // Chuyển đổi sang số nguyên
        if (mode == 1) {
            execlp("ls", "ls", "-l", NULL);
        } else if (mode == 2) {
            execlp("date", "date", NULL);
        } else {
            printf("Giá trị EXEC_MODE không hợp lệ (chỉ chấp nhận 1 hoặc 2).\n");
            return 1;
        }

        perror("exec failed"); // Nếu exec thất bại
    } else { // Tiến trình cha
        wait(NULL); // Đợi tiến trình con kết thúc
        printf("Tiến trình cha kết thúc.\n");
    }

    return 0;
}

// Điều gì xảy ra sau khi exec được gọi?
// •	Khi execlp() hoặc execvp() được gọi, chương trình cũ bị thay thế
//     hoàn toàn bằng chương trình mới (ls hoặc date).
// •	Tiến trình con không quay lại thực thi dòng lệnh tiếp theo trong
//     chương trình C, vì exec đã thay đổi toàn bộ nội dung của tiến trình.
// •	Nếu exec thất bại (ví dụ, gọi một chương trình không tồn tại),
//     tiến trình con sẽ in lỗi và tiếp tục thực thi.

// export EXEC_MODE=1
// ./codebt2 # Chạy lệnh ls -l
// export EXEC_MODE=2
// ./codebt2 # Chạy lệnh date
// export EXEC_MODE=3
// ./codebt2 # In ra lỗi vì giá trị EXEC_MODE không hợp lệ