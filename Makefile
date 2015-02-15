CFLAGS=-Os -std=c99 -pedantic -Wall -Wextra -mmcu=atmega8a -DF_CPU=8000000UL
LDFLAGS=-Wl,-O1,-s,--sort-common,--as-needed

all: lib test

lib:
	avr-gcc ${CFLAGS} -c -IPCF8574 HD44780.c -o HD44780.o

test: lib
	avr-gcc ${CFLAGS} ${LDFLAGS} -IPCF8574 PCF8574/I2C/I2C.o PCF8574/PCF8574.o HD44780.o HD44780_test.c -o HD44780_test.bin
	avr-objcopy -O ihex HD44780_test.bin HD44780_test.hex

flash: test
	avrdude -c usbasp -p m8 -U flash:w:HD44780_test.hex
