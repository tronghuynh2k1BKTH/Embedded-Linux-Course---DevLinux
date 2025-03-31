// header/Gateway.h
#ifndef GATEWAY_H
#define GATEWAY_H

#include "SensorData.h"
#include "ConnectionManager.h"
#include "DataManager.h"
#include "StorageManager.h"
#include <thread>

class Gateway {
public:
    Gateway();
    void run();

private:
    // SensorData object to manage sensor data
    SensorData sensor_data;
    ConnectionManager conn_mgr;
    DataManager data_mgr;
    StorageManager storage_mgr;
    std::thread conn_thread, data_thread, storage_thread;
};

#endif