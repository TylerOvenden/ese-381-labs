/***********
*********************************************************
 * File:          asynchronous_sw_send.c
 * Author:      Tyler Ovenden
 * Created:     3/16/2021
 *
 * Notes: 
 * Description: sends a given char to USART_sw_write
  which writes the binary representation of this char
  to PORTB PIN0 at the set baud rate. this method represents 
  with a 80ms delay between sending characters
 *
 *******************************************************************/
#define F_CPU 4000000
#define DELAY 208.4
#include <avr/io.h>
#include<stdio.h>
#include <util/delay.h>
#include<stdlib.h>


volatile uint8_t temp = 0x02; 
	
int main(void)
{
	PORTB_DIR = 0x01; //set portb0 as output
	PORTB_OUT = 0x01;	
	volatile char c;  
	c = 'd';

for(int i = 0; i<20; i++){

	USART_sw_write(c);
	_delay_ms(1);	
	
}
	
    /* Replace with your application code */
    while (1) 
    {

    }
	return 0;
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

