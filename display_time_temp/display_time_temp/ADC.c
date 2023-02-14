//***************************************************************************
//
// File Name            : ADC.c
// Title                :
// Date                 : 4/13/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  contains all functions that use ADC, includes function for 
// initializing the ADC, running a conversion, displaying hex representation 
// of vout & converting uint_16 to packed BCD
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
#include <avr/interrupt.h>
#include<stdio.h>
#include <util/delay.h>
#include<stdlib.h>
#include "DOG_LCD.h"
#include "ADC.h"

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
	PORTD_DIR = 0x00; 
	//set PORTD6 as input
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
	//	displayVolt(result);
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
	
	
	sprintf(dsp_buff2, "Temp : %03", mult);
	

	if(boolean == 1){
		dsp_buff2[6] = '-';
	}
	dsp_buff2[12] = 'c';
	dsp_buff2[11] = 0xDE;
	dsp_buff2[10] = dsp_buff2[9];
	dsp_buff2[9] = '.';
	
			boolean = 0;
}

