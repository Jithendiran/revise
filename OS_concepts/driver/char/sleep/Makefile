FILNAM := signal_dr
obj-m := $(FILNAM).o
CONFIG_MODULE_SIG=n

KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
DISTDIR := build

# Add debugging and optimization flags
# EXTRA_CFLAGS += -g -O0 -fno-inline-functions -fno-optimize-sibling-calls


modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 

install:
	insmod $(FILNAM).ko

uninstall:
	rmmod $(FILNAM).ko

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.mod modules.order *.symvers *.dwo *.o.d .*.o.d

.PHONY: modules  clean
