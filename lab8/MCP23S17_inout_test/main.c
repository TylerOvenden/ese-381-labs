
//***************************************************************************
//
// File Name            : MCP23S17_inout_test.c
// Title                :
// Date                 : 3/30/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  uses SPI0 to write to MCP23S17 with the
//MCP23S17_SPI_write,  MCP23S17_SPI_read & MCP23S17_SPI_init functions.
// In this program, switch inputs are polls through the MCP23S17 
// and these values are display on the LEDs through PORTB of
// the MCP23S17. 
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
#define SPI_DORDMSB_bm 0x00


#define IOCONaddr_b0 0x0A
#define IOCONadrr_b1 0x05
#define IODIRAaddr_b1 0x00
#define IODIRBaddr_b1 0x10
#define GPPUAaddr_b1 0x06
#define GPIOAaddr_b1 0x09
#define OLATBaddr_b1 0x1A
#define WRITE_opcode 0x40
#define READ_opcode 0x41
#define BANK  0x80


#include <avr/io.h>
#include <avr/interrupt.h>
#include<stdio.h>
#include <util/delay.h>
#include<stdlib.h>


volatile uint8_t values;
volatile uint8_t check;


void MCP23S17_SPI_write(uint8_t, uint8_t);
void MCP23S17_SPI_initalize();
uint8_t MCP23S17_SPI_read( uint8_t);
int main(void)
{
	MCP23S17_SPI_initalize();
	
	
	/* Replace with your application code */
	while (1)
	{
		values = MCP23S17_SPI_read(GPIOAaddr_b1);
		MCP23S17_SPI_write(OLATBaddr_b1, ~values);
		
		
	}
}
//***************************************************************************
//
// Function Name        : "MCP23S17_SPI_initalize  "
// Date                 : 3/30/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: initializes the SPI & MCP23S17 to communicate.
// uses the MCP23S17_SPI_write method to configure MCP23S17
// as well.
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void MCP23S17_SPI_initalize() {
	PORTA_DIR = 0xD0;
	PORTF_DIR = 0x04;
	//configures SPI
	SPI0_CTRLA = SPI_ENABLE_bm || SPI_MASTER_bm || SPI_DORDMSB_bm;
	SPI0_CTRLB = SPI_MODE1_bp;
	SPI0_INTCTRL = SPI_IE_bm;
	//configures MCP23S17 registers
	MCP23S17_SPI_write(IOCONaddr_b0, BANK);
	MCP23S17_SPI_write(IODIRAaddr_b1, 0x00);
	MCP23S17_SPI_write(IODIRBaddr_b1, 0xFF);
	MCP23S17_SPI_write(GPPUAaddr_b1, 0xFF );
	

	
	
}



//***************************************************************************
//
// Function Name        : "MCP23S17_SPI_read  "
// Date                 : 3/30/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
//DESCRIPTION: uses SPI to read the data from the given address. 
// addr is the MCP23S17 address we're given. 
//return tem which is the data read from this address.
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************

uint8_t MCP23S17_SPI_read( uint8_t addr){
		check = SPI0_INTFLAGS;
		check %=  SPI_IF_bm;
		//checks if flag for can write/read is set
		if(check !=  SPI_IF_bm){
			check = SPI0_INTFLAGS;
			check %=  SPI_IF_bm;
		}
		
		//puts opcode for writing into register
		SPI0_DATA = READ_opcode;
		check = SPI0_INTFLAGS;
		check %=  SPI_IF_bm;
		//checks if flag for can write is set
		if(check !=  SPI_IF_bm){
			check = SPI0_INTFLAGS;
			check %=  SPI_IF_bm;
		}
		//reads
		uint8_t tem = SPI0_DATA;
		return tem;
	
	
}



//***************************************************************************
//
// Function Name        : "MCP23S17_SPI_write  "
// Date                 : 3/30/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: uses SPI to write to the given MCP23S17 address. passes 2 parameters,
// the address and data to be written. first writes the opcode for writing,
// then writes the register & then the data. deselects MCP23S17 at the end
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
void MCP23S17_SPI_write(uint8_t addr, uint8_t data) {
	check = SPI0_INTFLAGS;
	check %=  SPI_IF_bm;
	//checks if flag for can write/read is set
	if(check !=  SPI_IF_bm){
		check = SPI0_INTFLAGS;
		check %=  SPI_IF_bm;
	}
	
	//puts opcode for writing into register
	SPI0_DATA = WRITE_opcode;
	check = SPI0_INTFLAGS;
	check %=  SPI_IF_bm;
	//checks if flag for can write is set
	if(check !=  SPI_IF_bm){
		check = SPI0_INTFLAGS;
		check %=  SPI_IF_bm;
	}
	//sends register opcode
	SPI0_DATA = addr;
	
	SPI0_DATA = WRITE_opcode;
	check = SPI0_INTFLAGS;
	check %=  SPI_IF_bm;
	//checks if flag for can write is set
	if(check !=  SPI_IF_bm){
		check = SPI0_INTFLAGS;
		check %=  SPI_IF_bm;
	}
	//sends data
	SPI0_DATA = data;
	
	//deselects SS
	PORTF_OUT = 0x02;
	
}