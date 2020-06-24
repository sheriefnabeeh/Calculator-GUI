

#include "stdMacros.h"
#include "EDC.h"
#include "PWM.h"
#include "LCD.h"
#include "KEYPAD.h"
#include "uart.h"

//#include "TIMER0.h"
 volatile  static uint8_t isr_flag ;
 volatile static uint8_t uart_value ; 
void checks(void);

int main(void)
{
	uint8_t flag_mode1=0 ;
	uint8_t flag_mode2=0;
	uint8_t flag_mode3=0;
	
	SETBIT(DDRD,DDD4);
	SETBIT( DDRD,DDD7);
	SETBIT( DDRD,DDD6);
	SETBIT( DDRD,DDD5);
	SETBIT( DDRD,DDD3);
	CLRBIT(MCUCR,ISC00);//SET ISC00 AND ISC01 TO GENERATE THE INTERRUPT AT THE falling EDGE
	SETBIT(MCUCR,ISC01);
	SETBIT(GICR,6);//SET GICR AT Bit 6 TO MAKE INT0: External Interrupt Request 0 Enable
	sei();//SETBIT(SREG,7);
	
	keypad_init();
	PWM_ocr1A_init();
	ADC1_init();
	UART_INIT();
	lcd_init();
	
	isr_flag=0;
	uart_value=0;
	flag_mode1=0 ;
	while(1){
		UART_TX(uart_value);
		isr_flag=(UART_RX());
		if(isr_flag==1 ){   // FIRST MODE 
			
			flag_mode2=0;
			flag_mode3=0;
			
			SETBIT(TCCR1A,COM1B1);
			CLRBIT(ADMUX,MUX0);
			if(flag_mode1==0)    // TO PREVENT PRINTING MORE THAN ONCE , CAN BE REMOVED BUT INSTEAD OF CHANGING IN A WORKING PART OF THE CODE 
			{	lcd_write_cmd_char(0x01,0);
				lcd_write_txt("mode 1");
				flag_mode1 =1;
			}

			_delay_ms(10);
			
			PWM_ocr1A_duty(ADC1_read()*(((float)100)/((float)1023)));
		}
		else
		if(isr_flag==2)
		{
			SETBIT(ADMUX,MUX0);
			CLRBIT(TCCR1A,COM1B1);
			PWM_ocr1A_duty(0);
			flag_mode1 =0;
			
			if(flag_mode2==0){
				lcd_write_cmd_char(0x01,0);
				lcd_write_txt("mode 2 " );
				flag_mode2=1 ;
			}
			_delay_ms(100);
			//lcd_write_cmd_char(0xc0,0);
			uint8_t readings=(ADC1_read()*(((float)100)/((float)1023)));
			/* lcd_write_number(readings);*/
			
			if(readings>50){   //CAN BE RESCALED 
				SETBIT(PORTD,PIND3);
				_delay_ms(10);
			}
			else {
				CLRBIT(PORTD,PIND3);
			}
			
		} 
		else  if(isr_flag==3){
			CLRBIT(PORTD,PIND3);
			if(flag_mode3==0){
				lcd_write_cmd_char(0x01,0);
				lcd_write_txt("mode 3" );
				flag_mode3=1 ;
			}
			_delay_ms(100);
			checks(); // CALCULATOR FUNCTION IN KEYPAD.C 
		}
	}
}
	  	  	  
ISR(INT0_vect)
{
	uart_value++ ;
	if(uart_value==4)
	uart_value=1;
}
ISR(USART_RXC_vect){  // NEED TO BE WRITTEN EVEN IF EMPTY 
 
}
