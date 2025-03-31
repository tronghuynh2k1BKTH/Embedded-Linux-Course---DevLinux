// src/Gateway.cpp
#include "../header/Gateway.h"
#include <iostream>
#include <unistd.h>

Gateway::Gateway()
    : sensor_data(10), // Hỗ trợ tối đa 10 sensor
      conn_mgr(sensor_data),
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
    // Create 3 threads for the managers
    conn_thread = std::thread(&ConnectionManager::run, &conn_mgr);
    data_thread = std::thread(&DataManager::run, &data_mgr);
    storage_thread = std::thread(&StorageManager::run, &storage_mgr);

    // Wait for threads to finish
    conn_thread.join();
    data_thread.join();
    storage_thread.join();

    std::cout << "All threads finished, Main Process exiting\n";
    std::cout.flush();
}

int main() {
    std::cout << "Starting Main Process (PID: " << getpid() << ")\n";
    std::cout.flush();

    Gateway gateway;
    gateway.run();

    return 0;
}