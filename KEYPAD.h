

#ifndef KEYPAD_H_
#define KEYPAD_H_
#include "stdmacros.h"
#define KEYPAD_INIT_PORT DDRC=0xf0;PORTC=0x0f;

#define SIGN_TURN 3 

void keypad_init();
uint8_t keypad_read();
uint8_t *switch_function(uint8_t data) ;
void calculation_fun(void);
void checks(void);
void clear_elements_calc(void);
uint8_t *multiply_by_ten (uint8_t *data);
/*void print_elements(void);*/
#endif /* KEYPAD_H_ */