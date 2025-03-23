---

# README - Peer-to-Peer Chat Application

This project implements a peer-to-peer (P2P) chat application using socket programming in C. The application allows multiple peers to connect, exchange messages, and manage connections through a command-line interface. It supports bidirectional communication between peers, disconnection, and listing connections, with thread-safe operations using mutex locks.

## Overview

The program creates a chat server that listens on a specified port and allows peers to connect. Each peer can act as both a server (accepting incoming connections) and a client (initiating outgoing connections). The application uses TCP sockets to ensure reliable communication and supports multiple simultaneous connections (up to `MAX_CLIENTS`).

### Key Features
- **Bidirectional Communication**: Peers can connect and establish bidirectional communication.
- **Command-Line Interface**: Users can interact with the application using commands like `connect`, `send`, `list`, `terminate`, and `exit`.
- **Connection Management**: Supports listing active connections, disconnecting specific connections, and handling closed connections.
- **Thread Safety**: Uses mutex locks to ensure thread-safe access to shared resources.
- **Retry Mechanism**: Implements a retry mechanism when establishing connections to handle temporary errors.

## Important Components

### 1. **Data Structures and Global Variables**
- **Connection Structure**:
  ```c
  typedef struct
  {
      int socket;      // Socket descriptor
      struct sockaddr_in address; // Peer address (IP and port)
      int is_outgoing; // 1 if outgoing socket, 0 if incoming socket
  } Connection;
  ```
  - Stores information about each connection, including the socket descriptor, peer address, and `is_outgoing` flag to distinguish between outgoing (`is_outgoing = 1`) and incoming (`is_outgoing = 0`) sockets.
  - The `is_outgoing` flag. Each peer acts as both a client (connecting to other peers) and a server (accepting connections from other peers), so connecting two peers creates two sockets on each peer.

- **Global Variables**:
  ```c
  Connection connections[MAX_CLIENTS];
  int connection_count = 0;
  int server_socket, listening_port;
  pthread_mutex_t lock;
  ```
  - `connections`: Array storing all active connections (up to `MAX_CLIENTS = 10`).
  - `connection_count`: Tracks the current number of connections.
  - `server_socket`: Main server socket for accepting incoming connections.
  - `listening_port`: Port on which the peer listens for incoming connections.
  - `lock`: Mutex lock to ensure thread-safe access to the `connections` array.

### 2. **Main Function**
The `main()` function initializes the server socket, sets up a thread to receive messages, and provides a command-line interface for user interaction.

- **Server Socket Setup**:
  ```c
  listening_port = atoi(argv[1]);
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;
  setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  struct sockaddr_in server_addr = {AF_INET, htons(listening_port), inet_addr("192.168.90.135")};
  bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  listen(server_socket, MAX_CLIENTS);
  ```
  - Creates a TCP socket and sets the `SO_REUSEADDR` option to allow reuse of the port immediately after the program exits.
  - Binds the socket to the IP address `192.168.90.135` and the specified port.
  - Listens for incoming connections with a maximum queue of `MAX_CLIENTS`.

- **Message Receiving Thread**:
  ```c
  pthread_mutex_init(&lock, NULL);
  pthread_t recv_thread;
  pthread_create(&recv_thread, NULL, receive_messages, NULL);
  ```
  - Initializes the mutex lock for thread safety.
  - Creates a separate thread (`recv_thread`) to handle incoming connections and messages using the `receive_messages()` function.

- **Command-Line Interface**:
  ```c
  while (1)
  {
      printf(">");
      fgets(command, BUFFER_SIZE, stdin);
      command[strcspn(command, "\n")] = 0;
      // Handle commands: help, myip, myport, connect, list, terminate, send, exit
  }
  ```
  - Continuously reads commands from the user and processes them using functions like `connect_to_peer()`, `send_message()`, `list_connections()`, and `terminate_connection()`.

### 3. **Connection Management**

#### **Establishing Connection (`connect_to_peer`)**
- **Function**:
  ```c
  void connect_to_peer(char *ip, int port)
  ```
  - Initiates a connection to another peer at the specified IP and port.
  - Implements a retry mechanism (up to 5 times, with a 1-second interval) to handle temporary connection errors.
  - Sends a `PORT:<port>` message to the peer to share its listening port, enabling bidirectional communication.
  - Adds the new connection to the `connections` array with `is_outgoing = 1`.

- **Retry Mechanism**:
  ```c
  int max_retries = 5;
  int retry_delay = 1;
  for (int i = 0; i < max_retries; i++)
  {
      if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
      {
          connected = 1;
          break;
      }
      else
      {
          printf("Retry %d/%d: Failed to connect to %s:%d, retrying in %d second(s)...\n", i + 1, max_retries, ip, port, retry_delay);
          sleep(retry_delay);
      }
  }
  ```
  - Ensures the connection is reliably established by retrying if the initial connection attempt fails.

#### **Listing Connections (`list_connections`)**
- **Function**:
  ```c
  void list_connections()
  ```
  - Displays a list of active connections, ensuring no duplicate connections by comparing IP and port.
  - Each connection is assigned a unique ID for use in commands like `send` and `terminate`.

- **Duplicate Filtering**:
  ```c
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
  ```
  - Ensures each peer appears only once in the list, even though there may be multiple sockets (incoming and outgoing) for the same peer.

#### **Terminating Connection (`terminate_connection`)**
- **Function**:
  ```c
  void terminate_connection(int id)
  ```
  - Terminates the connection specified by the ID in the displayed list (`list`).
  - Sends a `TERMINATE` message to the peer to notify about the disconnection.
  - Closes all sockets (both incoming and outgoing) related to that peer and updates the `connections` array.

- **Notification**:
  ```c
  if (outgoing_socket != -1)
  {
      const char *terminate_msg = "TERMINATE";
      send(outgoing_socket, terminate_msg, strlen(terminate_msg), 0);
  }
  ```
  - Ensures the remote peer is notified about the disconnection, allowing it to update its connection list.

### 4. **Message Handling**

#### **Sending Message (`send_message`)**
- **Function**:
  ```c
  void send_message(int id, char *message)
  ```
  - Sends a message to the peer corresponding to the specified ID in the displayed list (`list`).
  - Uses the same duplicate filtering logic as `list_connections()` to map the ID to the correct peer.
  - Sends the message through the outgoing socket (`is_outgoing = 1`) to ensure accurate communication.

- **ID Mapping**:
  ```c
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
  ```
  - Maps the user-provided ID to the correct peer by filtering duplicates, ensuring the message is sent to the correct recipient.

#### **Receiving Messages (`receive_messages`)**
- **Function**:
  ```c
  void *receive_messages(void *arg)
  ```
  - Uses the `select()` system call to monitor multiple sockets (server and client sockets) for activity.
  - Handles three types of events:
    1. **New Connection**: Accepts incoming connections and processes `PORT:<port>` messages to establish bidirectional communication.
    2. **Messages**: Receives and displays messages from peers.
    3. **Disconnection**: Detects closed connections and updates the `connections` array.

- **Bidirectional Communication**:
  ```c
  if (strncmp(port_msg, "PORT:", 5) == 0)
  {
      int peer_listening_port = atoi(port_msg + 5);
      char peer_ip[16];
      strcpy(peer_ip, inet_ntoa(client_addr.sin_addr));
      if (peer_listening_port != listening_port && !is_duplicate)
      {
          printf("Attempting to connect back to %s:%d\n", peer_ip, peer_listening_port);
          connect_to_peer(peer_ip, peer_listening_port);
      }
  }
  ```
  - When a peer connects, it sends its listening port via a `PORT:<port>` message. The receiving peer uses this port to connect back, establishing bidirectional communication.

- **Disconnection Handling**:
  ```c
  if (strcmp(buffer, "TERMINATE") == 0)
  {
      char *peer_ip = inet_ntoa(connections[i].address.sin_addr);
      int peer_port = ntohs(connections[i].address.sin_port);
      printf("Received TERMINATE from %s:%d, closing connection...\n", peer_ip, peer_port);
      // Close all related sockets
  }
  ```
  - Handles `TERMINATE` messages from peers to close connections and update the connection list.

### 5. **Thread Safety**
- **Mutex Locks**:
  ```c
  pthread_mutex_t lock;
  pthread_mutex_lock(&lock);
  pthread_mutex_unlock(&lock);
  ```
  - The `lock` ensures thread-safe access to the `connections` array, which is shared between the main thread (for sending messages and managing connections) and the `receive_messages` thread (for handling incoming messages and connections).

### 6. **Error Handling**
- The program includes error handling for socket operations (`socket()`, `bind()`, `listen()`, `connect()`, `send()`, `recv()`), thread creation, and connection limits.
- Example:
  ```c
  if (connection_count >= MAX_CLIENTS)
  {
      printf("Connection limit reached!\n");
      return;
  }
  ```
  - Prevents the program from exceeding the maximum number of connections (`MAX_CLIENTS`).

## Usage Instructions

1. **Compile the Program**:
   ```bash
   gcc -o chat chat.c -pthread
   ```

2. **Run the Program**:
   - Start a peer on a specific port:
     ```bash
     ./chat 4001
     ```
   - Start another peer on a different port:
     ```bash
     ./chat 5000
     ```

3. **Connect Peers**:
   - On the first peer (port 4001), connect to the second peer (port 5000):
     ```
     > connect 192.168.90.135 5000
     ```

4. **List Connections**:
   ```
   > list
   Active Connections:
   1: 192.168.90.135 5000
   ```

5. **Send Message**:
   - Send a message to the peer with ID 1:
     ```
     > send 1 Hello
     Message sent to 192.168.90.135:5000
     ```

6. **Terminate Connection**:
   - Terminate the connection with ID 1:
     ```
     > terminate 1
     Sent TERMINATE message to 192.168.90.135:5000
     Connection 1 terminated.
     ```

7. **Exit the Program**:
   ```
   > exit
   Closing all connections and exiting...
   ```

## Video Demonstration
You can watch a video demonstration of the application [here](https://drive.google.com/file/d/11zXOX0zxy2DhdaL3UOI6euwImjPF_ANP/view?usp=sharing).

## Common Commands
### Check if a process is using port 4000
```bash
sudo netstat -tulnp | grep :4000
```

### Check and stop a process with a specific PID
```bash
ps -aux | grep <PID>
kill -9 <PID>
```

### Commands in the code
```c
else if (strncmp(command, "myip", 4) == 0) {
    system("hostname -I");
} 
else if (strncmp(command, "myport", 6) == 0) {
    printf("Port: %d\n", listening_port);
}
```

### Monitor the chat process
```bash
watch -n 1 "ps aux | grep chat | grep -v grep"
```

Reference: [Project Socket Application](https://www.notion.so/duancuahuynh/project-socket-application-1b935291510881089ed0e2b8fc9aa738?p=1bf35291510880299866f82696bb9d04&pm=c)