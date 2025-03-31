// header/ConnectionManager.h
#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include "SensorData.h"

class ConnectionManager {
public:
    ConnectionManager(SensorData& sensor_data);
    void run();

private:
    SensorData& sensor_data;
};

#endif