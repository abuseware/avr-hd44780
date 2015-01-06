#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "PCF8574/PCF8574.h"
#include "HD44780.h"

static void lcd_write_nibble(uint8_t data){
  uint8_t i;

  for(i = 0; i < 4; i++){
    if(((data >> i) & 1) == 1){
      pcf8574_pin_on(HD44780_ADDR, HD44780_DATA + i);
    } else {
      pcf8574_pin_off(HD44780_ADDR, HD44780_DATA + i);
    }
  }
}
static void lcd_write(uint8_t data){
  lcd_write_nibble(data >> 4);
  pcf8574_pin_blink(HD44780_ADDR, HD44780_E);
  
  lcd_write_nibble(data);
  pcf8574_pin_blink(HD44780_ADDR, HD44780_E);
  
}

void lcd_write_command(uint8_t data){
  pcf8574_pin_off(HD44780_ADDR, HD44780_RS);
  lcd_write(data);
}

void lcd_write_data(uint8_t data){
  pcf8574_pin_on(HD44780_ADDR, HD44780_RS);
  lcd_write(data);
}

void lcd_write_text(char *data){
  while(*data)
    lcd_write_data(*data++);
}

void lcd_position(uint8_t x, uint8_t y){
  lcd_write_command(HD44780_DDRAM_SET | (x + (0x40 * y)));
}

void lcd_clear(void){
  lcd_write_command(HD44780_CLEAR);
  _delay_ms(2);
}

void lcd_home(void){
  lcd_write_command(HD44780_HOME);
  _delay_ms(2);
}

void lcd_init(void){
  uint8_t i;

  for(i = 0; i < 3; i++) {
    lcd_write_nibble(0x03);
    pcf8574_pin_blink(HD44780_ADDR, HD44780_E);
    _delay_ms(5);
  }

  lcd_write_nibble(0x02);
  pcf8574_pin_blink(HD44780_ADDR, HD44780_E);
  _delay_ms(2);
  
  lcd_write_command(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT);
  lcd_write_command(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF);
  lcd_write_command(HD44780_CLEAR);
  lcd_write_command(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);
  lcd_write_command(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK);
}
