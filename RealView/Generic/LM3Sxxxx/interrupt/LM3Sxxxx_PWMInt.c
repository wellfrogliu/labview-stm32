//*****************************************************************************
//
// LM3Sxxxx_PWM0.c - Implements PWM initialization for use
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
#include "pwm.h"

//
// Array to map PWM channel numbers to PWM generators.
//
static unsigned long const ulPWMGen[] =
{   PWM_GEN_0, PWM_GEN_0, PWM_GEN_1, PWM_GEN_1, PWM_GEN_2, PWM_GEN_2    };

//
// Maps the PWM channel number to PWM output
//
static unsigned long const ulPWMOut[] =
{   PWM_OUT_0, PWM_OUT_1, PWM_OUT_2, PWM_OUT_3, PWM_OUT_4, PWM_OUT_5 };

//
// Maps channel to corresponding interrupt vector
//
static unsigned long const ulPWMIntMap[] =
{
    INT_PWM0, INT_PWM0, INT_PWM1, INT_PWM1, INT_PWM2, INT_PWM2
};

// calculates PWM width in cycles
unsigned int
LM3Sxxxx_PWMCycles(unsigned int uWidth, unsigned int uFreq, unsigned int uType)
{
	switch (uType) {
	
	case 0:  // percent
		return ((50000000 / uFreq) * uWidth) / 100;
		
	case 1:  // hundreths of a percent
		return ((50000000 / uFreq) * uWidth) / 10000;
		
	case 2:  // duty cycle
		return uWidth;
		
	default:
		return 0;
	}
}


//*****************************************************************************
//
// Configures a PWM to generate periodic interrupts.
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
LM3Sxxxx_PWMIntInit(unsigned int uChannelMask, unsigned int uFreq, 
					unsigned int uFlags, unsigned int uDivider, unsigned int uFlags0, 
					unsigned int uFlags1, unsigned int uWidth0, unsigned int uType0, 
					unsigned int uWidth1, unsigned int uType1, unsigned int ulIntFlags)
{
	unsigned int i = 0;
	while(!(uChannelMask & 0x3)) {
		uChannelMask >>= 2;
		i+=2;
	}
	
	// initialize PWM generator 0
	LM3Sxxxx_PWMGen_Initialize(i, uFreq, uFlags, uDivider);
	
	// enable PWM outputs
    if(uChannelMask & 0x1) {
		uWidth0 = LM3Sxxxx_PWMCycles(uWidth0, uFreq, uType0);
		LM3Sxxxx_PWMOut_Initialize(i, uFlags0);
		PWMPulseWidthSet(PWM_BASE, ulPWMOut[i], uWidth0);
	}
	if(uChannelMask & 0x2) {
		uWidth1 = LM3Sxxxx_PWMCycles(uWidth1, uFreq, uType1);
		LM3Sxxxx_PWMOut_Initialize(i+1, uFlags1);
		PWMPulseWidthSet(PWM_BASE, ulPWMOut[i+1], uWidth1);
	}
	
    // configure interrupts
	PWMIntEnable(PWM_BASE, ulPWMGen[i]);
	PWMGenIntTrigEnable(PWM_BASE, ulPWMGen[i], ulIntFlags);
	IntPrioritySet(ulPWMIntMap[i], 4 << 5);
}
