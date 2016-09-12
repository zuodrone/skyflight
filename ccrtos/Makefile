# src Makefile

include ../Makefile.common

OBJS+=arch.o
OBJS+=core.o
OBJS+=event.o
OBJS+=flag.o
OBJS+=hook.o
OBJS+=kernelHeap.o
OBJS+=mbox.o
OBJS+=mm.o
OBJS+=mutex.o
OBJS+=port.o
OBJS+=queue.o
OBJS+=sem.o
OBJS+=serviceReq.o
OBJS+=task.o
OBJS+=time.o
OBJS+=timer.o
OBJS+=utility.o

all: ccrtos

ccrtos: ccrtos.a

ccrtos.a: $(OBJS)
		$(AR) cr ccrtos.a $(OBJS)
.PHONY: src clean tshow

clean:
		rm -f ccrtos.a $(OBJS)
tshow:
		@echo "######################################################################################################"
		@echo "################# optimize settings: $(InfoTextLib), $(InfoTextSrc)"
		@echo "######################################################################################################"
