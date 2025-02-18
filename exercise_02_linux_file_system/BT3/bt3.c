#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s filename num-bytes [r/w] \"message\"\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    int num_bytes = atoi(argv[2]);
    char *mode = argv[3];
    char *message = argv[4];

    // printf("Filename: %s\n", filename);
    // printf("Num bytes: %d\n", num_bytes);
    // printf("Mode: %s\n", mode);
    // printf("Message: %s\n", message);

    if (strcmp(mode, "r") == 0){
        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("Error opening file");
            return 1;
        }

        char buffer[num_bytes + 1];
        int bytes_read = read(fd, buffer, num_bytes);
        if (bytes_read == -1) {
            perror("Error reading file");
            close(fd);
            return 1;
        }

        buffer[bytes_read] = '\0';
        printf("Read from file: %s\n", buffer);
        close(fd);
    }
    else if (strcmp(mode, "w") == 0) {
        int fd = open(filename, O_WRONLY | O_TRUNC, 0644);
        if (fd == -1) {
            perror("Error opening file");
            return 1;
        }

        int bytes_written = write(fd, message, strlen(message));
        if (bytes_written == -1) {
            perror("Error writing to file");
            close(fd);
            return 1;
        }
        printf("Wrote to file: %s\n", message);
        close(fd);
    }
    else {
        fprintf(stderr, "Invalid mode\n");
        return 1;
    }

    return 0;
}