//***************************************************************************
//
// File Name            : DOG_LCD_BasicTest.c
// Title                :
// Date                 : 4/6/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION: test program to confirm  the DOGM163W module was 
// properly wired & that the driver code works. writes to the 3 buffer
// strings & then calls the update_lcd_dog function to display 
// the transmitted text
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


void lcd_spi_transmit_CMD (unsigned char cmd);
void lcd_spi_transmit_DATA (unsigned char cmd);
void init_spi_lcd (void);
void init_lcd_dog (void);
void update_lcd_dog(void);


// Display buffer for DOG LCD using sprintf()
char dsp_buff1[17];
char dsp_buff2[17];
char dsp_buff3[17];
int main(void) {
	
	init_lcd_dog();
	
	while(1) {

		sprintf(dsp_buff1, "Test Message 001");
		sprintf(dsp_buff2, "Test Message 002"));
		sprintf(dsp_buff3, "Test Message 003"));
		update_lcd_dog();
	}
}
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
		PORTC_OUT &= 0x00;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready 
		SPI0_DATA = cmd;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready		
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
		PORTC_OUT |= 0x01;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
		SPI0_DATA = data;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
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

	