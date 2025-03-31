// header/StorageManager.h
#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include "SensorData.h"

class StorageManager {
public:
    StorageManager(SensorData& sensor_data);
    void run();

private:
    SensorData& sensor_data;
};

#endif