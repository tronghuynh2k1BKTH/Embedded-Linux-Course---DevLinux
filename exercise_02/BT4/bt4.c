#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

void print_file_type(mode_t mode) {
    if (S_ISREG(mode))
        printf("Loại file: Regular file\n");
    else if (S_ISDIR(mode))
        printf("Loại file: Directory\n");
    else if (S_ISLNK(mode))
        printf("Loại file: Symbolic link\n");
    else
        printf("Loại file: Other\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct stat file_stat;
    if (stat(argv[1], &file_stat) == -1) {
        perror("Error getting file info");
        return 1;
    }

    printf("Tên file: %s\n", argv[1]);
    print_file_type(file_stat.st_mode);
    printf("Kích thước file: %ld bytes\n", file_stat.st_size);
    printf("Thời gian chỉnh sửa cuối: %s", ctime(&file_stat.st_mtime));

    return 0;
}

// Tên file: test.txt
// Loại file: Regular file
// Kích thước file: 14 bytes
// Thời gian chỉnh sửa cuối: Tue Feb 11 09:24:39 2025