#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    // File name
    const char *filename = "test.txt";

    // Open file with O_APPEND flag
    int fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1)
    {
        perror("Error opening file");
        return 1;
    }

    // Move the file pointer to the beginning of the file
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("Error seeking");
    }

    // Write data to the file
    const char *data = "HELLO\n";
    write(fd, data, strlen(data));

    // Close the file
    close(fd);

    return 0;
}