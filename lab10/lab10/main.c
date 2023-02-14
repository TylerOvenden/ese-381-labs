//***************************************************************************
//
// File Name            : temp_meas.c
// Title                :
// Date                 : 4/13/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  uses SPI0 to write to the DOGM163W module,
// update_lcd_dog function uses helper functions,
// measures temperature through a MCP9700A connected to the AVR
// by taking the voltage & running an ADC conversion.
// does aritmatic to convert the result to BCD & then ASCII to 
// be displayed as a temperature
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
#define RES12_Bm 0x00

void lcd_spi_transmit_CMD (unsigned char cmd);
void lcd_spi_transmit_DATA (unsigned char cmd);
void init_spi_lcd (void);
void init_lcd_dog (void);
void update_lcd_dog(void);
void convert (void);  
void displayVolt(uint16_t r);
 uint32_t convertToBCD(uint16_t dec);

//void init_convert ()  void displayVolt(uint16_t r)

// Display buffer for DOG LCD using sprintf()
char dsp_buff1[17];
char dsp_buff2[17];
char dsp_buff3[17];


char hex[16] = "0123456789ABCDEF";
char tempHex[3];
volatile uint16_t result;
volatile uint8_t temp;
volatile int boolean = 0;
int main(void) {
	
	init_lcd_dog();
	init_adc();
	while(1) {

		convert();
		update_lcd_dog();
		_delay_ms(500);
	}
}
//***************************************************************************
//
// Function Name        : "init_adc  "
// Date                 : 4/13/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: initializes the ADC0's CTRLA, CTRLB, CTRLC & MUXPOS
// registers to perform a conversions. turns on the ADCO's enable bit
// & sets PORTD6 as an input 
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void init_adc (void) {
	ADC0_CTRLA = RES12_Bm | ADC_FREERUN_bm;
	ADC0_CTRLB = ADC_SAMPNUM_ACC8_gc;
	ADC0_MUXPOS = ADC_MUXPOS_AIN6_gc;
	//turns on enable bit
	ADC0_CTRLA |= ADC_ENABLE_bm;
	PORTD_DIR = 0x00; //set PORTD6 as input
	//enable pullups
	PORTD_PIN6CTRL = PORT_PULLUPEN_bm;
}

//***************************************************************************
//
// Function Name        : "convert  "
// Date                 : 4/13/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: performs ADC conversion, takes the result from the ADC0_RES
// register. first places this result in displayVolt() method to convert the 
// the original result to hex. then converts vout to mv, subtracts the offset of
// 500 from it to get the tempature. then converts this value to packed bcd 
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void convert () {
	//start conversion
	ADC0_COMMAND = ADC_STCONV_bm; 
	
	//polls for RESRDY flag
	while(ADC0_INTFLAGS != ADC_RESRDY_bm){}
	//takes result of conversion	
	result = ADC0_RES;	
	
	displayVolt(result);
	//parses the result to 32 bits for multiplication result
	volatile uint32_t mult = (uint32_t)result;
	
	//arithmetic for getting vout in mv
	mult = (mult*2500)/4096;
	
	//checks if result is > or < 500
	//-500 done to get the temperature
	if(mult > 500){
		mult -= 500;
	}
	else {
		//if mult < 500, subtract mult from 500
		//mark variable to signify result is negative
		mult = 500-mult;
		boolean = 1;
	}
	
	
	//converts result to bcd
	//parses to uint16_t since mult would never be > 500
	//from testing
	volatile uint32_t bcd = convertToBCD((uint16_t)mult);
		
}

//***************************************************************************
//
// Function Name        : "displayVolt  "
// Date                 : 4/13/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: gets the hex representation of the vout values 
// then sends this to the first buffer to display
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void displayVolt(uint16_t r){
	volatile uint16_t tem;
	volatile int con;
	//converts voltage to hex by nibble
	for(int i = 0; i<3;i++){
	tem = r;
	//shifts the result by 8,4,0 to the right
	tem <<= (8-(4*i));
	//isolates the first nibble
	tem &= 0x000F;
	//converts to int
	con = (int)tem;
	//gets the hex equivalent of this number & adds to
	//temp array of data
	tempHex[i] = hex[con];	
			
	}
//send result to buffer
		sprintf(dsp_buff1, "s", tempHex);
}
//***************************************************************************
//
// Function Name        : "convertToBCD  "
// Date                 : 4/13/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: converts uint16_t to packed BCD.
//returns the converted result as a uint32_t
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
uint32_t convertToBCD(uint16_t val)
{
	uint32_t result = 0;
	int shift = 0;
//loops until value is 0
	while (val > 0)
	{
		result +=  (val % 10) << shift;
		val = val / 10;
		//adds shift of one nibble
		shift += 4;
	}
	return result;
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
		PORTA_OUT |= 0x80;
		PORTC_OUT &= 0x00;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready 
		SPI0_DATA = cmd;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready		
			PORTA_OUT &= 0x7F;	
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
		PORTA_OUT |= 0x80;
		PORTC_OUT |= 0x01;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
		SPI0_DATA = data;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
		PORTA_OUT &= 0x7F;
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

	