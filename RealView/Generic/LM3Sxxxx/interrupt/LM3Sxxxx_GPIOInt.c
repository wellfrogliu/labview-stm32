//*****************************************************************************
//
// LM3Sxxxx_GPIOInt.c - Implements GPIO initialization so that a GPIO pin
//                      can generate interrupts.
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

#include "ARM_irq.h"             // LabView interrupt interface

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_gpio.h"
#include "interrupt.h"
#include "gpio.h"
#include "sysctl.h"

extern unsigned long const g_ulGPIOPortMap[];
extern unsigned long const g_ulGPIOIntMap[];

//*****************************************************************************
//
// Configures a GPIO pin to generate interrupts
//
// ulPort - value specifies which GPIO port (0-6 <--> A-G)
// ulPin - bit mask of the pin to configure
// ulConfig - pin interrupt configuration - uses the same values as
// DriverLib
//
// This function is called by the LabView managed interrupts interface.
// It configures a GPIO pin to generate an interrupt according to the
// ulConfig parameter.  The pin can generate an interrupt on rising or
// falling edges, or both, or on a level.  After this function runs, the
// appropriate condition on the GPIO pin will generate an interrupt, but the
// interrupts are still not enabled on the interrupt controller.  A separate
// function call takes care of that.
//
//*****************************************************************************
void
LM3Sxxxx_GPIOIntInit(unsigned long ulPort,
                     unsigned long ulMask,
                     unsigned long ulSense,
                     unsigned long ulEvent,
                     unsigned long ulBothEdge,
                     unsigned long ulPullUp,
                     unsigned long ulPullDown)
{
    unsigned long ulBase;

    //
    // GPIO ports are assumed to be enabled, so nothing is needed for that.
    //

    //
    // Figure out the port base address
    //
    ulBase = g_ulGPIOPortMap[ulPort];

    //
    // Turn off the alt function and configure the pin as an input
    // Set digital enable
    //
    LM3Sxxxx_GPIO_Init(ulPort, (unsigned char)ulMask, GPIO_DIR_MODE_IN);
    HWREG(ulBase + GPIO_O_DEN) |= ulMask;

    //
    // Set the interrupt sense, event, and edge registers according
    // to the input parameters
    //
    HWREG(ulBase + GPIO_O_IS) = (HWREG(ulBase + GPIO_O_IS) & ~ulMask) |
                                 ulSense;
    HWREG(ulBase + GPIO_O_IEV) = (HWREG(ulBase + GPIO_O_IEV) & ~ulMask) |
                                  ulEvent;
    HWREG(ulBase + GPIO_O_IBE) = (HWREG(ulBase + GPIO_O_IBE) & ~ulMask) |
                                  ulBothEdge;

    //
    // Set the pullup and pulldown regs according to the parms
    //
    HWREG(ulBase + GPIO_O_PUR) = (HWREG(ulBase + GPIO_O_PUR) & ~ulMask) |
                                  (ulPullUp & ulMask);
    HWREG(ulBase + GPIO_O_PDR) = (HWREG(ulBase + GPIO_O_PDR) & ~ulMask) |
                                  (ulPullDown & ulMask);

    //
    // Clear any pending interrupt
    //
    HWREG(ulBase + GPIO_O_ICR) = ulMask;

    //
    // Enable the pin for interrupt
    //
    HWREG(ulBase + GPIO_O_IM) |= ulMask;

    //
    // Set the priority for the GPIO interrupt
    //
    IntPrioritySet(g_ulGPIOIntMap[ulPort], 4 << 5);
}
