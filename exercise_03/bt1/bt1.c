#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork(); // Tạo process con

    if (pid < 0) { // Kiểm tra lỗi khi tạo process
        perror("Fork failed");
        return 1;
    } else if (pid == 0) { // Code của process con
        printf("Child Process: PID = %d\n", getpid());
    } else { // Code của process cha
        printf("Parent Process: Child PID = %d\n", getpid());
    }

    return 0;
}

// cả hai nhánh if (pid == 0) và else đều được thực thi là vì fork()
// tạo ra một bản sao của process hiện tại, tức là sau khi gọi fork(),
// có hai process đang chạy cùng một đoạn code nhưng với giá trị pid khác nhau:
// 	• Process cha (Parent process): fork() trả về PID của process con (một số nguyên dương).
// 	• Process con (Child process): fork() trả về 0.