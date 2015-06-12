CROSS_COMPILE = /home/peeter/arm-2009q1/bin/arm-none-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
ARM_LINUX = /home/peeter/linux-2.6.28.6-ok6410

ifneq ($(KERNELRELEASE),)
obj-m :=led_driver.o
else

all :main
	make -C $(ARM_LINUX) M=`pwd`
main : main.o
	$(CC) -o S@ $^
main.o: main.c
	$(CC) -c -o $@ $<
endif

clean:
	make -C $(ARM_LINUX) M=`pwd` clean
	rm -f *.o main
