/***************************************************************************************
	NI CONFIDENTIAL

	MCB2300_LCD.c - Function to detect if the MCB2300 INT0 button is pressed - no debounce.

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

/* Push Button Definitions */
#define S2     0x00000400  /* P2.10 */

/* Function that initializes the button */
void MCB2300_ButtonInit(int32 nErrIn, int32* pnErrOut) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	FIO2DIR |= (1 << 9);
	FIO2CLR  |= (1 << 9);
	if (pnErrOut) {
		*pnErrOut = 0;
	}
}

/* Function that indicates if the INT0 button is pressed */
void MCB2300_IsButtonPressed(int32 nErrIn, int32* pnErrOut, unsigned char *pbPressed) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	*pbPressed = ((FIO2PIN & S2) == 0);
	if (pnErrOut) {
		*pnErrOut = 0;
	}
}
