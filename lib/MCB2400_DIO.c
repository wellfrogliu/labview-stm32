/***************************************************************************************
	NI CONFIDENTIAL

	MCB2400_DIO.c - Functions to read or write DGPIO lines.

	© 2003-2008 National Instruments Corporation.
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Embedded Development Module) and may not be copied, modified, or redistributed.  The code may
     only be used in accordance with the terms of the license agreement entered into between
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.
	
	Author: Nick Van Sickel
********************************************************************************************/
#ifndef MCB2400_DIO_C
#define MCB2400_DIO_C

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include <LPC23xx.H>                    /* LPC23xx/LPC24xx definitions               */

//MASK for GPIO.  There are certain pins on the MCB2400 that we don't
//want the user to touch.

#if defined(MCB2400BOARD)
#define PORT3MASK 0xBFFFFFFF
#define PINSEL7MASK	0xCFFFFFFF
#else
#define PORT3MASK 0xFFFFFFFF
#define PINSEL7MASK	0xFFFFFFFF
#endif
#define PORT0MASK 0xFFFFFFFF
#define PINSEL0MASK 0xFFFFFFFF
#define PINSEL1MASK 0xFFFFFFFF
#define PORT1MASK 0xFFFFFFFF
#define PINSEL2MASK 0xFFFFFFFF
#define PINSEL3MASK 0xFFFFFFFF
#define PORT2MASK 0xFFFFFFFF
#define PINSEL4MASK 0xFFFFFFFF  
#define PINSEL5MASK	0xFFFFFFFF
#define PINSEL6MASK	0xFFFFFFFF

void MCB2400_DigitalOutput( int32 nErrIn, int32* pnErrOut, int32 port, uInt8 pin, uInt8 cValue ) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	//check if user OOB
	if (pin > 31) {
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

void MCB2400_DigitalPortOutput32( int32 nErrIn, int32* pnErrOut, uInt8 port, uInt32 nValue ) {
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
			PINSEL0 &= ~PINSEL0MASK;
			PINSEL1 &= ~PINSEL1MASK;
			FIO0MASK &= ~PORT0MASK;
			FIO0DIR |=  PORT0MASK;
			FIO0PIN = nValue&PORT0MASK;
			break;
		case 1 :
			PINSEL2 &= ~PINSEL2MASK;
			PINSEL3 &= ~PINSEL3MASK;
			FIO1MASK &= ~PORT1MASK;
			FIO1DIR |=  PORT1MASK;
			FIO1PIN = nValue&PORT1MASK;
			break;
		case 2 :
			PINSEL4 &= ~PINSEL4MASK;
			PINSEL5 &= ~PINSEL5MASK;
			PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
			FIO2MASK &= ~PORT2MASK;
			FIO2DIR |=  PORT2MASK;
			FIO2PIN = nValue&PORT2MASK;
			break;
		case 3 :
			PINSEL6 &= ~PINSEL6MASK;
			PINSEL7 &= ~PINSEL7MASK;
			FIO3MASK &= ~PORT3MASK;
			FIO3DIR |=  PORT3MASK;
			FIO3PIN = nValue&PORT3MASK;
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

void MCB2400_DigitalPortOutput16( int32 nErrIn, int32* pnErrOut, uInt8 port, Boolean bUpper, uInt16 sValue ) {
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
				PINSEL0 &= ~PINSEL0MASK;
				FIO0MASKL &= ~PORT0MASK;
				FIO0DIRL |=  PORT0MASK;
				FIO0PINL = sValue&PORT0MASK;
			}
			else {
				PINSEL1 &= ~PINSEL1MASK;
				FIO0MASKU &= ~(PORT0MASK >> 16);
				FIO0DIRU |=  (PORT0MASK >> 16);
				FIO0PINU = sValue&(PORT0MASK >> 16);
			}
			break;
		case 1 :
			if (!bUpper) {
				PINSEL2 &= ~PINSEL2MASK;
				FIO1MASKL &= ~PORT1MASK;
				FIO1DIRL |=  PORT1MASK;
				FIO1PINL = sValue&PORT1MASK;
			}
			else {
				PINSEL3 &= ~PINSEL3MASK;
				FIO1MASKU &= ~(PORT1MASK >> 16);
				FIO1DIRU |=  (PORT1MASK >> 16);
				FIO1PINU = sValue&(PORT1MASK >> 16);
			}
			break;
		case 2 :
			if (!bUpper) {
				PINSEL4 &= ~PINSEL4MASK;
				PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
				FIO2MASKL &= ~PORT2MASK;
				FIO2DIRL |=  PORT2MASK;
				FIO2PINL = sValue&PORT2MASK;
			}
			else {
				PINSEL5 &= ~PINSEL5MASK;
				FIO2MASKU &= ~(PORT2MASK >> 16);
				FIO2DIRU |=  (PORT2MASK >> 16);
				FIO2PINU = sValue&(PORT2MASK >> 16);
			}
			break;
		case 3 :
			if (!bUpper) {
				PINSEL6 &= ~PINSEL6MASK;
				FIO3MASKL &= ~PORT3MASK;
				FIO3DIRL |=  PORT3MASK;
				FIO3PINL = sValue&PORT3MASK;
			}
			else {
				PINSEL7 &= ~PINSEL7MASK;
				FIO3MASKU &= ~(PORT3MASK >> 16);
				FIO3DIRU |=  (PORT3MASK >> 16);
				FIO3PINU = sValue&(PORT3MASK >> 16);
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

void MCB2400_DigitalPortOutput8( int32 nErrIn, int32* pnErrOut, uInt8 port, uInt8 nBytePosition, uInt8 cValue ) {
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
				PINSEL0 &= ~(PINSEL0MASK&0xFFFF);
				FIO0MASK0 &= ~PORT0MASK;
				FIO0DIR0 |=  PORT0MASK;
				FIO0PIN0 = cValue&PORT0MASK;
			}
			else if (nBytePosition == 1) {
				PINSEL0 &= ~(PINSEL0MASK&0xFFFF0000);
				FIO0MASK1 &= ~(PORT0MASK >> 8);
				FIO0DIR1 |=  (PORT0MASK >> 8);
				FIO0PIN1 = cValue&(PORT0MASK >> 8);
			}
			else if (nBytePosition == 2) {
				PINSEL1 &= ~(PINSEL1MASK&0xFFFF);
				FIO0MASK2 &= ~(PORT0MASK >> 16);
				FIO0DIR2 |=  (PORT0MASK >> 16);
				FIO0PIN2 = cValue&(PORT0MASK >> 16);
			}
			else if (nBytePosition == 3) {
				PINSEL1 &= ~(PINSEL1MASK&0xFFFF0000);
				FIO0MASK3 &= ~(PORT0MASK >> 24);
				FIO0DIR3 |=  (PORT0MASK >> 24);
				FIO0PIN3 = cValue&(PORT0MASK >> 24);
			}
			break;
		case 1 :
			if (nBytePosition == 0) {
				PINSEL2 &= ~(PINSEL2MASK&0xFFFF);
				FIO1MASK0 &= ~PORT1MASK;
				FIO1DIR0 |=  PORT1MASK;
				FIO1PIN0 = cValue&PORT1MASK;
			}
			else if (nBytePosition == 1) {
				PINSEL2 &= ~(PINSEL2MASK&0xFFFF0000);
				FIO1MASK1 &= ~(PORT1MASK >> 8);
				FIO1DIR1 |=  (PORT1MASK >> 8);
				FIO1PIN1 = cValue&(PORT1MASK >> 8);
			}
			else if (nBytePosition == 2) {
				PINSEL3 &= ~(PINSEL3MASK&0xFFFF);
				FIO1MASK2 &= ~(PORT1MASK >> 16);
				FIO1DIR2 |=  (PORT1MASK >> 16);
				FIO1PIN2 = cValue&(PORT1MASK >> 16);
			}
			else if (nBytePosition == 3) {
				PINSEL3 &= ~(PINSEL3MASK&0xFFFF0000);
				FIO1MASK3 &= ~(PORT1MASK >> 24);
				FIO1DIR3 |=  (PORT1MASK >> 24);
				FIO1PIN3 = cValue&(PORT1MASK >> 24);
			}
			break;
		case 2 :
			if (nBytePosition == 0) {
				PINSEL4 &= ~(PINSEL4MASK&0xFFFF);
				PINSEL10 = 0;   //disable ETM
				FIO2MASK0 &= ~PORT2MASK;
				FIO2DIR0 |=  PORT2MASK;
				FIO2PIN0 = cValue&PORT2MASK;
			}
			else if (nBytePosition == 1) {
				PINSEL4 &= ~(PINSEL4MASK&0xFFFF0000);
				FIO2MASK1 &= ~(PORT2MASK >> 8);
				FIO2DIR1 |=  (PORT2MASK >> 8);
				FIO2PIN1 = cValue&(PORT2MASK >> 8);
			}
			else if (nBytePosition == 2) {
				PINSEL5 &= ~(PINSEL5MASK&0xFFFF);
				FIO2MASK2 &= ~(PORT2MASK >> 16);
				FIO2DIR2 |=  (PORT2MASK >> 16);
				FIO2PIN2 = cValue&(PORT2MASK >> 16);
			}
			else if (nBytePosition == 3) {
				PINSEL5 &= ~(PINSEL5MASK&0xFFFF0000);
				FIO2MASK3 &= ~(PORT2MASK >> 24);
				FIO2DIR3 |=  (PORT2MASK >> 24);
				FIO2PIN3 = cValue&(PORT2MASK >> 24);
			}
			break;
		case 3 :
			if (nBytePosition == 0) {
				PINSEL6 &= ~(PINSEL6MASK&0xFFFF);
				FIO3MASK0 &= ~PORT3MASK;
				FIO3DIR0 |=  PORT3MASK;
				FIO3PIN0 = cValue&PORT3MASK;
			}
			else if (nBytePosition == 1) {
				PINSEL6 &= ~(PINSEL6MASK&0xFFFF0000);
				FIO3MASK1 &= ~(PORT3MASK >> 8);
				FIO3DIR1 |=  (PORT3MASK >> 8);
				FIO3PIN1 = cValue&(PORT3MASK >> 8);
			}
			else if (nBytePosition == 2) {
				PINSEL7 &= ~(PINSEL7MASK&0xFFFF);
				FIO3MASK2 &= ~(PORT3MASK >> 16);
				FIO3DIR2 |=  (PORT3MASK >> 16);
				FIO3PIN2 = cValue&(PORT3MASK >> 16);
			}
			else if (nBytePosition == 3) {
				PINSEL7 &= ~(PINSEL7MASK&0xFFFF0000);
				FIO3MASK3 &= ~(PORT3MASK >> 24);
				FIO3DIR3 |=  (PORT3MASK >> 24);
				FIO3PIN3 = cValue&(PORT3MASK >> 24);
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

void MCB2400_DigitalInput( int32 nErrIn, int32* pnErrOut, uInt8 port, uInt8 pin, unsigned char *pcValue ) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (pin > 31) {
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

void MCB2400_DigitalPortInput32( int32 nErrIn, int32* pnErrOut, uInt8 port, uInt32 *pnValue ) {
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
			PINSEL0 &= ~PINSEL0MASK;
			PINSEL1 &= ~PINSEL1MASK;
			FIO0MASK &= ~PORT0MASK;
			FIO0DIR &=  ~PORT0MASK;
			*pnValue = FIO0PIN&PORT0MASK;
			break;
		case 1 :
			PINSEL2 &= ~PINSEL2MASK;
			PINSEL3 &= ~PINSEL3MASK;
			FIO1MASK &= ~PORT1MASK;
			FIO1DIR &=  ~PORT1MASK;
			*pnValue = FIO1PIN&PORT1MASK;
			break;
		case 2 :
			PINSEL4 &= ~PINSEL4MASK;
			PINSEL5 &= ~PINSEL5MASK;
			PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
			FIO2MASK &= ~PORT2MASK;
			FIO2DIR &=  ~PORT2MASK;
			*pnValue = FIO2PIN&PORT2MASK;
			break;
		case 3 :
			PINSEL6 &= ~PINSEL6MASK;
			PINSEL7 &= ~PINSEL7MASK;
			FIO3MASK &= ~PORT3MASK;
			FIO3DIR &=  ~PORT3MASK;
			*pnValue = FIO3PIN&PORT3MASK;
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
void MCB2400_DigitalPortInput16( int32 nErrIn, int32* pnErrOut, uInt8 port,Boolean bUpper, uInt16 *psValue ) {
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
				PINSEL0 &= ~PINSEL0MASK;
				FIO0MASKL &= ~PORT0MASK;
				FIO0DIRL &=  ~PORT0MASK;
				*psValue = FIO0PINL&PORT0MASK;
			}
			else {
				PINSEL1 &= ~PINSEL1MASK;
				FIO0MASKU &= ~(PORT0MASK >> 16);
				FIO0DIRU &=  ~(PORT0MASK >> 16);
				*psValue = FIO0PINU&(PORT0MASK >> 16);
			}
			break;
		case 1 :
			if (!bUpper) {
				PINSEL2 &= ~PINSEL2MASK;
				FIO1MASKL &= ~PORT1MASK;
				FIO1DIRL &=  ~PORT1MASK;
				*psValue = FIO1PINL&PORT1MASK;
			}
			else {
				PINSEL3 &= ~PINSEL3MASK;
				FIO1MASKU &= ~(PORT1MASK >> 16);
				FIO1DIRU &=  ~(PORT1MASK >> 16);
				*psValue = FIO1PINU&(PORT1MASK >> 16);
			}
			break;
		case 2 :
			if (!bUpper) {
				PINSEL4 &= ~PINSEL4MASK;
				PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
				FIO2MASKL &= ~PORT2MASK;
				FIO2DIRL &=  ~PORT2MASK;
				*psValue = FIO2PINL&PORT2MASK;
			}
			else {
				PINSEL5 &= ~PINSEL5MASK;
				FIO2MASKU &= ~(PORT2MASK >> 16);
				FIO2DIRU &=  ~(PORT2MASK >> 16);
				*psValue = FIO2PINU&(PORT2MASK >> 16);
			}
			break;
		case 3 :
			if (!bUpper) {
				PINSEL6 &= ~PINSEL6MASK;
				FIO3MASKL &= ~PORT3MASK;
				FIO3DIRL &=  ~PORT3MASK;
				*psValue = FIO3PINL&PORT3MASK;
			}
			else {
				PINSEL7 &= ~PINSEL7MASK;
				FIO3MASKU &= ~(PORT3MASK >> 16);
				FIO3DIRU &=  ~(PORT3MASK >> 16);
				*psValue = FIO3PINU&(PORT3MASK >> 16);
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

void MCB2400_DigitalPortInput8( int32 nErrIn, int32* pnErrOut, uInt8 port, uInt8 nBytePosition, uInt8 *pcValue ) {
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
				PINSEL0 &= ~(PINSEL0MASK&0xFFFF);
				FIO0MASK0 &= ~PORT0MASK;
				FIO0DIR0 &=  ~PORT0MASK;
				*pcValue = FIO0PIN0&PORT0MASK;
			}
			else if (nBytePosition == 1) {
				PINSEL0 &= ~(PINSEL0MASK&0xFFFF0000);
				FIO0MASK1 &= ~(PORT0MASK >> 8);
				FIO0DIR1 &=  ~(PORT0MASK >> 8);
				*pcValue = FIO0PIN1&(PORT0MASK >> 8);
			}
			else if (nBytePosition == 2) {
				PINSEL1 &= ~(PINSEL1MASK&0xFFFF);
				FIO0MASK2 &= ~(PORT0MASK >> 16);
				FIO0DIR2 &=  ~(PORT0MASK >> 16);
				*pcValue = FIO0PIN2&(PORT0MASK >> 16);
			}
			else if (nBytePosition == 3) {
				PINSEL1 &= ~(PINSEL1MASK&0xFFFF0000);
				FIO0MASK3 &= ~(PORT0MASK >> 24);
				FIO0DIR3 &=  ~(PORT0MASK >> 24);
				*pcValue = FIO0PIN3&(PORT0MASK >> 24);
			}
			break;
		case 1 :
			if (nBytePosition == 0) {
				PINSEL2 &= ~(PINSEL2MASK&0xFFFF);
				FIO1MASK0 &= ~PORT1MASK;
				FIO1DIR0 &=  ~PORT1MASK;
				*pcValue = FIO1PIN0&PORT1MASK;
			}
			else if (nBytePosition == 1) {
				PINSEL2 &= ~(PINSEL2MASK&0xFFFF0000);
				FIO1MASK1 &= ~(PORT1MASK >> 8);
				FIO1DIR1 &=  ~(PORT1MASK >> 8);
				*pcValue = FIO1PIN1&(PORT1MASK >> 8);
			}
			else if (nBytePosition == 2) {
				PINSEL3 &= ~(PINSEL3MASK&0xFFFF);
				FIO1MASK2 &= ~(PORT1MASK >> 16);
				FIO1DIR2 &=  ~(PORT1MASK >> 16);
				*pcValue = FIO1PIN2&(PORT1MASK >> 16);
			}
			else if (nBytePosition == 3) {
				PINSEL3 &= ~(PINSEL3MASK&0xFFFF0000);
				FIO1MASK3 &= ~(PORT1MASK >> 24);
				FIO1DIR3 &=  ~(PORT1MASK >> 24);
				*pcValue = FIO1PIN3&(PORT1MASK >> 24);
			}
			break;
		case 2 :
			if (nBytePosition == 0) {
				PINSEL4 &= ~(PINSEL4MASK&0xFFFF);
				PINSEL10 = 0;   //disable ETM
				FIO2MASK0 &= ~PORT2MASK;
				FIO2DIR0 &=  ~PORT2MASK;
				*pcValue = FIO2PIN0&PORT2MASK;
			}
			else if (nBytePosition == 1) {
				PINSEL4 &= ~(PINSEL4MASK&0xFFFF0000);
				FIO2MASK1 &= ~(PORT2MASK >> 8);
				FIO2DIR1 &=  ~(PORT2MASK >> 8);
				*pcValue = FIO2PIN1&(PORT2MASK >> 8);
			}
			else if (nBytePosition == 2) {
				PINSEL5 &= ~(PINSEL5MASK&0xFFFF);
				FIO2MASK2 &= ~(PORT2MASK >> 16);
				FIO2DIR2 &=  ~(PORT2MASK >> 16);
				*pcValue = FIO2PIN2&(PORT2MASK >> 16);
			}
			else if (nBytePosition == 3) {
				PINSEL5 &= ~(PINSEL5MASK&0xFFFF0000);
				FIO2MASK3 &= ~(PORT2MASK >> 24);
				FIO2DIR3 &=  ~(PORT2MASK >> 24);
				*pcValue = FIO2PIN3&(PORT2MASK >> 24);
			}
			break;
		case 3 :
			if (nBytePosition == 0) {
				PINSEL6 &= ~(PINSEL6MASK&0xFFFF);
				FIO3MASK0 &= ~PORT3MASK;
				FIO3DIR0 &=  ~PORT3MASK;
				*pcValue = FIO3PIN0&PORT3MASK;
			}
			else if (nBytePosition == 1) {
				PINSEL6 &= ~(PINSEL6MASK&0xFFFF0000);
				FIO3MASK1 &= ~(PORT3MASK >> 8);
				FIO3DIR1 &=  ~(PORT3MASK >> 8);
				*pcValue = FIO3PIN1&(PORT3MASK >> 8);
			}
			else if (nBytePosition == 2) {
				PINSEL7 &= ~(PINSEL7MASK&0xFFFF);
				FIO3MASK2 &= ~(PORT3MASK >> 16);
				FIO3DIR2 &=  ~(PORT3MASK >> 16);
				*pcValue = FIO3PIN2&(PORT3MASK >> 16);
			}
			else if (nBytePosition == 3) {
				PINSEL7 &= ~(PINSEL7MASK&0xFFFF0000);
				FIO3MASK3 &= ~(PORT3MASK >> 24);
				FIO3DIR3 &=  ~(PORT3MASK >> 24);
				*pcValue = FIO3PIN3&(PORT3MASK >> 24);
			}
			break;
		case 4 :
			if (nBytePosition == 0) {
				PINSEL8 &= ~(0xFFFF);
				FIO4MASK0 = 0;
				FIO4DIR0 =  0;
				*pcValue = FIO4PIN0;
			}
			else if (nBytePosition == 1) {
				PINSEL8 &= ~(0xFFFF0000);
				FIO4MASK1 = 0;
				FIO4DIR1 =  0;
				*pcValue = FIO4PIN1;
			}
			else if (nBytePosition == 2) {
				PINSEL9 &= ~(0xFFFF);
				FIO4MASK2 = 0;
				FIO4DIR2 =  0;
				*pcValue = FIO4PIN2;
			}
			else if (nBytePosition == 3) {
				PINSEL9 &= ~(0xFFFF0000);
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

#endif
