// src/SensorData.cpp
#include "../header/SensorData.h"
#include <ctime>
#include <iostream>

SensorData::SensorData(size_t max_sensors) {
    readings.resize(max_sensors);
    for (size_t i = 0; i < max_sensors; i++) {
        readings[i].id = static_cast<int>(i);
    }
}

std::string SensorData::getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buffer);
}

void SensorData::addReading(int id, double temperature) {
    std::lock_guard<std::mutex> lock(data_mutex);
    if (id < 0 || id >= static_cast<int>(readings.size())) {
        std::cerr << "SensorData: Invalid sensor ID: " << id << "\n";
        std::cerr.flush();
        return;
    }

    std::string timestamp = getCurrentTimestamp();
    readings[id].temperatures.push_back(temperature);
    readings[id].timestamps.push_back(timestamp);
    readings[id].valid.push_back(true);

    std::cout << "SensorData: Added reading - ID: " << id << ", Temp: " << temperature
              << ", Timestamp: " << timestamp << "\n";
    std::cout.flush();
}

SensorReading SensorData::getReading(int id) {
    std::lock_guard<std::mutex> lock(data_mutex);
    if (id < 0 || id >= static_cast<int>(readings.size())) {
        std::cerr << "SensorData: Invalid sensor ID: " << id << "\n";
        std::cerr.flush();
        return SensorReading();
    }
    return readings[id];
}

void SensorData::markProcessed(int id, size_t index) {
    std::lock_guard<std::mutex> lock(data_mutex);
    if (id < 0 || id >= static_cast<int>(readings.size())) {
        std::cerr << "SensorData: Invalid sensor ID: " << id << "\n";
        std::cerr.flush();
        return;
    }
    if (index >= readings[id].valid.size()) {
        std::cerr << "SensorData: Invalid index: " << index << " for sensor ID: " << id << "\n";
        std::cerr.flush();
        return;
    }
    readings[id].valid[index] = false;
}

size_t SensorData::getReadingCount(int id) {
    std::lock_guard<std::mutex> lock(data_mutex);
    if (id < 0 || id >= static_cast<int>(readings.size())) {
        std::cerr << "SensorData: Invalid sensor ID: " << id << "\n";
        std::cerr.flush();
        return 0;
    }
    return readings[id].temperatures.size();
}