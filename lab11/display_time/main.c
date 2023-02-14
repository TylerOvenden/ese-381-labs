//***************************************************************************
//
// File Name            : display_time.c
// Title                :
// Date                 : 4/20/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  
//
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
#include "DS1306_RTCDrivers.h"
#include "DOG_LCD.h"


#include <avr/interrupt.h>
#include<stdio.h>
#include <util/delay.h>
#define ALARM0SEC_write 0x87

void initInterupt();


int main(void)
{
	
	
	SPI_rtc_ds1306_config();
	initInterupt();
	sei();
	/* Replace with your application code */
	while (1)
	{
		
		asm volatile ("nop");
		
		
	}
}

//***************************************************************************
//
// Function Name        : "ISR (PORTF_PORT_vect)  "
// Date                 : 3/30/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: ISR that is triggered by INTA (PF2)
// reads the switch values at this interupt through 
// the INTCAPA register & calls the function for writing these
//  values to PORTB of MCP23S17 to display it on the LEDs
// clears the flags after this.
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
ISR (PORTC_PORT_vect) {
	cli();
	//clears the flags
	
	 write_read_RTC_test();
	update_lcd_dog();
	sei();
 PORTC_INTFLAGS = PORT_INT2_bm;
	return;
}


void initInterupt(){
		PORTC_DIR = 0x00;
		
	PORTC_PIN2CTRL = PORT_ISC_LEVEL_gc | 0x08;

		PORTC_PIN3CTRL = PORT_ISC_LEVEL_gc | PORT_PULLUPEN_bm;
//make sure WP bit 0
		write_RTC (CTRL_REG, 0x00);
		write_RTC (CTRL_REG, 0x01);
		write_RTC (ALARM0SEC_write, 0x01);

}