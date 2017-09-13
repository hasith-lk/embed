# Makefile skeleton adapted from Peter Harrison's - www.micromouse.com

# MCU name and submodel
MCU      = cortex-m3
SUBMDL   = stm32f103

# toolchain (using code sourcery now)
TCHAIN = arm-none-eabi
THUMB    = -mthumb
THUMB_IW = -mthumb-interwork

# Target file name (without extension).
BUILDDIR = build
TARGET = $(BUILDDIR)\joystick
BINDIR = binaries

ST_LIB = stm32f1_lib
ST_USB = stmusb_lib
ST_STD = stmstd_lib

# Optimization level [0,1,2,3,s]
OPT ?= s
DEBUG = 
#DEBUG = dwarf-2

INCDIRS = ./$(ST_LIB) ./$(ST_USB) ./$(ST_STD)

CFLAGS = $(DEBUG)
CFLAGS += -O$(OPT)
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall -Wimplicit
#CFLAGS += -Wcast-align
CFLAGS += -Wpointer-arith -Wswitch
#CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -Wreturn-type -Wshadow -Wunused
CFLAGS += -Wa,-adhlns=$(BUILDDIR)/$(subst $(suffix $<),.lst,$<)
CFLAGS += $(patsubst %,-I%,$(INCDIRS))

# Aeembler Flags
ASFLAGS = -Wa,-adhlns=$(BUILDDIR)/$(<:.s=.lst)#,--g$(DEBUG)

LDFLAGS = -nostartfiles -Wl,-Map=$(TARGET).map,--cref,--gc-sections
LDFLAGS += -lc -lgcc



# Define programs and commands.
SHELL = sh
CC = $(TCHAIN)-gcc
CPP = $(TCHAIN)-g++
AR = $(TCHAIN)-ar
OBJCOPY = $(TCHAIN)-objcopy
OBJDUMP = $(TCHAIN)-objdump
SIZE = $(TCHAIN)-size
NM = $(TCHAIN)-nm
REMOVE = del /F /Q
REMOVEDIR = rd /S /Q
COPY = copy

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = "-------- begin --------"
MSG_ETAGS = Created TAGS File
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before:
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:

# Combine all necessary flags and optional flags.
# Add target processor to flags.
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d
ALL_CFLAGS  = -g -mcpu=$(MCU) $(THUMB_IW) -I. $(CFLAGS) $(TARGETFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -g -mcpu=$(MCU) $(THUMB_IW) -I. -x assembler-with-cpp $(ASFLAGS)

# --------------------------------------------- #
# file management
# ASRC = $(ST_LIB)/c_only_startup.s $(ST_LIB)/cortexm3_macro.s
ASRC = $(ST_LIB)\startup_stm32f10x_md.s

_STM32SRCS = system_stm32f10x.c

STM32SRCS = $(patsubst %, $(ST_LIB)/%,$(_STM32SRCS))

_STMSTDSRCS = misc.c \
stm3210b_eval.c \
stm32f10x_adc.c \
stm32f10x_bkp.c \
stm32f10x_can.c \
stm32f10x_cec.c \
stm32f10x_crc.c \
stm32f10x_dac.c \
stm32f10x_dbgmcu.c \
stm32f10x_dma.c \
stm32f10x_exti.c \
stm32f10x_flash.c \
stm32f10x_fsmc.c \
stm32f10x_gpio.c \
stm32f10x_i2c.c \
stm32f10x_iwdg.c \
stm32f10x_pwr.c \
stm32f10x_rcc.c \
stm32f10x_rtc.c \
stm32f10x_sdio.c \
stm32f10x_spi.c \
stm32f10x_tim.c \
stm32f10x_usart.c \
stm32f10x_wwdg.c

STMSTDSRCS = $(patsubst %, $(ST_STD)/%,$(_STMSTDSRCS))

_STM32USBSRCS = usb_regs.c \
usb_int.c \
usb_init.c \
usb_core.c \
usb_mem.c \
usb_sil.c

STM32USBSRCS = $(patsubst %, $(ST_USB)/%,$(_STM32USBSRCS))

# SRCS = usb.c usb_callbacks.c usb_descriptor.c main.c hardware.c dfu.c
SRCS = hw_config.c \
usb_pwr.c \
main.c \
stm32_it.c \
usb_desc.c \
usb_endp.c \
usb_istr.c \
usb_prop.c

SRC = $(SRCS) $(STM32SRCS) $(STM32USBSRCS) $(STMSTDSRCS)

# Define all object files.
_COBJ =  $(SRC:.c=.o)
_AOBJ =  $(ASRC:.s=.o)
COBJ = $(patsubst %, $(BUILDDIR)/%,$(_COBJ))
AOBJ = $(patsubst %, $(BUILDDIR)/%,$(_AOBJ))

# Define all listing files.
_LST  =  $(ASRC:.s=.lst)
_LST +=  $(SRC:.c=.lst)
LST = $(patsubst %, $(BUILDDIR)/%,$(_LST))

# Display size of file.
HEXSIZE = $(SIZE) --target=binary $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf

# go!
all: begin gccversion build sizeafter finished end
maple-mini: begin gccversion build_maple-mini sizeafter finished  copy_maple_mini end
build: elf bin lss sym

build_maple-mini: TARGETFLAGS= -DTARGET_MAPLE_MINI -DSTM32F10X_MD -DUSE_STM3210B_EVAL -DUSE_STDPERIPH_DRIVER
# Set the linker script
build_maple-mini: LDFLAGS +=-T$(ST_LIB)/c_only_md_high_density.ld

build_maple-mini: elf bin lss sym
copy_maple_mini: 
	@echo
	@echo "Copying to binaries folder"
	@echo
	$(COPY) $(TARGET).bin $(BINDIR)\$(TARGET).bin
	@echo

bin: $(TARGET).bin
elf: $(TARGET).elf
lss: $(TARGET).lss
sym: $(TARGET).sym
dfu: $(TARGET).bin
	sudo dfu-util -d 0110:1001 -a 0 -D $(TARGET).bin

begin:
	mkdir $(BUILDDIR)\stm32f1_lib
	mkdir $(BUILDDIR)\stmstd_lib
	mkdir $(BUILDDIR)\stmusb_lib
	@echo --
	@echo $(MSG_BEGIN)
	@echo $(COBJ)
	@echo --

finished:
	@echo $(MSG_ERRORS_NONE)
tags:
	etags `find . -name "*.c" -o -name "*.cpp" -o -name "*.h"`
	@echo $(MSG_ETAGS)
end:
	@echo $(MSG_END)
	@echo
sizeafter:
# @if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi
	echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo;
gccversion:
	@$(CC) --version

program:
	@echo "Flash-programming with OpenOCD"
	cp $(TARGET).bin flash/tmpflash.bin
	cd flash && openocd -f flash.cfg

program_serial:
	@echo "Flash-programming with stm32loader.py"
	./flash/stm32loader.py -p /dev/ttyUSB0 -evw build/maple_boot.bin

debug: $(TARGET).bin
	@echo "Flash-programming with OpenOCD - DEBUG"
	cp $(TARGET).bin flash/tmpflash.bin
	cd flash && openocd -f debug.cfg

install: $(TARGET).bin
	cp $(TARGET).bin build/main.bin
	openocd -f flash/perry_flash.cfg

run: $(TARGET).bin
	openocd -f flash/run.cfg

# Create final output file (.hex) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O binary $< $@

# Create final output file (.bin) from ELF output file.
%.bin: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O binary $< $@


# Create extended listing file from ELF output file.
# testing: option -C
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -D $< > $@


# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(COBJ) $(AOBJ)

%.elf:  $(COBJ) $(AOBJ)
	@echo ' '
	@echo $(MSG_LINKING) $@
	$(CC) $(THUMB) $(ALL_CFLAGS) $(AOBJ) $(COBJ) --output $@ $(LDFLAGS)	

# Compile: create object files from C source files. ARM/Thumb
$(COBJ) : $(BUILDDIR)/%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(THUMB) $(ALL_CFLAGS) $< -o $@

# Assemble: create object files from assembler source files. ARM/Thumb
$(AOBJ) : $(BUILDDIR)/%.o : %.s
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(THUMB) $(ALL_ASFLAGS) $< -o $@

clean: begin clean_list finished end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(COBJ)
	$(REMOVE) $(AOBJ)
	$(REMOVE) $(LST)
	$(REMOVE) flash/tmpflash.bin
#	$(REMOVE) $(SRC:.c=.s)
#	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) .dep/*

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Listing of phony targets.
.PHONY : all begin finish tags end sizeafter gccversion \
build elf hex bin lss sym clean clean_list program cscope

cscope:
	rm -rf *.cscope
	find . -iname "*.[hcs]" | grep -v examples | xargs cscope -R -b
