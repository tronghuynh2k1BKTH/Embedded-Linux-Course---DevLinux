Tiến trình Zombie
Định nghĩa: Tiến trình zombie là một tiến trình đã hoàn thành việc thực thi (kết thúc), nhưng thông tin về nó vẫn còn tồn tại trong bảng tiến trình của hệ điều hành.

Lý do xuất hiện: Khi một tiến trình con kết thúc, nó sẽ gửi một tín hiệu (SIGCHLD) cho tiến trình cha để thông báo về việc kết thúc. Tiến trình cha có sec "thu thập" thông tin về tiến trình con đã kết thúc để dọn dẹp. Quá trình này được gọi là "wait".

Nếu tiến trình cha không thực hiện việc "wait" này, tiến trình con đã kết thúc sẽ rơi vào trạng thái zombie. Nó vẫn tồn tại trong bảng tiến trình, nhưng không còn thực hiện bất kỳ công việc nào nữa.

-----

Tiến trình mồ côi (Orphan)

Định nghĩa: Tiến trình mồ côi là một tiến trình mà tiến trình cha của nó đã kết thúc trước khi nó kết thúc.

Lý do xuất hiện: Khi một tiến trình cha kết thúc, tất cả các tiến trình con của nó sẽ tự động được "nhận nuôi" bởi tiến trình init (PID 1). init là tiến trình đặc biệt, luôn chạy đầu tiên khi hệ thống khởi động. Nó có trách nhiệm "dọn dẹp" các tiến trình mồ côi, đảm bảo chúng không trở thành zombie.

Đặc điểm: Tiến trình mồ côi vẫn tiếp tục chạy bình thường cho đến khi nó hoàn thành nhiệm vụ hoặc bị kết thúc. Sau khi nó kết thúc, init sẽ thu thập thông tin về nó và loại bỏ nó khỏi bảng tiến trình.

