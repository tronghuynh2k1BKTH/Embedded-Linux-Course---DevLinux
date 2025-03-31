#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"  // Địa chỉ của server (có thể thay đổi)
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = "Hello from client!";

    // 1️⃣ Tạo socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("❌ Client: Error creating socket");
        return -1;
    }
    printf("✅ Client: Socket created successfully\n");

    // 2️⃣ Cấu hình địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // 3️⃣ Kết nối đến server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("❌ Client: Connection failed");
        close(client_socket);
        return -1;
    }
    printf("✅ Client: Connected to server at %s:%d\n", SERVER_IP, PORT);

    while(1){
        // 4️⃣ Gửi tin nhắn đến server
        send(client_socket, buffer, strlen(buffer), 0);
        printf("📤 Client sent: %s\n", buffer);

        // 5️⃣ Nhận phản hồi từ server
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("📩 Client received: %s\n", buffer);
    }

    // 6️⃣ Đóng kết nối
    close(client_socket);
    printf("❌ Client: Connection closed\n");

    return 0;
}