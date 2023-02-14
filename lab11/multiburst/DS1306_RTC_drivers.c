/*
 * lab11.c
 *
 * Created: 4/20/2021 11:37:30 AM
 * Author : vleen
 */ 

#include <avr/io.h>
#include "DS1306_RTC_drivers.h"





//******************************************************************************
// Function : void SPI_rtc_ds1306_config (void)
// Date and version : 041221, version 1.0
// Target MCU : AVR128 @ 4MHz
// Author : Ken Short
// DESCRIPTION
// This function unselects the ds_1306 and configures an AVR128DB48 operated at
// 4 MHz to communicate with the ds1306. Pin PC1 of the AVR128DB48 is used to
// select the ds_1306. SCLK is operated a the maximum possible frequency for
// the ds1306.
//******************************************************************************

void SPI_rtc_ds1306_config (void){
	
			PORTA_DIR = 0xF0;
			PORTC_DIR = 0x02;
			
			PORTC_OUT = 0x00;
				//configures SPI
			SPI0_CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm;
			SPI0_CTRLB = SPI_MODE0_bm;
}


//***************************************************************************
// Function :
// void write_RTC (unsigned char reg_RTC, unsigned char data_RTC)
//
// Target MCU : AVR128DB48 @ 4MHz
// Target Hardware ;
// Author : Ken Short
// DESCRIPTION
// This function writes data to a register in the RTC. To accomplish this, it
// must first write the register's address (reg_RTC) followed by writing the
// data (data_RTC). In the DS1306 data sheet this operation is called an SPI
// single-byte write.
//**************************************************************************

