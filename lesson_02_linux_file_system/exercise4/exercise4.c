#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

void print_file_type(mode_t mode)
{
    if (S_ISREG(mode))
        printf("File type: Regular file\n");
    else if (S_ISDIR(mode))
        printf("File type: Directory\n");
    else if (S_ISLNK(mode))
        printf("File type: Symbolic link\n");
    else
        printf("File type: Other\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct stat file_stat;
    if (stat(argv[1], &file_stat) == -1)
    {
        perror("Error getting file info");
        return 1;
    }

    printf("File name: %s\n", argv[1]);
    print_file_type(file_stat.st_mode);
    printf("File size: %ld bytes\n", file_stat.st_size);
    printf("Last modified time: %s", ctime(&file_stat.st_mtime));

    return 0;
}

// File name: test.txt
// File type: Regular file
// File size: 14 bytes
// Last modified time: Tue Feb 11 09:24:39 2025