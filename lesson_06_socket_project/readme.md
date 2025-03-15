# Peer-to-Peer Chat Application

## Overview
This is a **peer-to-peer (P2P) chat application** that allows multiple clients to connect and communicate with each other via **TCP sockets**. The application runs as a **command-line interface (CLI)** and supports multiple concurrent connections using **multi-threading**.

## Features
- **Command-line Interface (CLI)**: Supports user interaction through text commands.
- **P2P Connection**: Each instance acts as both a client and a server.
- **Multi-threading**: Uses `pthread` to handle multiple connections concurrently.
- **Message Exchange**: Send and receive messages between connected peers.
- **Manage Connections**: List active connections and terminate specific connections.
- **Graceful Exit**: Closes all connections before exiting.

## Installation
### **1. Prerequisites**
- A Linux-based operating system (Ubuntu, Debian, etc.).
- A compiler supporting `gcc`.
- Basic networking knowledge (IP addresses, ports, sockets).

### **2. Compile the Program**
```bash
gcc chat.c -o chat -pthread
```

## Usage
### **1. Start the Chat Application**
Run the application with a port number:
```bash
./chat <port>
```
Example:
```bash
./chat 5000
```
This starts the chat program and listens for incoming connections on port **5000**.

### **2. Available Commands**
| Command | Description |
|---------|-------------|
| `help` | Show list of available commands. |
| `myip` | Display the local IP address. |
| `myport` | Show the port this instance is listening on. |
| `connect <ip> <port>` | Connect to another peer using IP and port. |
| `list` | Display all active connections. |
| `terminate <id>` | Close a specific connection by its ID from `list`. |
| `send <id> <message>` | Send a message to a connected peer. |
| `exit` | Close all connections and terminate the program. |

### **3. Example Usage**
#### **Step 1: Start the First Peer (Listening on Port 4000)**
```bash
./chat 4000
```

#### **Step 2: Start the Second Peer (Listening on Port 5000)**
```bash
./chat 5000
```

#### **Step 3: Peer 1 Connects to Peer 2**
In Peer 1 terminal:
```bash
connect 192.168.1.10 5000
```

#### **Step 4: Send a Message from Peer 1 to Peer 2**
```bash
send 1 Hello from Peer 1!
```

#### **Step 5: Peer 2 Receives the Message**
```
Message from 192.168.1.11:4000 -> Hello from Peer 1!
```

#### **Step 6: View Active Connections**
```bash
list
```
Example output:
```
Active Connections:
1: 192.168.1.10 5000
```

#### **Step 7: Terminate a Connection**
```bash
terminate 1
```

#### **Step 8: Exit the Program**
```bash
exit
```

## Technical Details
### **1. Connection Handling**
- Each peer runs a **server socket** that listens for incoming connections.
- The program maintains an **array of active connections**.
- Incoming messages are processed using `select()` for efficient non-blocking I/O.

### **2. Multi-threading**
- The `receive_messages` function runs in a separate thread using `pthread_create()`.
- A **mutex (`pthread_mutex_t`)** ensures thread safety when modifying shared resources.

### **3. Message Sending & Receiving**
- Messages are sent using `send()`.
- Incoming messages are handled using `recv()` in the `receive_messages` thread.
- Messages are displayed in the format:
  ```
  Message from <IP>:<Port> -> <Message>
  ```

## Troubleshooting
### **1. Connection Fails**
- Ensure the target peer is running and listening on the correct port.
- Check your firewall settings (`sudo ufw allow <port>`).
- Verify IP addresses using `myip`.

### **2. Message Not Received**
- Ensure both peers are connected (`list` command).
- Check if the message format is correct (`send <id> <message>`).
- Restart the application and try again.

## Future Improvements
- **Usernames for Peers** instead of using IP addresses.
- **Encryption** for secure message exchange.
- **File Transfer Feature** between peers.

## License
This project is open-source and licensed under the MIT License.

## Code Flow
1. Create socket (process).
2. Bind and listen.
3. Create thread to receive messages.
4. Use `while(1)` loop to wait for user commands.

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
