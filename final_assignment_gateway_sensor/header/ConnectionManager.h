// header/ConnectionManager.h
#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include "SensorData.h"

class ConnectionManager {
public:
    ConnectionManager(int port, SensorData& sensor_data); // Thêm port
    void run();

private:
    int port;
    SensorData& sensor_data;
};

#endif