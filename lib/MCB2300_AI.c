/***************************************************************************************
	NI CONFIDENTIAL

	MCB2300_AI.c - Functions to implement analog input (D2A).

	© 2003-2008 National Instruments Corporation.
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Embedded Development Module) and may not be copied, modified, or redistributed.  The code may
     only be used in accordance with the terms of the license agreement entered into between
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.
	
	Author: Nick Van Sickel
********************************************************************************************/

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include <LPC23xx.H>                    /* LPC23xx definitions               */
#define MainClock	12 // MHz

uInt8 _gn2300AnalogChanEnable = 0;

void MCB2300_AIInit(int32 nErrIn, uInt8 nChanEnable, int32* pnErrOut) {
	int32 m=0, n=0, pll=0, cclksel=0, cclk=0, pclk=0, clkdiv;
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}

	if(nChanEnable != _gn2300AnalogChanEnable)
	{
 	m = PLLSTAT & 0x7FFF;  // PLL multiplier (bits 14:0)
	n = (PLLSTAT & 0xFF0000) >> 16;	// PLL divider (bits 23:16)
	pll = 2 * (m+1) * MainClock / (n+1); // MHz
	cclksel = CCLKCFG & 0xFF;  // pll divider to get clock 
	cclk = pll / (cclksel + 1);	 // clck in MHz
	pclk = cclk/4;
	clkdiv = (10 * pclk + 45)/45;  // ADC clock must be <= 4.5MHz

	if (nChanEnable & 0x1) { //AD0.0 enable
		PINSEL1 &= ~0x8000;
		PINSEL1 |= 0x4000;
	}
	if (nChanEnable & 0x2) { //AD0.1 enable
		PINSEL1 &= ~0x20000;
		PINSEL1 |= 0x10000;
	}
	if (nChanEnable & 0x4) { //AD0.2 enable
		PINSEL1 &= ~0x80000;
		PINSEL1 |= 0x40000;
	}
	if (nChanEnable & 0x8) { //AD0.3 enable
		PINSEL1 &= ~0x200000;
		PINSEL1 |= 0x100000;
	}
	if (nChanEnable & 0x10) { //AD0.4 enable
		PINSEL3 |= 0x30000000;
	}
	if (nChanEnable & 0x20) { //AD0.5 enable
		PINSEL3 |= 0xC0000000;
	}
	if (nChanEnable & 0x40) { //AD0.6 enable
		PINSEL0 |= 0x3000000;
	}
	if (nChanEnable & 0x80) { //AD0.7 enable
		PINSEL0 |= 0xC000000;
	}

	PCONP |= 0x1000; //power on adc
	AD0CR = 0x200000 | (clkdiv << 8);	// enable adc
	AD0INTEN = 0;  // disable interrupts

	_gn2300AnalogChanEnable |= nChanEnable;
	}

	if (pnErrOut) {
		*pnErrOut = 0;
	}
	return;
}

void MCB2300_AnalogIn(uInt8 nChan, int32 nErrIn, int32 *pnValue, Boolean *pbOverrun, int32* pnErrOut) {	
	int32 nValue=0, bOverrun=false;
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}

    //check if channel is valid and has been initialized
	if( (nChan > 7) || ((!_gn2300AnalogChanEnable)&(0x01 << nChan)) )  
	{
	 	if (pnErrOut) {
			*pnErrOut = 1;
		}
		return;
	}

	AD0CR |= 0x1000000 | (1 << nChan); // enable channel
	switch( nChan ) {
		case 0 :
			while (	!(AD0DR0 & 0x80000000)) { // wait for completion
				;
			}
			nValue = ((AD0DR0 & 0xFFC0) >> 6);
			bOverrun = (AD0DR0 & 0x4000000)?1:0;
			break;
		case 1 :
			while (	!(AD0DR1 & 0x80000000)) { // wait for completion
				;
			}
			nValue = ((AD0DR1 & 0xFFC0) >> 6);
			bOverrun = (AD0DR1 & 0x4000000)?1:0;
			break;
		case 2 :
			while (	!(AD0DR2 & 0x80000000)) { // wait for completion
				;
			}
			nValue = ((AD0DR2 & 0xFFC0) >> 6);
			bOverrun = (AD0DR2 & 0x4000000)?1:0;
			break;
		case 3 :
			while (	!(AD0DR3 & 0x80000000)) { // wait for completion
				;
			}
			nValue = ((AD0DR3 & 0xFFC0) >> 6);
			bOverrun = (AD0DR3 & 0x4000000)?1:0;
			break;
		case 4 :
			while (	!(AD0DR4 & 0x80000000)) { // wait for completion
				;
			}
			nValue = ((AD0DR4 & 0xFFC0) >> 6);
			bOverrun = (AD0DR4 & 0x4000000)?1:0;
			break;
		case 5 :
			while (	!(AD0DR5 & 0x80000000)) { // wait for completion
				;
			}
			nValue = ((AD0DR5 & 0xFFC0) >> 6);
			bOverrun = (AD0DR5 & 0x4000000)?1:0;
			break;
		case 6 :
			while (	!(AD0DR6 & 0x80000000)) { // wait for completion
				;
			}
			nValue = ((AD0DR6 & 0xFFC0) >> 6);
			bOverrun = (AD0DR6 & 0x4000000)?1:0;
			break;
		case 7 :
			while (	!(AD0DR7 & 0x80000000)) { // wait for completion
				;
			}
			nValue = ((AD0DR7 & 0xFFC0) >> 6);
			bOverrun = (AD0DR7 & 0x4000000)?1:0;
			break;
	}
	AD0CR &= ~(1 << nChan);	  //disable channel
	if (pnValue) {
		*pnValue = nValue;
	}
	if (pbOverrun) {
		*pbOverrun = bOverrun;
	}
	if (pnErrOut) {
		*pnErrOut = 0;
	}
	return;
}
