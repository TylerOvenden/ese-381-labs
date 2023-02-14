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
// by calling the write_read_RTC_test function from DS1306_driver
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

//******************************************************************************
int main(void)
{

	write_read_RTC_test();
	
	
	while (1)
	{
		
	}
}
