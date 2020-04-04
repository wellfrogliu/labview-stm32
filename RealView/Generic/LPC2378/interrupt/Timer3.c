/***************************************************************
 * © Copyright 2007 by National Instruments Corp.
 * All rights reserved.
 * Author: Bill Pittman, Richard Sewell
***************************************************************/

#include <rtl.h>
#include <LPC23xx.H>                    // LPC23xx definitions
#include "ARM_irq.h"                     // Interrupt controller definitions

void Timer3Init(int nCount, BOOL bMillisec) {
	// Set up Timer 3
  PCONP |= 0x800000;                    // Turn on power to Timer3
	if(bMillisec)
		T3MR0 = nCount * 12000 - 1;     // 1msec = 12000-1 at 12 MHz
	else
		T3MR0 = nCount;
	T3MCR         = 3;                  // Interrupt and Reset on MR0
	T3TCR         = 1;                  // Timer3 Enable
}

void Timer3PreFunc(int isr_vector_num, int isr_param) {
	T3IR = 1;                           // Ack the interrupt in the timer control block
}

void Timer3PostFunc(int isr_vector_num, int isr_param) {
	ARM_irq_ack (isr_vector_num);		// Ack the interrupt for the interrupt control block
}

