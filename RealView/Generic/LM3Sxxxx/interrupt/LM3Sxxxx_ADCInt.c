//*****************************************************************************
//
// LM3Sxxxx_ADC.c - Implements ADC initialization for use
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
#include "adc.h"
#include "gpio.h"


//
// Maps sequence numbers to the interrupt vector
//
static unsigned long const ulADCIntMap[] =
{
    INT_ADC0, INT_ADC1, INT_ADC2
};

//*****************************************************************************
//
// Configures an ADC sequence to generate periodic interrupts.
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
LM3Sxxxx_ADCIntInit(unsigned int sequence, unsigned int samples, unsigned int speed,
					unsigned int trigger, unsigned int pins, unsigned int priority, unsigned int differential)
{

	int i;
	
	// enable external pin if needed
	if(trigger == ADC_TRIGGER_EXTERNAL) {
		// The external pin is enabled by using the interrupt manager to specifiy PB4 as an interrupt.
	}
	
	// initialize ADC peripheral
	LM3Sxxxx_ADC_Init(speed);
	
	//
    // configure sequencer
    //
    ADCSequenceConfigure(ADC_BASE, sequence, trigger, priority);
	
	//
    // Program the sequencer to take specified number of samples and generate interrupt at the end
    //
	for(i = 0; i < samples; i++) {
		if(i < (samples - 1)) {
			ADCSequenceStepConfigure(ADC_BASE, sequence, i, pins | differential);
		}
		else {
			ADCSequenceStepConfigure(ADC_BASE, sequence, i, ADC_CTL_IE | ADC_CTL_END | pins | differential);
		}
	}
	
    //
    // enable the sequencer
    //
    ADCSequenceEnable(ADC_BASE, sequence);
	
    // configure interrupt
	ADCIntEnable(ADC_BASE, sequence);
	IntPrioritySet(ulADCIntMap[sequence], 4 << 5);
}
