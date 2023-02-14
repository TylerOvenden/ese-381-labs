/***********
*********************************************************
 * File:           USART_asynch3_test.c
 * Author:      Tyler Ovenden 112122685
 * Created:     3/23/2021
 *
 * Notes: 
 * Description:uses USART to echo by recieving an char & transmiting 
 this back
 *
 *******************************************************************/

#define F_CPU 4000000
#define BAU ((64)*(4)*1000000)/(16*9600)
#include <avr/io.h>
#include <avr/interrupt.h>
#include<stdio.h>
#include <ctype.h>
#include <util/delay.h>
#include<stdlib.h>

volatile char c;
volatile uint8_t temp;
volatile uint8_t pass;

 void asynch_test( );
int main(void)
{
		
	//configures baud rate
	USART3_BAUD = BAU;
	
	//configure ctrlc
	USART3_CTRLC =  USART_CMODE_ASYNCHRONOUS_gc ||  USART_PMODE_DISABLED_gc
	|| USART_CHSIZE_8BIT_gc ||  USART_SBMODE_1BIT_gc ;
	
	//configure reciever & transmiter
	USART3_CTRLB = USART_RXEN_bm || USART_TXEN_bm;
	
			
			
			
	PORTB_DIR = 0xFE;
	c = 'u';



    /* Replace with your application code */
    while (1) 
    {
		asynch_test();
    }
}

 void asynch_test() {

	temp = USART3_STATUS;	 
	temp &= SPI_RXCIF_bm;
	//polls DRIEF to determine if you can write
	while(temp != SPI_RXCIF_bm){
	_delay_us(10);
	temp = USART3_STATUS;
	temp &= SPI_RXCIF_bm;
	
	}	 
	
	//recieves the char
	pass = USART3.RXDATAL;
	
	temp = USART3_STATUS;
	temp &= SPI_DREIF_bm;
	//polls DRIEF to determine if you can write
	while(temp != SPI_DREIF_bm){
		_delay_us(10);
		temp = USART3_STATUS;
		temp &= SPI_DREIF_bm;
	}
	
	//transmits the char back
	USART3.TXDATAL = pass;
	
	
	 
 }