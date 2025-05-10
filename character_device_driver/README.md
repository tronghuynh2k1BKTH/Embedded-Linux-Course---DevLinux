# Simple Character Device Driver Development

This document explains the steps involved in developing a simple character device driver.

---

## Steps

### ✅ 01-init-exit – Module Initialization and Exit
- Define two main functions:
    - `__init simple_init()` → Runs when you `insmod` the module.
    - `__exit simple_exit()` → Runs when you `rmmod` the module.
- **Purpose**: Register and clean up your driver with the Linux kernel.

---

### ✅ 02-major-minor – Assign Major/Minor Numbers
- Use `alloc_chrdev_region()` to dynamically allocate device numbers.
- **Major number**: Identifies the driver.
- **Minor number**: Identifies the specific device.
- Verify using `dmesg` or `cat /proc/devices`.

---

### ✅ 03-device-file – Create Device File in `/dev`
- Use `class_create()` and `device_create()` to create `/dev/simple_dev`.
- This file is used by userspace applications for read/write operations.
- If `udev` is not active, manually create the file using `mknod`.

---

### ✅ 04-file-operations – Set Up `file_operations`
- Define handlers for:
    ```c
    .open = dev_open,
    .release = dev_close,
    .read = dev_read,
    .write = dev_write
    ```
- These functions handle user interactions like `cat`, `echo`, etc.

---

### ✅ 05-fops-rw – Implement `read()` and `write()`
- `read()`:
    - Uses `copy_to_user()` to send data to userspace.
- `write()`:
    - Uses `copy_from_user()` to receive data from userspace.
- Use `offset` to track the current read/write position.
- Print debug messages to `dmesg` using `pr_info()`.

---

## Build and Test the Module

### Build the Module
```bash
make
```

### Insert the Module
```bash
sudo insmod char_device_driver.ko
dmesg | tail
```

### Create Device Node (if needed)
```bash
sudo chmod 777 /dev/simple_dev
```

### Test the Device
```bash
echo "hello world" > /dev/simple_dev
cat /dev/simple_dev
```

### Remove the Module
```bash
sudo rmmod char_device_driver
```
