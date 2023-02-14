/***********
*********************************************************
 * File:         ic_test_v2.h
 * Author:      Tyler Ovenden
 * Created:     3/2/2021
 *
 * Notes:
 *
 * Description:
 *  This program is meant to test if the chosen IC gates work
 *  starts with POST run which turns on all the diode LEDs & last 3 bargraph LEDs
 *  then the user uses the button 3 switches of the dip switch & press the  
 *  push button to pick which gate to test, the code tests out all possible 
 * inputs for that ic & uses an array to confirm if the results match up with 
 the appropriate gate. the leds confirm if the ICs were correct or not.
 3/2/2021: set PORTB pin 5 to represent the power, when a gate is not being tested
 * the IC gate's inputs are floated by setting the pins representing it to all 0s
 * & toggling PORTB5 to a 0. when a button is pushed, it is toggled backed to 1 to 
 * turn on power 
 *******************************************************************/
#include <avr/io.h>
#include <stdbool.h>

#include <util/delay.h>
#define POST 0x18
#define TWOZERO  0x07
#define ISOSWITCH 0xE0
#define CLEARLED 0x1F
#define CLEARMOST 0x0F
#define ZEROZERO 0x00
#define ZEROONE 0x55
#define ONEZERO 0xAA
#define ONEONE 0xFF
#define TOGGLEPOWER 0x20
#define NANDSTATE 0
#define ANDSTATE 1
#define ORSTATE 2
#define XORSTATE 3
#define NANDCO 4

//B
int main(void)
{
	PORTA_DIR = 0x1F; //setting pins 7-5 to inputs for switch
	PORTB_DIR = 0xFB;  //setting portb 3 as an input
	PORTC_DIR = 0xE7; //setting pins 7-5, 2-0 as inputs
	PORTD_DIR = 0xFF;  //setting PORTD as outputs, using this for 
	//the inputs in 
	PORTE_DIR = 0x00; //
	
	//clear the leds
	PORTC_OUT = 0xFF;
	PORTD_OUT = 0xFF;	
	
	/*
	uint8_t temptest = 0xE0 >> 5;
	PORTC.OUT &= 0x8F;
	PORTC.OUT &= ~temptest;
	 //PORTC.OUT = temptest;
	*/
	
	PORTD_OUT = 0x20;
	PORTA_PIN7CTRL = PORT_PULLUPEN_bm;
	PORTA_PIN6CTRL = PORT_PULLUPEN_bm;
	PORTA_PIN5CTRL = PORT_PULLUPEN_bm;
	
	
	PORTB_OUT |= TOGGLEPOWER;
	PORTB_PIN3CTRL = PORT_PULLUPEN_bm;
	
	
	//array of all the outputs, 
	uint8_t outputs[5][4] = {
		{0x0F, 0x0F, 0x0F, 0x00},
		{0x00, 0x00, 0x00, 0x0F},
		{0x00, 0x0F, 0x0F, 0x0F},
		{0x00, 0x0F, 0x0F, 0x00},
		{0x0F, 0x0F, 0x0F, 0x00}
	};
	
     int ic = -1;
	//turn on leds for POS
		PORTC_OUT = POST;
		//_delay_ms(1000);

	
		PORTC_OUT |= 0xFF;
		bool success = true;
   
   
    while (1) 
    {
		PORTD_OUT = 0x00;
		PORTB_OUT &= ~TOGGLEPOWER;
		uint8_t push = PORTB_IN;
		push &= 0x04; //isolate PB2 for push button
		//checks if push button is pushed
		if(push == 0x00){
		
		PORTC.OUT |= 0xFF;
		
		uint8_t test = PORTA_IN;
		test &= ISOSWITCH; //make mask
		//check which state the dip switch is representing
		if(test == 0x00){
			ic = NANDSTATE;
			} 
		if(test == 0x20)
			{
			ic = ANDSTATE;	
		
		}
		if(test == 0x40){
			ic = ORSTATE;
		}
		if(test == 0x60)
		{
			ic = XORSTATE;
			
		}
		
		if(test == 0x80){
			ic = NANDCO;
		}
		//change this 


		if(ic != -1)
		{
		    uint8_t testShift = test >> 5;
			testShift &= TWOZERO; //isolate bits 3-1			
			PORTC.OUT |= 0xFF;
			PORTC.OUT &= ~testShift;
		}
		 
		success = true;
		//checks gates
			
			
		//turn on power
		PORTB_OUT |= TOGGLEPOWER;
		
		
		
		for(int i = 0; i<4;i++){
			
			//turn on blue led
			PORTC.OUT &= 0x7F;
			if(i == 0){
				PORTD_OUT = ZEROZERO;
			}
			if(i == 1){
				PORTD_OUT = ZEROONE;
			}
			if(i == 2){
				PORTD_OUT = ONEZERO;
			}
			if(i == 3){
				PORTD_OUT = ONEONE;
			}
			//propergation delay
			//
			__asm("nop");
			
			//clears bits 7-4 of PORTE, where the IC outputs are stored
			//ends loop if IC output doesn't match the value in the array of results
			uint8_t check = PORTE_IN;
			check &= CLEARMOST;
			if(check != outputs[ic][i]){
			success = false;
			break;
				}
			}
			//turns off blue led
			PORTC.OUT |= 0xFF;
			//turns on green led
			if(!success){
 
			 PORTC.OUT &= 0xDF;
	
			}
			//turns on red led
			if(success){
				PORTC.OUT &= 0xBF;
			}
			
			//_delay_ms(1000);
				
			}
		}
		
		
		
    
}

