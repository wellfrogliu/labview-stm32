/***************************************************************************************
	NI CONFIDENTIAL

	MCB2400_AO.c - Functions to implement analog out (D2A).

	© 2003-2008 National Instruments Corporation.
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Embedded Development Module) and may not be copied, modified, or redistributed.  The code may
     only be used in accordance with the terms of the license agreement entered into between
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.
	
	Author: Nick Van Sickel
********************************************************************************************/
#ifndef MCB2400_AO_C
#define MCB2400_AO_C

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include <LPC23xx.H>                    /* LPC23xx/LPC24xx definitions                */

void MCB2400_AnalogOut( uInt16 nValue, int32 nErrIn, int32 *pnErrOut ) {

	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}

	//check if user OOB
	if (nValue > 1023)
	{
	   	if (pnErrOut) {
			*pnErrOut = 1;
		}
		return;
	}

	PINSEL1 |= 0x200000; //use pin 0.26 
   	DACR = ((nValue & 0x3FF) << 6);	 //default to high-power mode

	if (pnErrOut) {
		*pnErrOut = 0;
	}
}


#endif
