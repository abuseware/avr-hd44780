#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <PCF8574.h>
#include "HD44780.h"

static void lcd_write_nibble(HD44780 *device, uint8_t data){
  uint8_t i;

  for(i = 0; i < 4; i++){
    if(((data >> i) & 1) == 1){
      pcf8574_pin_on(device->controller, HD44780_DATA + i);
    } else {
      pcf8574_pin_off(device->controller, HD44780_DATA + i);
    }
  }
}

static void lcd_write(HD44780 *device, uint8_t data){
  lcd_write_nibble(device, data >> 4);
  pcf8574_pin_blink(device->controller, HD44780_E);

  lcd_write_nibble(device, data);
  pcf8574_pin_blink(device->controller, HD44780_E);

}

static void lcd_write_command(HD44780 *device, uint8_t data){
  pcf8574_pin_off(device->controller, HD44780_RS);
  lcd_write(device, data);
}

static void lcd_write_data(HD44780 *device, uint8_t data){
  pcf8574_pin_on(device->controller, HD44780_RS);
  lcd_write(device, data);
}

void lcd_set_char(HD44780 *device, uint8_t index, uint8_t *data){
  if(index > 7) return;

  lcd_write_command(device, HD44780_CGRAM_SET | (index * 8));
  for(uint8_t i = 0; i < 8; i++){
    lcd_write_data(device, data[i]);
  }
}

void lcd_update(HD44780 *device, uint8_t y, uint8_t x, char *data, uint8_t length){
  if(length == 0){
    length = strlen(data);
  }
  memcpy(&device->buffer[y][x], data, length);
}

void lcd_refresh(HD44780 *device){
  for(uint8_t y = 0; y < HD44780_LINES; y++){
    lcd_write_command(device, HD44780_DDRAM_SET | (0x40 * y));
    for(uint8_t x = 0; x < HD44780_CHARS; x++){
      lcd_write_data(device, device->buffer[y][x]);
    }
  }
}

void lcd_clear(HD44780 *device){
  for(uint8_t i = 0; i < HD44780_LINES; i++){
    memset(device->buffer[i], 0x20, HD44780_CHARS);
  }
  lcd_write_command(device, HD44780_CLEAR);
  _delay_ms(2);
}

HD44780 *lcd_init(uint8_t addr){
  HD44780 *device = (HD44780 *)malloc(sizeof(HD44780));
  device->controller = pcf8574_init(addr);

  for(uint8_t i = 0; i < HD44780_LINES; i++){
    memset(device->buffer[i], 0x20, HD44780_CHARS);
  }

  for(uint8_t i = 0; i < 3; i++) {
    lcd_write_nibble(device, 0x03);
    pcf8574_pin_blink(device->controller, HD44780_E);
    _delay_ms(5);
  }

  lcd_write_nibble(device, 0x02);
  pcf8574_pin_blink(device->controller, HD44780_E);
  _delay_ms(2);

  lcd_write_command(device, HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT);
  lcd_write_command(device, HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF);
  lcd_write_command(device, HD44780_CLEAR);
  lcd_write_command(device, HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);
  lcd_write_command(device, HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK);

  return device;
}

void lcd_free(HD44780 *device){
  pcf8574_free(device->controller);
  free(device->buffer);
  free(device);
}
