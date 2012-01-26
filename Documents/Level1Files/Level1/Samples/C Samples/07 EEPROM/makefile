# simple makefile for Atmel ATmega16 processor
#
# on the command line:
#   make all      - build the program
#   make clean    - remove all built files
#   make program  - download program (using AVRDUDE defines below)
#   make setfuse  - set fuse bits (using AVRDUDE defines below)
#
# stuff you may want to edit:
#
#   PRG         - the name of the program
#   SRC         - the list of C files to be compiled
#   MCU_TARGET  - the microprocessor for which we're building
#
#   AVRDUDE_PART        - AVRDUDE's part id for the microprocessor
#   AVRDUDE_PROGRAMMER  - the programmer used for downloading
#   AVRDUDE_PORT        - the port the programmer is plugged into
#
# more advanced stuff:
#   change to the commented LDFLAGS definition to get a .map file
#   change to the commented "all" rule to get a .lst file
#   change to the commented line for the "clean" rule to handle
#      removing the files from the previous two changes
#
# revisions:
#	10-22-05	added support for system variables for AVRDUDE_PROGRAMMER and
#					AVRDUDE_PORT

PRG            = EEPROM
SRC            = EEPROM.c Util.c
MCU_TARGET     = atmega16

OBJ            = $(SRC:.c=.o)
OPTIMIZE       = -Os
DEFS           =
LIBS           =

# compile of .c auto with $(CC) -c $(CPPFLAGS) $(CFLAGS)
# compile of .cpp auto with (CXX) -c $(CPPFLAGS) $(CXXFLAGS)
CC             = avr-gcc
CXX            = avr-g++

# Override is only needed by avr-lib build system.
override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override CPPFLAGS      = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl
#override LDFLAGS       = -Wl,-Map,$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump
AVRDUDE        = avrdude     # for downloading

# AVRDUDE definitions; m16 is the ATmega16;
# for "bascom" programmer, use lpt1/etc; for "avrisp" programmer, use com1/etc
AVRDUDE_PART       = m16
ifndef AVRDUDE_PROGRAMMER
AVRDUDE_PROGRAMMER = bascom
endif
ifndef AVRDUDE_PORT
AVRDUDE_PORT       = lpt1
endif
AVRDUDE_FLAGS      = -p $(AVRDUDE_PART) -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_PORT) 

# (this is the default rule since it's first)
all: $(PRG).elf bin
#all: $(PRG).elf lst bin

clean:
	rm -rf *.o $(PRG).elf *.bin
#	rm -rf *.o $(PRG).elf *.lst *.map *.bin

program: $(PRG).bin
	avrdude $(AVRDUDE_FLAGS) -e -U flash:w:$(PRG).bin:r

# make sure fuse bits are set for 8MHz, brown-out detect, no JTAG, no EEPROM clear
setfuse:
	avrdude $(AVRDUDE_FLAGS) -u -U lfuse:w:0xA4:m
	avrdude $(AVRDUDE_FLAGS) -u -U hfuse:w:0xD1:m

# rebuild the obj files if any .h changes
$(OBJ):		Util.h

# binary file (to download to robot)
bin:  $(PRG).bin

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# code/asm listing
lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@
