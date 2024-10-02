FILNAM := mutex
obj-m := $(FILNAM).o
CONFIG_MODULE_SIG=n

KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
DISTDIR := build

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	insmod $(FILNAM).ko

uninstall:
	rmmod $(FILNAM).ko

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.mod modules.order *.symvers *.dwo *.o.d

.PHONY: modules  clean
