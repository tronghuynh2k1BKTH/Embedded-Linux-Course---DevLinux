obj-m += mgpio.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo insmod mgpio.ko

remove:
	sudo rmmod mgpio

test:
	dmesg | tail -n 10
