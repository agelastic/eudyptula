CFLAGS_helloworld.o = -DDEBUG

obj-m += helloworld.o

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
SMATCH := $(shell which smatch) 

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
sparse:
	$(MAKE) C=2 -C $(KDIR) M=$(PWD) modules
smatch:
	$(MAKE) C=2 CHECK="$(SMATCH) -p kernel" -C $(KDIR) M=$(PWD) modules
