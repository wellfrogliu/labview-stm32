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

typedef unsigned char tBoolean;
#include "hw_sysctl.h"
#include "sysctl.h"
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
	SysCtlPeripheralEnable(index);
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
	SysCtlPeripheralDisable(index);
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
	*((volatile unsigned long *)SYSCTL_RCGC0) |= 0xFFFFFCFF;  // enable peripheral in register 0, but don't alter max adc clock
	*((volatile unsigned long *)SYSCTL_RCGC1) = 0xFFFFFFFF;  // register 1
	*((volatile unsigned long *)SYSCTL_RCGC2) = 0xFFFFFFFF;  // register 2
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
	*((volatile unsigned long *)SYSCTL_RCGC0) &= 0x300;  // disable peripheral in register 0, but don't alter max adc clock
	#if defined(USE_HW_PROFILING) && (defined(_Include_Profiling) || defined(_Include_LVGetTicksExt))
	*((volatile unsigned long *)SYSCTL_RCGC1) = 0x10000;  // register 1 - do not disable Timer 0 as it is used for usecond timing with profiling
	#else
	*((volatile unsigned long *)SYSCTL_RCGC1) = 0;  // register 1
	#endif
	*((volatile unsigned long *)SYSCTL_RCGC2) = 0;  // register 2
}
#endif
