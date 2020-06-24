
#include "uart.h"

void UART_INIT(){
	UBRRL=77;
	SETBIT(UCSRB,TXEN);
	SETBIT(UCSRB,RXEN);
	SETBIT(UCSRB,RXCIE) ; 
	
	SETBIT(UCSRC,UCSZ0);
	SETBIT(UCSRC,UCSZ1);
	sei()  ;//Enable interrupt in general;
	
}

void UART_TX(uint8_t data){
	while(!READBIT(UCSRA,UDRE));
	UDR=data;	
}

uint8_t UART_RX() {
	while(!READBIT(UCSRA,RXC));
	return UDR;
}
