#ifndef __PROFILE_TIMER_H__
#define __PROFILE_TIMER_H__

#include <LPC23xx.H>                    // LPC23xx definitions

// Define USE_HW_PROFILING to enable microsecond profiling (requires a hardware timer).  You must implement all of the macros and functions below.
#define USE_HW_PROFILING

// OS_USECOND_RATE represents the count rate of your hardware timer (e.g. 50000000 would represent a 50MHz timer)
#define OS_USECOND_RATE 1000000

// USECOND_TIME is used to retrieve the current value of the hardware timer.   The counter is expected to be an up-counter (thus if it is a down-coiunter, you will need to subtract the current value from the max value).
#define USECOND_TIME T1TC

//  Initialize the hardware timer.
void microsecond_init(void);

void microsecond_fini(void);

#endif  // DO NOT add anything after this line
