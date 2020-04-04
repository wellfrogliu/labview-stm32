//*****************************************************************************
//
// lm3s_pins.c - Implements peripheral pin mapping for LM3Sxxxx parts.
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
// This is part of revision 132 of the Stellaris NI-LabVIEW support package.
//
//*****************************************************************************

#include "hw_types.h"
#include "hw_memmap.h"
#include "gpio.h"

#ifndef LM3S_PART_DEFINED
#define PART_LM3S8962
#endif

#include "pin_map.h"

//
// Maps PWM channel number to GPIO port
//
unsigned long g_ulPWMPort[] =
{   PWM0_PORT, PWM1_PORT, PWM2_PORT, PWM3_PORT, PWM4_PORT, PWM5_PORT };

//
// Maps PWM channel number to physical GPIO pins
//
unsigned long g_ulPWMPin[] =
{   PWM0_PIN, PWM1_PIN, PWM2_PIN, PWM3_PIN, PWM4_PIN, PWM5_PIN };

//
// Maps PWM Fault pin
//
unsigned long g_ulPWMFaultPort = FAULT_PORT;
unsigned long g_ulPWMFaultPin = FAULT_PIN;

