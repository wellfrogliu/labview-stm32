//*****************************************************************************
//
// LM3Sxxxx_Timer3.c - Implements simple Timer initialization for use
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

#include "ARM_irq.h"             // LabView interrupt interface

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_timer.h"
#include "interrupt.h"
#include "timer.h"
#include "sysctl.h"

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
LM3Sxxxx_Timer3Init(unsigned long ulCount, unsigned char ucUnits)
{
    //
	// Adjust input by requested unit type
	//	
	if(ucUnits == 1)  // milliseconds
	{
		ulCount *= SysCtlClockGet() / 1000;
	}
	else if (ucUnits == 2)  // microseconds
	{
		ulCount *= SysCtlClockGet() / 1000000;
	}

    //
    // Configure the timer for 32-bit periodic.  Set it to
    // stall on debugger stop.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    TimerConfigure(TIMER3_BASE, TIMER_CFG_32_BIT_PER);
    TimerControlStall(TIMER3_BASE, TIMER_A, 1);

    //
    // Set the timer load value.
    //
    TimerLoadSet(TIMER3_BASE, TIMER_A, ulCount);

    //
    // Enable the timer to run, and enable the timer to generate interrupts,
    // and set the interrupt priority.
    // Interrupts still need to be enabled on the master.
    //
    TimerEnable(TIMER3_BASE, TIMER_A);
    TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    IntPrioritySet(INT_TIMER3A, 4 << 5);
}
