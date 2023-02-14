/***********
*********************************************************
 * File:         interrupt_echo_line.c
 * Author:      Tyler Ovenden
 * Created:     3/17/2021
 *
 * Notes: 
 * Description: uses the USART_sw_write & interrupt triggered
 USART_sw_read methods to read a line of chars, stores these in an 
 array & writes the entire line, echoing back the chars
 *
 *******************************************************************/

#define F_CPU 4000000
#define DELAY 208.4
#include <avr/io.h>
#include <avr/interrupt.h>
#include<stdio.h>
#include <ctype.h> 
#include <util/delay.h>
#include<stdlib.h>
void USART_sw_write(char c);
char USART_sw_read();

int end = 0;
volatile char cha;
volatile char res;
volatile uint8_t use;
volatile uint8_t tempuse;
volatile int items = 0;
volatile char chars[80];
volatile uint8_t temp;
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
	
	while(end == 0) {
	res = USART_sw_read();
	//adds char to line 
	chars[items] = res;
	items++;
	//checks if control character entered
	if(res == 0x0D)
		end = 1;
	
	}
	//send every character 
	for(int i = 0; i<items-1;i++){
	//capitalizes the char
	
	USART_sw_write(toupper(chars[i]));	
	_delay_ms(80);
	}
	//clears array of characters
	items = 0;
	memset(chars, 0, sizeof chars);
	end = 0;
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
 
 
 void USART_sw_write(char c) {
	 PORTB_OUT = 0x00; //start bit
	 _delay_us(DELAY);
	 for(int i = 0; i<8; i++){
		 temp = (uint8_t) c;	//convert ascii to uint8_t
		 temp = temp >> i;   //move desired bit to front
		 temp &= 0x01;  //isolate first bit
		 PORTB_OUT = temp;
		 _delay_us(DELAY);
	 }
	 
	 PORTB_OUT = 0x01; //write end bit
	 _delay_us(DELAY);
	 
 }
