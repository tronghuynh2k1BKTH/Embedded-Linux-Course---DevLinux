#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Connection struct to store client socket and address
typedef struct
{
    int socket; // storage socket descriptor
    struct sockaddr_in address;
} Connection;

// create an array of connections and a counter
Connection connections[MAX_CLIENTS];
int connection_count = 0;
int server_socket, listening_port;
pthread_mutex_t lock;

void print_help()
{
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

void list_connections()
{
    pthread_mutex_lock(&lock);
    printf("\nActive Connections:\n");
    for (int i = 0; i < connection_count; i++)
    {
        printf("%d: %s %d\n", i + 1, inet_ntoa(connections[i].address.sin_addr), ntohs(connections[i].address.sin_port));
    }
    pthread_mutex_unlock(&lock);
}

void connect_to_peer(char *ip, int port)
{
    if (connection_count >= MAX_CLIENTS)
    {
        printf("Connection limit reached!\n");
        return;
    }

    // Create client socket and connect to server
    int client_socket = socket(AF_INET, SOCK_STREAM, 0); // TCP (SOCK_STREAM), IPv4 (AF_INET)
    if (client_socket < 0)
    {
        perror("Socket creation failed");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(client_socket);
        return;
    }

    pthread_mutex_lock(&lock);
    connections[connection_count].socket = client_socket;
    connections[connection_count].address = server_addr;
    connection_count++;
    pthread_mutex_unlock(&lock);

    printf("Connected to %s:%d\n", ip, port);
}

void terminate_connection(int id)
{
    if (id < 1 || id > connection_count)
    {
        printf("Invalid connection ID\n");
        return;
    }

    int index = id - 1;
    close(connections[index].socket);
    printf("Connection %d terminated.\n", id);

    pthread_mutex_lock(&lock);
    for (int i = index; i < connection_count - 1; i++)
    {
        connections[i] = connections[i + 1];
    }
    connection_count--;
    pthread_mutex_unlock(&lock);
}

void send_message(int id, char *message)
{
    if (id < 1 || id > connection_count)
    {
        printf("Invalid connection ID\n");
        return;
    }

    int socket = connections[id - 1].socket;
    if (send(socket, message, strlen(message), 0) < 0)
    {
        perror("Message send failed");
    }
    else
    {
        printf("Message sent to %s:%d\n",
               inet_ntoa(connections[id - 1].address.sin_addr),
               ntohs(connections[id - 1].address.sin_port));
    }
}

void *receive_messages(void *arg)
{
    // Create file descriptor set
    fd_set read_fds;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    // max_sd is used in select() to determine the highest file descriptor
    int max_sd, activity, new_socket;
    // Buffer to store incoming messages
    char buffer[BUFFER_SIZE];

    while (1)
    {
        // Clear file descriptor set
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        max_sd = server_socket;

        pthread_mutex_lock(&lock);
        // Add client sockets to file descriptor set
        // Use FD_SET() to add all connection sockets to the read_fds set
        // so select() can monitor them.
        // Update max_sd (the highest socket descriptor) to use with select().
        for (int i = 0; i < connection_count; i++)
        {
            FD_SET(connections[i].socket, &read_fds);
            if (connections[i].socket > max_sd)
                max_sd = connections[i].socket;
        }
        pthread_mutex_unlock(&lock);

        // Wait for activity on any of the sockets using select()
        // select() will block until there is activity on any of the sockets
        // or until the timeout expires.
        // The return value of select() is the number of sockets with activity.
        activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);

        if (FD_ISSET(server_socket, &read_fds))
        {
            addr_len = sizeof(client_addr);
            new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
            if (new_socket >= 0)
            {
                pthread_mutex_lock(&lock);
                connections[connection_count].socket = new_socket;
                connections[connection_count].address = client_addr;
                connection_count++;
                pthread_mutex_unlock(&lock);
                printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            }
        }

        pthread_mutex_lock(&lock);
        for (int i = 0; i < connection_count; i++)
        {
            if (FD_ISSET(connections[i].socket, &read_fds))
            {
                memset(buffer, 0, BUFFER_SIZE);
                int bytes_received = recv(connections[i].socket, buffer, BUFFER_SIZE, 0);
                if (bytes_received > 0)
                {
                    buffer[bytes_received] = '\0';
                    printf("\nMessage from %s:%d -> %s\n", inet_ntoa(connections[i].address.sin_addr), ntohs(connections[i].address.sin_port), buffer);
                }
            }
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    // Create server socket
    listening_port = atoi(argv[1]);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {AF_INET, htons(listening_port), inet_addr("192.168.90.135")};

    // Bind socket to address
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Listen for incoming connections
    listen(server_socket, MAX_CLIENTS);
    printf("Chat server running on port %d...\n", listening_port);

    // Create thread to receive messages
    pthread_mutex_init(&lock, NULL);
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    char command[BUFFER_SIZE];
    while (1)
    {
        printf(">");
        fgets(command, BUFFER_SIZE, stdin);
        // Remove newline character
        // input: "connect 192.168.1.1 5000\n\0"
        // output: "connect 192.168.1.1 5000\0"
        command[strcspn(command, "\n")] = 0;

        // compare 4 first characters of command with "exit"
        if (strncmp(command, "exit", 4) == 0)
        {
            printf("Closing all connections and exiting...\n");
            for (int i = 0; i < connection_count; i++)
                close(connections[i].socket);
            close(server_socket);
            pthread_mutex_destroy(&lock);
            exit(0);
        }
        else if (strncmp(command, "help", 4) == 0)
            print_help();
        else if (strncmp(command, "myip", 4) == 0)
            system("hostname -I");
        else if (strncmp(command, "myport", 6) == 0)
            printf("Listening port: %d\n", listening_port);
        else if (strncmp(command, "list", 4) == 0)
            list_connections();
        else if (strncmp(command, "terminate", 9) == 0)
        {
            int id;
            if (sscanf(command, "terminate %d", &id) == 1)
                terminate_connection(id);
        }
        else if (strncmp(command, "connect", 7) == 0)
        {
            char ip[16]; // 255.255.255.255 + null terminator is 16 bytes
            int port;
            // sscanf returns the number of items successfully matched
            if (sscanf(command, "connect %s %d", ip, &port) == 2)
                connect_to_peer(ip, port);
        }
        else if (strncmp(command, "send", 4) == 0)
        {
            int id;
            char message[BUFFER_SIZE];
            if (sscanf(command, "send %d %[^\n]", &id, message) == 2)
                send_message(id, message);
        }
        else
            printf("Invalid command. Type 'help' for command list.\n");
    }

    return 0;
}