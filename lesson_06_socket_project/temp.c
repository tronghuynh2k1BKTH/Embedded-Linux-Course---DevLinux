#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int socket_fd;
    struct sockaddr_in address;
} Connection;

Connection client_list[MAX_CLIENTS];
int client_count = 0;
int server_socket;

// Function to start the server
void start_server(int port) {
    struct sockaddr_in server_addr;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server started on port %d. Waiting for connections...\n", port);
}

// Function to connect to another socket
void connect_to_socket(char *ip, int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sockfd);
        return;
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sockfd);
        return;
    }

    if (client_count < MAX_CLIENTS) {
        client_list[client_count].socket_fd = sockfd;
        client_list[client_count].address = server_addr;
        client_count++;
        printf("Connected to %s:%d successfully.\n", ip, port);
    } else {
        printf("Client list full. Cannot store more connections.\n");
        close(sockfd);
    }
}

// Function to send a message to a connected socket
void send_message(char *ip, int port, char *message) {
    for (int i = 0; i < client_count; i++) {
        if (ntohs(client_list[i].address.sin_port) == port &&
            strcmp(inet_ntoa(client_list[i].address.sin_addr), ip) == 0) {
            send(client_list[i].socket_fd, message, strlen(message), 0);
            printf("Message sent to %s:%d\n", ip, port);
            return;
        }
    }
    printf("Error: No connection found for %s:%d\n", ip, port);
}

// Function to terminate a connection
void terminate_connection(char *ip, int port) {
    for (int i = 0; i < client_count; i++) {
        if (ntohs(client_list[i].address.sin_port) == port &&
            strcmp(inet_ntoa(client_list[i].address.sin_addr), ip) == 0) {
            close(client_list[i].socket_fd);
            printf("Connection to %s:%d terminated.\n", ip, port);
            for (int j = i; j < client_count - 1; j++) {
                client_list[j] = client_list[j + 1];
            }
            client_count--;
            return;
        }
    }
    printf("Error: No connection found for %s:%d\n", ip, port);
}

// Function to list all active connections
void list_connections() {
    printf("Active connections:\n");
    for (int i = 0; i < client_count; i++) {
        printf("Socket FD: %d | IP: %s | Port: %d\n",
               client_list[i].socket_fd,
               inet_ntoa(client_list[i].address.sin_addr),
               ntohs(client_list[i].address.sin_port));
    }
}

// Function to handle incoming connections
void *handle_clients(void *arg) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    while (1) {
        int new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        if (client_count < MAX_CLIENTS) {
            client_list[client_count].socket_fd = new_socket;
            client_list[client_count].address = client_addr;
            client_count++;
        } else {
            printf("Max clients reached. Connection refused.\n");
            close(new_socket);
        }

        memset(buffer, 0, BUFFER_SIZE);
        read(new_socket, buffer, BUFFER_SIZE);
        printf("Received: %s\n", buffer);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    pthread_t client_thread;
    start_server(port);
    pthread_create(&client_thread, NULL, handle_clients, NULL);

    char command[256];
    while (1) {
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline

        if (strncmp(command, "connect", 7) == 0) {
            char ip[50];
            int port;
            if (sscanf(command, "connect %s %d", ip, &port) == 2) {
                connect_to_socket(ip, port);
            } else {
                printf("Usage: connect <ip> <port>\n");
            }
        } else if (strncmp(command, "send", 4) == 0) {
            char ip[50], message[BUFFER_SIZE];
            int port;
            if (sscanf(command, "send %s %d %[^\n]", ip, &port, message) == 3) {
                send_message(ip, port, message);
            } else {
                printf("Usage: send <ip> <port> <message>\n");
            }
        } else if (strncmp(command, "terminate", 9) == 0) {
            char ip[50];
            int port;
            if (sscanf(command, "terminate %s %d", ip, &port) == 2) {
                terminate_connection(ip, port);
            } else {
                printf("Usage: terminate <ip> <port>\n");
            }
        } else if (strcmp(command, "list") == 0) {
            list_connections();
        } else if (strcmp(command, "exit") == 0) {
            for (int i = 0; i < client_count; i++) {
                close(client_list[i].socket_fd);
            }
            close(server_socket);
            printf("Server shutting down.\n");
            break;
        } else if (strcmp(command, "myip") == 0) {
            system("hostname -I");
        } else if (strcmp(command, "myport") == 0) {
            printf("Port: %d\n", port);
        } else if (strcmp(command, "help") == 0) {
            printf("\nCommands:\n\n");
            printf("connect <ip> <port>\n");
            printf("send <ip> <port> <message>\n");
            printf("terminate <ip> <port>\n");
            printf("list: show all connections\n");
            printf("exit: to exit all sockets\n");
            printf("myip: show current ip process\n");
            printf("myport: show current port process\n");
            printf("help: show all commands for app\n\n");
        }
        else {
            printf("Unknown command.\n");
        }
    }

    return 0;
}