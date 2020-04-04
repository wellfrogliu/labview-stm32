/***************************************************************************************
	NI CONFIDENTIAL

	MCB2400_PWM.c - Functions to implement PWM output.

	© 2003-2008 National Instruments Corporation.
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Embedded Development Module) and may not be copied, modified, or redistributed.  The code may
     only be used in accordance with the terms of the license agreement entered into between
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.
	
	Author: Nick Van Sickel
********************************************************************************************/
#ifndef MCB2400_PWM_C
#define MCB2400_PWM_C

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include <LPC23xx.H>                    /* LPC23xx/LPC24xx definitions                */

#define MainClock	12 // MHz
static int32 gnCycleTime = 0L;
static char called = 0;

void MCB2400_PWM0Init(int32 nCycleTime, int32 nErrIn, int32* pnErrOut) {
	int32 m=0, n=0, pll=0, cclksel=0, cclk=0, pclk=0;

	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}

	if(!called)
	{

    PCONP |= 0x20;              //enable power to PWM0

	m = PLLSTAT & 0x7FFF;  // PLL multiplier (bits 14:0)
	n = (PLLSTAT & 0xFF0000) >> 16;	// PLL divider (bits 23:16)
	pll = 2 * (m+1) * MainClock / (n+1); // MHz
	cclksel = CCLKCFG & 0xFF;  // pll divider to get clock 
	cclk = pll / (cclksel + 1);	 // clck in MHz
	pclk = cclk/4;
   	PWM0PR = pclk;	 // pre scale register to get 1us
	PWM0MR0 = nCycleTime;	 // match register 0 gives cycle time
	
	PWM0MCR	= 2;
	
	PWM0CTCR = 0; // PWM count control register - timer mode
	PWM0TCR |= 0x9; // PWM timer control register - enable timer counter and prescale counter
	PWM0LER |= 1;
	gnCycleTime = nCycleTime;

	//set flag
	called = 1;
	}

	if (pnErrOut) {
		*pnErrOut = 0;
	}
}

void MCB2400_PWM0Out( uInt8 nChan, 
					uInt8 nOffset, 
					Boolean bDoubleEdge, 
					Boolean bPolarity, 
					uInt8 nDuration, 
					int32 nErrIn, 
					int32 *pnErrOut ) {

	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}

	if (pnErrOut) {
		*pnErrOut = 0;
	}

	if (nChan < 1 || nChan > 6) {
		if (pnErrOut) {
			*pnErrOut = k_Unexist_Ch_Err;
		}
		return;
	}

	if (bDoubleEdge && ((nOffset + nDuration) > 100)) {
		if (pnErrOut) {
			*pnErrOut = k_Duration_OOB;
		}
		return;
	}
	else if (!bDoubleEdge && nOffset > 100) {
		if (pnErrOut) {
			*pnErrOut = k_Duration_OOB;
		}
		return;
	}

	PWM0PCR |= (1 << (nChan + 8));	 // enable PWM
	PWM0MCR &= ~(7 << (nChan*3)); // no special match control operations
	if (bDoubleEdge) {
		PWM0PCR |= (1 << nChan ); //select double edge
	}
	else {
		PWM0PCR &= ~(1 << nChan ); //select single edge
	}
	switch( nChan ) {
		case 1 :
			if(bDoubleEdge) {   //channel 1 does not support double edge
				if (pnErrOut) {
					*pnErrOut = 1;
				}
				return;	
			}
			PWM0MR1 = nOffset * gnCycleTime / 100; // single edge offset only
			PINSEL7 |= 2; // mark pin 3.16 as PWM1
			break;
		case 2 :
			if (bDoubleEdge) {
				if (bPolarity) {
					PWM0MR1 = nOffset * gnCycleTime / 100;
					PWM0MR2 = (nOffset + nDuration) * gnCycleTime / 100;
				}
				else {
					PWM0MR2 = nOffset * gnCycleTime / 100;
					PWM0MR1 = (nOffset + nDuration) * gnCycleTime / 100;
				}
			}
			else {
				PWM0MR2 = nOffset * gnCycleTime / 100;  // single edge offset only
			}
			PINSEL7 |= 8; // mark pin 3.17 as PWM2
			break;
		case 3 :
			if (bDoubleEdge) {
				if (bPolarity) {
					PWM0MR2 = nOffset * gnCycleTime / 100;
					PWM0MR3 = (nOffset + nDuration) * gnCycleTime / 100;
				}
				else {
					PWM0MR3 = nOffset * gnCycleTime / 100;
					PWM0MR2 = (nOffset + nDuration) * gnCycleTime / 100;
				}
			}
			else {
				PWM0MR3 = nOffset * gnCycleTime / 100; // single edge offset only
			}
			PINSEL7 |= 0x20; // mark pin 3.18 as PWM3
			break;
		case 4 :
			if (bDoubleEdge) {
				if (bPolarity) {
					PWM0MR3 = nOffset * gnCycleTime / 100;
					PWM0MR4 = (nOffset + nDuration) * gnCycleTime / 100;
				}
				else {
					PWM0MR4 = nOffset * gnCycleTime / 100;
					PWM0MR3 = (nOffset + nDuration) * gnCycleTime / 100;
				}
			}
			else {
				PWM0MR4 = nOffset * gnCycleTime / 100;	 // single edge offset only
			}
			PINSEL7 |= 0x80;// mark pin 3.19 as PWM4
			break;
		case 5 :
			if (bDoubleEdge) {
				if (bPolarity) {
					PWM0MR4 = nOffset * gnCycleTime / 100;
					PWM0MR5 = (nOffset + nDuration) * gnCycleTime / 100;
				}
				else {
					PWM0MR5 = nOffset * gnCycleTime / 100;
					PWM0MR4 = (nOffset + nDuration) * gnCycleTime / 100;
				}
			}
			else {
				PWM0MR5 = nOffset * gnCycleTime / 100;  // single edge offset only
			}
			PINSEL7 |= 0x200;// mark pin 3.20 as PWM5
			break;
		case 6 :
			if (bDoubleEdge) {
				if (bPolarity) {
					PWM0MR5 = nOffset * gnCycleTime / 100;
					PWM0MR6 = (nOffset + nDuration) * gnCycleTime / 100;
				}
				else {
					PWM0MR6 = nOffset * gnCycleTime / 100;
					PWM0MR5 = (nOffset + nDuration) * gnCycleTime / 100;
				}
			}
			else {
				PWM0MR6 = nOffset * gnCycleTime / 100;	 // single edge offset only
			}
			PINSEL7 |= 0x800; // mark pin 3.21 as PWM6
			break;
		default :
			if (pnErrOut) {
				*pnErrOut = 1;
			}
			return;
	}
	// activate PWM
	if (bDoubleEdge && nChan > 1) {
		PWM0LER |= ((3 << (nChan-1))); 
	}
	else {
		PWM0LER |= (1 << nChan );
	}

    PWM0TCR = 0x00000002;                      // Reset counter and prescaler           
    PWM0TCR = 0x00000009;                      // enable counter and PWM, release counter from reset 

	return;
}

void MCB2400_PWM1Init(int32 nCycleTime, int32 nErrIn, int32* pnErrOut) {
	int32 m=0, n=0, pll=0, cclksel=0, cclk=0, pclk=0;

	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}

	if(gnCycleTime != nCycleTime)
	{

    PINSEL10 = 0;                //disable ETM interface

	PCONP |= 0x40;              //enable power to PWM1

	m = PLLSTAT & 0x7FFF;  // PLL multiplier (bits 14:0)
	n = (PLLSTAT & 0xFF0000) >> 16;	// PLL divider (bits 23:16)
	pll = 2 * (m+1) * MainClock / (n+1); // MHz
	cclksel = CCLKCFG & 0xFF;  // pll divider to get clock 
	cclk = pll / (cclksel + 1);	 // clck in MHz
	pclk = cclk/4;
   	PWM1PR = pclk;	 // pre scale register to get 1us
	PWM1MR0 = nCycleTime;	 // match register 0 gives cycle time
	
	PWM1MCR	= 2;
	
	PWM1CTCR = 0; // PWM count control register - timer mode
	PWM1TCR |= 0x9; // PWM timer control register - enable timer counter and prescale counter
	PWM1LER |= 1;
	gnCycleTime = nCycleTime;

	}
	if (pnErrOut) {
		*pnErrOut = 0;
	}
}

void MCB2400_PWM1Out( uInt8 nChan, 
					uInt8 nOffset, 
					Boolean bDoubleEdge, 
					Boolean bPolarity, 
					uInt8 nDuration, 
					int32 nErrIn, 
					int32 *pnErrOut ) {

	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}

	if (pnErrOut) {
		*pnErrOut = 0;
	}

	//check if user OOB
	if (nChan < 1 || nChan > 6) {
		if (pnErrOut) {
			*pnErrOut = k_Unexist_Ch_Err;
		}
		return;
	}

	//user cannot perform double edge on channel 1
	if (nChan == 1 && bDoubleEdge) {
		if (pnErrOut) {
			*pnErrOut = 1;
		}
		return;
	}

	if (bDoubleEdge && ((nOffset + nDuration) > 100)) {
		if (pnErrOut) {
			*pnErrOut = k_Duration_OOB;
		}
		return;
	}
	else if (!bDoubleEdge && nOffset > 100) {
		if (pnErrOut) {
			*pnErrOut = k_Duration_OOB;
		}
		return;
	}

	PWM1PCR |= (1 << (nChan + 8));	 // enable PWM
	PWM1MCR &= ~(7 << (nChan*3)); // no special match control operations
	if (bDoubleEdge) {
		PWM1PCR |= (1 << nChan ); //select double edge
	}
	else {
		PWM1PCR &= ~(1 << nChan ); //select single edge
	}
	switch( nChan ) {
		case 1 :
			PWM1MR1 = nOffset * gnCycleTime / 100; // single edge offset only
			PINSEL4 |= 1; // mark pin 2.0 as PWM1
			break;
		case 2 :
			if (bDoubleEdge) {
				if (bPolarity) {
					PWM1MR1 = nOffset * gnCycleTime / 100;
					PWM1MR2 = (nOffset + nDuration) * gnCycleTime / 100;
				}
				else {
					PWM1MR2 = nOffset * gnCycleTime / 100;
					PWM1MR1 = (nOffset + nDuration) * gnCycleTime / 100;
				}
			}
			else {
				PWM1MR2 = nOffset * gnCycleTime / 100;  // single edge offset only
			}
			PINSEL4 |= 4; // mark pin 2.1 as PWM2
			break;
		case 3 :
			if (bDoubleEdge) {
				if (bPolarity) {
					PWM1MR2 = nOffset * gnCycleTime / 100;
					PWM1MR3 = (nOffset + nDuration) * gnCycleTime / 100;
				}
				else {
					PWM1MR3 = nOffset * gnCycleTime / 100;
					PWM1MR2 = (nOffset + nDuration) * gnCycleTime / 100;
				}
			}
			else {
				PWM1MR3 = nOffset * gnCycleTime / 100; // single edge offset only
			}
			PINSEL4 |= 0x10; // mark pin 2.2 as PWM3
			break;
		case 4 :
			if (bDoubleEdge) {
				if (bPolarity) {
					PWM1MR3 = nOffset * gnCycleTime / 100;
					PWM1MR4 = (nOffset + nDuration) * gnCycleTime / 100;
				}
				else {
					PWM1MR4 = nOffset * gnCycleTime / 100;
					PWM1MR3 = (nOffset + nDuration) * gnCycleTime / 100;
				}
			}
			else {
				PWM1MR4 = nOffset * gnCycleTime / 100;	 // single edge offset only
			}
			PINSEL4 |= 0x40;// mark pin 2.3 as PWM4
			break;
		case 5 :
			if (bDoubleEdge) {
				if (bPolarity) {
					PWM1MR4 = nOffset * gnCycleTime / 100;
					PWM1MR5 = (nOffset + nDuration) * gnCycleTime / 100;
				}
				else {
					PWM1MR5 = nOffset * gnCycleTime / 100;
					PWM1MR4 = (nOffset + nDuration) * gnCycleTime / 100;
				}
			}
			else {
				PWM1MR5 = nOffset * gnCycleTime / 100;  // single edge offset only
			}
			PINSEL4 |= 0x100;// mark pin 2.4 as PWM5
			break;
		case 6 :
			if (bDoubleEdge) {
				if (bPolarity) {
					PWM1MR5 = nOffset * gnCycleTime / 100;
					PWM1MR6 = (nOffset + nDuration) * gnCycleTime / 100;
				}
				else {
					PWM1MR6 = nOffset * gnCycleTime / 100;
					PWM1MR5 = (nOffset + nDuration) * gnCycleTime / 100;
				}
			}
			else {
				PWM1MR6 = nOffset * gnCycleTime / 100;	 // single edge offset only
			}
			PINSEL4 |= 0x400; // mark pin 2.5 as PWM6
			break;
		default :
			if (pnErrOut) {
				*pnErrOut = 1;
			}
			return;
	}
	// activate PWM
	if (bDoubleEdge && nChan > 1) {
		PWM1LER |= ((3 << (nChan-1))); 
	}
	else {
		PWM1LER |= (1 << nChan );
	}

	PWM1TCR = 0x00000002;                      // Reset counter and prescaler           
    PWM1TCR = 0x00000009;                      // enable counter and PWM, release counter from reset 

	return;
}

/*Wrapper for the two different PWM*/
void MCB2400_PWMInit(uInt8 select, int32 nCycleTime, int32 nErrIn, int32* pnErrOut) {
	if(select){
		MCB2400_PWM1Init(nCycleTime, nErrIn, pnErrOut);
	}
	else{
	 	MCB2400_PWM0Init(nCycleTime, nErrIn, pnErrOut);
	}
}

void MCB2400_PWMOut(uInt8 select, 
					uInt8 nChan, 
					uInt8 nOffset, 
					Boolean bDoubleEdge, 
					Boolean bPolarity, 
					uInt8 nDuration, 
					int32 nErrIn, 
					int32 *pnErrOut ) {
	if(select == 1){
		MCB2400_PWM1Out( nChan, 
					     nOffset, 
						 bDoubleEdge, 
						 bPolarity, 
						 nDuration, 
						 nErrIn, 
						 pnErrOut );
	}
	else if(select == 0){
	 	MCB2400_PWM0Out( nChan, 
					     nOffset, 
						 bDoubleEdge, 
						 bPolarity, 
						 nDuration, 
						 nErrIn, 
						 pnErrOut );
	}
	else{   //user OOB
		if (pnErrOut) {
			*pnErrOut = 1;
		}
	}
}


#endif
