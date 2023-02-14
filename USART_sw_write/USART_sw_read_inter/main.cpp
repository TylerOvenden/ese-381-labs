/***********
*********************************************************
 * File:           asynchronous_sw_read_interrupt.c
 * Author:      Tyler Ovenden
 * Created:     3/16/2021
 *
 * Notes: 
 * Description: sets frequency to 1mHZ for oscillator,
  sets CLK_OUT to CLK_PER & selects the oscillator as main clock
 *
 *******************************************************************/

#define F_CPU 4000000
#define DELAY 208.4
#include <avr/io.h>
#include <avr/interrupt.h>
#include<stdio.h>
#include <util/delay.h>
#include<stdlib.h>

char USART_sw_read();


volatile char cha;
volatile uint8_t use;
volatile uint8_t tempuse;
int main(void)
{
    /* Replace with your application code */
    PORTB_DIR = 0xFE;
    PORTB_PIN1CTRL = 0x08 || 0x03;
    
	
	sei();
	while (1) 
    {
		
		asm volatile ("nop");
    }
}

ISR (PORTB_PORT_vect) {
    cli();
	char res = USART_sw_read();
	
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