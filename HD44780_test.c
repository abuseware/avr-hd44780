#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include "HD44780.h"

char buff[16];

ISR(TIMER1_COMPA_vect){

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

  sprintf(buff, "%s%uw %ud %02u%s%02u%s%02u", overflow ? "+" : " ", w, d, h, (s % 2) ? ":" : " ", m, (s % 2) ? ":" : " ", s);
}

int main(void){
  HD47780 *lcd;

  set_sleep_mode(SLEEP_MODE_IDLE);

  lcd = lcd_init(0x20);
  lcd_position(lcd, 5, 0);
  lcd_write_text(lcd, "Uptime");
  lcd_position(lcd, 4, 1);
  lcd_write_text(lcd, "by Licho");

  OCR1A = 31250; // 31250 (CTC) * 256 (Prescaler) = 8M
  TCCR1B |=
    (1 << WGM12) | // Timer1 Mode 4 (CTC on OCR1A)
    (1 << CS12); //Timer1 Prescaler 256
  TIMSK |= (1 << OCIE1A); //Interrupt on match
  
  sei();
  
  _delay_ms(3000);

  while(1){
    sleep_mode();
    lcd_position(lcd, 0, 1);
    lcd_write_text(lcd, buff);
  }

  lcd_free(lcd);
    
}
