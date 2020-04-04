/*----------------------------------------------------------------------------
 *      Name:    ARM_RTClock.c
 *      Purpose: Real time clock driver for LabVIEW
 *      Version: v1.00
 *      Note(s):
 *----------------------------------------------------------------------------
 *        NI CONFIDENTIAL
 *
 *        © 2003-2006 National Instruments Corporation.
 *        This code is for internal use only and may not be used outside of the
 *        LabVIEW embedded product (LabVIEW Embedded Development Module) and may
 *        not be copied, modified, or redistributed.  The code may only be used
 *        in accordance with the terms of the license agreement entered into
 *        between National Instruments Corporation and the authorized user.  Your
 *        use of the code designates your acceptance of the terms of the license
 *        agreement.
 *
 *        Author: Bill Pittman
 *---------------------------------------------------------------------------*/

#include <time.h>
#include <string.h>

/*
 * This driver hooks on to the SysTick interrupt which fires once per
 * millisecond and is used by RTX for timing.  This driver just updates
 * an internal counter and then passes control to the RTX SysTick handler.
 * This approach provides a real-time clock with the accuracy of the
 * system time base, without taking up any additional hardware timing
 * resources
 */

/*
 * The number of systicks per second used by RTX
 */
#define SYSTICKS_PER_SECOND     1000

/*
 * This counter tracks seconds.  It can be used for *nix time().
 */
static time_t tSeconds = 0;

/*
 * This handler is called once per millisecond by the SysTick timer.
 * It is used to maintain a real-time counter in seconds, which can
 * be used by *nix time functions.  The SysTick is used by RTX, so
 * control is passed to the RTX handler.
 */
void
UpdateSeconds(void)
{
    static unsigned long ulSysTicks = SYSTICKS_PER_SECOND;

    if(--ulSysTicks == 0)
    {
        ulSysTicks = SYSTICKS_PER_SECOND;
        ++tSeconds;
    }
}

__asm void
SysTick_Target_Handler(void)
{
    import  UpdateSeconds
    import  SysTick_Handler
    bl      UpdateSeconds
    b       SysTick_Handler
}

/*
 * Target specific version of time().  Returns the value of the
 * seconds counter.
 */
time_t ARM_rtc_time(time_t *t)
{
    if(t)
    {
        *t = tSeconds;
    }
    return(tSeconds);
}

/*
 * Target specific version of stime().  Sets the seconds counter.
 */
int ARM_rtc_stime(time_t *t)
{
    tSeconds = *t;
    return(0);
}
/*
 * Target specific gettime() function.  This is called by LabVIEW
 */
void ARM_rtc_gettime(struct tm *timeptr)
{
    struct tm *ptm;

    ptm = localtime(&tSeconds);
    memcpy(timeptr, ptm, sizeof(struct tm));
}

// Set the RTC to use January 1, 1970
void ARM_init_rtc()
{
    tSeconds = 0;
}

