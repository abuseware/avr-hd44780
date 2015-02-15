#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "HD44780.h"

HD44780 *lcd;

char timer[16];

uint8_t char_array_left[8] = {0x2,0x6,0xa,0x12,0xa,0x6,0x2,0x0};
uint8_t char_array_right[8] = {0x8,0xc,0xa,0x9,0xa,0xc,0x8,0x0};

ISR(TIMER2_OVF_vect){

  static uint8_t overflow, w, d, h, m, s;

  if(++s > 59){
    s = 0;
    if(++m > 59){
      m = 0;
      if(++h > 23){
        h = 0;
        if(++d > 6){
          d = 0;
          if( ++w > 9){
            w = 0;
            overflow = 1;
          }
        }
      }
    }
  }

  sprintf(timer, "%s%uw %ud %02u%s%02u%s%02u", overflow ? "+" : " ", w, d, h, (s % 2) ? ":" : " ", m, (s % 2) ? ":" : " ", s);
  lcd_update(lcd, 1, 0, timer, 15);
}

int main(void){
  set_sleep_mode(SLEEP_MODE_IDLE);

  lcd = lcd_init(0x20);
  lcd_set_char(lcd, 1, char_array_left);
  lcd_set_char(lcd, 2, char_array_right);

  lcd_update(lcd, 0, 3, "\1 Uptime \2", 10);
  lcd_update(lcd, 1, 4, "by Licho", 8);
  lcd_refresh(lcd);

  ASSR |= (1<< AS2); //Timer2 asyncrhonous from 32.768kHz on XTAL
  TCCR2 |= (1 << CS22 | 1 << CS20); //Timer2 with XTAL/128
  TIMSK |= (1 << TOIE2); //Interrupt on match
  
  sei();
  
  _delay_ms(3000);

  while(1){
    sleep_mode();
    lcd_refresh(lcd);
  }

  lcd_free(lcd);
    
}
