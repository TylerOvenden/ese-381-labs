//***************************************************************************
//
// File Name            : DS1306_write_read_test.c
// Title                :
// Date                 : 4/20/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  tests if SPI communications between the AVR & DS1306 work
//by writing values to the first ten NVRAM registers and reads these back
//from the read addresses of the same registers.
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

#include <avr/io.h>
#include "DS1306_driver.h"

#define CLKCTRL_REG 0x8F
unsigned char writeReg[10] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5,
	0xA6, 0xA7, 0xA8, 0xA9 };
unsigned char readReg[10] = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
0x26, 0x27, 0x28, 0x29 };
unsigned char RTC_byte[10];
//******************************************************************************
int main(void)
{
	
	
	
	
	
	/* Replace with your application code */
	
	write_read_RTC_test();
	
	
	while (1)
	{
		
	}
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
//**************************************************************************void write_read_RTC_test() {//writes byte to CLK reg	SPI_rtc_ds1306_config();	write_RTC(CLKCTRL_REG, 0x00);		//loops through bytes 0xA0-0xA9	// & 0x20-0x29	for(int i = 0;i<10;i++){		//write 0x01 to each write register address		write_RTC(writeReg[i],0x01);		//reads it back		RTC_byte[i] = RTC_byte(readReg[i]);	}	}