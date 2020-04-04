//*****************************************************************************
//
// LM3Sxxxx_Timer0.c - Implements simple Timer initialization for use
//                     with managed interrupts
//
// Copyright (c) 2008 Luminary Micro, Inc.  All rights reserved.
// 
// Software License Agreement
// 
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
// 
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 61 of the Stellaris NI-LabVIEW support package.
//
//*****************************************************************************

#include <cstdarg>

#include "ARM_irq.h"             // LabView interrupt interface

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_timer.h"
#include "interrupt.h"
#include "timer.h"
#include "sysctl.h"

//
// Maps mode to corresponding interrupt vector
//
static unsigned long const ulTimerIntMap[] =
{
    TIMER_TIMA_TIMEOUT, TIMER_CAPA_EVENT, TIMER_CAPA_MATCH
};

//*****************************************************************************
//
// Configures a Timer to generate periodic interrupts.
//
// ulCount - the timer period in clock ticks, or milliseconds
// bMillisec - if TRUE, then ulCount represents milliseconds, otherwise
// system clock ticks.
//
// This function is called by the LabView managed interrupts interface.
// It configures a timer to operate in 32-bit periodic mode, and to
// generate an interrupt.  After this function runs, the timer will be
// running and generating interrupts at the specified rate, but the
// interrupts are still not enabled on the interrupt controller.  A separate
// function call takes care of that.
//
//*****************************************************************************
void
LM3Sxxxx_Timer0Init_new(unsigned long ulCount0, unsigned char ucUnits,
					unsigned long ulMode, unsigned long ulCount1,
					unsigned long ulCount2, unsigned long ulTrigger1, unsigned long ulTrigger2)
{
    
	// clock the peripheral and set to stall on debug stop
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerControlStall(TIMER0_BASE, TIMER_A, 1);
	
	switch(ulMode) {
	
		case 0: // TIMER_TIMA_TIMEOUT
			//
			// Adjust input by requested unit type
			//	
			if(ucUnits == 1)  // milliseconds
			{
				ulCount0 *= SysCtlClockGet() / 1000;
			}
			else if (ucUnits == 2)  // microseconds
			{
				ulCount0 *= SysCtlClockGet() / 1000000;
			}

			// Configure the timer for 32-bit periodic. 
			TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
			
			//
			// Set the timer load value.
			//
			TimerLoadSet(TIMER0_BASE, TIMER_A, ulCount0);
			
			break;
			
		case 1: // TIMER_CAPA_EVENT
		
			// set pin CCP0 to capture edges
			GPIOPinTypeTimer(GPIO_PORTD_BASE, 0x10);
		
			// Configure the timer for 16-bit count capture. 
			TimerConfigure(TIMER0_BASE, TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_CAP_TIME);
			
			// set trigger
			TimerControlEvent(TIMER0_BASE, TIMER_A, ulTrigger1);
			
			//
			// Set the timer load value.
			//
			TimerLoadSet(TIMER0_BASE, TIMER_A, ulCount1);
		
			break;
			
		case 2: // TIMER_CAPA_MATCH
		
			// set pin CCP0 to capture edges
			GPIOPinTypeTimer(GPIO_PORTD_BASE, 0x10);
			
			// Configure the timer for 16-bit edge counter. 
			TimerConfigure(TIMER0_BASE, TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_CAP_COUNT);
			
			// set trigger
			TimerControlEvent(TIMER0_BASE, TIMER_A, ulTrigger2);
			
			// set match register to 0
			TimerMatchSet(TIMER0_BASE, TIMER_A, 0);
			
			//
			// Set the timer load value.
			//
			TimerLoadSet(TIMER0_BASE, TIMER_A, ulCount2);
		
			break;
			
		default:
		
			break;
	}

    //
    // Enable the timer to run, and enable the timer to generate interrupts,
    // and set the interrupt priority.
    // Interrupts still need to be enabled on the master.
    //
    TimerEnable(TIMER0_BASE, TIMER_A);
    TimerIntEnable(TIMER0_BASE, ulTimerIntMap[ulMode]);
    IntPrioritySet(INT_TIMER0A, 4 << 5);
}

// We use va_list here to handle the possibility of different parameters, since we cannoty overload functions in C.
// This is needed because users of previous versions of LV may have two parameters, but users of the new version will have 8 parameters.
void
LM3Sxxxx_Timer0Init(unsigned long marker_or_ulCount, ... )
{
	unsigned long ulCount0, bMillisec, ulCount1, ulCount2, ulTrigger1, ulTrigger2;
	unsigned char ulMode;
	va_list arguments;
	
	va_start(arguments, marker_or_ulCount);
	
	if(marker_or_ulCount == (unsigned long)-1) {  // -1 serves as a special marker here
	
		ulCount0 = va_arg(arguments, unsigned long);
		bMillisec = va_arg(arguments, unsigned char);
		ulMode = va_arg(arguments, unsigned long);
		ulCount1 = va_arg(arguments, unsigned long);
		ulCount2 = va_arg(arguments, unsigned long);
		ulTrigger1 = va_arg(arguments, unsigned long);
		ulTrigger2 = va_arg(arguments, unsigned long);
		
		LM3Sxxxx_Timer0Init_new(ulCount0, bMillisec, ulMode, ulCount1, ulCount2, ulTrigger1, ulTrigger2);
	}
	else {
	
		bMillisec = va_arg(arguments, unsigned char);
		
		LM3Sxxxx_Timer0Init_new(marker_or_ulCount, bMillisec, 0, 0, 0, 0, 0);
	}
	
	va_end (arguments);
}	

