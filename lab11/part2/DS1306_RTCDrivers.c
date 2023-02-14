
//***************************************************************************
//
// File Name            : DS1306_RTCDrivers.c
// Title                :
// Date                 : 4/20/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  includes the functions for writing and reading to
//registers in DS1306. SPI_rtc_ds1306_config configures SPI for the
//given specifications. also includes write_read_RTC_test
// which tests the ability to write and read RTC registers
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


volatile unsigned char RTC_time_date_write[7] = { 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00 };


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
	
			PORTA_DIR = 0xF0;
			PORTC_DIR = 0x02;
			
			PORTC_OUT = 0x00;
				//configures SPI
			SPI0_CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm;
			SPI0_CTRLB = SPI_MODE0_bm;
}












void block_write_RTC (volatile unsigned char *array_ptr, 
unsigned char strt_addr, unsigned char count){
	
		PORTC_OUT |= 0x02;
		SPI0.DATA = strt_addr;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
		for(int i = 0;i<count;i++)
{
	//SPI0.DATA = *(array_ptr + i);
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
			
	//array_ptr++;
	
	}//		SPI0.DATA = data_RTC;

		PORTC_OUT &= 0xFD;
	
	
}
void block_read_RTC(volatile unsigned char *array_ptr, 
unsigned char strt_addr, unsigned char count){
	
	PORTC_OUT |= 0x02;
	SPI0.DATA = strt_addr;
	while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
	for(int i = 0;i<count;i++)
	{
		
	
		SPI0.DATA = 0x01;
		while(SPI0_INTFLAGS !=  SPI_IF_bm){}    // wait until Tx ready
		array_ptr = SPI0.DATA;
		array_ptr++;
		
	}
	PORTC_OUT &= 0xFD;
		
	
	
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
//**************************************************************************void write_read_RTC_test() {	SPI_rtc_ds1306_config();	//RTC_time_date_write()	     block_write_RTC (RTC_time_date_write, RTCWrite, 7);
	   block_read_RTC (RTC_time_date_read, RTCRead, 7);
			}