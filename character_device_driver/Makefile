# character device driver

EXTRA_CFLAGS = -Wall
obj-m = char_device_driver.o

KDIR = /lib/modules/`uname -r`/build

all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` clean