
//***************************************************************************
//
// File Name            : DS1306_RTC.c
// Title                :
// Date                 : 4/20/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  tests RTC using 
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

//#define NVRAM 0x8F



#include <util/delay.h>
//p = RTC_time_date_write;
//******************************************************************************
int main(void)
{
	
	
	
	
	
	/* Replace with your application code */
	
	
	
	
	while (1)
	{
	write_read_RTC_test();	
	_delay_ms(500);
	}
}
