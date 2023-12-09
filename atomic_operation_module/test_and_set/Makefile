obj-m := test_thread.o

KDIR    := /lib/modules/$(shell uname -r)/build


all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
    
clean:
	rm -f *.o *.ko *.mod *.mod.c Module.symvers modules.order
