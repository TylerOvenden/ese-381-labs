//***************************************************************************
//
// File Name            : display_time_temp_.c
// Title                :
// Date                 : 4/27/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  displays the temperature & time using one ISR connected to 1hz.
// another ISR lets users view & change values for hours, mins & seconds using 
//FSM methods & the teraterm
// Warnings             :
// Restrictions         : none
// Algorithms           : none
// References           :
//
// Revision History     : Initial version
//
//
//**************************************************************************

#include <avr/io.h>
#include "DS1306_RTCDrivers.h"
#include "DOG_LCD.h"
#include "ADC.h"
#include "USART3.h"
#include "FSM.h"


#define F_CPU 4000000
#include <avr/interrupt.h>
#include<stdio.h>
#include <util/delay.h>
#define ALARM0SEC_write 0x87
#define READ_STATUS 0x10

void initInterupt();


int main(void)
{
	
	
	SPI_rtc_ds1306_config();
	init_adc();
	initInterupt();
	init_USART();
	//initalize 
	present_state = idle;
	
	
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
// DESCRIPTION: ISR that is triggered by 1HZ (PC1)
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
	//block_read_RTC(temp, CTRL_REG, 8);
	//clear flag
	 write_read_RTC_test();
	
	
	convert();
	update_lcd_dog();
	sei();
    PORTC_INTFLAGS = PORT_INT2_bm;
	return;
}
//***************************************************************************
//
// Function Name        : "ISR(USART3_RXC_vect)  "
// Date                 : 4/27/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: ISR for USART3_RXC interrupt 
// has the user communicate with terateam by parsing the chars typed
// each char is placed into fsm methods from the fsm class
// used to 
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
ISR(USART3_RXC_vect){
	volatile char char_received;
	char_received = USART3_RXDATAL;
	if(char_received == 's')
		fsm(present_state, s);

	else if(char_received == 'h')
	fsm(present_state, h);	
	
	else if(char_received == 'm')
	fsm(present_state, m);
	
	//char for secs since s taken
	else if(char_received == 'c')
	fsm(present_state, sec);
	
	//check if digit entered
	else if((char_received >= '0') && (char_received <= '9')){
		digit_val = char_received & 0x0F;
		fsm(present_state, digit);
	}
	//checked if enter key pushed
	else if(char_received == 0x0d){
			fsm(present_state,enter);
		}
	else 
	//eol in fsm
	fsm(present_state, char_received);

}

//***************************************************************************
//
// Function Name        : "initInterupt  "
// Date                 : 4/20/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION:configures the 1HZ interrupt & PORTC2, the pin assigned to it
//write to the control protection bit, then the bit 1hz 
// in CTRL_REG 
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************


void initInterupt(){
		PORTC_DIR = 0x00;
		PORTC_PIN3CTRL = PORT_ISC_FALLING_gc | 0x08;
		write_RTC (CTRL_REG, 0x00);
//set 1hz interrupt
		write_RTC (CTRL_REG, 0x04);
	//	write_RTC (ALARM0SEC_write, 0x01);

}
