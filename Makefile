CFLAGS=-Os -std=c99 -pedantic -mmcu=atmega8a -DF_CPU=8000000UL

all: bin hex

bin:
	avr-gcc ${CFLAGS} -IPCF8574 -IPCF8574/I2C  PCF8574/I2C/I2C.c  PCF8574/PCF8574.c HD44780.c HD44780_test.c -o HD44780_test.bin

hex:
	avr-objcopy -O ihex HD44780_test.bin HD44780_test.hex

flash:
	avrdude -c usbasp -p m8 -U flash:w:HD44780_test.hex
