//***************************************************************************
//
// File Name            : main.c
// Title                :
// Date                 : 4/13/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  driver file for temp_meas_mm, does the same thing
// as temp_meas but by using multifiles programing.
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
#include "ADC.h"


int main(void) {
	
	init_lcd_dog();
	init_adc();
	while(1) {

		convert();
		update_lcd_dog();
		_delay_ms(500);
	}
}

