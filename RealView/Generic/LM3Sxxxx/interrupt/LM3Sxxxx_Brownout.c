//*****************************************************************************
//
// LM3Sxxxx_Brownout.c - Implements sysctl initialization for use
//                     with managed interrupts
//
//
//*****************************************************************************

#include "ARM_irq.h"             // LabView interrupt interface

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "interrupt.h"
#include "sysctl.h"


//*****************************************************************************
//
// Configures a brown-out to generate interrupts
//
// params -  none
// 
// 
//
// This function is called by the LabView managed interrupts interface.
//
//
//*****************************************************************************
void
LM3Sxxxx_BrownoutIntInit()
{
	
	// configure the brownout
	//SysCtlBrownOutConfigSet(0, 0);  // this should be startup value
	
    // configure interrupt
	SysCtlIntEnable(SYSCTL_INT_BOR);
	IntPrioritySet(INT_SYSCTL, 4 << 5);
}
