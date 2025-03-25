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

typedef struct
{
    int socket;
    struct sockaddr_in address;
    int is_outgoing;
} Connection;

Connection connections[MAX_CLIENTS];
int connection_count = 0;
int server_socket, listening_port;
pthread_mutex_t lock;
volatile int running = 1; // Biến cờ để dừng thread

void print_help()
{
    printf("\n\nCommands:\n\n");
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
    int display_count = 0;
    int displayed[MAX_CLIENTS] = {0};

    for (int i = 0; i < connection_count; i++)
    {
        char *ip = inet_ntoa(connections[i].address.sin_addr);
        int port = ntohs(connections[i].address.sin_port);
        int already_displayed = 0;

        for (int j = 0; j < i; j++)
        {
            if (strcmp(inet_ntoa(connections[j].address.sin_addr), ip) == 0 &&
                ntohs(connections[j].address.sin_port) == port)
            {
                already_displayed = 1;
                break;
            }
        }

        if (!already_displayed)
        {
            printf("%d: %s %d\n", display_count + 1, ip, port);
            display_count++;
        }
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

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        perror("Socket creation failed");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    int max_retries = 5;
    int retry_delay = 1;
    int connected = 0;

    for (int i = 0; i < max_retries; i++)
    {
        if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        {
            connected = 1;
            break;
        }
        else
        {
            if (i < max_retries - 1)
            {
                printf("Retry %d/%d: Failed to connect to %s:%d, retrying in %d second(s)...\n", i + 1, max_retries, ip, port, retry_delay);
                sleep(retry_delay);
            }
        }
    }

    if (!connected)
    {
        perror("Connection failed after retries");
        printf("Failed to connect to %s:%d\n", ip, port);
        close(client_socket);
        return;
    }

    char port_msg[32];
    snprintf(port_msg, sizeof(port_msg), "PORT:%d", listening_port);
    if (send(client_socket, port_msg, strlen(port_msg), 0) < 0)
    {
        perror("Failed to send listening port");
        close(client_socket);
        return;
    }

    pthread_mutex_lock(&lock);
    connections[connection_count].socket = client_socket;
    connections[connection_count].address = server_addr;
    connections[connection_count].is_outgoing = 1;
    connection_count++;
    pthread_mutex_unlock(&lock);

    printf("Connected to %s:%d\n", ip, port);
}

void terminate_connection(int id)
{
    pthread_mutex_lock(&lock);
    int display_id = 0;
    char *target_ip = NULL;
    int target_port = 0;
    int outgoing_socket = -1;

    for (int i = 0; i < connection_count; i++)
    {
        char *ip = inet_ntoa(connections[i].address.sin_addr);
        int port = ntohs(connections[i].address.sin_port);
        int already_displayed = 0;

        for (int j = 0; j < i; j++)
        {
            if (strcmp(inet_ntoa(connections[j].address.sin_addr), ip) == 0 &&
                ntohs(connections[j].address.sin_port) == port)
            {
                already_displayed = 1;
                break;
            }
        }

        if (!already_displayed)
        {
            display_id++;
            if (display_id == id)
            {
                target_ip = ip;
                target_port = port;
                break;
            }
        }
    }

    if (target_ip == NULL)
    {
        printf("Invalid connection ID\n");
        pthread_mutex_unlock(&lock);
        return;
    }

    for (int i = 0; i < connection_count; i++)
    {
        if (strcmp(inet_ntoa(connections[i].address.sin_addr), target_ip) == 0 &&
            ntohs(connections[i].address.sin_port) == target_port &&
            connections[i].is_outgoing == 1)
        {
            outgoing_socket = connections[i].socket;
            break;
        }
    }

    if (outgoing_socket != -1)
    {
        const char *terminate_msg = "TERMINATE";
        if (send(outgoing_socket, terminate_msg, strlen(terminate_msg), 0) < 0)
        {
            perror("Failed to send TERMINATE message");
        }
        else
        {
            printf("Sent TERMINATE message to %s:%d\n", target_ip, target_port);
        }
    }

    for (int i = 0; i < connection_count; i++)
    {
        if (strcmp(inet_ntoa(connections[i].address.sin_addr), target_ip) == 0 &&
            ntohs(connections[i].address.sin_port) == target_port)
        {
            close(connections[i].socket);
            for (int j = i; j < connection_count - 1; j++)
            {
                connections[j] = connections[j + 1];
            }
            connection_count--;
            i--;
        }
    }

    printf("Connection %d terminated.\n", id);
    pthread_mutex_unlock(&lock);
}

void send_message(int id, char *message)
{
    pthread_mutex_lock(&lock);
    int display_id = 0;
    char *target_ip = NULL;
    int target_port = 0;
    int target_socket = -1;

    for (int i = 0; i < connection_count; i++)
    {
        char *ip = inet_ntoa(connections[i].address.sin_addr);
        int port = ntohs(connections[i].address.sin_port);
        int already_displayed = 0;

        for (int j = 0; j < i; j++)
        {
            if (strcmp(inet_ntoa(connections[j].address.sin_addr), ip) == 0 &&
                ntohs(connections[j].address.sin_port) == port)
            {
                already_displayed = 1;
                break;
            }
        }

        if (!already_displayed)
        {
            display_id++;
            if (display_id == id)
            {
                target_ip = ip;
                target_port = port;
                break;
            }
        }
    }

    if (target_ip == NULL)
    {
        printf("Invalid connection ID\n");
        pthread_mutex_unlock(&lock);
        return;
    }

    for (int i = 0; i < connection_count; i++)
    {
        if (strcmp(inet_ntoa(connections[i].address.sin_addr), target_ip) == 0 &&
            ntohs(connections[i].address.sin_port) == target_port &&
            connections[i].is_outgoing == 1)
        {
            target_socket = connections[i].socket;
            break;
        }
    }

    if (target_socket == -1)
    {
        printf("No outgoing connection found for %s:%d\n", target_ip, target_port);
        pthread_mutex_unlock(&lock);
        return;
    }

    if (send(target_socket, message, strlen(message), 0) < 0)
    {
        perror("Message send failed");
    }
    else
    {
        printf("Message sent to %s:%d\n", target_ip, target_port);
    }

    pthread_mutex_unlock(&lock);
}

void *receive_messages(void *arg)
{
    fd_set read_fds;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    int max_sd, activity, new_socket;
    char buffer[BUFFER_SIZE];

    while (running)
    {
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        max_sd = server_socket;

        pthread_mutex_lock(&lock);
        for (int i = 0; i < connection_count; i++)
        {
            FD_SET(connections[i].socket, &read_fds);
            if (connections[i].socket > max_sd)
                max_sd = connections[i].socket;
        }
        pthread_mutex_unlock(&lock);

        activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0)
        {
            perror("Select error");
            continue;
        }

        if (FD_ISSET(server_socket, &read_fds))
        {
            addr_len = sizeof(client_addr);
            new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
            if (new_socket >= 0)
            {
                pthread_mutex_lock(&lock);
                connections[connection_count].socket = new_socket;
                connections[connection_count].address = client_addr;
                connections[connection_count].is_outgoing = 0;
                connection_count++;
                int new_connection_index = connection_count - 1;
                pthread_mutex_unlock(&lock);

                char port_msg[32];
                int bytes_received = recv(new_socket, port_msg, sizeof(port_msg) - 1, 0);
                if (bytes_received > 0)
                {
                    port_msg[bytes_received] = '\0';
                    printf("Debug: Received PORT message: %s\n", port_msg);
                    if (strncmp(port_msg, "PORT:", 5) == 0)
                    {
                        int peer_listening_port = atoi(port_msg + 5);
                        char peer_ip[16];
                        strcpy(peer_ip, inet_ntoa(client_addr.sin_addr));
                        printf("New connection from %s:%d\n", peer_ip, peer_listening_port);

                        pthread_mutex_lock(&lock);
                        connections[new_connection_index].address.sin_port = htons(peer_listening_port);
                        pthread_mutex_unlock(&lock);

                        int is_duplicate = 0;
                        pthread_mutex_lock(&lock);
                        for (int i = 0; i < connection_count - 1; i++)
                        {
                            if (strcmp(inet_ntoa(connections[i].address.sin_addr), peer_ip) == 0 &&
                                ntohs(connections[i].address.sin_port) == peer_listening_port)
                            {
                                is_duplicate = 1;
                                break;
                            }
                        }
                        pthread_mutex_unlock(&lock);

                        if (peer_listening_port != listening_port && !is_duplicate)
                        {
                            printf("Attempting to connect back to %s:%d\n", peer_ip, peer_listening_port);
                            connect_to_peer(peer_ip, peer_listening_port);
                        }
                    }
                }
                else
                {
                    printf("Failed to receive PORT message from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    close(new_socket);
                    pthread_mutex_lock(&lock);
                    connection_count--;
                    pthread_mutex_unlock(&lock);
                }
            }
        }

        pthread_mutex_lock(&lock);
        for (int i = 0; i < connection_count; i++)
        {
            if (FD_ISSET(connections[i].socket, &read_fds))
            {
                memset(buffer, 0, BUFFER_SIZE);
                int bytes_received = recv(connections[i].socket, buffer, BUFFER_SIZE - 1, 0);
                if (bytes_received > 0)
                {
                    buffer[bytes_received] = '\0';
                    if (strcmp(buffer, "TERMINATE") == 0)
                    {
                        char *peer_ip = inet_ntoa(connections[i].address.sin_addr);
                        int peer_port = ntohs(connections[i].address.sin_port);
                        printf("Received TERMINATE from %s:%d, closing connection...\n", peer_ip, peer_port);

                        for (int j = 0; j < connection_count; j++)
                        {
                            if (strcmp(inet_ntoa(connections[j].address.sin_addr), peer_ip) == 0 &&
                                ntohs(connections[j].address.sin_port) == peer_port)
                            {
                                close(connections[j].socket);
                                for (int k = j; k < connection_count - 1; k++)
                                {
                                    connections[k] = connections[k + 1];
                                }
                                connection_count--;
                                j--;
                            }
                        }
                    }
                    else
                    {
                        printf("Debug: Received %d bytes on socket %d (is_outgoing: %d)\n", bytes_received, connections[i].socket, connections[i].is_outgoing);
                        printf("\nMessage from %s:%d -> %s\n", inet_ntoa(connections[i].address.sin_addr), ntohs(connections[i].address.sin_port), buffer);
                    }
                }
                else if (bytes_received == 0)
                {
                    char *peer_ip = inet_ntoa(connections[i].address.sin_addr);
                    int peer_port = ntohs(connections[i].address.sin_port);
                    printf("Connection closed by %s:%d\n", peer_ip, peer_port);

                    for (int j = 0; j < connection_count; j++)
                    {
                        if (strcmp(inet_ntoa(connections[j].address.sin_addr), peer_ip) == 0 &&
                            ntohs(connections[j].address.sin_port) == peer_port)
                        {
                            close(connections[j].socket);
                            for (int k = j; k < connection_count - 1; k++)
                            {
                                connections[k] = connections[k + 1];
                            }
                            connection_count--;
                            j--;
                        }
                    }
                    i--;
                }
                else
                {
                    perror("Recv error");
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

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Server socket creation failed");
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Setsockopt failed");
        close(server_socket);
        return 1;
    }

    struct sockaddr_in server_addr = {AF_INET, htons(atoi(argv[1])), inet_addr("192.168.90.135")};
    listening_port = atoi(argv[1]);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, MAX_CLIENTS) < 0)
    {
        perror("Listen failed");
        close(server_socket);
        return 1;
    }
    printf("Chat server running on port %d...\n", listening_port);

    pthread_mutex_init(&lock, NULL);
    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, receive_messages, NULL) != 0)
    {
        perror("Thread creation failed");
        return 1;
    }
    pthread_detach(recv_thread);

    char command[BUFFER_SIZE];
    while (1)
    {
        printf(">");
        fgets(command, BUFFER_SIZE, stdin);
        command[strcspn(command, "\n")] = 0;

        if (strncmp(command, "exitt", 5) == 0)
        {
            printf("Closing all connections and exiting...\n");
            running = 0;
        
            for (int i = 0; i < connection_count; i++)
                close(connections[i].socket);
            close(server_socket);
        
            pthread_cancel(recv_thread); // Buộc dừng thread
            usleep(100000);
        
            pthread_mutex_destroy(&lock);
            return 0;
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
            char ip[16];
            int port;
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