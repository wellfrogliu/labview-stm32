//*****************************************************************************
//
// LM3Sxxxx_ADC.c - Implements Elemental IO function to read ADC channels.
//
// This implementation was written for the LM3S8962, but should work for
// for any Luminary Micro Stellaris microcontroller with ADC inputs.
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
// This is part of revision 26 of the Stellaris NI-LabVIEW support package.
//
//*****************************************************************************

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_adc.h"
#include "sysctl.h"
#include "adc.h"

//
// The ADC sequencer to use for EIO single conversions.
//
#define EIO_ADC_SEQ 3

static unsigned char bADCInit = 0;

//*****************************************************************************
//
// initialize the adc peripheral
//
//*****************************************************************************
void
LM3Sxxxx_ADC_Init(unsigned int adcSpeed)
{
	if(!bADCInit)
    {
		bADCInit = 1;
	
		//
		// enable and reset the ADC block
		//
		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
		SysCtlPeripheralReset(SYSCTL_PERIPH_ADC);

		//
		// set the sampling frequency
		//
		SysCtlADCSpeedSet(adcSpeed);
	}
	
}

//*****************************************************************************
//
// Reads an ADC channel in a polling fashion and returns the result.
//
// This function reads the specified ADC channel and returns the result
// to the caller through a pointer parameter.  Sequencer 3 is used, set
// up to take one sample at the lowest priority.
//
// No error checking is done on the inputs.  At the moment we assume
// this is only being called from well behaved code (the EIO model VI).
//
//*****************************************************************************
void
LM3Sxxxx_ADC_Read_FN(unsigned int uChannel, unsigned int *puValue)
{
    static unsigned char bInit = 0;
    unsigned long ulADCBuf[8];
    unsigned long ulCount;

    //
    // perform one-time initializations
    //
    LM3Sxxxx_ADC_Init(SYSCTL_ADCSPEED_500KSPS);
	if(!bInit)
	{
		bInit = 1;
		
		//
        // use sequencer #3 for ADC EIO, use lowest priority
        //
        ADCSequenceConfigure(ADC_BASE, EIO_ADC_SEQ, ADC_TRIGGER_PROCESSOR, 3);
	}

    //
    // Program the sequencer to take one sample on the channel
    //
    ADCSequenceStepConfigure(ADC_BASE, EIO_ADC_SEQ, 0, ADC_CTL_END | uChannel);

    //
    // enable the sequencer
    //
    ADCSequenceEnable(ADC_BASE, EIO_ADC_SEQ);

    //
    // read the data to flush anything in the FIFO
    //
    ADCSequenceDataGet(ADC_BASE, EIO_ADC_SEQ, ulADCBuf);

    //
    // start a conversion
    //
    ADCProcessorTrigger(ADC_BASE, EIO_ADC_SEQ);

    //
    // try to read the data until data is available
    //
    do
    {
        ulCount = ADCSequenceDataGet(ADC_BASE, EIO_ADC_SEQ, ulADCBuf);
    } while(ulCount == 0);

    //
    // copy the ADC value to the caller
    //
    *puValue = ulADCBuf[0];
}
