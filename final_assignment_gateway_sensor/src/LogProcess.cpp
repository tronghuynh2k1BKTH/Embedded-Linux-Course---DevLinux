#include "../header/LogProcess.h"
#include <fcntl.h>
#include <unistd.h>
#include <fstream>

LogProcess::LogProcess() {}

void LogProcess::run() {
    int fd = open("logFIFO", O_RDONLY);
    if (fd < 0) {
        return;
    }

    std::ofstream log_file("gateway.log", std::ios::app);
    char buffer[256];

    while (true) {
        int n = read(fd, buffer, 255);
        if (n > 0) {
            buffer[n] = '\0';
            log_file << buffer;
            log_file.flush();
        }
    }

    log_file.close();
    close(fd);
}
