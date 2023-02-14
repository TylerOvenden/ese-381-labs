/***********
*********************************************************
 * File:          one_MHz.c
 * Author:      Tyler Ovenden
 * Created:     3/9/2021
 *
 * Notes: 
 * Description: sets frequency to 1mHZ for oscillator,
  sets CLK_OUT to CLK_PER & selects the oscillator as main clock
 *
 *******************************************************************/

#include <avr/io.h>


int main(void)
{
	
	
	//assigns PORTA7 as output
	PORTA.DIR = PIN7_bm;
	
	
	CPU_CCP = CCP_IOREG_gc; //changes access of protected reg.
	//selects frequency for Oscillator to 1mHZ
	CLKCTRL.OSCHFCTRLA = CLKCTRL_FREQSEL_1M_gc;
	CPU_CCP = CCP_IOREG_gc;//changes access of protected reg.
	
	//enables clkout & selects OSCHF for main clock
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm || CLKCTRL_CLKSEL_OSCHF_gc;	

	PORTD.DIR = 0x00;
    /* Replace with your application code */
    while (1) 
    {
    }
}


