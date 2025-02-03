# BÀI THỰC HÀNH BUỔI 1

## BT1: Tạo một static library (.a)
## BT2: Tạo một shared library (.so)

**Lưu ý:** Sử dụng Makefile thực hiện tự động build tất cả các bước.

---

## Một số thông tin

- **fPIC ý nghĩa trong việc tạo object:**
  - Thông thường, mã máy sẽ sử dụng địa chỉ **tuyệt đối** (absolute address). Nhưng với PIC, trình biên dịch sử dụng địa chỉ **tương đối** (relative address).
  - Thay vì nói **“truy cập địa chỉ 0x4000”**, nó sẽ nói **“truy cập địa chỉ cách đây 100 bytes từ vị trí hiện tại”**.
  - Điều này giúp mã không bị phụ thuộc vào vị trí cố định trong bộ nhớ.

- **`rcs` là các tùy chọn cho `ar`:**
  - `r` có nghĩa là chèn các tệp vào archive, thay thế bất kỳ tệp nào có cùng tên.
  - `c` có nghĩa là tạo archive nếu nó chưa tồn tại.
  - `s` có nghĩa là tạo một chỉ mục cho archive, giúp tăng tốc quá trình liên kết.

- **Phân biệt 2 cách dùng này:**
  ```sh
  gcc $(OBJ_DIR)/main.o $(LIB_SHARED_DIR)/libhelloshared.so -o $(BIN_DIR)/shared_libs
  gcc $(OBJ_DIR)/main.o -L$(LIB_STATIC_DIR) -lhello -o $(BIN_DIR)/static_libs

---

[My Notion](https://duancuahuynh.notion.site/lession-1-18f3529151088071bf44d92bd65da83c?pvs=4)
```