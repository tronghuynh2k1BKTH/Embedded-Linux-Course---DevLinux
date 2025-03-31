// header/SensorData.h
#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <string>
#include <vector>
#include <mutex>

struct SensorReading {
    int id;              // Sensor ID
    double temperature;  // Temperature value
    std::string timestamp; // Timestamp of the reading
    bool valid;          // Flag to indicate if the data is new/unprocessedl, with true meaning new/unprocessed
    SensorReading() : id(-1), temperature(0.0), timestamp(""), valid(false) {}
};

class SensorData {
public:
    SensorData(size_t max_sensors); // Constructor: max_sensors là số lượng sensor tối đa
    void addReading(int id, double temperature); // Thêm dữ liệu cảm biến
    SensorReading getReading(int id); // Lấy dữ liệu cảm biến
    void markProcessed(int id); // Đánh dấu dữ liệu đã được xử lý

private:
    std::vector<SensorReading> readings; // Vector lưu trữ dữ liệu cảm biến
    std::mutex data_mutex; // Mutex để đảm bảo thread-safe
    std::string getCurrentTimestamp(); // Helper để lấy timestamp
};

#endif