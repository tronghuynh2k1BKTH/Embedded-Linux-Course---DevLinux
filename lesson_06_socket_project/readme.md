
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

----------------- QnA

Không cần khai báo trước 3 socket cố định!
    - Chỉ cần **1 socket “listen”** để chờ kết nối.
    - Khi có client kết nối đến, **tạo thêm socket mới động (dynamically)** để xử lý từng kết nối.

why has a different for 1 thread has 2 port
    - side on main thread: myport is 4000
    - side on connection: list is another port is 51464
    -----> no no no

open many socket to listen on 1 process?
