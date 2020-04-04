/***************************************************************************************
	NI CONFIDENTIAL

	MCB2300_array.c - this file contains functions that use array utility functions. This is in a seperate object file to allow the linker
	to link these functions only when array utils is used.

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
#include "CCGArrSupport.h"
#include <LPC23xx.H>                    /* LPC23xx definitions               */

extern uInt8 _gn2300AnalogChanEnable;

void MCB2300_AnalogInScan(int32 nErrIn, PDAArrPtr *arrValues, PDAArrPtr *arrOverruns, int32* pnErrOut) {	
	uInt8 ds = 8, *ptr2 = NULL;
    int32 *ptr1 = NULL;
	PDAArrPtr vhOutVal=NULL, vhOutOver=NULL;
	
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if(!arrValues && !arrOverruns)  //nothing to do
	{
	 	return;
	}
	
	if (arrValues) {
		vhOutVal = PDAFUNC(PDAArrNew1D)( ds, int32DataType FUNCSTATICARGS1(buf));
	    if (!vhOutVal){
		   if (pnErrOut) 
		{
			*pnErrOut = 1;
		}
		return;
		}

		ptr1 = (int32 *)FirstElem(vhOutVal);
	}
	if (arrOverruns) {
		vhOutOver = PDAFUNC(PDAArrNew1D)( ds, BooleanDataType FUNCSTATICARGS1(buf));
	    if (!vhOutOver){
		   if (pnErrOut) 
		{
			*pnErrOut = 1;
		}
		return;
		}

		ptr2 = FirstElem(vhOutOver); 
	}
	
	AD0CR |= _gn2300AnalogChanEnable; //enable the channels
	AD0CR |= 0x10000; //enable burst mode

if (_gn2300AnalogChanEnable & 0x1) {
		while (	!(AD0DR0 & 0x80000000));  //wait for complete
		if (arrValues) {
			*ptr1 = ((AD0DR0 & 0xFFC0) >> 6);
		}
		if (arrOverruns) {
			*ptr2 = (AD0DR0 & 0x4000000)?1:0;
		}
	}
	else {
		if (arrValues) {
			*ptr1 = 0;
		}
		if (arrOverruns) {
			*ptr2 = 0;
		}
	}
	if (_gn2300AnalogChanEnable & 0x2) {
	while (	!(AD0DR1 & 0x80000000));   //wait for complete
		if (arrValues) {
			*(ptr1+1) = ((AD0DR1 & 0xFFC0) >> 6);
		}
		if (arrOverruns) {
			*(ptr2+1) = (AD0DR1 & 0x4000000)?1:0;
		}
	}
	else {
		if (arrValues) {
			*(ptr1+1) = 0;
		}
		if (arrOverruns) {
			*(ptr2+1) = 0;
		}
	}
	if (_gn2300AnalogChanEnable & 0x4) {
	while (	!(AD0DR2 & 0x80000000));	//wait for complete
		if (arrValues) {
			*(ptr1+2) = ((AD0DR2 & 0xFFC0) >> 6);
		}
		if (arrOverruns) {
			*(ptr2+2) = (AD0DR2 & 0x4000000)?1:0;
		}
	}
	else {
		if (arrValues) {
			*(ptr1+2) = 0;
		}
		if (arrOverruns) {
			*(ptr2+2) = 0;
		}
	}
	if (_gn2300AnalogChanEnable & 0x8) {
	while (	!(AD0DR3 & 0x80000000));	//wait for complete
		if (arrValues) {
			*(ptr1+3) = ((AD0DR3 & 0xFFC0) >> 6);
		}
		if (arrOverruns) {
			*(ptr2+3) = (AD0DR3 & 0x4000000)?1:0;
		}
	}
	else {
		if (arrValues) {
			*(ptr1+3) = 0;
		}
		if (arrOverruns) {
			*(ptr2+3) = 0;
		}
	}
	if (_gn2300AnalogChanEnable & 0x10) {
	while (	!(AD0DR4 & 0x80000000));   //wait for complete
		if (arrValues) {
			*(ptr1+4) = ((AD0DR4 & 0xFFC0) >> 6);
		}
		if (arrOverruns) {
			*(ptr2+4) = (AD0DR4 & 0x4000000)?1:0;
		}
	}
	else {
		if (arrValues) {
			*(ptr1+4) = 0;
		}
		if (arrOverruns) {
			*(ptr2+4) = 0;
		}
	}
	if (_gn2300AnalogChanEnable & 0x20) {
	while (	!(AD0DR5 & 0x80000000));   //wait for complete
		if (arrValues) {
			*(ptr1+5) = ((AD0DR5 & 0xFFC0) >> 6);
		}
		if (arrOverruns) {
			*(ptr2+5) = (AD0DR5 & 0x4000000)?1:0;
		}
	}
	else {
		if (arrValues) {
			*(ptr1+5) = 0;
		}
		if (arrOverruns) {
			*(ptr2+5) = 0;
		}
	}
	if (_gn2300AnalogChanEnable & 0x40) {
	while (	!(AD0DR6 & 0x80000000));   //wait for complete
		if (arrValues) {
			*(ptr1+6) = ((AD0DR6 & 0xFFC0) >> 6);
		}
		if (arrOverruns) {
			*(ptr2+6) = (AD0DR6 & 0x4000000)?1:0;
		}
	}
	else {
		if (arrValues) {
			*(ptr1+6) = 0;
		}
		if (arrOverruns) {
			*(ptr2+6) = 0;
		}
	}
	if (_gn2300AnalogChanEnable & 0x80) {
	while (	!(AD0DR7 & 0x80000000));   //wait for complete
		if (arrValues) {
			*(ptr1+7) = ((AD0DR7 & 0xFFC0) >> 6);
		}
		if (arrOverruns) {
			*(ptr2+7) = (AD0DR7 & 0x4000000)?1:0;
		}
	}
	else {
		if (arrValues) {
			*(ptr1+7) = 0;
		}
		if (arrOverruns) {
			*(ptr2+7) = 0;
		}
	}
	
	if(arrValues)
	{
		*arrValues = vhOutVal; 
	}
	if(arrOverruns)
	{
		*arrOverruns = vhOutOver;
	}	
	
	AD0CR &= ~(0x10000); // disable burst mode 
				   
 	if (pnErrOut) {
		*pnErrOut = 0;
	}
	return;
}
