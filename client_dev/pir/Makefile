KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := pir_dev.o

PWD := $(shell pwd)

default :
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
