/***************************************************************
 * © Copyright 2007 by National Instruments Corp.
 * All rights reserved.
 * Author: Bill Pittman, Richard Sewell
***************************************************************/

#include "LVFuncsUsed.h"
#include "profile_timer.h"

#if defined(USE_HW_PROFILING) && defined(_Include_Profiling)
  #error Timer 1 is used for microsecond timing.  You must disable profiling to use the Timer 1 interrupt.
#endif

void Timer1Init(int, BOOL);
void Timer1PreFunc(int isr_vector_num, int isr_param);
void Timer1PostFunc(int isr_vector_num, int isr_param);
