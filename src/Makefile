# src Makefile

include ../Makefile.common

OBJS+=startup.o
OBJS+=main.o
OBJS+=sys.o
OBJS+=timer.o
OBJS+=delay.o
OBJS+=mpu6050.o
OBJS+=kalman.o
OBJS+=usart.o
OBJS+=sky_mavlink.o
OBJS+=fpu.o
OBJS+=control.o
OBJS+=IMU.o

all: src

src: app.a

app.a: $(OBJS)
		$(AR) cr app.a $(OBJS)
.PHONY: src clean tshow

clean:
		rm -f app.a $(OBJS)
tshow:
		@echo "######################################################################################################"
		@echo "################# optimize settings: $(InfoTextLib), $(InfoTextSrc)"
		@echo "######################################################################################################"
