// src/StorageManager.cpp
#include "../header/StorageManager.h"
#include <iostream>
#include <sqlite3.h>
#include <unistd.h>

StorageManager::StorageManager(SensorData& sensor_data) : sensor_data(sensor_data) {}

static int callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    std::cout << "\n";
    return 0;
}

void StorageManager::run() {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc = sqlite3_open("sensor_data.db", &db);
    if (rc) {
        std::cerr << "Storage Manager: Cannot open database: " << sqlite3_errmsg(db) << "\n";
        std::cerr.flush();
        sqlite3_close(db);
        return;
    }

    const char* sql_create_table = 
        "CREATE TABLE IF NOT EXISTS sensor_readings ("
        "sensor_id INTEGER NOT NULL, "
        "temperature REAL NOT NULL, "
        "timestamp TEXT NOT NULL);";
    
    rc = sqlite3_exec(db, sql_create_table, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Storage Manager: SQL error (create table): " << zErrMsg << "\n";
        std::cerr.flush();
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return;
    }

    std::cout << "Storage Manager: Database initialized\n";
    std::cout.flush();

    while (true) {
        for (int id = 0; id < 10; id++) {
            SensorReading reading = sensor_data.getReading(id);
            size_t count = sensor_data.getReadingCount(id);

            for (size_t i = 0; i < count; i++) {
                if (!reading.valid[i]) {
                    continue;
                }

                std::string sql_insert = "INSERT INTO sensor_readings (sensor_id, temperature, timestamp) VALUES ("
                    + std::to_string(id) + ", "
                    + std::to_string(reading.temperatures[i]) + ", '"
                    + reading.timestamps[i] + "');";

                rc = sqlite3_exec(db, sql_insert.c_str(), callback, 0, &zErrMsg);
                if (rc != SQLITE_OK) {
                    std::cerr << "Storage Manager: SQL error (insert): " << zErrMsg << "\n";
                    std::cerr.flush();
                    sqlite3_free(zErrMsg);
                } else {
                    std::cout << "Storage Manager: Stored - ID: " << id
                              << ", Temp: " << reading.temperatures[i]
                              << ", Timestamp: " << reading.timestamps[i] << "\n";
                    std::cout.flush();
                    sensor_data.markProcessed(id, i);
                }
            }
        }

        sleep(5);
    }

    sqlite3_close(db);
}