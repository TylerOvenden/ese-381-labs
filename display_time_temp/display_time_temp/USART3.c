//***************************************************************************
//
// File Name            : USART3.c
// Title                :
// Date                 : 4/27/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  class containing USART3 methods
// init_USART initializes USART3 & transmit_USART runs a USART transmission
// Warnings             :
// Restrictions         : none
// Algorithms           : none
// References           :
//
// Revision History     : Initial version
//
//
//**************************************************************************
#define F_CPU 4000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include<stdio.h>
#include <util/delay.h>
#include<stdlib.h>
#include "DOG_LCD.h"
#include "USART3.h"

//***************************************************************************
// Function Name : void init_USART()
// Target MCU : AVR128DB48 @ 4MHz
// Author : Tyler Ovenden
// DESCRIPTION
// method for properly initializing USART3 to work with terraterm
//**************************************************************************
void init_USART(){
	//configures baud rate
	USART3_BAUD = BAU;
	
	//configure ctrlc
	USART3_CTRLC =  USART_CMODE_ASYNCHRONOUS_gc |  USART_PMODE_DISABLED_gc
	| USART_CHSIZE_8BIT_gc | USART_SBMODE_1BIT_gc ;
	
	//configure reciever & transmiter
	USART3_CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	//USART3_CTRLA = USART_LBME_bm;
	
	
	//enables RXCIE interupt 
	USART3_CTRLA = USART_RXCIE_bm;


}

//***************************************************************************
// Function Name : void transmit_USART(int data)
// Target MCU : AVR128DB48 @ 4MHz
// Author : Tyler Ovenden
// DESCRIPTION
// runs a USART3 transmission to write the entered number to terreterm
//**************************************************************************

void transmit_USART(int data)
{
	uint8_t temp = USART3_STATUS;
	temp &= USART_RXCIF_bm;
	//polls DRIEF to determine if you can write
	while(temp != USART_RXCIF_bm){
		
		temp = USART3_STATUS;
		temp &= USART_RXCIF_bm;
		
	}
	//transmits the number, displays it in terraterm
	USART3.TXDATAL = data;

}
