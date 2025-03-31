// src/DataManager.cpp
#include "../header/DataManager.h"
#include <iostream>
#include <unistd.h>

DataManager::DataManager(SensorData& sensor_data) : sensor_data(sensor_data) {}

void DataManager::run() {
    for (int i = 0; i < 5; i++) {
        std::cout << "Data Manager running, iteration " << i + 1 << "\n";
        std::cout.flush();
        sleep(1);
    }
    std::cout << "Data Manager exiting\n";
    std::cout.flush();
}