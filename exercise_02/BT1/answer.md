
### Chương trình:

1. Mở một file với cờ `O_APPEND`.
2. Dùng `lseek()` để di chuyển con trỏ về đầu file.
3. Ghi dữ liệu mới vào file.

### Run Command:

```sh
gcc bt1.c -o bt1
./bt1
cat test.txt
```

### Input:

`test.txt`

```
This is content for text.txt, BT1
```

### Output:

`test.txt`

```
This is content for text.txt, BT1HELLO
```

Mặc dù chương trình đã seek về đầu file, dữ liệu vẫn sẽ được thêm vào cuối file. Lý do là vì cờ `O_APPEND` luôn đảm bảo dữ liệu ghi vào cuối file, bất kể con trỏ file ở đâu.

### Tại sao `O_APPEND` lại hoạt động như vậy?

- Khi file được mở với `O_APPEND`, hệ thống sẽ tự động di chuyển con trỏ đến cuối file trước mỗi lần ghi (`write()`).
- Vì vậy, dù `lseek()` có thay đổi vị trí con trỏ file, `write()` vẫn sẽ ghi vào cuối file.
