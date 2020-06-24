

#ifndef UART_H_
#define UART_H_
#include "stdmacros.h"

void UART_INIT();
void UART_TX(uint8_t data);
uint8_t UART_RX();

#endif 