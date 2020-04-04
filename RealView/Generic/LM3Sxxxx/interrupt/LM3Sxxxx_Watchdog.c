//*****************************************************************************
//
// LM3Sxxxx_Watchdog.c - Implements Watchdog initialization for use
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
// Configures a Watchdog to generate interrupts
//
// params - 
// 	bLock - specifies if we should lock the watchdog registers after config
// 	ucUnits - specifies units of ulCount
//	ulCount - value that is loaded into watchdog timer
//
// This function is called by the LabView managed interrupts interface.
//
//
//*****************************************************************************
void
LM3Sxxxx_WatchdogIntInit(unsigned char bLock, unsigned char ucUnits, unsigned int ulCount)
{

	// clock the peripheral
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG);
	
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
	
	// configure the watchdog reset timer
	WatchdogReloadSet(WATCHDOG_BASE, ulCount);
	
	// configure watchdog stall on debug
	WatchdogStallEnable(WATCHDOG_BASE);
	
	// enable the watchdog timer (only if we arent in debug mode)
	#ifndef LV_DBG_MODE
	WatchdogResetEnable(WATCHDOG_BASE);
	WatchdogEnable(WATCHDOG_BASE);
	#else
	#warning The watchdog peripheral will not be enabled while in debug mode.
	#endif
	
    // configure interrupt
	WatchdogIntEnable(WATCHDOG_BASE);
	IntPrioritySet(INT_WATCHDOG, 7 << 5);  // set to low priority
	
	// lock registers if requested
	if(bLock)
		WatchdogLock(WATCHDOG_BASE);
}
