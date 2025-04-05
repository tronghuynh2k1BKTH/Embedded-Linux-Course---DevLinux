// header/StorageManager.h
#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include "SensorData.h"
#include <thread>

class StorageManager {
public:
    StorageManager(SensorData& sensor_data);
    void run();

private:
    SensorData& sensor_data;
    std::thread storage_thread;
};

#endif