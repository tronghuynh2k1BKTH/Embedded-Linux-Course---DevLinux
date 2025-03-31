// src/StorageManager.cpp
#include "../header/StorageManager.h"
#include <iostream>
#include <unistd.h>

StorageManager::StorageManager(SensorData& sensor_data) : sensor_data(sensor_data) {}

void StorageManager::run() {
    for (int i = 0; i < 5; i++) {
        std::cout << "Storage Manager running, iteration " << i + 1 << "\n";
        std::cout.flush();
        sleep(1);
    }
    std::cout << "Storage Manager exiting\n";
    std::cout.flush();
}