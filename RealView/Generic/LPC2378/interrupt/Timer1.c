/***************************************************************
 * © Copyright 2007 by National Instruments Corp.
 * All rights reserved.
 * Author: Bill Pittman, Richard Sewell
***************************************************************/

#include <rtl.h>
#include <LPC23xx.H>                    // LPC23xx definitions
#include "ARM_irq.h"                     // Interrupt controller definitions

void Timer1Init(int nCount, BOOL bMillisec) {
	// Set up Timer 1
	if(bMillisec)
		T1MR0 = nCount * 12000 - 1;        // 1msec = 12000-1 at 12 MHz
	else
		T1MR0 = nCount;
	T1MCR         = 3;                  // Interrupt and Reset on MR0
	T1TCR         = 1;                  // Timer1 Enable
}

void Timer1PreFunc(int isr_vector_num, int isr_param) {
	T1IR = 1;                           // Ack the interrupt in the timer control block
}

void Timer1PostFunc(int isr_vector_num, int isr_param) {
	ARM_irq_ack (isr_vector_num);		// Ack the interrupt for the interrupt control block
}
