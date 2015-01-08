#pragma once

#include <stdint.h>
#include <PCF8574.h>

//Config
#define HD44780_ADDR 0x20
#define HD44780_RS   0
#define HD44780_E    1
#define HD44780_DATA 4

//Maps
#define HD44780_CLEAR 0x01

#define HD44780_HOME 0x02

#define HD44780_ENTRY_MODE 0x04
#define HD44780_EM_SHIFT_CURSOR 0
#define HD44780_EM_SHIFT_DISPLAY 1
#define HD44780_EM_DECREMENT 0
#define HD44780_EM_INCREMENT 2

#define HD44780_DISPLAY_ONOFF 0x08
#define HD44780_DISPLAY_OFF 0
#define HD44780_DISPLAY_ON 4

#define HD44780_CURSOR_OFF 0
#define HD44780_CURSOR_ON 2
#define HD44780_CURSOR_NOBLINK 0
#define HD44780_CURSOR_BLINK 1

#define HD44780_DISPLAY_CURSOR_SHIFT 0x10
#define HD44780_SHIFT_CURSOR 0
#define HD44780_SHIFT_DISPLAY 8
#define HD44780_SHIFT_LEFT 0
#define HD44780_SHIFT_RIGHT 4

#define HD44780_FUNCTION_SET 0x20

#define HD44780_FONT5x7 0
#define HD44780_FONT5x10 4

#define HD44780_ONE_LINE 0
#define HD44780_TWO_LINE 8

#define HD44780_4_BIT 0
#define HD44780_8_BIT 16

#define HD44780_CGRAM_SET 0x40
#define HD44780_DDRAM_SET 0x80

//Types
typedef PCF8574 HD47780;

//Functions
HD47780 *lcd_init(uint8_t addr);
void lcd_free(HD47780 *device);

void lcd_write_command(HD47780 *device, uint8_t data);
void lcd_write_data(HD47780 *device, uint8_t data);
void lcd_write_text(HD47780 *device, char * data);

void lcd_position(HD47780 *device, uint8_t x, uint8_t y);
void lcd_clear(HD47780 *device);
void lcd_home(HD47780 *device);
