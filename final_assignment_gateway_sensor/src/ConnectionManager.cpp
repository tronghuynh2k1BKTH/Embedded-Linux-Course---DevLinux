// src/ConnectionManager.cpp
#include "../header/ConnectionManager.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

ConnectionManager::ConnectionManager(int port, SensorData& sensor_data)
    : port(port), sensor_data(sensor_data) {}

void ConnectionManager::run() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Connection Manager: Failed to create socket\n";
        std::cerr.flush();
        return;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Connection Manager: Failed to set socket options\n";
        std::cerr.flush();
        close(server_fd);
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection Manager: Failed to bind to port " << port << "\n";
        std::cerr.flush();
        close(server_fd);
        return;
    }

    if (listen(server_fd, 5) < 0) {
        std::cerr << "Connection Manager: Failed to listen\n";
        std::cerr.flush();
        close(server_fd);
        return;
    }

    std::cout << "Connection Manager: Listening on port " << port << "\n";
    std::cout.flush();

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            std::cerr << "Connection Manager: Failed to accept connection\n";
            std::cerr.flush();
            continue;
        }

        std::cout << "Connection Manager: New connection accepted\n";
        std::cout.flush();

        while (true) {
            char buffer[256];
            int n = read(client_fd, buffer, 255);
            if (n <= 0) {
                std::cout << "Connection Manager: Connection closed by client\n";
                std::cout.flush();
                break;
            }
            buffer[n] = '\0';

            std::string data(buffer);
            std::cout << "Connection Manager: Received data: " << data << "\n";
            std::cout.flush();

            int id = -1;
            double temp = 0.0;
            std::stringstream ss(data);
            std::string token;

            if (std::getline(ss, token, ',')) {
                if (token.substr(0, 3) == "ID:") {
                    try {
                        id = std::stoi(token.substr(3));
                    } catch (...) {
                        std::cerr << "Connection Manager: Invalid ID format: " << token << "\n";
                        std::cerr.flush();
                        continue;
                    }
                }
            }

            if (std::getline(ss, token, ',')) {
                if (token.substr(0, 5) == "Temp:") {
                    try {
                        temp = std::stod(token.substr(5));
                    } catch (...) {
                        std::cerr << "Connection Manager: Invalid temperature format: " << token << "\n";
                        std::cerr.flush();
                        continue;
                    }
                }
            }

            if (id >= 0) {
                sensor_data.addReading(id, temp);
            } else {
                std::cerr << "Connection Manager: Invalid sensor ID: " << id << "\n";
                std::cerr.flush();
            }
        }

        close(client_fd);
    }

    close(server_fd);
}