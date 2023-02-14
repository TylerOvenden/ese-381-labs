/***********
*********************************************************
 * File:         ic_test_ident.h
 * Author:      Tyler Ovenden
 * Created:     2/22/2021
 *
 * Notes:
 *
 * Description:
 *  This program is meant to test if the chosen IC gates work
 *  starts with POST run which turns on all the diode LEDs & last 3 bargraph LEDs
 *  then the user uses the button 3 switches of the dip switch & press the  
 *  push button to pick which gate to test, the code tests out all possible 
 * inputs for that ic & uses an array to confirm if the results match up with 
 the appropriate gate. the leds confirm if the ICs were correct or not
 
 update 3/2/2021:added option to test an unknown IC
 * if 111 is entered on the switches, runs separate tests that loop through the 
 * possible gates
 * then tests the possible IC inputs to determine if it could be this gate
 * afterwards it displays the assigned binary code for the gate or 111 if the gate was
 * not determined
 *
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
#define ONEONEONE 0xEF
#define NANDSTATE 0
#define TOGGLEPOWER 0x20
#define ANDSTATE 1
#define ORSTATE 2
#define XORSTATE 3
#define NANDCO 4
#define MYSTERY 5

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
	
	
	
	PORTB_PIN3CTRL = PORT_PULLUPEN_bm;
	
	
	//array of all the outputs, 
	uint8_t outputs[5][4] = {
		{0x0F, 0x0F, 0x0F, 0x00},
		{0x00, 0x00, 0x00, 0x0F},
		{0x00, 0x0F, 0x0F, 0x0F},
		{0x00, 0x0F, 0x0F, 0x00},
		{0x0F, 0x0F, 0x0F, 0x00}
	};
	
	//array of all states to test
	uint8_t states[4] = {ZEROONE, ZEROONE, ONEZERO, ONEONE};
		
//array of what to display for each gate		
	uint8_t gates[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x07};
	 
//int representing the gates	 
     int ic = -1;
	//turn on leds for POS
		PORTC_OUT = POST;
		//_delay_ms(1000);
	
	
		PORTC_OUT |= 0xFF;
		bool success = true;
    /* R_eplace with your application code */
    while (1) 
    {
		//floats IC inputs & turns off power
		PORTD_OUT = 0x00;
		PORTB_OUT &= ~TOGGLEPOWER;
		uint8_t push = PORTB_IN;
		push &= 0x04; //isolate PB2 for push button
		//checks if push button is pushed
		if(push == 0x00){
		
		//turns power for IC on
		PORTB_OUT |= TOGGLEPOWER;
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
		if(test == ONEONEONE){
			
			ic = MYSTERY;
		}
		//change this 


		if(ic > -1 & (ic < MYSTERY))
		{
		    uint8_t testShift = test >> 5;
			testShift &= TWOZERO; //isolate bits 3-1			
			PORTC.OUT |= 0xFF;
			PORTC.OUT &= ~testShift;
		}
		 

		if(ic != MYSTERY ){
		success = true;
		
//checks gate by looping through the inputs for assigned ic
		for(int i = 0; i<4;i++){

			//turn on blue led
			PORTC.OUT &= 0x7F;
			PORTD_OUT = states[i];
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
			if(success)
				PORTC.OUT &= 0xBF;
		
			
		}
			
		
			//if 111 was entered loops runs to determine gate
				if(ic == MYSTERY){
					
					//loops through each possible gate
						 for(int i = 0; i<5;i++){
							 
							 //enables pullups for outputs of ic to check
							 //if it is TTL 
							 if(i == 4){
								 	PORTE_PIN2CTRL = PORT_PULLUPEN_bm;
								 	PORTE_PIN1CTRL = PORT_PULLUPEN_bm;
								 	PORTE_PIN0CTRL = PORT_PULLUPEN_bm;
								 
							 }
							 //if gate was found end main loop
							 if(ic != MYSTERY)
							 break;
							
					//loops though each possible input 		
							 for(int j = 0; j<4;j++){
								 PORTD_OUT = states[j];
								 __asm("nop");
								 
					//clears bits 7-4 of PORTE, where the IC outputs are stored
					//ends inner loop if IC output doesn't match the value
					// in the array of results
								 uint8_t check = PORTE_IN;
								 check &= CLEARMOST;
								 if(check != outputs[i][j]){
									 break;
								 }
						//if made it to this statement, the gate was determined		 
								 if(j == 3)
								 ic = i;
							 }
							 
						 }
						 
						//display the gates
					PORTC_OUT &= ~gates[ic];	 
						 
						 
					 }
	
			
			}
		}
		
		
	
}

