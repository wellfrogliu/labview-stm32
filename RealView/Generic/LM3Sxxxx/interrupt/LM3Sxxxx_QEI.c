//*****************************************************************************
//
// LM3Sxxxx_QEI.c - Implements QEI initialization for use
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
#include "qei.h"

// Maps channel to corresponding interrupt vector
static unsigned long const ulQEIBaseMap[] =
{
    QEI0_BASE, QEI1_BASE
};

// Maps channel to corresponding interrupt vector
static unsigned long const ulQEIIntMap[] =
{
    INT_QEI0, INT_QEI1
};


//*****************************************************************************
//
// Configures a QEI to generate interrupts
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
LM3Sxxxx_QEIIntInit(unsigned int ulChannel, unsigned int ulIntFlags, unsigned char bQuadMode, unsigned char bSwap,
					unsigned char bCapAB, unsigned int ulMaxPosition, unsigned char bResetIdx, unsigned char bVelEnable, 
					unsigned int ulPreDiv, unsigned int ulPeriod)
{	
	
	//clock peripheral and configure the GPIO pins for QEI functionality
	if(ulChannel == 0) {  // QEI0
		SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
		GPIOPinTypeQEI(GPIO_PORTC_BASE, 0x50); // PC4 and PC6 (phA/clk and phB/dir)
		GPIOPinTypeQEI(GPIO_PORTD_BASE, 0x80); // PD7 (index)
	}
	else {  // QEI1
		SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
		GPIOPinTypeQEI(GPIO_PORTE_BASE, 0xC); // PE2 and PE3	 (phA/clk and phB/dir)
		GPIOPinTypeQEI(GPIO_PORTF_BASE, 0x2); // PF1  (index)
	}
	
	// configure the QEI peripheral
	if(bQuadMode) {
		QEIConfigure(ulQEIBaseMap[ulChannel], QEI_CONFIG_QUADRATURE | (bSwap << 1) | (bResetIdx << 4) | (bCapAB << 3) | (1 << 12), ulMaxPosition);
	}
	else {
		QEIConfigure(ulQEIBaseMap[ulChannel], QEI_CONFIG_NO_SWAP | QEI_CONFIG_CLOCK_DIR | (bResetIdx << 4) | (1 << 12), ulMaxPosition);
	}
	
	// enable QEI
	QEIEnable(ulQEIBaseMap[ulChannel]);
	
	// configuire and enable velocity measurements
	if(bVelEnable) {
        // adjust period to millisecond equivelent
		ulPeriod *= SysCtlClockGet() / 1000;
		QEIVelocityConfigure(ulQEIBaseMap[ulChannel], ulPreDiv, ulPeriod);
		QEIVelocityEnable(ulQEIBaseMap[ulChannel]);
	}
	
    // configure interrupt
	QEIIntEnable(ulQEIBaseMap[ulChannel], ulIntFlags);
	IntPrioritySet(ulQEIIntMap[ulChannel], 4 << 5);
}
