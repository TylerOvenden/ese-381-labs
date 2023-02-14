/***********
*********************************************************
 * File:          asynchronous_sw_read.c
 * Author:      Tyler Ovenden
 * Created:     3/16/2021
 *
 * Notes: 
 * Description: uses polling to read to detect a falling edge,
  checks if there is a false start & if there is not, reads 
  PORTB PIN1 to get the bits that represent the char and saves this char 
 *
 *******************************************************************/
#define F_CPU 4000000
#define DELAY 208.4
#include <avr/io.h>
#include<stdio.h>
#include <util/delay.h>
#include<stdlib.h>

char USART_sw_read();
volatile char cha; 
volatile uint8_t use;
volatile uint8_t tempuse;

int main(void)
{
	PORTB_DIR = 0xFC;
	PORTB_PIN1CTRL = 0x08;
	
	
    /* Replace with your application code */
    while (1) 
    {
	use = PORTB_IN;
	use &= 0x02;  //isolate PORTB1
	if(use == 0x00){
		//check for falling edge
	 _delay_us(DELAY/2);
	use = PORTB_IN;
	use &= 0x02;
	
	//checks if pin still low
		if(use == 0x00) {
			//reads 
			char res = USART_sw_read();
		}
	}
	
	
	
    }
}

char USART_sw_read(){
	use = 0x00;
	//runs 8 times to get entire 
	for(int i = 0;i< 8;i++){
		tempuse = PORTA_IN;
		//isolates pin1
		tempuse &= 0x02;
		//shifts it once to right
		tempuse >>= 1;
		//move it to the left the neccasry amount of times 
		tempuse = tempuse << i;
		
		//sets the 1th bit of the char being formed
		use |= tempuse;
		
		
		_delay_us(DELAY);
	}
	//converts the binary representation of the char into char
	cha = (char) use;
	return cha;
}