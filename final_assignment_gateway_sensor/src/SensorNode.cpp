#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <random>
#include <thread>
#include <chrono>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <sensor_id>\n";
        return 1;
    }
    int sensor_id = std::stoi(argv[1]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // printf all datas to debug
    std::cout << "Sensor ID: " << sensor_id << "\n";
    std::cout << "Socket created: " << sock << "\n";
    std::cout << "Server address: " << inet_ntoa(server_addr.sin_addr) << "\n";
    std::cout << "Server port: " << ntohs(server_addr.sin_port) << "\n";
    std::cout << "Connecting to server...\n";

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed\n";
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 40.0);

    while (true) {
        double temp = dis(gen);
        std::stringstream msg;
        msg << "ID:" << sensor_id << ",Temp:" << temp;
        write(sock, msg.str().c_str(), msg.str().length());
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    close(sock);
    return 0;
}
