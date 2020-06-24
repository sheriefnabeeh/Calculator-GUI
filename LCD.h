
#ifndef LCD_H_
#define LCD_H_
#include <avr/io.h>
#include "stdmacros.h"
//#define  INIT_PORT_LCD DDRA |= 15<<4; DDRB|=7<<1

//#define RW(x) if(x==1) SETBIT(PORTB,2);else CLRBIT(PORTB,2);
#define RW(x) (x==1)?SETBIT(PORTB,2):CLRBIT(PORTB,2);
#define RS(x) (x==1)?SETBIT(PORTB,1):CLRBIT(PORTB,1);
#define EN(x) (x==1)?SETBIT(PORTB,3):CLRBIT(PORTB,3);
#define D7(x) (x==1)?SETBIT(PORTA,7):CLRBIT(PORTA,7);
#define D6(x) (x==1)?SETBIT(PORTA,6):CLRBIT(PORTA,6);
#define D5(x) (x==1)?SETBIT(PORTA,5):CLRBIT(PORTA,5);
#define D4(x) (x==1)?SETBIT(PORTA,4):CLRBIT(PORTA,4);

void lcd_init();
void lcd_write_cmd_char(uint8_t,uint8_t);
void lcd_write_txt(char *txt);
void lcd_write_number(uint16_t num);
void lcd_write_float(float floating);
#endif /* LCD_H_ */