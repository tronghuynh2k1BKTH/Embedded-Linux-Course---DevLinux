Giải thích hàm select trong receive_message()

Mục đích của đoạn code: theo dõi nhiều socket cùng lúc, bao gồm:

- Theo dõi server_socket để phát hiện kết nối mới (khi một peer khác kết nối đến).
- Theo dõi các socket trong connections[] để phát hiện tin nhắn hoặc sự kiện ngắt kết nối từ các peer đã kết nối.

Hoạt động:
- Kiểm tra xem có dữ liệu đến (tin nhắn, thông báo, hoặc kết nối mới) trên bất kỳ socket nào không.
- Xử lý đồng thời nhiều socket mà không cần tạo luồng riêng cho từng socket, giúp tiết kiệm tài nguyên.

Tổng quan về select()
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

Tham số
- nfds: Số mô tả file (file descriptor) lớn nhất cần theo dõi, cộng thêm 1.
- readfds: Tập hợp các socket cần theo dõi sự kiện "có dữ liệu để đọc" (read events).
- writefds: Tập hợp các socket cần theo dõi sự kiện "sẵn sàng để ghi" (write events).
- exceptfds: Tập hợp các socket cần theo dõi sự kiện ngoại lệ (exception events).
- timeout: Thời gian chờ tối đa (nếu NULL, select() sẽ chặn vô thời hạn).

fd_set
- là một kiểu dữ liệu được định nghĩa trong lập trình socket trên các hệ điều hành POSIX (như Linux, Unix, macOS). Nó được sử dụng cùng với hàm select() để quản lý và theo dõi nhiều mô tả file (file descriptor) cùng lúc.



Lưu ý
- fd_set là một bitmask: Bạn không thể truy cập trực tiếp vào các bit trong fd_set (như một mảng thông thường). Thay vào đó, bạn phải sử dụng các macro như FD_SET, FD_CLR, FD_ISSET để thao tác.
- Hiệu suất: Với số lượng socket nhỏ (như trong chương trình của bạn, tối đa MAX_CLIENTS = 10), fd_set và select() hoạt động tốt.

https://www.notion.so/duancuahuynh/project-socket-application-1b935291510881089ed0e2b8fc9aa738?p=1bf35291510880299866f82696bb9d04&pm=c