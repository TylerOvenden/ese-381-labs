//***************************************************************************
//
// File Name            : FSM.c
// Title                :
// Date                 : 4/27/2021
// Version              : 1.0
// Target MCU           :
// Target Hardware      ;
// Author               : Tyler Ovenden 112122685
// DESCRIPTION:  contains the subgraphs and array of these subgraphs
// to create the FSM. calls methods from FSM_Methods in the FSM method 
// as determined by the entered input to the FSM by the user. 
// Warnings             :
// Restrictions         : none
// Algorithms           : none
// References           :
//
// Revision History     : Initial version
//
//
//**************************************************************************
#include <avr/io.h>

#include "FSM.h"
#include "DS1306_RTCDrivers.h"

//idle state subtable
	//set state subtable			

	
	const transition idle_transitions[]= {
		
		// INPUT , NEXT_STATE, TASK
		{s,    set,        set_fn},
		
		{eol,   idle,     error_fn}	};
		
		const transition set_transitions[]= {
			// INPUT , NEXT_STATE, TASK
			{h,    hours,        set_hours_fn},
			
			{m,    mins,        set_mins_fn},
			
			{sec,    secs,        set_sec_fn},
			
			{eol,   idle,     error_fn}
		};
		
		
		//hours state subtable
		const transition hours_transitions[]= {
			// INPUT , NEXT_STATE, TASK
			{digit,    hours,      digit_in_fn},
			
			{s,    set,        set_fn},
			
			{enter,   idle,    enter_hour_fn	},
			
			{eol,   idle,     error_fn}
		};
		
		
		//minutes state subtable
		const transition minutes_transitions[]= {
			// INPUT ,  NEXT_STATE,    TASK
			{digit,    mins,      digit_in_fn},
			
			{s,    set,        set_fn},
			
			{enter,   idle,    enter_min_fn},
			
			{eol,   idle,     error_fn}
		};
		
		//seconds state subtable
		const transition seconds_transitions[]= {
			// INPUT , NEXT_STATE, TASK
			{digit,    sec,      digit_in_fn},
			
			{s,    set,        set_fn},
			
			{enter,   idle,    enter_sec_fn},
			
			{eol,   idle,     error_fn}
		};
		
		//pointer of all the subtables
		const transition * ps_transition_ptr[5]=

		{idle_transitions,
			set_transitions,
			hours_transitions,
			minutes_transitions,
			seconds_transitions
		};	

// The finite state machine is implemented as a function with parameters
// corresponding to the present state and key value that has been input.
//***************************************************************************
// Function Name :void fsm (state ps, key keyval)

// Target MCU : AVR128DB48 @ 4MHz
// Author : Tyler Ovenden
// DESCRIPTION: runs through the FSM to run the method assioshcated with this 
// state & go to the next state as determined by the state & key that 
// matches a search
//**************************************************************************
void fsm (state ps, key keyval)
{
	// Search the array of transition structures corresponding to the
	// present state for the transition structure that has has keyvalue
	// field value that is equal to current input key value or equal
	// to eol.
	
	int i;
	for (i = 0; (ps_transition_ptr[ps][i].keyval != keyval)
	&& (ps_transition_ptr[ps][i].keyval != eol); i++);

	// i now has the value of the index of the transition structure
	// corresponding to the current intput key value.

	// Invoke the task function pointed to by the task function pointer
	// of the current transition structure.

	ps_transition_ptr[ps][i].tf_ptr();

	// Make present state equal to the next state value of the current
	// transition structure.

	present_state = ps_transition_ptr[ps][i].next_state;

}	



