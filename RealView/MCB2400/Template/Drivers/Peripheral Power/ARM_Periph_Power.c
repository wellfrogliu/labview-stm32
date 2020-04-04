/***************************************************************************************
	NI CONFIDENTIAL

	ARM_Periph_Power.c - peripheral power configuration

	© 2003-2009 National Instruments Corporation.  
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Embedded Development Module) and may not be copied, modified, or redistributed.  The code may 
     only be used in accordance with the terms of the license agreement entered into between 
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.
	
	Author: Nick Van Sickel
********************************************************************************************/

#include <LPC23xx.H>      // LPC24xx definitions
#include "LVFuncsUsed.h"  // needed for #ifdefs below
#include "profile_timer.h"

#ifdef _Include_ARM_Peripheral_Enable
void ARM_Peripheral_Enable(unsigned int index, int error_in, int* error_out)
{
	if(error_in) {
		if(error_out) {
			*error_out = error_in;
		}
		return;	
	}
	PCONP |= (1 << index);
}
#endif

#ifdef _Include_ARM_Peripheral_Disable
void ARM_Peripheral_Disable(unsigned int index, int error_in, int* error_out)
{
	if(error_in) {
		if(error_out) {
			*error_out = error_in;
		}
		return;	
	}
	PCONP &= ~(1 << index);
}
#endif

#ifdef _Include_ARM_Peripheral_Enable_All
void ARM_Peripheral_Enable_All(int error_in, int* error_out)
{
	if(error_in) {
		if(error_out) {
			*error_out = error_in;
		}
		return;	
	}
	PCONP = 0xFFF87FFE;  // enable all peripherals
}
#endif

#ifdef _Include_ARM_Peripheral_Disable_All
void ARM_Peripheral_Disable_All(int error_in, int* error_out)
{
	if(error_in) {
		if(error_out) {
			*error_out = error_in;
		}
		return;	
	}
	#if defined(USE_HW_PROFILING) && (defined(_Include_Profiling) || defined(_Include_LVGetTicksExt))
	PCONP = 0x806;  //do not disable Timer1 as well, as it is used to usecond profile timing
	#else
	PCONP = 0x802;	//disable all peripheral but Timer0 (used by the OS) and EMC (used to access ext memory)
	#endif
}
#endif
