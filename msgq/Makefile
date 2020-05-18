#CPPFLAGS = g++ -ansi -o reader -pedantic -Wall
#reader: reader.cpp mq.h
#	$(CPPFLAGS) reader.cpp
# this is a minimal makefile for demo purposes, keep it that way...
obj-m:=ipc_msgq.o
ipc_msgq-objs:=msgq.o
V?=0
KDIR?=/lib/modules/$(shell uname -r)/build


# in an embedded system:
#KDIR:=[where the system maker gave me kernel headers+makefile or the kernel itself which has a makefile]
#ARCH:=arm
#CROSS_COMPILE:=[path+common prefix to where the toolchain is]
# You could also use W=2 and W=3 for more warnings but most kernel headers
# fail those...

W?=1

.PHONY: all
all: modules writer reader

.PHONY: modules
modules:
	$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) W=$(W) modules

.PHONY: clean
clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) clean
	rm -f *.stamp
	rm -f writer
	rm -f reader

writer: writer.cpp
	g++ -O2 -Wall -Werror -ansi -pedantic -o $@ $<

reader: reader.cpp
	g++ -O2 -Wall -Werror -ansi -pedantic -o $@ $<

.PHONY: insmod
insmod:
	sudo insmod ./ipc_msgq.ko
	sleep 1
	sudo chmod 666 /dev/ipc_msgq*

.PHONY: rmmod
rmmod:
	sudo rmmod ipc_msgq

.PHONY: write
write:
	for num in $$(seq 1 5);\
	do \
	./writer /dev/ipc_msgq0;\
	done

.PHONY: read
read:
	for num in $$(seq 1 5);\
	do \
	./reader /dev/ipc_msgq0;\
	done

.PHONY: clear
clear:
	sudo dmesg --clear