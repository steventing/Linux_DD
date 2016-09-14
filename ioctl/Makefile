
ccflags-y += -Wall
CFILES = devone.c 

obj-m += sample.o
sample-objs := $(CFILES:.c=.o)

all:
		$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
		$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

