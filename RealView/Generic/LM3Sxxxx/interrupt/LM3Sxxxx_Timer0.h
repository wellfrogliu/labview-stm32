//*****************************************************************************
//
// LM3Sxxxx_Timer0.h - Prototypes for Timer managed interrupts
//
// Copyright (c) 2008 Luminary Micro, Inc.  All rights reserved.
// 
// Software License Agreement
// 
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
// 
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 38 of the Stellaris NI-LabVIEW support package.
//
//*****************************************************************************

#ifndef __LM3Sxxxx_TIMER0_H__
#define __LM3Sxxxx_TIMER0_H__

#include "arm_irq.h"

#include "LVFuncsUsed.h"
#include "profile_timer.h"

#if defined(USE_HW_PROFILING) && defined(_Include_Profiling)
  #error Timer 0 is used for microsecond timing.  You must disable profiling to use the Timer 0 interrupt.
#endif

//*****************************************************************************
//
// Function prototypes
//
//*****************************************************************************
extern lv_irq_fp LM3Sxxxx_Timer0HandlerP;
extern void LM3Sxxxx_Timer0Init(unsigned long marker_or_ulCount, ... );

#endif
