obj-m:= char.o
#obi-y := ./
#KBUILD_EXTRA_SYMBOLS = /home/rong/project/c/zhangming/
KERNELDIR = /lib/modules/$(shell uname -r)/build/

default:

	make -C $(KERNELDIR) M=$(shell pwd) modules
clean:
	make -C $(KERNELDIR) M=$(shell pwd) clean
	rm -rf *.o *.ko
