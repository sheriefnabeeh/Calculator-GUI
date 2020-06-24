/*
 * LCD.c
 *  Author: sherif othman
 */ 
#include "LCD.h"

void lcd_init(){
	SETBIT(DDRA,DDA4);//D4
	SETBIT(DDRA,DDA5);//D5		#DEFINE INIT_PORT_LCD DDRA |= 15<<4; DDRB|=7<<1   
	SETBIT(DDRA,DDA6);//D6
	SETBIT(DDRA,DDA7);//D7
	SETBIT(DDRB,DDB1);//TO SELECT COMMAND OR DATA
	SETBIT(DDRB,DDB2);//TO READ FROM LCD
	SETBIT(DDRB,DDB3);//TO ENABLE PUSHING DATA
	RW(0);
	lcd_write_cmd_char(0x02,0);
	lcd_write_cmd_char(0x01,0);
	lcd_write_cmd_char(0x0c,0);
	lcd_write_cmd_char(0x06,0);
	lcd_write_cmd_char(0x80,0);
	_delay_ms(20) ; 
}
void lcd_write_cmd_char(uint8_t data,uint8_t select){

	RS(select);
	
	D7(READBIT(data,7));
	D6(READBIT(data,6));
	D5(READBIT(data,5));
	D4(READBIT(data,4));	
	EN(1)
	_delay_ms(1);
	EN(0);
	_delay_ms(1);
	D7(READBIT(data,3));
	D6(READBIT(data,2));
	D5(READBIT(data,1));
	D4(READBIT(data,0));
	EN(1)
	_delay_ms(1);
	EN(0);
	_delay_ms(1);
	
	
}
void lcd_write_txt(char *txt){
	uint8_t i;
	for (i=0;txt[i]!='\0';i++)
	{lcd_write_cmd_char(txt[i],1);
	}
}	
void lcd_write_number(uint16_t num){
	int8_t i;
	uint8_t txt[10];
	if(0==num){
		lcd_write_cmd_char(48,1);
	}
	else{
		for(i=0;(num!=0);i++){
			txt[i]=num%10;
			num/=10;
			//lcd_write_cmd_char(num+48,1);
		}
		i--;
		for (;i>=0;i--)
		{lcd_write_cmd_char(txt[i]+48,1);
		}
	}
}
void lcd_write_float(float floating_input){
	if(floating_input<0)
	{
		lcd_write_cmd_char('-',1);
		floating_input=floating_input*-1;
	}
	uint16_t charact_part = floating_input;
	float out_float =charact_part;
	 float out1_float=(floating_input-out_float )*1000;
	uint16_t division_out=out1_float;
	lcd_write_number(charact_part);
	lcd_write_cmd_char('.',1);
	
	// IF RESULT LESS THAN 0.1 THEN UNCOMMENT THE FOLLOWING TO PRINT A ZERO  
	// OTHERWISE IT WILL PRINT 0.2 INSTEAD OF 0.02 
	 
// 	if (division_out<10)
// 	{
// 		lcd_write_cmd_char(48,1);
// 		lcd_write_cmd_char(48,1);
// 	}
// 	else
// 	if (division_out<100)
// 	{
// 		lcd_write_cmd_char(48,1);
// 	}
	lcd_write_number(division_out);

	
}
