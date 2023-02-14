
/***********
*********************************************************
 * File:         clk_main_32768Hz.c
 * Author:      Tyler Ovenden
 * Created:     3/9/2021
 *
 * Notes: 
 * Description: 
  sets CLK_OUT to CLK_PER & external crystal oscillator as main clock
 *
 *******************************************************************/

#include <avr/io.h>


int main(void)
{
	
	
	PORTA.DIR = PIN7_bm;
	CPU_CCP = CCP_IOREG_gc; //changes access of protected reg.
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm || CLKCTRL_CLKSEL_XOSC32K_gc;
	 
    /* Replace with your application code */
    while (1) 
    {
    }
}

