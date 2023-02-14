/***********
*********************************************************
 * File:           USART_asynch3_test.c
 * Author:      Tyler Ovenden 112122685
 * Created:     3/23/2021
 *
 * Notes: 
 * Description: transmits & recievers a-z in a loop, when each letter is
 transmitted, this letter is read from the recieve register & capitalized to be 
 written back
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
volatile char alpha[26] = {'a', 'b','c','d','e','f','g',
'h','i','j','l','m','o','p','q','r','s','t','u','v','w','x','y','z'};


volatile uint8_t temp;
volatile uint8_t pass;

void asynch_test(char );
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


	int i = 0;
	/* Replace with your application code */
	while (1)
	{
	//loops a-z, % used to seamlessly repeat 
		asynch_test(alpha[i%26]);
		i++;
	}
}

void asynch_test(char c) {

	temp = USART3_STATUS;
	temp &= SPI_DREIF_bm;
	//polls DRIEF to determine if you can write
	while(temp != SPI_DREIF_bm){
		_delay_us(10);
		temp = USART3_STATUS;
		temp &= SPI_DREIF_bm;
		
	}
	
	pass = (uint8_t)c;
	USART3.TXDATAL = pass;
	
	temp = USART3_STATUS;
	temp &= SPI_TXCIF_bm;
	//polls TXCIF to check if transmit is complete
	while(temp != SPI_TXCIF_bm){
		_delay_us(10);
		temp = USART3_STATUS;
		temp &= SPI_TXCIF_bm;
		
	}
	
	temp = USART3_STATUS;
	temp &= SPI_RXCIF_bm;
	//polls DRIEF to determine if you can write
	while(temp != SPI_RXCIF_bm){
		_delay_us(10);
		temp = USART3_STATUS;
		temp &= SPI_RXCIF_bm;
		
	}
	pass = USART3.RXDATAL;
	//capitalizes letter
	pass += 0x20;
	
	//writes it back
	USART3.TXDATAL = pass;
	
	
}