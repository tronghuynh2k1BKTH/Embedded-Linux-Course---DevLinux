---

# README - Ứng dụng Chat Peer-to-Peer

Dự án này triển khai một ứng dụng chat peer-to-peer (P2P) sử dụng lập trình socket trong ngôn ngữ C. Ứng dụng cho phép nhiều peer kết nối với nhau, trao đổi tin nhắn, và quản lý kết nối thông qua giao diện dòng lệnh. Nó hỗ trợ giao tiếp hai chiều giữa các peer, ngắt kết nối, và liệt kê danh sách kết nối, với các thao tác an toàn luồng (thread-safe) sử dụng khóa mutex.

## Tổng quan

Chương trình tạo một server chat lắng nghe trên một cổng được chỉ định và cho phép các peer kết nối với nhau. Mỗi peer có thể vừa đóng vai trò server (chấp nhận kết nối đến) vừa đóng vai trò client (khởi tạo kết nối đi). Ứng dụng sử dụng socket TCP để đảm bảo giao tiếp đáng tin cậy và hỗ trợ nhiều kết nối đồng thời (tối đa `MAX_CLIENTS`).

### Các tính năng chính
- **Giao tiếp hai chiều**: Các peer có thể kết nối với nhau và thiết lập giao tiếp hai chiều.
- **Giao diện dòng lệnh**: Người dùng có thể tương tác với ứng dụng bằng các lệnh như `connect`, `send`, `list`, `terminate`, và `exit`.
- **Quản lý kết nối**: Hỗ trợ liệt kê các kết nối đang hoạt động, ngắt kết nối cụ thể, và xử lý khi kết nối bị đóng.
- **An toàn luồng**: Sử dụng khóa mutex để đảm bảo truy cập an toàn vào tài nguyên dùng chung.
- **Cơ chế thử lại**: Triển khai cơ chế thử lại khi thiết lập kết nối để xử lý các lỗi tạm thời.

## Các thành phần quan trọng

### 1. **Cấu trúc dữ liệu và biến toàn cục**
- **Cấu trúc Connection**:
  ```c
  typedef struct
  {
      int socket;      // Mô tả socket
      struct sockaddr_in address; // Địa chỉ peer (IP và cổng)
      int is_outgoing; // 1 nếu là socket đi, 0 nếu là socket đến
  } Connection;
  ```
  - Lưu trữ thông tin về mỗi kết nối, bao gồm mô tả socket, địa chỉ peer, và cờ `is_outgoing` để phân biệt giữa socket đi (`is_outgoing = 1`) và socket đến (`is_outgoing = 0`).
  - Cờ `is_outgoing` giúp xác định socket nào dùng để gửi tin nhắn (đi) và socket nào dùng để nhận tin nhắn (đến).

- **Biến toàn cục**:
  ```c
  Connection connections[MAX_CLIENTS];
  int connection_count = 0;
  int server_socket, listening_port;
  pthread_mutex_t lock;
  ```
  - `connections`: Mảng lưu trữ tất cả các kết nối đang hoạt động (tối đa `MAX_CLIENTS = 10`).
  - `connection_count`: Theo dõi số lượng kết nối hiện tại.
  - `server_socket`: Socket chính của server để chấp nhận các kết nối đến.
  - `listening_port`: Cổng mà peer lắng nghe các kết nối đến.
  - `lock`: Khóa mutex để đảm bảo truy cập an toàn luồng vào mảng `connections`.

### 2. **Hàm main**
Hàm `main()` khởi tạo socket server, thiết lập một luồng để nhận tin nhắn, và cung cấp giao diện dòng lệnh để người dùng tương tác.

- **Thiết lập socket server**:
  ```c
  listening_port = atoi(argv[1]);
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;
  setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  struct sockaddr_in server_addr = {AF_INET, htons(listening_port), inet_addr("192.168.90.135")};
  bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  listen(server_socket, MAX_CLIENTS);
  ```
  - Tạo một socket TCP và thiết lập tùy chọn `SO_REUSEADDR` để cho phép tái sử dụng cổng ngay sau khi chương trình thoát.
  - Gắn socket với địa chỉ IP `192.168.90.135` và cổng được chỉ định.
  - Lắng nghe các kết nối đến với hàng đợi tối đa `MAX_CLIENTS`.

- **Luồng nhận tin nhắn**:
  ```c
  pthread_mutex_init(&lock, NULL);
  pthread_t recv_thread;
  pthread_create(&recv_thread, NULL, receive_messages, NULL);
  ```
  - Khởi tạo mutex để đảm bảo an toàn luồng.
  - Tạo một luồng riêng (`recv_thread`) để xử lý các kết nối và tin nhắn đến bằng hàm `receive_messages()`.

- **Giao diện dòng lệnh**:
  ```c
  while (1)
  {
      printf(">");
      fgets(command, BUFFER_SIZE, stdin);
      command[strcspn(command, "\n")] = 0;
      // Xử lý các lệnh: help, myip, myport, connect, list, terminate, send, exit
  }
  ```
  - Liên tục đọc lệnh từ người dùng và xử lý chúng bằng các hàm như `connect_to_peer()`, `send_message()`, `list_connections()`, và `terminate_connection()`.

### 3. **Quản lý kết nối**

#### **Thiết lập kết nối (`connect_to_peer`)**
- **Chức năng**:
  ```c
  void connect_to_peer(char *ip, int port)
  ```
  - Khởi tạo kết nối đến một peer khác tại IP và cổng được chỉ định.
  - Triển khai cơ chế thử lại (tối đa 5 lần, mỗi lần cách nhau 1 giây) để xử lý các lỗi kết nối tạm thời.
  - Gửi tin nhắn `PORT:<port>` đến peer để chia sẻ cổng lắng nghe của mình, cho phép thiết lập giao tiếp hai chiều.
  - Thêm kết nối mới vào mảng `connections` với `is_outgoing = 1`.

- **Cơ chế thử lại**:
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
  - Đảm bảo kết nối được thiết lập một cách đáng tin cậy bằng cách thử lại nếu lần kết nối đầu tiên thất bại.

#### **Liệt kê kết nối (`list_connections`)**
- **Chức năng**:
  ```c
  void list_connections()
  ```
  - Hiển thị danh sách các kết nối đang hoạt động, đảm bảo không có kết nối trùng lặp bằng cách so sánh IP và cổng.
  - Mỗi kết nối được gán một ID duy nhất để sử dụng trong các lệnh như `send` và `terminate`.

- **Lọc trùng lặp**:
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
  - Đảm bảo mỗi peer chỉ xuất hiện một lần trong danh sách, mặc dù có thể có nhiều socket (đến và đi) cho cùng một peer.

#### **Ngắt kết nối (`terminate_connection`)**
- **Chức năng**:
  ```c
  void terminate_connection(int id)
  ```
  - Ngắt kết nối được chỉ định bởi ID trong danh sách hiển thị (`list`).
  - Gửi tin nhắn `TERMINATE` đến peer để thông báo về việc ngắt kết nối.
  - Đóng tất cả socket (cả đến và đi) liên quan đến peer đó và cập nhật mảng `connections`.

- **Thông báo**:
  ```c
  if (outgoing_socket != -1)
  {
      const char *terminate_msg = "TERMINATE";
      send(outgoing_socket, terminate_msg, strlen(terminate_msg), 0);
  }
  ```
  - Đảm bảo peer từ xa được thông báo về việc ngắt kết nối, cho phép nó cập nhật danh sách kết nối của mình.

### 4. **Xử lý tin nhắn**

#### **Gửi tin nhắn (`send_message`)**
- **Chức năng**:
  ```c
  void send_message(int id, char *message)
  ```
  - Gửi tin nhắn đến peer tương ứng với ID được chỉ định trong danh sách hiển thị (`list`).
  - Sử dụng logic lọc trùng lặp tương tự `list_connections()` để ánh xạ ID đến peer đúng.
  - Gửi tin nhắn qua socket đi (`is_outgoing = 1`) để đảm bảo giao tiếp chính xác.

- **Ánh xạ ID**:
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
  - Ánh xạ ID do người dùng cung cấp đến peer đúng bằng cách lọc trùng lặp, đảm bảo tin nhắn được gửi đến đúng đối tượng.

#### **Nhận tin nhắn (`receive_messages`)**
- **Chức năng**:
  ```c
  void *receive_messages(void *arg)
  ```
  - Chạy trong một luồng riêng để xử lý các kết nối và tin nhắn đến.
  - Sử dụng hệ thống gọi `select()` để theo dõi nhiều socket (socket server và socket client) để phát hiện hoạt động.
  - Xử lý ba loại sự kiện:
    1. **Kết nối mới**: Chấp nhận các kết nối đến và xử lý tin nhắn `PORT:<port>` để thiết lập giao tiếp hai chiều.
    2. **Tin nhắn**: Nhận và hiển thị tin nhắn từ các peer.
    3. **Ngắt kết nối**: Phát hiện các kết nối bị đóng và cập nhật mảng `connections`.

- **Giao tiếp hai chiều**:
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
  - Khi một peer kết nối, nó gửi cổng lắng nghe của mình qua tin nhắn `PORT:<port>`. Peer nhận sẽ sử dụng cổng này để kết nối ngược lại, thiết lập giao tiếp hai chiều.

- **Xử lý ngắt kết nối**:
  ```c
  if (strcmp(buffer, "TERMINATE") == 0)
  {
      char *peer_ip = inet_ntoa(connections[i].address.sin_addr);
      int peer_port = ntohs(connections[i].address.sin_port);
      printf("Received TERMINATE from %s:%d, closing connection...\n", peer_ip, peer_port);
      // Đóng tất cả socket liên quan
  }
  ```
  - Xử lý tin nhắn `TERMINATE` từ peer để đóng kết nối và cập nhật danh sách kết nối.

### 5. **An toàn luồng**
- **Khóa Mutex**:
  ```c
  pthread_mutex_t lock;
  pthread_mutex_lock(&lock);
  pthread_mutex_unlock(&lock);
  ```
  - Khóa `lock` đảm bảo truy cập an toàn luồng vào mảng `connections`, vốn được chia sẻ giữa luồng chính (để gửi tin nhắn và quản lý kết nối) và luồng `receive_messages` (để xử lý tin nhắn và kết nối đến).

### 6. **Xử lý lỗi**
- Chương trình bao gồm xử lý lỗi cho các thao tác socket (`socket()`, `bind()`, `listen()`, `connect()`, `send()`, `recv()`), tạo luồng, và giới hạn kết nối.
- Ví dụ:
  ```c
  if (connection_count >= MAX_CLIENTS)
  {
      printf("Connection limit reached!\n");
      return;
  }
  ```
  - Ngăn chương trình vượt quá số lượng kết nối tối đa (`MAX_CLIENTS`).

## Hướng dẫn sử dụng

1. **Biên dịch chương trình**:
   ```bash
   gcc -o chat chat.c -pthread
   ```

2. **Chạy chương trình**:
   - Khởi động một peer trên một cổng cụ thể:
     ```bash
     ./chat 4001
     ```
   - Khởi động một peer khác trên cổng khác:
     ```bash
     ./chat 5000
     ```

3. **Kết nối các peer**:
   - Trên peer đầu tiên (cổng 4001), kết nối đến peer thứ hai (cổng 5000):
     ```
     > connect 192.168.90.135 5000
     ```

4. **Liệt kê kết nối**:
   ```
   > list
   Active Connections:
   1: 192.168.90.135 5000
   ```

5. **Gửi tin nhắn**:
   - Gửi tin nhắn đến peer với ID 1:
     ```
     > send 1 Xin chào
     Message sent to 192.168.90.135:5000
     ```

6. **Ngắt kết nối**:
   - Ngắt kết nối với ID 1:
     ```
     > terminate 1
     Sent TERMINATE message to 192.168.90.135:5000
     Connection 1 terminated.
     ```

7. **Thoát chương trình**:
   ```
   > exit
   Closing all connections and exiting...
   ```

## Lưu ý
- **Địa chỉ IP**: Chương trình được mã hóa cứng để sử dụng địa chỉ IP `192.168.90.135`. Sửa `server_addr` trong `main()` để sử dụng IP khác nếu cần.
- **Xung đột cổng**: Đảm bảo cổng được chỉ định không bị sử dụng bởi tiến trình khác. Tùy chọn `SO_REUSEADDR` giúp giảm xung đột cổng.
- **Tường lửa**: Đảm bảo các cổng được sử dụng (ví dụ: 4001, 5000) được mở trong cài đặt tường lửa để cho phép kết nối đến.

## Cải tiến trong tương lai
- Thêm hỗ trợ phát hiện địa chỉ IP động (ví dụ: sử dụng `getifaddrs()` để tự động phát hiện IP cục bộ).
- Triển khai giao thức tin nhắn mạnh mẽ hơn (ví dụ: với tiêu đề tin nhắn để xác định loại và độ dài).
- Thêm hỗ trợ gửi tin nhắn broadcast đến tất cả các peer đã kết nối.
- Cải thiện xử lý lỗi cho các gián đoạn mạng và thời gian chờ.

---