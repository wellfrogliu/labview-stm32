/***************************************************************
 * © Copyright 2009 by National Instruments Corp.
 * All rights reserved.
 * Author: Nick Van Sickel
***************************************************************/

#include <LPC23xx.H>                    // LPC23xx definitions
#include "ARM_irq.h"                     // Interrupt controller definitions

// Set up external interrupt 0
void ExtInt0Init(unsigned char bMode, unsigned char bPositive) {

	// configure P2.10 as EINT0
	PINSEL4 &= ~(1UL << 21);
	PINSEL4 |= (1UL << 20);
	
	// writing a 1 bit sets edge-senstive mode
	EXTMODE |= bMode ? 0x1 : 0x0;
	
	// writing a 1 bit sets rising edge or positive level
	EXTPOLAR |= bPositive ? 0x1 : 0x0;
}

void ExtInt0PreFunc(int isr_vector_num, int isr_param) {
	if(EXTMODE & 0x1)
		EXTINT = 0x1;                        // Ack the interrupt in the external interrupt control block
}

// NOTE: We wait until the post func to set the EXTINT register in the case of level sensitive conifguration.  This way the user has a chance to reset the level in their VI.
void ExtInt0PostFunc(int isr_vector_num, int isr_param) {
	if(!(EXTMODE & 0x1))
		EXTINT = 0x1;					// Ack the interrupt in the external interrupt control block
	ARM_irq_ack(isr_vector_num);		// Ack the interrupt for the interrupt control block
}