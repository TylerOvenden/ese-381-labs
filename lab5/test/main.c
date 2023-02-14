/***********
*********************************************************
 * File:        is_CLK_CPU_independent_of_CLK_PER.c
 * Author:      Tyler Ovenden
 * Created:     3/9/2021
 *
 * Notes: 
 * Description: turns on CLKOUT using the main clock prescaler, 
 then toggles portcpin0 with a 100us delay
 in the lab, we can measure the wavelength of this delay by
 checking long the pin is on and off. if the length is 100us, 
 clk_cpu is indepent, if length is 200us changing clk_per
 would change clk_cpu
 *
 *******************************************************************/
#define F_CPU 4000000
#include <avr/io.h>
#include <util/delay.h>



int main(void)
{
	PORTA.DIR = PIN7_bm;
	PORTC.DIR |= PIN0_bm; //set PORTC PIN0 as pin to togggle
	
	
	
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = 0x03; //set prescaler to 2x
	CPU_CCP = CCP_IOREG_gc; 
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm;
	
	
    /* Replace with your application code */
    while (1) 
    {
		PORTC.OUT |= PIN0_bm;
		_delay_us(100);
		PORTC.OUT &= ~PIN0_bm;
		_delay_us(100);
  
    }
}

