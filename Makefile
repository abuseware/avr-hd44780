MMCU=atmega328p
FREQ=8000000

TARGET=a

INCLUDES=PCF8574 PCF8574/I2C
LIBS=PCF8574/I2C/I2C.o PCF8574/PCF8574.o HD44780.c



CROSS   ?=avr-
CC      :=$(CROSS)gcc
LD      :=$(CROSS)gcc
SIZE    :=$(CROSS)size
OBJCOPY :=$(CROSS)objcopy
OBJDUMP :=$(CROSS)objdump

CFLAGS=-Os -std=c99 -pedantic -Wall -Wextra -mmcu=$(MMCU) -DF_CPU=$(FREQ)UL
CFLAGS+=$(addprefix -I,$(INCLUDES))
LDFLAGS=$(CFLAGS) -Wl,-O1,-s,--sort-common,--as-needed,-Map=$(TARGET).map

SOURCES=$(wildcard *.c)
OBJECTS:=$(OBJECTS:.c=.o)



.PHONY: all flash clean

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -MMD -MP -MF $(@:%.o=%.d) $< -c

$(TARGET).elf: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -lm $(LIBS) -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary -R .eeprom -R .fuse -R .lock -R .signature $< $@

$(TARGET).lst: $(TARGET).elf
	$(OBJDUMP) -h -S $< > $@

all: $(TARGET).hex $(TARGET).lst

flash: test
	avrdude -q -c usbasp -p m8 -U flash:w:HD44780_test.hex

clean:
	-rm -f $(addprefix $(TARGET), .elf .hex .bin .lst .map)
	-rm -f $(OBJECTS) $(OBJECTS:.o=.d)