//*****************************************************************************
//
// LM3Sxxxx_PWM_Fault.c - configures the pwm fault to generate interrupts
//
//*****************************************************************************

#include "ARM_irq.h"             // LabView interrupt interface

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "interrupt.h"
#include "sysctl.h"
#include "pwm.h"

//*****************************************************************************
//
// Configures the PWM fault to generate an interrupt
//
// params - 
//
// This function is called by the LabView managed interrupts interface.
//
//*****************************************************************************
void
LM3Sxxxx_PWM_FaultIntInit()
{
	// ensure peripheral is clocked before setting register
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
	
	//configure fault interrupt register
	PWMIntEnable(PWM_BASE, PWM_INT_FAULT0);
    IntPrioritySet(INT_PWM_FAULT, 4 << 5);
}
