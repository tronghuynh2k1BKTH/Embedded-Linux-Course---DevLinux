// header/DataManager.h
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "SensorData.h"

class DataManager {
public:
    DataManager(SensorData& sensor_data);
    void run();

private:
    SensorData& sensor_data;
};

#endif