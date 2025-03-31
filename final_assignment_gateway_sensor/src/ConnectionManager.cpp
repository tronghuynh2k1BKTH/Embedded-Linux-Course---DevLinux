// src/ConnectionManager.cpp
#include "../header/ConnectionManager.h"
#include <iostream>
#include <unistd.h>

ConnectionManager::ConnectionManager(SensorData& sensor_data) : sensor_data(sensor_data) {}

void ConnectionManager::run() {
    for (int i = 0; i < 5; i++) {
        std::cout << "Connection Manager running, iteration " << i + 1 << "\n";
        std::cout.flush();
        sleep(1);
    }
    std::cout << "Connection Manager exiting\n";
    std::cout.flush();
}