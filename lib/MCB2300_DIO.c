/***************************************************************************************
	NI CONFIDENTIAL

	MCB2300_LCD.c - Functions to read or write DGPIO lines.

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

void MCB2300_DigitalOutput( int32 nErrIn, int32* pnErrOut, int32 port, int32 pin, unsigned char cValue ) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	//check if user OOB
	if (pin < 0 || pin > 31) {
		if (pnErrOut) {
			*pnErrOut = 1;
		}
		return;
	}
	if (pnErrOut) {
		*pnErrOut = 0;
	}
	switch( port ) {
		case 0 :
			if (pin < 16) {
				PINSEL0 &= ~(3 << (pin*2));	  //because each pin corresponds to 2 bits
			}
			else {
				PINSEL1 &= ~(3 << ((pin - 16)*2));
			}
			FIO0MASK = ~(1 << pin);
			FIO0DIR |=  (1 << pin);
			if (cValue) {
				FIO0SET =  (1 << pin);
			}
			else {
				FIO0CLR = (1 << pin);
			}
			break;
		case 1 :
			if (pin < 16) {
				PINSEL2 &= ~(3 << (pin*2));
			}
			else {
				PINSEL3 &= ~(3 << ((pin - 16)*2));
			}
			FIO1MASK = ~(1 << pin);
			FIO1DIR |=  (1 << pin);
			if (cValue) {
				FIO1SET =  (1 << pin);
			}
			else {
				FIO1CLR = (1 << pin);
			}
			break;
		case 2 :
			if (pin < 16) {
				PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
				PINSEL4 &= ~(3 << (pin*2));
			}
			else {
				PINSEL5 &= ~(3 << ((pin - 16)*2));
			}
			FIO2MASK = ~(1 << pin);
			FIO2DIR |=  (1 << pin);
			if (cValue) {
				FIO2SET =  (1 << pin);
			}
			else {
				FIO2CLR = (1 << pin);
			}
			break;
		case 3 :
			if (pin < 16) {
				PINSEL6 &= ~(3 << (pin*2));
			}
			else {
				PINSEL7 &= ~(3 << ((pin - 16)*2));
			}
			FIO3MASK = ~(1 << pin);
			FIO3DIR |=  (1 << pin);
			if (cValue) {
				FIO3SET =  (1 << pin);
			}
			else {
				FIO3CLR = (1 << pin);
			}
			break;
		case 4 :
			if (pin < 16) {
				PINSEL8 &= ~(3 << (pin*2));
			}
			else {
				PINSEL9 &= ~(3 << ((pin - 16)*2));
			}
			FIO4MASK = ~(1 << pin);
			FIO4DIR |=  (1 << pin);
			if (cValue) {
				FIO4SET =  (1 << pin);
			}
			else {
				FIO4CLR = (1 << pin);
			}
			break;
		default :
			if (pnErrOut) {
				*pnErrOut = 1;
			}
			break;
	}
	return;
}

void MCB2300_DigitalPortOutput32( int32 nErrIn, int32* pnErrOut, int32 port, unsigned long nValue ) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (pnErrOut) {
		*pnErrOut = 0;
	}
	switch( port ) {
		case 0 :
			PINSEL0 = 0;
			PINSEL1 = 0;
			FIO0MASK = 0;
			FIO0DIR =  0xFFFFFFFF;
			FIO0PIN = nValue;
			break;
		case 1 :
			PINSEL2 = 0;
			PINSEL3 = 0;
			FIO1MASK = 0;
			FIO1DIR =  0xFFFFFFFF;
			FIO1PIN = nValue;
			break;
		case 2 :
			PINSEL4 = 0;
			PINSEL5 = 0;
			PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
			FIO2MASK = 0;
			FIO2DIR =  0xFFFFFFFF;
			FIO2PIN = nValue;
			break;
		case 3 :
			PINSEL6 = 0;
			PINSEL7 = 0;
			FIO3MASK = 0;
			FIO3DIR =  0xFFFFFFFF;
			FIO3PIN = nValue;
			break;
		case 4 :
			PINSEL8 = 0;
			PINSEL9 = 0;
			FIO4MASK = 0;
			FIO4DIR =  0xFFFFFFFF;
			FIO4PIN = nValue;
			break;
		default :
			if (pnErrOut) {
				*pnErrOut = 1;
			}
			break;
	}
	return;
}

void MCB2300_DigitalPortOutput16( int32 nErrIn, int32* pnErrOut, int32 port, unsigned char bUpper, unsigned short sValue ) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (pnErrOut) {
		*pnErrOut = 0;
	}
	switch( port ) {
		case 0 :
			if (!bUpper) {
				PINSEL0 = 0;
				FIO0MASKL = 0;
				FIO0DIRL =  0xFFFF;
				FIO0PINL = sValue;
			}
			else {
				PINSEL1 = 0;
				FIO0MASKU = 0;
				FIO0DIRU =  0xFFFF;
				FIO0PINU = sValue;
			}
			break;
		case 1 :
			if (!bUpper) {
				PINSEL2 = 0;
				FIO1MASKL = 0;
				FIO1DIRL =  0xFFFF;
				FIO1PINL = sValue;
			}
			else {
				PINSEL3 = 0;
				FIO1MASKU = 0;
				FIO1DIRU =  0xFFFF;
				FIO1PINU = sValue;
			}
			break;
		case 2 :
			if (!bUpper) {
				PINSEL4 = 0;
				PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
				FIO2MASKL = 0;
				FIO2DIRL =  0xFFFF;
				FIO2PINL = sValue;
			}
			else {
				PINSEL5 = 0;
				FIO2MASKU = 0;
				FIO2DIRU =  0xFFFF;
				FIO2PINU = sValue;
			}
			break;
		case 3 :
			if (!bUpper) {
				PINSEL6 = 0;
				FIO3MASKL = 0;
				FIO3DIRL =  0xFFFF;
				FIO3PINL = sValue;
			}
			else {
				PINSEL7 = 0;
				FIO3MASKU = 0;
				FIO3DIRU =  0xFFFF;
				FIO3PINU = sValue;
			}
			break;
		case 4 :
			if (!bUpper) {
				PINSEL8 = 0;
				FIO4MASKL = 0;
				FIO4DIRL =  0xFFFF;
				FIO4PINL = sValue;
			}
			else {
				PINSEL9 = 0;
				FIO4MASKU = 0;
				FIO4DIRU =  0xFFFF;
				FIO4PINU = sValue;
			}
			break;
		default :
			if (pnErrOut) {
				*pnErrOut = 1;
			}
			break;
	}
	return;
}

void MCB2300_DigitalPortOutput8( int32 nErrIn, int32* pnErrOut, int32 port, unsigned int nBytePosition, unsigned char cValue ) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (nBytePosition > 3) {
		if (pnErrOut) {
			*pnErrOut = 1;
		}
		return;
	}
	if (pnErrOut) {
		*pnErrOut = 0;
	}
	switch( port ) {
		case 0 :
			if (nBytePosition == 0) {
				PINSEL0 &= ~(0xFFFF);
				FIO0MASK0 = 0;
				FIO0DIR0 =  0xFF;
				FIO0PIN0 = cValue;
			}
			else if (nBytePosition == 1) {
				PINSEL0 &= ~(0xFFFF0000);
				FIO0MASK1 = 0;
				FIO0DIR1 =  0xFF;
				FIO0PIN1 = cValue;
			}
			else if (nBytePosition == 2) {
				PINSEL1 &= ~(0xFFFF);
				FIO0MASK2 = 0;
				FIO0DIR2 =  0xFF;
				FIO0PIN2 = cValue;
			}
			else if (nBytePosition == 3) {
				PINSEL1 &= ~(0xFFFF0000);
				FIO0MASK3 = 0;
				FIO0DIR3 =  0xFF;
				FIO0PIN3 = cValue;
			}
			break;
		case 1 :
			if (nBytePosition == 0) {
				PINSEL2 &= ~(0xFFFF);
				FIO1MASK0 = 0;
				FIO1DIR0 =  0xFF;
				FIO1PIN0 = cValue;
			}
			else if (nBytePosition == 1) {
				PINSEL2 &= ~(0xFFFF0000);
				FIO1MASK1 = 0;
				FIO1DIR1 =  0xFF;
				FIO1PIN1 = cValue;
			}
			else if (nBytePosition == 2) {
				PINSEL3 &= ~(0xFFFF);
				FIO1MASK2 = 0;
				FIO1DIR2 =  0xFF;
				FIO1PIN2 = cValue;
			}
			else if (nBytePosition == 3) {
				PINSEL3 &= ~(0xFFFF0000);
				FIO1MASK3 = 0;
				FIO1DIR3 =  0xFF;
				FIO1PIN3 = cValue;
			}
			break;
		case 2 :
			if (nBytePosition == 0) {
				PINSEL4 &= ~(0xFFFF);
				PINSEL10 = 0;   //disable ETM
				FIO2MASK0 = 0;
				FIO2DIR0 =  0xFF;
				FIO2PIN0 = cValue;
			}
			else if (nBytePosition == 1) {
				PINSEL4 &= ~(0xFFFF0000);
				FIO2MASK1 = 0;
				FIO2DIR1 =  0xFF;
				FIO2PIN1 = cValue;
			}
			else if (nBytePosition == 2) {
				PINSEL5 &= ~(0xFFFF);
				FIO2MASK2 = 0;
				FIO2DIR2 =  0xFF;
				FIO2PIN2 = cValue;
			}
			else if (nBytePosition == 3) {
				PINSEL5 &= ~(0xFFFF0000);
				FIO2MASK3 = 0;
				FIO2DIR3 =  0xFF;
				FIO2PIN3 = cValue;
			}
			break;
		case 3 :
			if (nBytePosition == 0) {
				PINSEL6 &= ~(0xFFFF);
				FIO3MASK0 = 0;
				FIO3DIR0 =  0xFF;
				FIO3PIN0 = cValue;
			}
			else if (nBytePosition == 1) {
				PINSEL6 &= ~(0xFFFF0000);
				FIO3MASK1 = 0;
				FIO3DIR1 =  0xFF;
				FIO3PIN1 = cValue;
			}
			else if (nBytePosition == 2) {
				PINSEL7 &= ~(0xFFFF);
				FIO3MASK2 = 0;
				FIO3DIR2 =  0xFF;
				FIO3PIN2 = cValue;
			}
			else if (nBytePosition == 3) {
				PINSEL7 &= ~(0xFFFF0000);
				FIO3MASK3 = 0;
				FIO3DIR3 =  0xFF;
				FIO3PIN3 = cValue;
			}
			break;
		case 4 :
			if (nBytePosition == 0) {
				PINSEL8 &= ~(0xFFFF);
				FIO4MASK0 = 0;
				FIO4DIR0 =  0xFF;
				FIO4PIN0 = cValue;
			}
			else if (nBytePosition == 1) {
				PINSEL8 &= ~(0xFFFF0000);
				FIO4MASK1 = 0;
				FIO4DIR1 =  0xFF;
				FIO4PIN1 = cValue;
			}
			else if (nBytePosition == 2) {
				PINSEL9 &= ~(0xFFFF);
				FIO4MASK2 = 0;
				FIO4DIR2 =  0xFF;
				FIO4PIN2 = cValue;
			}
			else if (nBytePosition == 3) {
				PINSEL9 &= ~(0xFFFF0000);
				FIO4MASK3 = 0;
				FIO4DIR3 =  0xFF;
				FIO4PIN3 = cValue;
			}
			break;
		default :
			if (pnErrOut) {
				*pnErrOut = 1;
			}
			break;
	}
	return;
}

void MCB2300_DigitalInput( int32 nErrIn, int32* pnErrOut, int32 port, int32 pin, unsigned char *pcValue ) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (pin < 0 || pin > 31) {
		if (pnErrOut) {
			*pnErrOut = 1;
		}
		return;
	}
	if (pnErrOut) {
		*pnErrOut = 0;
	}
	if (!pcValue) {
		return;
	}
	switch( port ) {
		case 0 :
			if (pin < 16) {
				PINSEL0 &= ~(3 << (pin*2));		 //because each pin corresponds to 2 bits
			}
			else {
				PINSEL1 &= ~(3 << ((pin - 16)*2));
			}
			FIO0MASK = ~(1 << pin);
			FIO0DIR &= ~(1 << pin);
			*pcValue = (FIO0PIN & (1 << pin))?1:0;
			break;
		case 1 :
			if (pin < 16) {
				PINSEL2 &= ~(3 << (pin*2));
			}
			else {
				PINSEL3 &= ~(3 << ((pin - 16)*2));
			}
			FIO1MASK = ~(1 << pin);
			FIO1DIR &= ~(1 << pin);
			*pcValue = (FIO1PIN & (1 << pin))?1:0;
			break;
		case 2 :
			if (pin < 16) {
				PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
				PINSEL4 &= ~(3 << (pin*2));
			}
			else {
				PINSEL5 &= ~(3 << ((pin - 16)*2));
			}
			FIO2MASK = ~(1 << pin);
			FIO2DIR &= ~(1 << pin);
			*pcValue = (FIO2PIN & (1 << pin))?1:0;
			break;
		case 3 :
			if (pin < 16) {
				PINSEL6 &= ~(3 << (pin*2));
			}
			else {
				PINSEL7 &= ~(3 << ((pin - 16)*2));
			}
			FIO3MASK = ~(1 << pin);
			FIO3DIR &= ~(1 << pin);
			*pcValue = (FIO3PIN & (1 << pin))?1:0;
			break;
		case 4 :
			if (pin < 16) {
				PINSEL8 &= ~(3 << (pin*2));
			}
			else {
				PINSEL9 &= ~(3 << ((pin - 16)*2));
			}
			FIO4MASK = ~(1 << pin);
			FIO4DIR &= ~(1 << pin);
			*pcValue = (FIO4PIN & (1 << pin))?1:0;
			break;
		default :
			if (pnErrOut) {
				*pnErrOut = 1;
			}
			break;
	}
	return;
}

void MCB2300_DigitalPortInput32( int32 nErrIn, int32* pnErrOut, int32 port, unsigned long *pnValue ) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (pnErrOut) {
		*pnErrOut = 0;
	}
	if (!pnValue) {
		return;
	}
	switch( port ) {
		case 0 :
			PINSEL0 = 0;
			PINSEL1 = 0;
			FIO0MASK = 0;
			FIO0DIR = 0;
			*pnValue = FIO0PIN;
			break;
		case 1 :
			PINSEL2 = 0;
			PINSEL3 = 0;
			FIO1MASK = 0;
			FIO1DIR = 0;
			*pnValue = FIO1PIN;
			break;
		case 2 :
			PINSEL4 = 0;
			PINSEL5 = 0;
			FIO2MASK = 0;
			PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
			FIO2DIR = 0;
			*pnValue = FIO2PIN;
			break;
		case 3 :
			PINSEL6 = 0;
			PINSEL7 = 0;
			FIO3MASK = 0;
			FIO3DIR = 0;
			*pnValue = FIO3PIN;
			break;
		case 4 :
			PINSEL8 = 0;
			PINSEL9 = 0;
			FIO4MASK = 0;
			FIO4DIR =  0;
			*pnValue = FIO4PIN;
			break;
		default :
			if (pnErrOut) {
				*pnErrOut = 1;
			}
			break;
	}
	return;
}
void MCB2300_DigitalPortInput16( int32 nErrIn, int32* pnErrOut, int32 port, unsigned short bUpper, unsigned short *psValue ) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (pnErrOut) {
		*pnErrOut = 0;
	}
	if (!psValue) {
		return;
	}
	switch( port ) {
		case 0 :
			if (!bUpper) {
				PINSEL0 = 0;
				FIO0MASKL = 0;
				FIO0DIRL = 0;
				*psValue = FIO0PINL;
			}
			else {
				PINSEL1 = 0;
				FIO0MASKU = 0;
				FIO0DIRU = 0;
				*psValue = FIO0PINU;
			}
			break;
		case 1 :
			if (!bUpper) {
				PINSEL2 = 0;
				FIO1MASKL = 0;
				FIO1DIRL = 0;
				*psValue = FIO1PINL;
			}
			else {
				PINSEL3 = 0;
				FIO1MASKU = 0;
				FIO1DIRU = 0;
				*psValue = FIO1PINU;
			}
			break;
		case 2 :
			if (!bUpper) {
				PINSEL4 = 0;
				FIO2MASKL = 0;
				PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
				FIO2DIRL = 0;
				*psValue = FIO2PINL;
			}
			else {
				PINSEL5 = 0;
				FIO2MASKU = 0;
				FIO2DIRU = 0;
				*psValue = FIO2PINU;
			}
			break;
		case 3 :
			if (!bUpper) {
				PINSEL6 = 0;
				FIO3MASKL = 0;
				FIO3DIRL = 0;
				*psValue = FIO3PINL;
			}
			else {
				PINSEL7 = 0;
				FIO3MASKU = 0;
				FIO3DIRU = 0;
				*psValue = FIO3PINU;
			}
			break;
		case 4 :
			if (!bUpper) {
				PINSEL8 = 0;
				FIO4MASKL = 0;
				FIO4DIRL =  0;
				*psValue = FIO4PINL;
			}
			else {
				PINSEL9 = 0;
				FIO4MASKU = 0;
				FIO4DIRU =  0;
				*psValue = FIO4PINU;
			}
			break;
		default :
			if (pnErrOut) {
				*pnErrOut = 1;
			}
			break;
	}
	return;
}

void MCB2300_DigitalPortInput8( int32 nErrIn, int32* pnErrOut, int32 port, unsigned int nBytePosition, unsigned char *pcValue ) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (nBytePosition > 3) {
		if (pnErrOut) {
			*pnErrOut = 1;
		}
		return;
	}
	if (pnErrOut) {
		*pnErrOut = 0;
	}
	if (!pcValue) {
		return;
	}
	switch( port ) {
		case 0 :
			if (nBytePosition == 0) {
				PINSEL0 &= 0xFF00;
				FIO0MASK0 = 0;
				FIO0DIR0 = 0;
				*pcValue = FIO0PIN0;
			}
			else if (nBytePosition == 1) {
				PINSEL0 &= 0x00FF;
				FIO0MASK1 = 0xFF;
				FIO0DIR1 = 0;
				*pcValue = FIO0PIN1;
			}
			else if (nBytePosition == 2) {
				PINSEL1 &= 0xFF00;
				FIO0MASK2 = 0;
				FIO0DIR2 = 0;
				*pcValue = FIO0PIN2;
			}
			else if (nBytePosition == 3) {
				PINSEL1 &= 0x00FF;
				FIO0MASK3 = 0;
				FIO0DIR3 = 0;
				*pcValue = FIO0PIN3;
			}
			break;
		case 1 :
			if (nBytePosition == 0) {
				PINSEL2 &= 0xFF00;
				FIO1MASK0 = 0;
				FIO1DIR0 = 0;
				*pcValue = FIO1PIN0;
			}
			else if (nBytePosition == 1) {
				PINSEL2 &= 0x00FF;
				FIO1MASK1 = 0;
				FIO1DIR1 = 0;
				*pcValue = FIO1PIN1;
			}
			else if (nBytePosition == 2) {
				PINSEL3 &= 0xFF00;
				FIO1MASK2 = 0;
				FIO1DIR2 = 0;
				*pcValue = FIO1PIN2;
			}
			else if (nBytePosition == 3) {
				PINSEL3 &= 0x00FF;
				FIO1MASK3 = 0;
				FIO1DIR3 = 0;
				*pcValue = FIO1PIN3;
			}
			break;
		case 2 :
			if (nBytePosition == 0) {
				PINSEL4 &= 0xFF00;
				FIO2MASK0 = 0;
				PINSEL10 = 0;   //disable ETM
				FIO2DIR0 = 0;
				*pcValue = FIO2PIN0;
			}
			else if (nBytePosition == 1) {
				PINSEL4 &= 0x00FF;
				FIO2MASK1 = 0;
				FIO2DIR1 = 0;
				*pcValue = FIO2PIN1;
			}
			else if (nBytePosition == 2) {
				PINSEL5 &= 0xFF00;
				FIO2MASK2 = 0;
				FIO2DIR2 = 0;
				*pcValue = FIO2PIN2;
			}
			else if (nBytePosition == 3) {
				PINSEL5 &= 0x00FF;
				FIO2MASK3 = 0;
				FIO2DIR3 = 0;
				*pcValue = FIO2PIN3;
			}
			break;
		case 3 :
			if (nBytePosition == 0) {
				PINSEL6 &= 0xFF00;
				FIO3MASK0 = 0;
				FIO3DIR0 = 0;
				*pcValue = FIO3PIN0;
			}
			else if (nBytePosition == 1) {
				PINSEL5 &= 0x00FF;
				FIO3MASK1 = 0;
				FIO3DIR1 = 0;
				*pcValue = FIO3PIN1;
			}
			else if (nBytePosition == 2) {
				PINSEL6 &= 0xFF00;
				FIO3MASK2 = 0;
				FIO3DIR2 = 0;
				*pcValue = FIO3PIN2;
			}
			else if (nBytePosition == 3) {
				PINSEL6 &= 0x00FF;
				FIO3MASK3 = 0;
				FIO3DIR3 = 0;
				*pcValue = FIO3PIN3;
			}
			break;
		case 4 :
			if (nBytePosition == 0) {
				PINSEL7 &= 0xFF00;
				FIO4MASK0 = 0;
				FIO4DIR0 =  0;
				*pcValue = FIO4PIN0;
			}
			else if (nBytePosition == 1) {
				PINSEL7 &= 0x00FF;
				FIO4MASK1 = 0;
				FIO4DIR1 =  0;
				*pcValue = FIO4PIN1;
			}
			else if (nBytePosition == 2) {
				PINSEL8 &= 0xFF00;
				FIO4MASK2 = 0;
				FIO4DIR2 =  0;
				*pcValue = FIO4PIN2;
			}
			else if (nBytePosition == 3) {
				PINSEL8 &= 0x00FF;
				FIO4MASK3 = 0;
				FIO4DIR3 =  0;
				*pcValue = FIO4PIN3;
			}
			break;
		default :
			if (pnErrOut) {
				*pnErrOut = 1;
			}
			break;
	}
	return;
}
