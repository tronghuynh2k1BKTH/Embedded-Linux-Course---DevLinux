#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    // Tên file
    const char *filename = "test.txt";

    // Mở file với cờ O_APPEND
    int fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Lỗi mở file");
        return 1;
    }

    // Di chuyển con trỏ về đầu file
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Lỗi khi seek");
    }

    // Ghi dữ liệu vào file
    const char *data = "HELLO\n";
    write(fd, data, strlen(data));

    // Đóng file
    close(fd);

    return 0;
}