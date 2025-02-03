BÀI THỰC HÀNH BUỔI 1
BT1: Tạo một static library (.a)
BT2: Tạo một shared library (.so)
Lưu ý: Sử dụng Makefile thực hiện tự động build tất cả các bước.

-----
Một số thông tin
-----
- fPIC ý nghĩa trong việc tạo object
    Thông thường, mã máy sẽ sử dụng địa chỉ **tuyệt đối** (absolute address). Nhưng với PIC, trình biên dịch sử dụng địa chỉ **tương đối** (relative address).

    Thay vì nói **“truy cập địa chỉ 0x4000”**, nó sẽ nói **“truy cập địa chỉ cách đây 100 bytes từ vị trí hiện tại”**.

    Điều này giúp mã không bị phụ thuộc vào vị trí cố định trong bộ nhớ.

- `rcs` are options for `ar`:
    `r` means to insert the files into the archive, replacing any existing files of the same name.
    `c` means to create the archive if it does not already exist.
    `s` means to create an index for the archive, which speeds up linking.

- Phân biệt 2 cách dùng này
    gcc $(OBJ_DIR)/main.o $(LIB_SHARED_DIR)/libhelloshared.so -o $(BIN_DIR)/shared_libs
	gcc $(OBJ_DIR)/main.o -L$(LIB_STATIC_DIR) -lhello -o $(BIN_DIR)/static_libs
        lhello: Trình liên kết sẽ tìm kiếm một tệp có tên libhello.so (hoặc libhello.a nếu là thư viện tĩnh) trong thư mục được chỉ định bởi -L.

- file .h khai báo, một file c khác dùng để định nghĩa, và một hàm main để thực thi một nhiệm vụ nào đó
    .h
    function.c
    -> chỉ cần include .h này vào main là dùng được function trong main

- after make clean and make all
    run on server
    ./bin/shared_libs
    ./bin/static_libs
    the resutl, at main, call function that is writed on share lib
-----
my Notion: https://duancuahuynh.notion.site/lession-1-18f3529151088071bf44d92bd65da83c?pvs=4