//*****************************************************************************
//
// LM3Sxxxx_Comp.c - Implements analog comparator initialization for use
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
#include "comp.h"
#include "gpio.h"


//*****************************************************************************
//
// Configures an analog comparator to generate interrupts
//
// params - 
// 
// 
//
// This function is called by the LabView managed interrupts interface.
//
//
//*****************************************************************************
void
LM3Sxxxx_Comp0IntInit(unsigned int ulPinFlag, unsigned int ulVoltage,
					  unsigned int ulIntFlag, unsigned int ulInvert)
{

	//enable peripheral clock
	SysCtlPeripheralEnable(SYSCTL_PERIPH_COMP0);	
	
	// configure the GPIO pin for comparator input functionality (C0- or PB4)
	GPIOPinTypeComparator(GPIO_PORTB_BASE, 0x10);

	// configure the GPIO pin for comparator external voltage reference (C0+ or PB6), if needed
	if(ulPinFlag != COMP_ASRCP_REF) {
		GPIOPinTypeComparator(GPIO_PORTB_BASE, 0x40);
	}
													   	
	//configure the GPIO pin for output comparator value   (C0o or PB5)
    GPIODirModeSet(GPIO_PORTB_BASE, 0x20, GPIO_DIR_MODE_HW);
    GPIOPadConfigSet(GPIO_PORTB_BASE, 0x20, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
	
	// set the voltage reference, if needed
	if(ulPinFlag == COMP_ASRCP_REF) {
		ComparatorRefSet(COMP_BASE, ulVoltage);
	}
	
	// configure the comparator peripheral
	ComparatorConfigure(COMP_BASE, 0, ulIntFlag | ulPinFlag | ulInvert);
	
    // configure interrupt
	ComparatorIntEnable(COMP_BASE, 0);
	IntPrioritySet(INT_COMP0, 4 << 5);
}
