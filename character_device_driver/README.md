
# 📦 Simple Character Device Driver

## ✅ Build kernel module

```bash
make

📄 Output:

make -C /lib/modules/`uname -r`/build M=`pwd` modules
...
CC [M]  .../char_device_driver.o
...
LD [M]  .../char_device_driver.ko

🔌 Insert module

sudo insmod char_device_driver.ko

📄 Kiểm tra trong log kernel:

dmesg | tail

Expect:

[xxxxx] Simple char driver loaded

📄 Tạo device node

sudo mknod /dev/simple_dev c <major> 0
sudo chmod 0777 /dev/simple_dev

🔍 Lấy major number:

cat /proc/devices | grep simple_dev

Hoặc xem bằng dmesg.

🧪 Giao tiếp user ↔ kernel

✍️ Ghi từ user vào kernel

echo "123" > /dev/simple_dev

📖 Đọc từ kernel về user

cat /dev/simple_dev
# Output: 123

⚠️ Lỗi permission?

cat /dev/simple_dev
# cat: /dev/simple_dev: Permission denied

# Fix:
sudo chmod 0777 /dev/simple_dev

❌ Remove module

sudo rmmod char_device_driver



⸻

 Kernel log sample

[53213.431426] Simple char driver loaded
[53323.389750] Device open() called
[53323.389762] Device read() called
[53323.389769] Device close() called
[53334.592899] Device open() called
[53334.592934] Device write() called
[53334.592939] Data from user: 123
[53334.592948] Device close() called
[53336.892489] Device open() called
[53336.892506] Device read() called
[53336.892522] Device read() called
[53336.892530] Device close() called
[53352.717170] Simple char driver unloaded
