#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int socket;
    char ip[INET_ADDRSTRLEN];
    int port;
} Connection;

Connection connections[MAX_CLIENTS];
int connection_count = 0;
int server_socket, listening_port;

/* Hiển thị danh sách lệnh */
void print_help() {
    printf("\nCommands:\n");
    printf("help                 - Show command list\n");
    printf("myip                 - Display your IP address\n");
    printf("myport               - Show listening port\n");
    printf("connect <ip> <port>  - Connect to another peer\n");
    printf("list                 - Show active connections\n");
    printf("terminate <id>       - Terminate connection by ID\n");
    printf("send <id> <message>  - Send message to a connection\n");
    printf("exit                 - Close all connections and exit\n\n");
}

/* Hiển thị danh sách kết nối */
void list_connections() {
    printf("\nActive Connections:\n");
    for (int i = 0; i < connection_count; i++) {
        printf("%d: %s %d\n", i + 1, connections[i].ip, connections[i].port);
    }
}

/* Kết nối đến peer */
void connect_to_peer(char *ip, int port) {
    if (connection_count >= MAX_CLIENTS) {
        printf("Connection limit reached!\n");
        return;
    }

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        return;
    }

    strcpy(connections[connection_count].ip, ip);
    connections[connection_count].port = port;
    connections[connection_count].socket = client_socket;
    connection_count++;

    printf("Connected to %s:%d\n", ip, port);
}

/* Ngắt kết nối */
void terminate_connection(int id) {
    if (id < 1 || id > connection_count) {
        printf("Invalid connection ID\n");
        return;
    }

    int index = id - 1;
    close(connections[index].socket);
    printf("Connection %d terminated.\n", id);

    for (int i = index; i < connection_count - 1; i++) {
        connections[i] = connections[i + 1];
    }
    connection_count--;
}

/* Gửi tin nhắn */
void send_message(int id, char *message) {
    if (id < 1 || id > connection_count) {
        printf("Invalid connection ID\n");
        return;
    }

    int socket = connections[id - 1].socket;
    if (send(socket, message, strlen(message), 0) < 0) {
        perror("Message send failed");
    } else {
        printf("Message sent to %s:%d\n", connections[id - 1].ip, connections[id - 1].port);
    }
}

/* Nhận tin nhắn */
void receive_messages() {
    fd_set read_fds;
    int max_sd, activity, new_socket, addr_len;
    struct sockaddr_in client_addr;
    char buffer[BUFFER_SIZE];

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        max_sd = server_socket;

        for (int i = 0; i < connection_count; i++) {
            FD_SET(connections[i].socket, &read_fds);
            if (connections[i].socket > max_sd) max_sd = connections[i].socket;
        }

        activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);

        if (FD_ISSET(server_socket, &read_fds)) {
            addr_len = sizeof(client_addr);
            new_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);
            if (new_socket >= 0) {
                inet_ntop(AF_INET, &client_addr.sin_addr, connections[connection_count].ip, INET_ADDRSTRLEN);
                connections[connection_count].port = ntohs(client_addr.sin_port);
                connections[connection_count].socket = new_socket;
                connection_count++;
                printf("New connection from %s:%d\n", connections[connection_count - 1].ip, connections[connection_count - 1].port);
            }
        }

        for (int i = 0; i < connection_count; i++) {
            if (FD_ISSET(connections[i].socket, &read_fds)) {
                memset(buffer, 0, BUFFER_SIZE);
                int bytes_received = recv(connections[i].socket, buffer, BUFFER_SIZE, 0);
                if (bytes_received > 0) {
                    printf("\nMessage from %s:%d -> %s\n", connections[i].ip, connections[i].port, buffer);
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    listening_port = atoi(argv[1]);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {AF_INET, htons(listening_port), INADDR_ANY};

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);
    printf("Chat server running on port %d...\n", listening_port);

    /* Chạy nhận tin nhắn song song */
    if (fork() == 0) receive_messages();

    char command[BUFFER_SIZE];
    while (1) {
        printf(">>> ");
        fgets(command, BUFFER_SIZE, stdin);
        command[strcspn(command, "\n")] = 0; // Xóa dấu xuống dòng

        if (strncmp(command, "exit", 4) == 0) {
            printf("Closing all connections and exiting...\n");
            for (int i = 0; i < connection_count; i++) {
                close(connections[i].socket);
            }
            close(server_socket);
            exit(0);
        } 
        else if (strncmp(command, "help", 4) == 0) {print_help();} 
        else if (strncmp(command, "myip", 4) == 0) {system("hostname -I");} 
        else if (strncmp(command, "myport", 6) == 0) {printf("Port: %d\n", listening_port);} 
        else if (strncmp(command, "list", 4) == 0) {list_connections();}
        else if (strncmp(command, "connect", 7) == 0) {
            char ip[32];
            int port;
            if (sscanf(command, "connect %s %d", ip, &port) == 2) {
                connect_to_peer(ip, port);
            } else {
                printf("Usage: connect <IP> <Port>\n");
            }
        } 
        else if (strncmp(command, "terminate", 9) == 0) {
            int id;
            if (sscanf(command, "terminate %d", &id) == 1) {
                terminate_connection(id);
            } else {
                printf("Usage: terminate <connection_id>\n");
            }
        } 
        else if (strncmp(command, "send", 4) == 0) {
            int id;
            char message[BUFFER_SIZE];
            if (sscanf(command, "send %d %[^\n]", &id, message) == 2) {
                send_message(id, message);
            } else {
                printf("Usage: send <connection_id> <message>\n");
            }
        } 
        else {printf("Unknown command. Type 'help' for a list of commands.\n");}
    }

    close(server_socket);
    return 0;
}