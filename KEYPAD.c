#include "KEYPAD.h"
#include "LCD.h"
#define NO_VALUE 32
#define SIGN_VAL 35
/*#define EQUAL_SIGN 65*/

const uint8_t keypad_matrix[]={1,4,7,'c', 2 , 5 , 8 ,'0', 3,6,9,'=','/','*','-','+'};
	
	volatile uint8_t keypad_count;
    uint16_t data_matrix[6] ; 
	uint8_t  sign_matrix[6];
	uint8_t array_element_turn=0 ;
	float floating_output=0.0;
	
void keypad_init(){
	   KEYPAD_INIT_PORT;
	   clear_elements_calc();
	 //  floating_output=NO_VALUE;
   }
	
uint8_t keypad_read(){
	PORTC = 0b11101111;
	for (keypad_count=0;keypad_count<4;keypad_count++)
	{if(READBIT(PINC,keypad_count)==0) return keypad_matrix[keypad_count];
	}
	_delay_ms(1);
	PORTC = 0b11011111;
	for (keypad_count=0;keypad_count<4;keypad_count++)
	{if(READBIT(PINC,keypad_count)==0) return keypad_matrix[keypad_count+4];
	}
	_delay_ms(1);
	PORTC = 0b10111111;
	for (keypad_count=0;keypad_count<4;keypad_count++)
	{if(READBIT(PINC,keypad_count)==0) return keypad_matrix[keypad_count+8];
	}
	_delay_ms(1);
	PORTC = 0b01111111;
	for (keypad_count=0;keypad_count<4;keypad_count++)
	{if(READBIT(PINC,keypad_count)==0) return keypad_matrix[keypad_count+12];
	}
	_delay_ms(1);
	return 0;
}

void checks(void)
{  
	_delay_ms(100);
	uint8_t var=NO_VALUE;
	
	uint8_t data= keypad_read();
	if(data)
	{
		if (data_matrix[0]==NO_VALUE) // CLEAR THE SCREEN AT THE BEGINNING 
		{
			lcd_write_cmd_char(0x01,0);
		}
		var=*(switch_function(data));
		
	}
	
	if (var !=NO_VALUE) // THERE IS AN ENTERED VALUE
	{
		if (var != SIGN_VAL)
		{	
			lcd_write_number(var);// handle the zero case at first input
		}
		
		else  if((data_matrix[array_element_turn] !=NO_VALUE) & (var== SIGN_VAL) ) //check if the last input isn't a sign ?
		{
			lcd_write_cmd_char(sign_matrix[array_element_turn],1);
			array_element_turn++;
		}
		else if((data_matrix[array_element_turn] ==NO_VALUE) & (var== SIGN_VAL) & array_element_turn==0 & floating_output !=0) // Accumulation of input
		{
			data_matrix[0]=floating_output;
			lcd_write_float(floating_output);
		  	lcd_write_cmd_char(sign_matrix[array_element_turn],1);
		  	array_element_turn++;
		}
		else  //NO correct input !!
		{
			lcd_write_cmd_char(0x01,0);
			clear_elements_calc();
		}
	}
     return;
}	 		

void calculation_fun(void){   //PRESCEDENCE OF SIGNS   O(N) COMPLEXITY
	
	lcd_write_cmd_char('=',1);
    
	float floating_matrix[5];
	uint8_t sign_matrix_new[5];
	sign_matrix_new[0]=0;

	uint8_t counter=0 ;
	uint8_t flag_sign =0;
	uint8_t floating_counter=0;
	uint16_t check_variable;
	check_variable=floating_output;
	uint8_t flag_new_input=0;
	
	if (  data_matrix[0] ==check_variable) // CHECK IF THERE IS AN OUTPUT STORED AFTER AN EQUAL SIGN
	{    
		floating_matrix[0]=floating_output;
		flag_new_input=1;
		
	}
	while (sign_matrix[counter] !=NO_VALUE){ // LOOP UNTIL ARRAY OF SIGNS IS EMPTY ( ONE LOOP ONLY )
		
		switch (sign_matrix[counter])  //search for * and / ,calculate and add in the new floating array
		{
			case '/' :
			
			if (1==flag_sign) // CHECK IF THERE'S * OR / BEFORE TO PREVENT INCREMENTING IN THE FLOATING ARRAY
			{
				floating_matrix[floating_counter]=(float)(floating_matrix[floating_counter] /data_matrix[counter+1]);
			}
			else{
			    if (flag_new_input==0) // IF THERE HAS BEEN STORED VALUE IN FLOATING GLOBAL VARIABLE
			    {
				floating_matrix[floating_counter]= (float)(data_matrix[counter]);
				floating_matrix[floating_counter]=floating_matrix[floating_counter] /data_matrix[counter+1];
				flag_sign=1;
							
			}
			else
			{	
				floating_matrix[floating_counter]=(float)floating_matrix[floating_counter] /(data_matrix[counter+1]);
				flag_sign=1;
			}
			flag_new_input=0;
			}
						
			break;
			
			case '*' :
			
			if (1==flag_sign)
			{	
				floating_matrix[floating_counter]=(float)floating_matrix[floating_counter] * data_matrix[counter+1];
			}
			else{
					if (flag_new_input==0)
					{
				floating_matrix[floating_counter]= (float)(data_matrix[counter]);
				floating_matrix[floating_counter]=floating_matrix[floating_counter] *data_matrix[counter+1];
				flag_sign=1;
					}
					else
					{
				floating_matrix[floating_counter]=floating_matrix[floating_counter] *data_matrix[counter+1];	
				flag_sign=1;	
					}								
			}
			flag_new_input=0;
			break;
			
			case '-' :
			if (counter==0 & flag_new_input==0)
			{
				floating_matrix[0]=data_matrix[0];
				
			}
			floating_counter++;
			if (sign_matrix[counter+1] ==NO_VALUE)
			{
				floating_matrix[floating_counter]= data_matrix[counter+1];
			}
			else{
				if (1==flag_sign) //use count+1 ;
				{
					if ((sign_matrix[counter+1] =='+') | (sign_matrix[counter+1] =='-'))
					{
						floating_matrix[floating_counter]= data_matrix[counter+1];
					}
				}
				else if (flag_sign==0 )
				{
					floating_matrix[floating_counter] =data_matrix[counter+1];
				}
			}
			sign_matrix_new[floating_counter-1]='-';
			flag_sign=0;
			break;
			
			case '+' :
			if (counter==0 & flag_new_input==0)
			{
				floating_matrix[0]=data_matrix[0];
				
			}
			floating_counter++;
			if (sign_matrix[counter+1] ==NO_VALUE)
			{
				floating_matrix[floating_counter]= data_matrix[counter+1];
			}
			else{
				if (1==flag_sign) //use count+1 ;
				{
					if ((sign_matrix[counter+1] =='+') | (sign_matrix[counter+1] =='-'))
					{
						floating_matrix[floating_counter]= data_matrix[counter+1];
					}
				}
				else if (flag_sign==0)
				{
					floating_matrix[floating_counter] =data_matrix[counter+1];
				}
			}
			sign_matrix_new[floating_counter-1]='+';
			
			flag_sign=0;
			break ;
		}
		
		counter++;
	
	}// END OF INPUT TO FLOATING ARRAY
	
	
	//ADDITION AND SUBTRACTION AFTER SORTING OF MULTPILCATION AND DIVISION
	if (sign_matrix_new[0]==0)
	{
		floating_output=floating_matrix[0];
		
	}else{
	counter=0;
	
	while (sign_matrix_new[counter]!=0)
	{	
		if (counter==0)
		{
			floating_output =floating_matrix[0];
		}
		
		switch (sign_matrix_new[counter])
		{
			case '+':
			floating_output =floating_matrix[counter+1] +floating_output;
			
			break;
			case '-' :
			floating_output =floating_output - floating_matrix[counter+1] ;
			break  ;
		}
		counter++;
		
	}
	}	
	lcd_write_cmd_char(0x01,0);
	lcd_write_float(floating_output);
	clear_elements_calc();
	sign_matrix_new[0]=0;
}


void clear_elements_calc(void){
	uint8_t counter;
	
	for(counter=0;counter<=6 ;counter++) //CLEAR THE ARRAY
	{
		data_matrix[counter]=NO_VALUE;
	    sign_matrix[counter]=NO_VALUE;

	}
	array_element_turn=0;

}

uint8_t *multiply_by_ten (uint8_t *data ) //accumulate in the uint16
{
	
	static uint8_t  flag=0;

	if(data_matrix[0] !=NO_VALUE){
	if(data_matrix[array_element_turn] <100) //limit the input to 3 digits
	{  
	data_matrix[array_element_turn]= ((data_matrix[array_element_turn])*10) + *data;
	
	}
	else
	{
	lcd_write_cmd_char(0x01,0);
	clear_elements_calc();
	flag=1;
	}
	}		
		return  &flag;
	}

 
uint8_t *switch_function(uint8_t data){
	
	 //uint8_t flag_big_num=0;
	 static uint8_t switch_input =NO_VALUE;
	 uint8_t skip_all = 0;
	 uint8_t flag_wrong_val=1 ;
	 uint8_t  wrong_sign=1;
	 if(((data>=0) & (data<=9)) | (data=='0'))
	 {
		 if (data=='0')
		 {
			 switch_input=0;
			 flag_wrong_val=0;
		 }
		 else
		 {switch_input=data ;
			 flag_wrong_val=0;
		 }
	 }
	 
	 else
	 {
		 
		 switch(data)
		 {
			 case '/' :
			 
			 wrong_sign=0;
			 switch_input='/' ;
			 break;
			 
			 case '*' :
			 
			 wrong_sign=0;
			 switch_input='*' ;
			 
			 break;
			 
			 case '-' :
			 
			 wrong_sign=0;
			 switch_input='-' ;
			 
			 break;
			 
			 case '+' :
			 
			 wrong_sign=0;
			 switch_input='+' ;
			 break;
			 
			 case '=':
			 skip_all=1;
			 calculation_fun();
			 break;
			 
			 
		 }
		 
	 } //END OF LARGE IF COND
	 if(skip_all ==0){
		 if(flag_wrong_val==0 ) //DON'T ENTER IF EQUAL SIGN IS PRESSED AS THIS IS NOT AN OPERATOR OR OPERAND
		 {
			 if(data_matrix[array_element_turn] !=NO_VALUE)
			 {
				 multiply_by_ten(&switch_input);	 
			 }
			 else
			 {
				 multiply_by_ten(&switch_input);
				 data_matrix[array_element_turn] =switch_input;	 
			 }
			 
		 }
		 else if (wrong_sign ==0)// modify
		 {
			 
			 sign_matrix[array_element_turn]=switch_input;
			 switch_input= SIGN_VAL;
			 
		 }
		 else if((flag_wrong_val==1) & (wrong_sign==1))// INPUT IS NOT IN RANGE
		 {
			 switch_input=NO_VALUE;
			 
		 }
	 }
	 else{
		 switch_input=NO_VALUE;
	 }
	 return  &switch_input;
}