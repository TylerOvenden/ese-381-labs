/*
 * temp.c
 *
 * Created: 3/17/2021 8:50:53 PM
 * Author : vleen
 */ 

#include <avr/io.h>


int main(void)


{
	volatile test = 0xFF;
	test >>= 1;
	
	PORTD_DIR = 0xFF;
    /* Replace with your application code */
    while (1) 
    {
    }
}

