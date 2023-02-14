//***************************************************************************
//
// File Name            : DS1306_driver.c
// Title                :
// Date                 : 4/20/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  includes the functions for writing and reading to 
//registers in DS1306. SPI_rtc_ds1306_config configures SPI for the 
//given specifications. also includes write_read_RTC_test
// which tests the ability to read and write to registers in DS1306 
// using the other methods created.
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
#include "DS1306_driver.h"

#define ONEHZ 0x04
uint8_t yes = 0x8A;
unsigned char writeReg[10] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5,
0xA6, 0xA7, 0xA8, 0xA9 };
unsigned char readReg[10] = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
0x26, 0x27, 0x28, 0x29 };

//#define CLKCTRL_REG 0x8F



//******************************************************************************
// Function : void SPI_rtc_ds1306_config (void)
// Date and version : 041221, version 1.0
// Target MCU : AVR128 @ 4MHz
// Author : Ken Short
// DESCRIPTION
// This function unselects the ds_1306 and configures an AVR128DB48 operated at
// 4 MHz to communicate with the ds1306. Pin PC1 of the AVR128DB48 is used to
// select the ds_1306. SCLK is operated a the maximum possible frequency for
// the ds1306.
//******************************************************************************

void SPI_rtc_ds1306_config (void){
	//since the AVR runs at 4Mhz & the desired frequency for the ds_1306 is about
	//32kHz, we used the prescaler of divide by 128 to get the values closest to 
	//32kHz
			PORTA_DIR = 0xF0;
			PORTC_DIR = 0x02;
			
			PORTC_OUT = 0x00;
				//configures SPI
			SPI0_CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm  | SPI_PRESC_DIV128_gc;
			
			
    //in the Salaee diagrams given in the lecture, I saw that CPOL would be 0
	//as the CLK is 0 when idle, to find CPHA I observed the same diagrams and saw
	// it was being sampled at the falling edge so CPHA = 1
	//CPOL = 0, CPHA = 1 is mode 1 for SPI	
			SPI0_CTRLB = SPI_MODE1_bm;
}


//***************************************************************************
// Function :
// void write_RTC (unsigned char reg_RTC, unsigned char data_RTC)
//
// Target MCU : AVR128DB48 @ 4MHz
// Target Hardware ;
// Author : Ken Short
// DESCRIPTION
// This function writes data to a register in the RTC. To accomplish this, it
// must first write the register's address (reg_RTC) followed by writing the
// data (data_RTC). In the DS1306 data sheet this operation is called an SPI
// single-byte write.
//**************************************************************************

void write_RTC (unsigned char reg_RTC, unsigned char data_RTC){
	PORTC_OUT |= 0x02;
	SPI0.DATA = reg_RTC;
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	SPI0.DATA = data_RTC;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	
	PORTC_OUT &= 0xFD;
}


//***************************************************************************
// Function Name :
// unsigned char read_RTC (unsigned char reg_RTC)
// Target MCU : AVR128DB48 @ 4MHz
// Author : Ken Short
// DESCRIPTION
// This function reads data from a register in the RTC. To accomplish this, it
// must first write the register's address (reg_RTC) followed by writing a dummy
// byte to generate the SCLKs to read the data (data_RTC). In the DS1306 data
// sheet this operation is called am SPI single-byte read.
//**************************************************************************
unsigned char read_RTC (unsigned char reg_RTC) {
	PORTC_OUT |= 0x02;
	//writes the read address
	SPI0.DATA = reg_RTC;
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	//write dummy data
	SPI0.DATA = 0x01;
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	
	PORTC_OUT &= 0xFD;
	
	return SPI0_DATA;
	
}


//***************************************************************************
// Function Name : void write_read_RTC_test(void)
// Target MCU : AVR128DB48 @ 4MHz
// Author : Ken Short
// DESCRIPTION
// This function writes a byte to the NV RAM and then it reads back the location
// just written and places the result in a global array named RTC_byte[]. The
// function repeats this write/read sequence 10 times. The locations written are
// 0xA0 through 0xA9 and the corresponding locations read are 0x20 through 0x29.
//**************************************************************************void write_read_RTC_test() {//writes byte to CLK reg	SPI_rtc_ds1306_config();	write_RTC(CTRL_REG, 0x00);		write_RTC(CTRL_REG, ONEHZ);		//loops through bytes 0xA0-0xA9	// & 0x20-0x29	for(int i = 0;i<10;i++){		//write 0x01 to each write register address		write_RTC(writeReg[i],0x01);		//reads it back		RTC_byte[i] = read_RTC(readReg[i]);	}	}