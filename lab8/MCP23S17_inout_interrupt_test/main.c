
//***************************************************************************
//
// File Name            : MCP23S17_inout_interrupt.c
// Title                :
// Date                 : 3/30/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  uses interrupts instead of polling to read switch values
// from MCP23S17 and then writes these values to PORTB to display
// it on the LEDs. ISR is triggered by INTA, which is connected to PORTF3
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
#define INTCONAaddr_b1 0x04
#define INTCAPAaddr_b1 0x08
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
	
		sei();
	/* Replace with your application code */
	while (1)
	{
		
			asm volatile ("nop");
	
		
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
	
	PORTF_PIN3CTRL = 0x08;
	//configures SPI
	SPI0_CTRLA = SPI_ENABLE_bm || SPI_MASTER_bm || SPI_DORDMSB_bm;
	SPI0_CTRLB = SPI_MODE1_bp;
	SPI0_INTCTRL = SPI_IE_bm;
	//configures MCP23S17 registers
	MCP23S17_SPI_write(IOCONaddr_b0, BANK);
	MCP23S17_SPI_write(IODIRAaddr_b1, 0x00);
	MCP23S17_SPI_write(IODIRBaddr_b1, 0xFF);
	MCP23S17_SPI_write(GPPUAaddr_b1, 0xFF );
	MCP23S17_SPI_write(INTCONAaddr_b1, 0xFF);
	
	
}
//***************************************************************************
//
// Function Name        : "ISR (PORTF_PORT_vect)  "
// Date                 : 3/30/21
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden
// DESCRIPTION: ISR that is triggered by INTA (PF2)
// reads the switch values at this interupt through 
// the INTCAPA register & calls the function for writing these
//  values to PORTB of MCP23S17 to display it on the LEDs
// clears the flags after this.
// Warnings             : none
// Restrictions         : none
// Algorithms           : none
// References           : none
//
// Revision History     : Initial version
//
//**************************************************************************
ISR (PORTF_PORT_vect) {
	cli();
	//char res = USART_sw_read();

	 values = MCP23S17_SPI_read(INTCAPAaddr_b1);
	MCP23S17_SPI_write(OLATBaddr_b1, ~values);
	//clears the flags
	 PORTF_INTFLAGS = PORT_INT3_bm;
	sei();

	return;
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