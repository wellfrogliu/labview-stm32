//*****************************************************************************
//
// ARM_irq.c - Implements interrupt manager interface for LM3Sxxxx targets
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

#include "ARM_irq.h"

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_timer.h"
#include "hw_gpio.h"
#include "hw_pwm.h"
#include "pwm.h"
#include "hw_qei.h"
#include "hw_adc.h"
#include "hw_watchdog.h"
#include "hw_comp.h"
#include "hw_sysctl.h"
#include "interrupt.h"
#include "timer.h"

#include "LVCGenIntInit.h"

//*****************************************************************************
//
// Define the IRQ handlers that are available
//
//*****************************************************************************
#define ARM_IRQ_TIMER0A     (1ULL << INT_TIMER0A)
#define ARM_IRQ_TIMER1A     (1ULL << INT_TIMER1A)
#define ARM_IRQ_TIMER2A     (1ULL << INT_TIMER2A)
#define ARM_IRQ_TIMER3A     (1ULL << INT_TIMER3A)
#define ARM_IRQ_GPIOA       (1ULL << INT_GPIOA)
#define ARM_IRQ_GPIOB       (1ULL << INT_GPIOB)
#define ARM_IRQ_GPIOC       (1ULL << INT_GPIOC)
#define ARM_IRQ_GPIOD       (1ULL << INT_GPIOD)
#define ARM_IRQ_GPIOE       (1ULL << INT_GPIOE)
#define ARM_IRQ_GPIOF       (1ULL << INT_GPIOF)
#define ARM_IRQ_GPIOG       (1ULL << INT_GPIOG)
#define ARM_IRQ_PWM0		(1ULL << INT_PWM0)
#define ARM_IRQ_PWM1		(1ULL << INT_PWM1)
#define ARM_IRQ_PWM2		(1ULL << INT_PWM2)
#define ARM_IRQ_PWM_FAULT	(1ULL << INT_PWM_FAULT)
#define ARM_IRQ_QEI0		(1ULL << INT_QEI0)
#define ARM_IRQ_QEI1		(1ULL << INT_QEI1)
#define ARM_IRQ_ADC0		(1ULL << INT_ADC0)
#define ARM_IRQ_ADC1		(1ULL << INT_ADC1)
#define ARM_IRQ_ADC2		(1ULL << INT_ADC2)
#define ARM_IRQ_WATCHDOG	(1ULL << INT_WATCHDOG)
#define ARM_IRQ_COMP		(1ULL << INT_COMP0)
#define ARM_IRQ_SYSCTL		(1ULL << INT_SYSCTL)


#ifndef ARM_IRQ_AVAILABLE
#define ARM_IRQ_AVAILABLE  (ARM_IRQ_TIMER0A |                                 \
                            ARM_IRQ_TIMER1A |                                 \
                            ARM_IRQ_TIMER2A |                                 \
                            ARM_IRQ_TIMER3A |                                 \
                            ARM_IRQ_GPIOA |                                   \
                            ARM_IRQ_GPIOB |                                   \
                            ARM_IRQ_GPIOC |                                   \
                            ARM_IRQ_GPIOD |                                   \
                            ARM_IRQ_GPIOE |                                   \
                            ARM_IRQ_GPIOF |                                   \
                            ARM_IRQ_GPIOG |									  \
							ARM_IRQ_PWM0  |									  \
							ARM_IRQ_PWM1  |									  \
							ARM_IRQ_PWM2  |                                   \
							ARM_IRQ_PWM_FAULT  |                              \
							ARM_IRQ_QEI0  |									  \
							ARM_IRQ_QEI1  |									  \
							ARM_IRQ_ADC0  |									  \
							ARM_IRQ_ADC1  |									  \
							ARM_IRQ_ADC2  |									  \
							ARM_IRQ_WATCHDOG  |								  \
							ARM_IRQ_COMP0 |									  \
							ARM_IRQ_SYSCTL  								  \
							)
#endif

//*****************************************************************************
//
// TIMER SECTION
//
// For managed interrupts, timers are supported in 32-bit periodic mode.
// This means that only the "A" interrupt is needed.  This section contains
// interrupt function pointers and ISR handler stubs for the supported
// timer peripherals.
//
// The stub handlers are statically registered in the interrupt vector
// table (see Startup.s).
//
//*****************************************************************************

//*****************************************************************************
//
// Pointers to the VI handler function that is installed by LabView.
// These values are assigned by the ARM_irq_register() function (below).
// There is one function pointer for each supported peripheral interrupt.
//
//*****************************************************************************

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_TIMER0A)
lv_irq_fp LM3Sxxxx_Timer0HandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_TIMER1A)
lv_irq_fp LM3Sxxxx_Timer1HandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_TIMER2A)
lv_irq_fp LM3Sxxxx_Timer2HandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_TIMER3A)
lv_irq_fp LM3Sxxxx_Timer3HandlerP = 0;
#endif

//*****************************************************************************
//
// Handle the timer interrupt.
//
// This function is called when the timer interrupt fires.  It clears
// the interrupt status and then calls the installed VI handler, if one
// is present.
//
//*****************************************************************************
void
Timer0AHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_TIMER0A)

	unsigned long ulStatus;

    //
    // Clear the pending interrupt flags
    //
    ulStatus = TimerIntStatus(TIMER0_BASE, 1);
	TimerIntClear(TIMER0_BASE, ulStatus);
	
	// Re-enable timer as it is disabled in edge count mode
	if(ulStatus == TIMER_CAPA_MATCH) {
		 TimerLoadSet(TIMER0_BASE, TIMER_A, TimerLoadGet(TIMER0_BASE, TIMER_A));  // force timer reset (bug?)	
		 TimerEnable(TIMER0_BASE, TIMER_A);
	}

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_Timer0HandlerP)
    {
        LM3Sxxxx_Timer0HandlerP(INT_TIMER0A, 0);
    }

#endif
}


void
Timer1AHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_TIMER1A)

	unsigned long ulStatus;
	
    //
    // Clear the pending interrupt flags
    //
    ulStatus = TimerIntStatus(TIMER1_BASE, 1);
	TimerIntClear(TIMER1_BASE, ulStatus);
	

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_Timer1HandlerP)
    {
        LM3Sxxxx_Timer1HandlerP(INT_TIMER1A, 0);
    }

#endif
}


void
Timer2AHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_TIMER2A)
    
	unsigned long ulStatus;
	
    //
    // Clear the pending interrupt flags
    //
    ulStatus = TimerIntStatus(TIMER2_BASE, 1);
	TimerIntClear(TIMER2_BASE, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_Timer2HandlerP)
    {
        LM3Sxxxx_Timer2HandlerP(INT_TIMER2A, 0);
    }

#endif
}


void
Timer3AHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_TIMER3A)

	unsigned long ulStatus;
	
    //
    // Clear the pending interrupt flags
    //
    ulStatus = TimerIntStatus(TIMER3_BASE, 1);
	TimerIntClear(TIMER3_BASE, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_Timer3HandlerP)
    {
        LM3Sxxxx_Timer3HandlerP(INT_TIMER3A, 0);
    }

#endif
}


//*****************************************************************************
//
// GPIO SECTION
//
// For managed interrupts, GPIO pins are able to generate interrupts on
// either or both edges, or on a level.  This section contains interrupt
// function pointers and ISR handler stubs for each GPIO port.
//
// The stub handlers are statically registered in the interrupt vector
// table (see Startup.s).
//
//*****************************************************************************

//*****************************************************************************
//
// Pointers to the VI handler function that is installed by LabView.
// These values are assigned by the ARM_irq_register() function (below).
// There is one function pointer for each supported peripheral interrupt.
//
//*****************************************************************************

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOA)
lv_irq_fp LM3Sxxxx_GPIOAHandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOB)
lv_irq_fp LM3Sxxxx_GPIOBHandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOC)
lv_irq_fp LM3Sxxxx_GPIOCHandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOD)
lv_irq_fp LM3Sxxxx_GPIODHandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOE)
lv_irq_fp LM3Sxxxx_GPIOEHandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOF)
lv_irq_fp LM3Sxxxx_GPIOFHandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOG)
lv_irq_fp LM3Sxxxx_GPIOGHandlerP = 0;
#endif

//*****************************************************************************
//
// Handle the GPIO interrupt.
//
// This function is called when the GPIO interrupt fires.  It clears
// the interrupt status and then calls the installed VI handler, if one
// is present.
//
//*****************************************************************************
void
GPIOAHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOA)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
	ulStatus = GPIOPinIntStatus(GPIO_PORTA_BASE, 1);
    GPIOPinIntClear(GPIO_PORTA_BASE, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_GPIOAHandlerP)
    {
        LM3Sxxxx_GPIOAHandlerP(INT_GPIOA, ulStatus);
    }

#endif
}

void
GPIOBHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOB)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
    ulStatus = GPIOPinIntStatus(GPIO_PORTB_BASE, 1);
    GPIOPinIntClear(GPIO_PORTB_BASE, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_GPIOBHandlerP)
    {
        LM3Sxxxx_GPIOBHandlerP(INT_GPIOB, ulStatus);
    }

#endif
}


void
GPIOCHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOC)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
    ulStatus = GPIOPinIntStatus(GPIO_PORTC_BASE, 1);
    GPIOPinIntClear(GPIO_PORTC_BASE, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_GPIOCHandlerP)
    {
        LM3Sxxxx_GPIOCHandlerP(INT_GPIOC, ulStatus);
    }
#endif
}



void
GPIODHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOD)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
    ulStatus = GPIOPinIntStatus(GPIO_PORTD_BASE, 1);
    GPIOPinIntClear(GPIO_PORTD_BASE, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_GPIODHandlerP)
    {
        LM3Sxxxx_GPIODHandlerP(INT_GPIOD, ulStatus);
    }

#endif
}


void
GPIOEHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOE)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
    ulStatus = GPIOPinIntStatus(GPIO_PORTE_BASE, 1);
    GPIOPinIntClear(GPIO_PORTE_BASE, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_GPIOEHandlerP)
    {
        LM3Sxxxx_GPIOEHandlerP(INT_GPIOE, ulStatus);
    }

#endif
}


void
GPIOFHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOF)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
    ulStatus = GPIOPinIntStatus(GPIO_PORTF_BASE, 1);
    GPIOPinIntClear(GPIO_PORTF_BASE, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_GPIOFHandlerP)
    {
        LM3Sxxxx_GPIOFHandlerP(INT_GPIOF, ulStatus);
    }

#endif
}


void
GPIOGHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_GPIOG)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
    ulStatus = GPIOPinIntStatus(GPIO_PORTH_BASE, 1);
    GPIOPinIntClear(GPIO_PORTH_BASE, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_GPIOGHandlerP)
    {
        LM3Sxxxx_GPIOGHandlerP(INT_GPIOG, ulStatus);
    }

#endif
}

//*****************************************************************************
//
// PWM SECTION
//
// 
// 
// 
//
// The stub handlers are statically registered in the interrupt vector
// table (see Startup.s).
//
//*****************************************************************************

//*****************************************************************************
//
// Pointers to the VI handler function that is installed by LabView.
// These values are assigned by the ARM_irq_register() function (below).
// There is one function pointer for each supported peripheral interrupt.
//
//*****************************************************************************

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_PWM0)
lv_irq_fp LM3Sxxxx_PWM0HandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_PWM1)
lv_irq_fp LM3Sxxxx_PWM1HandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_PWM2)
lv_irq_fp LM3Sxxxx_PWM2HandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_PWM_FAULT)
lv_irq_fp LM3Sxxxx_PWM_FaultHandlerP = 0;
#endif

//*****************************************************************************
//
// Handle the PWM interrupt.
//
// This function is called when the PWM interrupt fires.  It clears
// the interrupt status and then calls the installed VI handler, if one
// is present.
//
//*****************************************************************************
void
PWM0Handler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_PWM0)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
    ulStatus = PWMGenIntStatus(PWM_BASE, PWM_GEN_0, 1);
    PWMGenIntClear(PWM_BASE, PWM_GEN_0, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_PWM0HandlerP)
    {
        LM3Sxxxx_PWM0HandlerP(INT_PWM0, ulStatus);
    }

#endif
}

void
PWM1Handler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_PWM1)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
    ulStatus = PWMGenIntStatus(PWM_BASE, PWM_GEN_1, 1);
    PWMGenIntClear(PWM_BASE, PWM_GEN_1, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_PWM1HandlerP)
    {
        LM3Sxxxx_PWM1HandlerP(INT_PWM1, ulStatus);
    }

#endif
}

void
PWM2Handler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_PWM2)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
    ulStatus = PWMGenIntStatus(PWM_BASE, PWM_GEN_2, 1);
    PWMGenIntClear(PWM_BASE, PWM_GEN_2, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_PWM2HandlerP)
    {
        LM3Sxxxx_PWM2HandlerP(INT_PWM2, ulStatus);
    }
#endif
}

void
PWM_FaultHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_PWM_FAULT)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
    ulStatus = PWMIntStatus(PWM_BASE, 1);
    PWMFaultIntClear(PWM_BASE);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_PWM_FaultHandlerP)
    {
        LM3Sxxxx_PWM_FaultHandlerP(INT_PWM_FAULT, 0);
    }

#endif
}

//*****************************************************************************
//
// QEI SECTION
//
// 
// 
// 
//
// The stub handlers are statically registered in the interrupt vector
// table (see Startup.s).
//
//*****************************************************************************

//*****************************************************************************
//
// Pointers to the VI handler function that is installed by LabView.
// These values are assigned by the ARM_irq_register() function (below).
// There is one function pointer for each supported peripheral interrupt.
//
//*****************************************************************************

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_QEI0)
lv_irq_fp LM3Sxxxx_QEI0HandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_QEI1)
lv_irq_fp LM3Sxxxx_QEI1HandlerP = 0;
#endif

//*****************************************************************************
//
// Handle the QEI interrupt.
//
// This function is called when the QEI interrupt fires.  It clears
// the interrupt status and then calls the installed VI handler, if one
// is present.
//
//*****************************************************************************
void
QEI0Handler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_QEI0)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
    ulStatus = QEIIntStatus(QEI0_BASE, 1);
    QEIIntClear(QEI0_BASE, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_QEI0HandlerP)
    {
        LM3Sxxxx_QEI0HandlerP(INT_QEI0, ulStatus);
    }

#endif
}

void
QEI1Handler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_QEI1)

    unsigned long ulStatus;

    //
    // Get the status and clear the pending interrupts
    //
    ulStatus = QEIIntStatus(QEI1_BASE, 1);
    QEIIntClear(QEI1_BASE, ulStatus);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_QEI1HandlerP)
    {
        LM3Sxxxx_QEI1HandlerP(INT_QEI1, ulStatus);
    }

#endif
}

//*****************************************************************************
//
// ADC SECTION
//
// 
// 
// 
//
// The stub handlers are statically registered in the interrupt vector
// table (see Startup.s).
//
//*****************************************************************************

//*****************************************************************************
//
// Pointers to the VI handler function that is installed by LabView.
// These values are assigned by the ARM_irq_register() function (below).
// There is one function pointer for each supported peripheral interrupt.
//
//*****************************************************************************

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_ADC0)
lv_irq_fp LM3Sxxxx_ADC0HandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_ADC1)
lv_irq_fp LM3Sxxxx_ADC1HandlerP = 0;
#endif
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_ADC2)
lv_irq_fp LM3Sxxxx_ADC2HandlerP = 0;
#endif

//*****************************************************************************
//
// Handle the ADC interrupt.
//
// This function is called when the ADC interrupt fires.  It clears
// the interrupt status and then calls the installed VI handler, if one
// is present.
//
//*****************************************************************************
void
ADC0Handler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_ADC0)

    //
    // Clear the pending interrupt
    //
    ADCIntClear(ADC_BASE, 0);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_ADC0HandlerP)
    {
        LM3Sxxxx_ADC0HandlerP(INT_ADC0, 0);
    }

#endif
}

void
ADC1Handler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_ADC1)

    //
    // Clear the pending interrupt
    //
    ADCIntClear(ADC_BASE, 1);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_ADC1HandlerP)
    {
        LM3Sxxxx_ADC1HandlerP(INT_ADC1, 0);
    }

#endif
}

void
ADC2Handler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_ADC2)

    //
    // Clear the pending interrupt
    //
    ADCIntClear(ADC_BASE, 2);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_ADC2HandlerP)
    {
        LM3Sxxxx_ADC2HandlerP(INT_ADC2, 0);
    }
#endif
}

//*****************************************************************************
//
// WATCHDOG SECTION
//
// 
// 
// 
//
// The stub handlers are statically registered in the interrupt vector
// table (see Startup.s).
//
//*****************************************************************************

//*****************************************************************************
//
// Pointers to the VI handler function that is installed by LabView.
// These values are assigned by the ARM_irq_register() function (below).
// There is one function pointer for each supported peripheral interrupt.
//
//*****************************************************************************

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_WATCHDOG)
lv_irq_fp LM3Sxxxx_WatchdogHandlerP = 0;
#endif

//*****************************************************************************
//
// Handle the Watchdog interrupt.
//
// This function is called when the watchdog interrupt fires.  It clears
// the interrupt status and then calls the installed VI handler, if one
// is present.
//
//*****************************************************************************
void
WatchdogHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_WATCHDOG)

	unsigned char bLocked;
	
	bLocked = WatchdogLockState(WATCHDOG_BASE);

    //
    // Clear the pending interrupt.  we must first unlock the registers if needed.
    //
	if(bLocked) {
		WatchdogUnlock(WATCHDOG_BASE);
	}
    WatchdogIntClear(WATCHDOG_BASE);
	if(bLocked) {
		WatchdogLock(WATCHDOG_BASE);
	}

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_WatchdogHandlerP)
    {
        LM3Sxxxx_WatchdogHandlerP(INT_WATCHDOG, 0);
    }

#endif
}

//*****************************************************************************
//
// ANALOG COMPARATOR SECTION
//
// 
// 
// 
//
// The stub handlers are statically registered in the interrupt vector
// table (see Startup.s).
//
//*****************************************************************************

//*****************************************************************************
//
// Pointers to the VI handler function that is installed by LabView.
// These values are assigned by the ARM_irq_register() function (below).
// There is one function pointer for each supported peripheral interrupt.
//
//*****************************************************************************

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_COMP)
lv_irq_fp LM3Sxxxx_Comp0HandlerP = 0;
#endif

//*****************************************************************************
//
// Handle the comparator interrupt.
//
// This function is called when the comparator interrupt fires.  It clears
// the interrupt status and then calls the installed VI handler, if one
// is present.
//
//*****************************************************************************
void
Comp0Handler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_COMP)

    //
    // Clear the pending interrupts
    //
    ComparatorIntClear(COMP_BASE, 0);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_Comp0HandlerP)
    {
        LM3Sxxxx_Comp0HandlerP(INT_COMP0, 0);
    }

#endif
}

//*****************************************************************************
//
// SYSTEM CONTROL SECTION
//
// 
// 
// 
//
// The stub handlers are statically registered in the interrupt vector
// table (see Startup.s).
//
//*****************************************************************************

//*****************************************************************************
//
// Pointers to the VI handler function that is installed by LabView.
// These values are assigned by the ARM_irq_register() function (below).
// There is one function pointer for each supported peripheral interrupt.
//
//*****************************************************************************

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_SYSCTL)
lv_irq_fp LM3Sxxxx_SysCtlHandlerP = 0;
#endif

//*****************************************************************************
//
// Handle the System Control interrupt.
//
// This function is called when the SysCtl interrupt fires.  It clears
// the interrupt status and then calls the installed VI handler, if one
// is present.
//
//*****************************************************************************
void
SysCtlHandler(void)
{
#if (ARM_IRQ_AVAILABLE & ARM_IRQ_SYSCTL)

    //
    // Clear the pending interrupt
    //
    SysCtlIntClear(SYSCTL_INT_BOR);

    //
    // If a VI handler is installed, then call it.
    //
    if(LM3Sxxxx_SysCtlHandlerP)
    {
        LM3Sxxxx_SysCtlHandlerP(INT_SYSCTL, 0);
    }

#endif
}

//*****************************************************************************
//
// LABVIEW MANAGED INTERRRUPT INTERFACE
//
// Below are the functions that LabView/RTX will call to install and
// configure interrupts.  The prototypes and intended behavior is
// defined by LabView/RTX.
//
//*****************************************************************************

//*****************************************************************************
//
// Initialize the interrupt system
//
// For our target there is nothing to do globally to initialize the
// interrupts, so just return.
//
// Possible actions could be to globally enable interrupts (which are enabled
// by default).  Also, this function could be used to set up dynamic
// registration if run-time registration was used.
//
//*****************************************************************************
int
ARM_irq_init(void)
{
    return 1;
}

//*****************************************************************************
//
// Uninitialize the interrupt system
//
// For our target there is nothing to do globally to uninitialize the
// interrupts, so just return.
//
// However, one possible action here could be to globally disable interrupts.
//
//*****************************************************************************
int
ARM_irq_uninit(void)
{
    return 1;
}

//*****************************************************************************
//
// Register an interrupt handler.
//
// isr_vector_num - vector number (not used here)
// isr_param - parameter to be passed to handler (not used here)
// isr_priority - priority for the interrupt (not used here)
// isr_startFunc - function to call before VI handler (repurposed here)
// isr_runFunc - entry point of VI code that is to be the interrupt handler
// isr_endFunc - function to call after VI handler runs (not used here)
//
// This function is used to install the address of an interrupt handler
// (which is VI code) to be called when an interrupt occurs.
//
// The isr_startFunc parameter is used to pass the address of the function
// pointer for the interrupt handler.  The parameter isr_runFunc is the
// address of the VI function itself.  The function pointer (provided by
// startFunc) will be set to point at the VI interrupt handler (runFunc).
// The stub interrupt handler for the peripheral (above) uses the function
// pointer to call the VI interrupt handler.
//
// Since there is no separate table of interrupt vectors maintained here,
// there is nothing to do with the vector number.  The parameter isr_param
// is not used.  The priority is not used at this time but it could be.
// All peripheral interrupts are set to default priority now.
//
//*****************************************************************************
int
ARM_irq_register(int isr_vector_num, int isr_param, int isr_priority,
                   lv_irq_fp isr_startFunc, lv_irq_fp isr_runFunc,
                   lv_irq_fp isr_endFunc)
{
    //
    // Check to make sure a non-NULL pointer is passed for the
    // function pointer.
    //
    if(isr_startFunc)
    {
        //
        // Load the address of the VI interrupt handler into the
        // interrupt function pointer, so the VI can be called when
        // the interrupt occurs.
        // Note the cast is needed because we are using isr_startFunc
        // as a pointer to a function pointer, instead of as a function
        // pointer itself (as defined by the prototype)
        //
        *((lv_irq_fp *)isr_startFunc) = isr_runFunc;
        return(1);
    }
    else
    {
        //
        // If the function pointer was NULL, then return an error.
        //
        return(0);
    }
}

//*****************************************************************************
//
// Unregister an interrupt handler.
//
// Unregistering a registered interrupt is not supported at this time.
// To support this feature would require keeping an additional table
// indexed by vector number, to remember which vector have which interrupt
// function pointers.
//
//*****************************************************************************
int
ARM_irq_unregister(int isr_vector_num)
{
    return(0);
}

//*****************************************************************************
//
// Enable interrupt on the master interrupt controller.
//
// This function will enable the specified interrupt vector on the
// interrupt controller (NVIC).
//
//*****************************************************************************
void
ARM_irq_enable(int isr_vector_num)
{
    IntEnable(isr_vector_num);
}

//*****************************************************************************
//
// Disable interrupt on the master interrupt controller.
//
// This function will disable the specified interrupt vector on the
// interrupt controller (NVIC).
//
//*****************************************************************************
void ARM_irq_disable (int isr_vector_num) {
    IntDisable(isr_vector_num);
}

//*****************************************************************************
//
// Acknowledge interrupt
//
// For CM3, there is nothing special to do to acknowledge the interrupt
// on the master interrupt controller (NVIC).  For each peripheral, the
// interrupt must be cleared, but this is handled by each peripherals
// interrupt stub handler (above).
//
//*****************************************************************************
void
ARM_irq_ack(int isr_vector_num)
{
}
