#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // 1️⃣ Tạo socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("❌ Error creating socket");
        return -1;
    }
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    printf("✅ Server: Socket created successfully\n");

    // 2️⃣ Cấu hình địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 3️⃣ Bind socket với địa chỉ IP & Port
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("❌ Bind failed");
        close(server_socket);
        return 1;
    }
    printf("✅ Server: Bind successful on port %d\n", PORT);

    while(1){
        // 4️⃣ Lắng nghe kết nối từ client
        if (listen(server_socket, 4) == -1) {
            perror("❌ Listen failed");
            close(server_socket);
            return 1;
        }
        printf("✅ Server: Listening for incoming connections...\n");

        // 5️⃣ Chấp nhận kết nối từ client
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("❌ Accept failed");
            close(server_socket);
            return 1;
        }
        printf("✅ Server: Connection accepted from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // 6️⃣ Nhận tin nhắn từ client
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("📩 Server received: %s\n", buffer);

        // 7️⃣ Gửi phản hồi lại client
        char *response = "Hello from server!";
        send(client_socket, response, strlen(response), 0);

    }

        // 8️⃣ Đóng kết nối
        close(client_socket);
        printf("❌ Server: Connection closed\n");
    return 0;
}