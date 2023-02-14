//***************************************************************************
//
// File Name            : DOG_LCD.c
// Title                :
// Date                 : 4/13/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  contains all functions for working with DOG_LCD & using SPI. 
// contains functions for intializing SPI, DOG_LCD, transmiting commmands & 
// data & updating the DOG_LCD.
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
#define F_CPU 4000000


#include <avr/io.h>
#include <avr/interrupt.h>
#include<stdio.h>
#include <util/delay.h>
#include<stdlib.h>
#include "DOG_LCD.h"


//***************************************************************************
//
// Function Name        : "init_spi_lcd  "
// Date                 : 4/6/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: initializes the SPI based on specificiations, sets the
//directions for all pins used & selects DOG to be used
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void init_spi_lcd (void) {
	
		PORTA_DIR = 0xF0;
		PORTC_DIR = 0x01;
		//configures SPI
		SPI0_CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm;
		SPI0_CTRLB = SPI_MODE1_bp;
		SPI0_INTCTRL = SPI_IE_bm;
		//select DOG
		PORTA_OUT |= 0x80;
	
	
	
}



//***************************************************************************
//
// Function Name        : "lcd_spi_transmit_CMD  "
// Date                 : 4/6/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: function to transmit a command to the DOG through 
//spi. selects the proper register to work with commands and writes 
// to SPI only after the flag is set
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void lcd_spi_transmit_CMD (unsigned char cmd) {
	SPI0_DATA = 0;
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	PORTA_OUT &= 0x7F;
	PORTC_OUT &= 0x00;
	SPI0_DATA = cmd;
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	PORTA_OUT |= 0x80;
}

	//***************************************************************************
//
// Function Name        : "lcd_spi_transmit_data  "
// Date                 : 4/6/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: function to transmit a char of data to the DOG through 
//spi. selects the proper register to work with data and writes 
// to SPI only after the flag is set
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void lcd_spi_transmit_DATA (unsigned char data) {
	SPI0_DATA = 0;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
			
			
			PORTA_OUT &= 0x7F;
		PORTC_OUT |= 0x01;
			SPI0_DATA = data;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	PORTA_OUT |= 0x80;	
			
	}

//***************************************************************************
//
// Function Name        : "init_lcd_dog  "
// Date                 : 4/6/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: initializes the DOG based on the flowchart provided. uses
// the lcd_spi_transmit_CMD function to write the proper commands & adds the 
//appropirate delay after each call
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************

void init_lcd_dog (void) {
	
	init_spi_lcd();		//Initialize mcu for LCD SPI
	
	//start_dly_40ms:
	_delay_ms(40);


	//func_set1:
	lcd_spi_transmit_CMD(0x39);   // sedn function set #1
		_delay_us(40);	//delay for command to be processed


	//func_set2:
	lcd_spi_transmit_CMD(0x39);	//send fuction set #2
		_delay_us(40);	//delay for command to be processed


	//bias_set:
	lcd_spi_transmit_CMD(0x1E);	//set bias value.
	_delay_us(40);//delay for command to be processed


	//power_ctrl:
	lcd_spi_transmit_CMD(0x55);	//~ 0x50 nominal for 5V
	//~ 0x55 for 3.3V (delicate adjustment).
	_delay_us(40);	//delay for command to be processed


	//follower_ctrl:
	lcd_spi_transmit_CMD(0x6C);	//follower mode on...
	_delay_ms(300);	//delay for command to be processed


	//display_on:
	lcd_spi_transmit_CMD(0x0c);	//display on, cursor off, blink off
	_delay_us(40);	//delay for command to be processed


	//clr_display:
	lcd_spi_transmit_CMD(0x01);	//clear display, cursor home
	_delay_us(40);	//delay for command to be processed


	//entry_mode:
	lcd_spi_transmit_CMD(0x06);	//clear display, cursor home
	_delay_us(40);	//delay for command to be processed
}

//***************************************************************************
//
// Function Name        : "lcd_spi_transmit_CMD  "
// Date                 : 4/6/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: function used to update the display. sends all three buffer
//lines char by char to be transmitted using the lcd_spi_transmit_CMD &
// lcd_spi_transmit_data functions. 
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void update_lcd_dog(void) {

	init_spi_lcd();		//init SPI port for LCD.

	// send line 1 to the LCD module.
	lcd_spi_transmit_CMD(0x80);	//init DDRAM addr-ctr
	_delay_us(40);
	for (int i = 0; i < 16; i++) {
		lcd_spi_transmit_DATA(dsp_buff1[i]);
		_delay_us(40);
	}
	
	// send line 2 to the LCD module.
	lcd_spi_transmit_CMD(0x90);	//init DDRAM addr-ctr
	_delay_us(40);
	for (int i = 0; i < 16; i++) {
		lcd_spi_transmit_DATA(dsp_buff2[i]);
		_delay_us(40);
	}
	
	// send line 3 to the LCD module.
	lcd_spi_transmit_CMD(0xA0);	//init DDRAM addr-ctr
	_delay_us(40);
	for (int i = 0; i < 16; i++) {
		lcd_spi_transmit_DATA(dsp_buff3[i]);
		_delay_us(40);
	}
}
