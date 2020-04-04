/*----------------------------------------------------------------------------
 *      Name:    RV_RTClock.c
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
#include <LPC23xx.H>                      // LPC23xx definitions

void ARM_rtc_gettime(struct tm *timeptr)
{
  unsigned int ctime0;
  unsigned int ctime1;
  unsigned int ctime2;

  if(!timeptr)
	  return;

  ctime0 = *(unsigned int *)0xE0024014;
  ctime1 = *(unsigned int *)0xE0024018;
  ctime2 = *(unsigned int *)0xE002401C;

  /*
  Field			struct tm structure		LPC2378 RealTime Clock		Differs
  tm_sec		0 to 60					0 to 59						
  tm_min		0 to 59					0 to 59
  tm_hour		0 to 23					0 to 23
  tm_mday		1 to 31					1 to 31
  tm_mon		0 to 11					1 to 12						*
  tm_year		years since 1900		0 to 4095					*
  tm_wday		0 to 6					0 to 6
  tm_yday		0 to 365				1 to 366					*
  tm_isdst		-1/0/1					0 // UTC never has DST		*
  */

  timeptr->tm_year = ((ctime1 >> 16) & 0xFFF) - 1900;
  timeptr->tm_mon = ((ctime1 >> 8) & 0x0F) - 1;
  timeptr->tm_mday = ctime1 & 0x1F;
  timeptr->tm_hour = (ctime0 >> 16) & 0x1F;
  timeptr->tm_min = (ctime0 >> 8) & 0x3F;
  timeptr->tm_sec = ctime0 & 0x3F;
  timeptr->tm_wday = (ctime0 >> 24) & 0x07;
  timeptr->tm_yday = (ctime2 & 0xFFF) - 1;
  timeptr->tm_isdst = 0;
}

// Set the RTC to use January 1, 1970
void ARM_init_rtc()
{
  // pause the RTC
  RTC_CCR = 0x12;

  // Midnight, January 1, 1970
  RTC_SEC = 0;
  RTC_MIN = 0;
  RTC_HOUR = 0;
  RTC_DOM = 1;
  RTC_DOW = 4;
  RTC_DOY = 1;
  RTC_MONTH = 1;
  RTC_YEAR = 1970;

  // start the RTC
  RTC_CCR = 0x11;
}
