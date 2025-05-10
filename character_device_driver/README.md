# Simple Character Device Driver

## Build the module

```bash
make

Insert the module

sudo insmod char_device_driver.ko
dmesg | tail

Create device node (if needed)

sudo chmod 777 /dev/simple_dev

Test the device

echo "hello world" > /dev/simple_dev
cat /dev/simple_dev

Remove the module

sudo rmmod char_device_driver
