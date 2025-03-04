HUONG TIEP CAN

1. create socket TCP & server
- Dùng **socket()** để tạo socket TCP.
- Dùng **bind()** để liên kết socket với cổng chỉ định.
- Dùng **listen()** để cho phép các máy khác kết nối.
- Dùng **accept()** để chấp nhận kết nối mới.



-----------------
CAU HOI THAO LUAN

Không cần khai báo trước 3 socket cố định!

- Chỉ cần **1 socket “listen”** để chờ kết nối.
- Khi có client kết nối đến, **tạo thêm socket mới động (dynamically)** để xử lý từng kết nối.

