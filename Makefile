obj-m += simple_chardev.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	gcc driver_runner.c -o driver_runner
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm driver_runner
