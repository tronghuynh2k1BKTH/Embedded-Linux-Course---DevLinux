line 23
if (strcmp(mode, "r") == 0)

vì mode lúc này là địa chỉ chứ không phải là nội dung
sử dụng strcmp để so sánh nội dung của mode với "r"
-----

line 30
char buffer[num_bytes + 1];
Nếu bạn đọc 5 ký tự, bạn cần 6 byte (5 + 1) để có thể lưu dữ liệu + \0.
----

line 38
buffer[bytes_read] = '\0';
Dòng buffer[bytes_read] = '\0'; được sử dụng để đảm bảo rằng chuỗi dữ liệu đọc được từ file kết thúc bằng ký tự null ('\0').