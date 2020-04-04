/***************************************************************************************
	NI CONFIDENTIAL

	MCB2300_LED.c - Functions turn on or off LEDs.

	© 2003-2008 National Instruments Corporation.
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Embedded Development Module) and may not be copied, modified, or redistributed.  The code may
     only be used in accordance with the terms of the license agreement entered into between
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.
	
	Author: Andrew Dove
********************************************************************************************/
#include "LVSysIncludes.h"
#include "LVCCG.h"
#include <LPC23xx.H>                    /* LPC23xx definitions                */

/* Function that turns on requested LED                                       */
void MCB2300_LEDOn (unsigned int num, int32 nErrIn, int32* pnErrOut) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	
	if( num > 7 )
	{
	  	if (pnErrOut) 
		{
			*pnErrOut = k_LED_OOB;
		}
		return;
	}
	
	PINSEL10 = 0;                         /* Disable ETM interface, enable LEDs */
	FIO2MASK0 = ~(1 << num);
	FIO2DIR0 |=  (1 << num);
	FIO2SET0 |= (1 << num);
	if (pnErrOut) {
		*pnErrOut = 0;
	}
}

/* Function that turns off requested LED                                      */
void MCB2300_LEDOff (unsigned int num, int32 nErrIn, int32* pnErrOut) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	
	if( num > 7 )
	{
	  	if (pnErrOut) 
		{
			*pnErrOut = k_LED_OOB;
		}
		return;
	}
	
	PINSEL10 = 0;                         /* Disable ETM interface, enable LEDs */
	FIO2MASK0 = ~(1 << num);
	FIO2DIR0 |=  (1 << num);
	FIO2CLR0 |= (1 << num);
	if (pnErrOut) {
		*pnErrOut = 0;
	}
}

/* Function that outputs value to LEDs                                        */
void MCB2300_LEDSet(unsigned int value, int32 nErrIn, int32* pnErrOut) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	PINSEL10 = 0;                         /* Disable ETM interface, enable LEDs */
	FIO2DIR0  = 0xFF;                /* P2.0..7 defined as Outputs         */
	FIO2MASK0 = 0x00;
	FIO2CLR0 = 0xFF;                       /* Turn off all LEDs                  */
	FIO2SET0 = (value & 0xFF);             /* Turn on requested LEDs             */
	if (pnErrOut) {
		*pnErrOut = 0;
	}
}
