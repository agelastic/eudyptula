CFLAGS_helloworld.o = -DDEBUG

obj-m += helloworld.o

KDIR ?= /lib/modules/$(shell uname -r)/build
# KDIR ?=../linux
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
