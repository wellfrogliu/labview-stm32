/*----------------------------------------------------------------------------
 *      Name:    RV_RTClock.h
 *      Purpose: Real time clock driver for LabVIEW
 *      Version: v1.00
 *      Note(s):
 *----------------------------------------------------------------------------
 *		NI CONFIDENTIAL
 *
 *		© 2003-2006 National Instruments Corporation.
 *		This code is for internal use only and may not be used outside of the 
 *		LabVIEW embedded product (LabVIEW Embedded Development Module) and may 
 *		not be copied, modified, or redistributed.  The code may only be used 
 *		in accordance with the terms of the license agreement entered into 
 *		between National Instruments Corporation and the authorized user.  Your 
 *		use of the code designates your acceptance of the terms of the license 
 *		agreement.
 *
 *		Author: Bill Pittman
 *---------------------------------------------------------------------------*/

#include <time.h>

void ARM_rtc_gettime(struct tm *timeptr);
void ARM_init_rtc(void);
