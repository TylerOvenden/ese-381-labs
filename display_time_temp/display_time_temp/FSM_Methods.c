//***************************************************************************
//
// File Name            : FSM.methods.c
// Title                :
// Date                 : 4/27/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  contains all functions for working with the FSM including 
// for setting, entering & displaying the hours, minutes & seconds. 
// utilizes methods from USART3 to display the desired time element & DS1306
// RTCDrivers to read or write hours, minutes and seconds.
// Warnings            :
// Restrictions         : none
// Algorithms           : none
// References           :
//
// Revision History     : Initial version
//
//
//**************************************************************************
#include <avr/io.h>

#include "FSM.h"
#include "USART3.h"
#include "DS1306_RTCDrivers.h"


void set_fn(){
	//clears value array
	vals[0] = 0;
	vals[1] = 0;
}
void error_fn(){
	asm volatile ("nop");
	
}
//following methods read back the hours, mins & secs
//using USART
void set_hours_fn(){
	read = read_RTC(READ_HOUR);
	transmit_USART(read);
	
}
void set_mins_fn(){

	read = read_RTC(READ_MIN);
	transmit_USART(read);
	
}
void set_sec_fn(){
	read = read_RTC(READ_SEC);
	transmit_USART(read);
}

//enters last entered digit to digits array
void digit_in_fn(){
	//shifts one result down,
	vals[0] = vals[1];
	vals[1] = digit_val;
}

//following 3 methods set the hours, mins or secs
//by using the write_rtc method
void enter_hour_fn(){
	//converts arr to int
	int i, k = 0;
	for (i = 0; i < 2; i++)
	k = 10 * k + vals[i];
	
	write_RTC(WRITE_HOUR, k);
	
	
}
void enter_min_fn(){
	//converts arr to int
	int i, k = 0;
	for (i = 0; i < 2; i++)
	k = 10 * k + vals[i];
	
	write_RTC(WRITE_MIN, k);
	
}
void enter_sec_fn(){
	//converts arr to int
	int i, k = 0;
	for (i = 0; i < 2; i++)
	k = 10 * k + vals[i];
	
	write_RTC(WRITE_SEC, k);
	
	
}