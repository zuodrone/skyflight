# general Makefile
# make OptLIB=0 OptSRC=0 all tshow  
include Makefile.common
LDFLAGS=$(COMMONFLAGS) -fno-exceptions -ffunction-sections -fdata-sections -L$(LIBDIR) -nostartfiles -Wl,--gc-sections,-Tlinker.ld

#LDFLAGS=$(COMMONFLAGS) -L$(LIBDIR) -Os  -g -Wall -mcpu=cortex-m3 -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant -Wall -finline-functions -Wdouble-promotion -std=c99 -fno-dwarf2-cfi-asm  -mno-thumb-interwork -ffunction-sections -fdata-sections -fno-common -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -nostartfiles
#LDLIBS+=-Wl,--gc-sections,-Tlinker.ld 
LDLIBS+=-lm -lc -lg
LDLIBS+=-lstm32

STARTUP=startup.c
PORT=port.c

all: ccrtos libs src
	$(CC) -o $(PROGRAM).elf $(LDFLAGS) \
		-Wl,--whole-archive \
			src/app.a \
			ccrtos/ccrtos.a \
		-Wl,--no-whole-archive \
			$(LDLIBS)
	$(OBJCOPY) -O ihex $(PROGRAM).elf $(PROGRAM).hex
	$(OBJCOPY) -O binary $(PROGRAM).elf $(PROGRAM).bin
#Extract info contained in ELF to readable text-files:
	arm-none-eabi-readelf -a $(PROGRAM).elf > $(PROGRAM).info_elf
	arm-none-eabi-size -d -B -t $(PROGRAM).elf > $(PROGRAM).info_size
	arm-none-eabi-objdump -S $(PROGRAM).elf > $(PROGRAM).info_code
	arm-none-eabi-nm -t d -S --size-sort -s $(PROGRAM).elf > $(PROGRAM).info_symbol

.PHONY: ccrtos libs src clean tshow

ccrtos:
	$(MAKE) -C ccrtos $@
	
libs:
	$(MAKE) -C libs $@
src:
	$(MAKE) -C src $@
clean:
	$(MAKE) -C ccrtos $@
	$(MAKE) -C src $@
	$(MAKE) -C libs $@
	rm -f $(PROGRAM).elf $(PROGRAM).hex $(PROGRAM).bin $(PROGRAM).info_elf $(PROGRAM).info_size
	rm -f $(PROGRAM).info_code
	rm -f $(PROGRAM).info_symbol
tshow:
		@echo "######################################################################################################"
		@echo "################# optimize settings: $(InfoTextLib), $(InfoTextSrc)"
		@echo "######################################################################################################"

flash:all
	./do_flash.pl $(TOP)/main.bin  
