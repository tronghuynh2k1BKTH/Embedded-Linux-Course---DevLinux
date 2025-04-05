// src/DataManager.cpp
#include "../header/DataManager.h"
#include <iostream>
#include <unistd.h>
#include <numeric>

DataManager::DataManager(SensorData& sensor_data) : sensor_data(sensor_data) {}

void DataManager::run() {
    while (true) {
        // Giả sử chúng ta có tối đa 10 sensor (theo max_sensors trong Gateway)
        for (int id = 0; id < 10; id++) {
            SensorReading reading = sensor_data.getReading(id);
            size_t count = sensor_data.getReadingCount(id);

            if (count == 0) {
                continue; // Không có dữ liệu cho sensor này
            }

            // Tính trung bình nhiệt độ của các giá trị chưa được xử lý
            double sum = 0.0;
            size_t valid_count = 0;
            for (size_t i = 0; i < count; i++) {
                if (reading.valid[i]) {
                    sum += reading.temperatures[i];
                    valid_count++;
                }
            }

            if (valid_count == 0) {
                continue; // Không có dữ liệu chưa xử lý
            }

            double avg_temp = sum / valid_count;
            std::cout << "Data Manager: Sensor ID: " << id << ", Average Temp: " << avg_temp << "\n";
            std::cout.flush();

            // Đưa ra quyết định
            if (avg_temp > 30.0) {
                std::cout << "Data Manager: Sensor ID: " << id << " - Too hot\n";
                std::cout.flush();
            } else if (avg_temp < 10.0) {
                std::cout << "Data Manager: Sensor ID: " << id << " - Too cold\n";
                std::cout.flush();
            }

            // Đánh dấu các giá trị đã xử lý
            for (size_t i = 0; i < count; i++) {
                if (reading.valid[i]) {
                    sensor_data.markProcessed(id, i);
                }
            }
        }

        sleep(5); // Đợi 5 giây trước khi kiểm tra lại
    }
}