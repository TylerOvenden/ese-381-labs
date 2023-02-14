

//***************************************************************************
//
// File Name            : DS1306_RTCDrivers.c
// Title                :
// Date                 : 4/20/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  class containing methods for working with RTC in DS1306
// liking writing and reading both for just doing so once & in 
// burst mode. the RTC values for hours, minutes & seconds are written to 
// an array to be displayed. 
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
#include <util/delay.h>
#include <avr/io.h>
#include "DS1306_RTCDrivers.h"
#include "DOG_LCD.h"


volatile unsigned char RTC_time_date_write[7] = { 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00 };


//******************************************************************************
// Function : void SPI_rtc_ds1306_config (void)
// Date and version : 041221, version 1.0
// Target MCU : AVR128 @ 4MHz
// Author : Tyler Ovenden
// DESCRIPTION
// This function unselects the ds_1306 and configures an AVR128DB48 operated at
// 4 MHz to communicate with the ds1306. Pin PC1 of the AVR128DB48 is used to
// select the ds_1306. SCLK is operated a the maximum possible frequency for
// the ds1306.
//******************************************************************************

void SPI_rtc_ds1306_config (void){

			PORTA_DIR = 0xD0;
			//PORTA_PIN5CTRL = PORT_PULLUPEN_bm;
			PORTC_DIR = 0x02;
			
		//	PORTC_OUT = 0x00;
				//configures SPI
		
	
	SPI0_CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm;
	
	
	//in the Salaee diagrams given in the lecture, I saw that CPOL would be 0
	//as the CLK is 0 when idle, to find CPHA I observed the same diagrams and saw
	// it was being sampled at the falling edge so CPHA = 1
	//CPOL = 0, CPHA = 1 is mode 1 for SPI
	SPI0_CTRLB = SPI_MODE_1_gc;
	
	
	PORTC_OUT &= 0xFD;
	SPI0.DATA = 0x01;
}




//***************************************************************************
// Function Name : "block_write_RTC"
// void block_write_RTC (volatile unsigned char *array_ptr,
// unsigned char strt_addr, unsigned char count)
// Target MCU : AVR128DB48 @ 4MHz
// Author : Tyler Ovenden
// DESCRIPTION
// This function writes a block of data from an array to the DS1306. strt_addr
// is the starting address in the DS1306. count is the number of data bytes to
// be transferred and array_ptr is the address of the source array.
//**************************************************************************


void block_write_RTC (volatile unsigned char *array_ptr,
unsigned char strt_addr, unsigned char count){
	
	if(SPI0_INTFLAGS !=  SPI_IF_bm){
		SPI0.DATA = 0x01;
	}
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}
	PORTA_OUT |= 0x80;
	_delay_us(3);
	SPI0.DATA = strt_addr;
	SPI0.DATA = strt_addr;
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	for(int i = 0;i<count;i++)
	{
		SPI0.DATA = *(array_ptr + i);
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
		
		//array_ptr++;
		
	}
		PORTC_OUT &= 0xFD;
	
	
}


//***************************************************************************
// Function Name : "block_read_RTC"
// void block_read_RTC (volatile unsigned char *array_ptr,
// unsigned char strt_addr, unsigned char count)
// Target MCU : AVR128DB48 @ 4MHz
// Author : Tyler Ovenden
// DESCRIPTION
// This function reads a block of data from the DS1306 and transfers it to an
// array. strt_addr is the starting address in the DS1306. count is the number
// of data bytes to be transferred and array_ptr is the address of the
// destination array.
//**************************************************************************


void block_read_RTC(volatile unsigned char *array_ptr,
unsigned char strt_addr, unsigned char count){
	//dummy data
	
	SPI0.DATA = 0x01;
	
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}
	PORTC_OUT |= 0x01;
	_delay_us(3);
	
	SPI0.DATA = strt_addr;
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	for(int i = 0;i<count;i++)
	{
		SPI0.DATA = 0x01;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
		*(array_ptr + i) = SPI0.DATA;
		
	}
	
	_delay_us(.17);
	PORTC_OUT &= 0xFD;
	//write dummy data	
	SPI0.DATA = 0x01;
	
	
}



//***************************************************************************
// Function :
// void write_RTC (unsigned char reg_RTC, unsigned char data_RTC)
//
// Target MCU : AVR128DB48 @ 4MHz
// Target Hardware ;
// Author :Tyler Ovenden
// DESCRIPTION
// This function writes data to a register in the RTC. To accomplish this, it
// must first write the register's address (reg_RTC) followed by writing the
// data (data_RTC). In the DS1306 data sheet this operation is called an SPI
// single-byte write.
//**************************************************************************

void write_RTC (unsigned char reg_RTC, unsigned char data_RTC){
	if(SPI0_INTFLAGS !=  SPI_IF_bm){
			SPI0.DATA = 0x01;
	}
	while(SPI0_INTFLAGS !=  SPI_IF_bm){} 
		
	PORTC_OUT |= 0x01;
	_delay_us(3);

	
	SPI0.DATA = reg_RTC;
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	SPI0.DATA = data_RTC;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	_delay_us(.17);
		PORTC_OUT &= 0xFD;
}

//***************************************************************************
// Function Name :
// unsigned char read_RTC (unsigned char reg_RTC)
// Target MCU : AVR128DB48 @ 4MHz
// Author : Tyler Ovenden
// DESCRIPTION
// This function reads data from a register in the RTC. To accomplish this, it
// must first write the register's address (reg_RTC) followed by writing a dummy
// byte to generate the SCLKs to read the data (data_RTC). In the DS1306 data
// sheet this operation is called am SPI single-byte read.
//**************************************************************************
unsigned char read_RTC (unsigned char reg_RTC) {
	//write dummy data
	SPI0.DATA = 0x01;
	while(SPI0_INTFLAGS !=  SPI_IF_bm){} 
	PORTC_OUT |= 0x01;
	_delay_us(3);
	//writes the read address
	SPI0.DATA = reg_RTC;
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	//write dummy data
	SPI0.DATA = 0x01;
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	unsigned char temp = SPI0.DATA;
	
	
	_delay_us(.17);

	PORTC_OUT &= 0xFD;
		//write dummy data
	SPI0.DATA = 0x01;
	return temp;
	
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
//**************************************************************************void write_read_RTC_test() {	//write_RTC(CTRL_REG, 0x00);		//RTC_time_date_write()	     block_write_RTC (RTC_time_date_write, RTCWrite, 7);
	   block_read_RTC (RTC_time_date_read, RTCRead, 7);
				sprintf(dsp_buff1, "time: %d:%d:%d", RTC_time_date_read[2],
			RTC_time_date_read[1],RTC_time_date_read[0]);
	}