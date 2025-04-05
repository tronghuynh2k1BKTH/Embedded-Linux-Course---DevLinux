// src/Gateway.cpp
#include "../header/Gateway.h"
#include <iostream>
#include <unistd.h>

Gateway::Gateway(int port)
    : sensor_data(10), // Hỗ trợ tối đa 10 sensor
      conn_mgr(port, sensor_data), // Truyền port vào ConnectionManager
      data_mgr(sensor_data),
      storage_mgr(sensor_data) {
    // Create Log Process using fork() (Req 1)
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed!\n";
        std::cerr.flush();
        exit(1);
    } else if (pid == 0) {
        // Child process (Log Process)
        std::cout << "Log Process started (PID: " << getpid() << ")\n";
        std::cout.flush();
        for (int i = 0; i < 5; i++) {
            std::cout << "Log Process running, iteration " << i + 1 << "\n";
            std::cout.flush();
            sleep(1);
        }
        std::cout << "Log Process exiting\n";
        std::cout.flush();
        exit(0);
    } else {
        // Parent process (Main Process)
        std::cout << "Main Process continues (PID: " << getpid() << "), Log Process PID: " << pid << "\n";
        std::cout.flush();
    }
}

void Gateway::run() {
    conn_thread = std::thread(&ConnectionManager::run, &conn_mgr);
    data_thread = std::thread(&DataManager::run, &data_mgr);
    storage_thread = std::thread(&StorageManager::run, &storage_mgr);

    conn_thread.join();
    data_thread.join();
    storage_thread.join();

    std::cout << "All threads finished, Main Process exiting\n";
    std::cout.flush();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>\n";
        return 1;
    }

    int port = std::stoi(argv[1]); // Lấy port từ command-line
    std::cout << "Starting Main Process (PID: " << getpid() << ") on port " << port << "\n";
    std::cout.flush();

    Gateway gateway(port);
    gateway.run();

    return 0;
}