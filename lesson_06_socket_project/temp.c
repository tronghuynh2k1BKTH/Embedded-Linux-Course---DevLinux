#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <errno.h>
#include <netdb.h>

#define PORT 8080
#define MAX_CONNECTIONS 10

void print_help() {
    printf("Available commands:\n");
    printf("help - Show this help message\n");
    printf("myip - Show the IP address of this machine\n");
    printf("myport - Show the port number this server is listening on\n");
    printf("connect <ip> <port> - Connect to a server\n");
    printf("list - Show all connections\n");
    printf("terminate <ip> <port> - Terminate a connection\n");
    printf("send <ip> <port> \"message\" - Send a message to a connection\n");
    printf("exit - Exit the application\n");
}

void show_my_ip() {
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        int family = ifa->ifa_addr->sa_family;
        if (family == AF_INET) {
            int s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                                host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, host);
        }
    }

    freeifaddrs(ifaddr);
}

void show_my_port() {
    printf("Listening on port: %d\n", PORT);
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char command[256];

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline character

        if (strcmp(command, "help") == 0) {
            print_help();
        } else if (strcmp(command, "myip") == 0) {
            show_my_ip();
        } else if (strcmp(command, "myport") == 0) {
            show_my_port();
        } else if (strncmp(command, "connect", 7) == 0) {
            // Implement connect functionality
        } else if (strcmp(command, "list") == 0) {
            // Implement list functionality
        } else if (strncmp(command, "terminate", 9) == 0) {
            // Implement terminate functionality
        } else if (strncmp(command, "send", 4) == 0) {
            // Implement send functionality
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Unknown command. Type 'help' for a list of commands.\n");
        }
    }

    close(server_fd);
    return 0;
}