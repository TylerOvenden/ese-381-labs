
/***********
*********************************************************
 * File:         toggle_every_xxx_us.c
 * Author:      Tyler Ovenden
 * Created:     3/9/2021
 *
 * Notes: 
 * Description: 
  toggles PORTC bit 0 at a rate decided by dip switch inputs
  PORTA bits 7&6. picks if the period is 52.08333 us, 104.1666 us
  or 208.333 us
 *
 *******************************************************************/
#define F_CPU 4000000
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	
	
	PORTA_DIR = 0x3F;
	PORTC_DIR |= PIN0_bm;
	
	PORTA_DIR = PIN7_bm;
	CPU_CCP = CCP_IOREG_gc; //changes access of protected reg.
	
	//enables clkout & selects OSCHF for main clock
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm || CLKCTRL_CLKSEL_OSCHF_gc;
	
	PORTA_PIN7CTRL = PORT_PULLUPEN_bm;
	PORTA_PIN6CTRL = PORT_PULLUPEN_bm;
	
	
    /* Replace with your application code */
    while (1) 
    {
		uint8_t test = PORTA_IN;
		test &= 0xB0;
	//turns on pin0
		PORTC_OUT |= PIN0_bm;


	// PIN7&6 0s	
		if(test == 0x00){
			_delay_us(52.08333);
			PORTC_OUT &= ~PIN0_bm;	//toggles it off
		}
		
	// PIN7 1 PIN6 0	
		if(test == 0x80){
			_delay_us(104.1666);
			PORTC_OUT &= ~PIN0_bm; //toggles it off
		}
		
	// PIN7 0 PIN6 1
		if(test == 0x40){
				_delay_us(208.333);
				PORTC_OUT &= ~PIN0_bm;
		}
		
			
			
		
		
    }
}

