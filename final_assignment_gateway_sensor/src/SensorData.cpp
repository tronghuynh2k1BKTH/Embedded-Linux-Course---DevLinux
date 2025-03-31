// src/SensorData.cpp
#include "../header/SensorData.h"
#include <ctime>

SensorData::SensorData(size_t max_sensors) {
    readings.resize(max_sensors); // Khởi tạo vector với kích thước max_sensors
}

std::string SensorData::getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buffer);
}

void SensorData::addReading(int id, double temperature) {
    std::lock_guard<std::mutex> lock(data_mutex); // Khóa mutex
    if (id >= 0 && id < static_cast<int>(readings.size())) {
        readings[id].id = id;
        readings[id].temperature = temperature;
        readings[id].timestamp = getCurrentTimestamp();
        readings[id].valid = true;
    }
}

SensorReading SensorData::getReading(int id) {
    std::lock_guard<std::mutex> lock(data_mutex); // Khóa mutex
    if (id >= 0 && id < static_cast<int>(readings.size())) {
        return readings[id];
    }
    return SensorReading(); // Trả về giá trị mặc định nếu ID không hợp lệ
}

void SensorData::markProcessed(int id) {
    std::lock_guard<std::mutex> lock(data_mutex); // Khóa mutex
    if (id >= 0 && id < static_cast<int>(readings.size())) {
        readings[id].valid = false;
    }
}