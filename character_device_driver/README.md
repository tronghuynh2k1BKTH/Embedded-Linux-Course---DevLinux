# README.md: Character Device Driver Example

## Build the Driver

```bash
make -C /lib/modules/`uname -r`/build M=`pwd` modules
make[1]: Entering directory '/usr/src/linux-headers-5.4.0-214-generic'
  CC [M]  /home/huynh/ref-git-linux-course/Linux-Bringup-N-Drivers/01-character-device-driver/my_ex/char_device_driver.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC [M]  /home/huynh/ref-git-linux-course/Linux-Bringup-N-Drivers/01-character-device-driver/my_ex/char_device_driver.mod.o
  LD [M]  /home/huynh/ref-git-linux-course/Linux-Bringup-N-Drivers/01-character-device-driver/my_ex/char_device_driver.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.4.0-214-generic'
```

## Install and Test the Driver

```bash
ll /dev/simple_dev 
crw------- 1 root root 235, 0 Apr 24 15:34 /dev/simple_dev

cat /dev/simple_dev 
cat: /dev/simple_dev: Permission denied

sudo chmod 0777 /dev/simple_dev 

cat /dev/simple_dev 

echo "123" > /dev/simple_dev 

cat /dev/simple_dev 
123
     
sudo rmmod char_device_driver 
```

## Kernel Terminal Logs

```
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
```