
concept:
    ./temp 4000 -> create a thread and create socket on thread
-main:
    - handle argument port -> create thread
	- create socket, bind
	- while(1)
		- check user type:
            - help: show all cmd definded
            - myip: show ip of thread
            - list: show all socket connected
            - connect <ip> <port>: to connect to this socket
            - terminate <ip> <port>: to terminate this socket
            - send <ip> <port> <message>: to send a message to this socket
            - exit: end all socket in this process (temp.c)

-----------------
HUONG TIEP CAN

1. create socket TCP & server
- Dùng **socket()** để tạo socket TCP.
- Dùng **bind()** để liên kết socket với cổng chỉ định.
- Dùng **listen()** để cho phép các máy khác kết nối.
- Dùng **accept()** để chấp nhận kết nối mới.


----------------- common cmds
check process has port 4000
    sudo netstat -tulnp | grep :4000

check & stop process with PID
    ps -aux | grep 46051
    kill -9 46051

else if (strncmp(command, "myip", 4) == 0) {system("hostname -I");} 
else if (strncmp(command, "myport", 6) == 0) {printf("Port: %d\n", listening_port);} 

watch -n 1 "ps aux | grep chat | grep -v grep"

----------------- QnA

Không cần khai báo trước 3 socket cố định!
    - Chỉ cần **1 socket “listen”** để chờ kết nối.
    - Khi có client kết nối đến, **tạo thêm socket mới động (dynamically)** để xử lý từng kết nối.

open many socket to listen on 1 process?

code hiện tại không đảm bảo peer to peer
    Cách sửa để chuyển thành Peer-to-Peer (P2P)
	•	Cả hai process vừa làm server, vừa làm client.
	•	Mỗi process cần:
	1.	Tạo socket, bind(), listen() (để chờ kết nối từ peer).
	2.	Có thể gọi connect() đến peer khác (chứ không chỉ chờ nhận kết nối).
	3.	Gửi và nhận dữ liệu giữa các peers, không có một “Server chính”.
