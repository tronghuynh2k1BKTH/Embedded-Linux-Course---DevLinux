// src/SensorNode.cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <random>
#include <cstring> // Thêm để dùng strerror

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <sensor_id>\n";
        return 1;
    }
    int sensor_id = std::stoi(argv[1]);
    std::cout << "Sensor ID: " << sensor_id << "\n";
    std::cout.flush();

    // Create TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Sensor Node: Failed to create socket: " << std::strerror(errno) << "\n";
        return 1;
    }
    std::cout << "Socket created: " << sock << "\n";
    std::cout.flush();

    // Connect to gateway
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    std::cout << "Server address: 127.0.0.1\n";
    std::cout << "Server port: 1234\n";
    std::cout.flush();

    std::cout << "Connecting to server...\n";
    std::cout.flush();
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Sensor Node: Failed to connect to gateway: " << std::strerror(errno) << "\n";
        close(sock);
        return 1;
    }
    std::cout << "Connected to server\n"; // Thêm thông báo sau khi kết nối thành công
    std::cout.flush();

    // Generate random temperature and send data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 40.0);

    for (int i = 0; i < 5; i++) {
        double temp = dis(gen);
        std::stringstream msg;
        msg << "ID:" << sensor_id << ",Temp:" << temp;
        std::string data = msg.str();

        int n = write(sock, data.c_str(), data.length());
        if (n < 0) {
            std::cerr << "Sensor Node: Failed to send data: " << std::strerror(errno) << "\n";
            break;
        }
        std::cout << "Sensor Node " << sensor_id << ": Sent data: " << data << "\n";
        std::cout.flush();

        sleep(1);
    }

    close(sock);
    std::cout << "Sensor Node: Connection closed\n";
    std::cout.flush();
    return 0;
}