//*****************************************************************************
//
// LM3Sxxxx_GPIOInt.h - Prototypes for GPIO managed interrupts
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
// This is part of revision 95 of the Stellaris NI-LabVIEW support package.
//
//*****************************************************************************

#ifndef __LM3Sxxxx_GPIOINT_H__
#define __LM3Sxxxx_GPIOINT_H__

#include "arm_irq.h"

//*****************************************************************************
//
// Macros needed for GPIOInt functions.
//
//*****************************************************************************
#define GPIO_PORT_A     0
#define GPIO_PORT_B     1
#define GPIO_PORT_C     2
#define GPIO_PORT_D     3
#define GPIO_PORT_E     4
#define GPIO_PORT_F     5
#define GPIO_PORT_G     6
#define GPIO_PORT_H     7
#define GPIO_PIN_0      0x01
#define GPIO_PIN_1      0x02
#define GPIO_PIN_2      0x04
#define GPIO_PIN_3      0x08
#define GPIO_PIN_4      0x10
#define GPIO_PIN_5      0x20
#define GPIO_PIN_6      0x40
#define GPIO_PIN_7      0x80

//*****************************************************************************
//
// Function prototypes
//
//*****************************************************************************
extern lv_irq_fp LM3Sxxxx_GPIOAHandlerP;
extern lv_irq_fp LM3Sxxxx_GPIOBHandlerP;
extern lv_irq_fp LM3Sxxxx_GPIOCHandlerP;
extern lv_irq_fp LM3Sxxxx_GPIODHandlerP;
extern lv_irq_fp LM3Sxxxx_GPIOEHandlerP;
extern lv_irq_fp LM3Sxxxx_GPIOFHandlerP;
extern lv_irq_fp LM3Sxxxx_GPIOGHandlerP;
extern lv_irq_fp LM3Sxxxx_GPIOHHandlerP;
extern void LM3Sxxxx_GPIOIntInit(unsigned long ulPort,
                                 unsigned long ulMask,
                                 unsigned long ulSense,
                                 unsigned long ulEvent,
                                 unsigned long ulBothEdge,
                                 unsigned long ulPullUp,
                                 unsigned long ulPullDown);

#endif
