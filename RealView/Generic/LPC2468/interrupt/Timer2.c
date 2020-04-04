/***************************************************************
 * © Copyright 2007 by National Instruments Corp.
 * All rights reserved.
 * Author: Bill Pittman, Richard Sewell
***************************************************************/

#include <rtl.h>
#include <LPC23xx.H>                    // LPC23xx definitions
#include "ARM_irq.h"                     // Interrupt controller definitions

void Timer2Init(int nCount, BOOL bMillisec) {
	// Set up Timer 2
  PCONP |= 0x400000;                    // Turn on power to Timer2
	if(bMillisec)
		T2MR0 = nCount * 12000 - 1;     // 1msec = 12000-1 at 12 MHz
	else
		T2MR0 = nCount;
	T2MCR         = 3;                  // Interrupt and Reset on MR0
	T2TCR         = 1;                  // Timer2 Enable
}

void Timer2PreFunc(int isr_vector_num, int isr_param) {
	T2IR = 1;                           // Ack the interrupt in the timer control block
}

void Timer2PostFunc(int isr_vector_num, int isr_param) {
	ARM_irq_ack (isr_vector_num);		// Ack the interrupt for the interrupt control block
}

