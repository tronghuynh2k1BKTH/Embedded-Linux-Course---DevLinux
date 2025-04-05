// header/SensorData.h
#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <string>
#include <vector>
#include <mutex>

struct SensorReading {
    int id;                        // Sensor ID
    std::vector<double> temperatures; // Danh sách các giá trị nhiệt độ
    std::vector<std::string> timestamps; // Danh sách timestamp tương ứng
    std::vector<bool> valid;       // Trạng thái valid cho từng giá trị
    SensorReading() : id(-1) {}
};

class SensorData {
public:
    SensorData(size_t max_sensors); // Constructor: max_sensors là số lượng sensor tối đa
    void addReading(int id, double temperature); // Thêm dữ liệu cảm biến
    SensorReading getReading(int id); // Lấy dữ liệu cảm biến
    void markProcessed(int id, size_t index); // Đánh dấu một giá trị cụ thể đã được xử lý
    size_t getReadingCount(int id); // Lấy số lượng giá trị nhiệt độ của sensor

private:
    std::vector<SensorReading> readings; // Vector lưu trữ dữ liệu cảm biến
    std::mutex data_mutex; // Mutex để đảm bảo thread-safe
    std::string getCurrentTimestamp(); // Helper để lấy timestamp
};

#endif