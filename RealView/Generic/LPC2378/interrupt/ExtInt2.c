/***************************************************************
 * © Copyright 2009 by National Instruments Corp.
 * All rights reserved.
 * Author: Nick Van Sickel
***************************************************************/

#include <LPC23xx.H>                    // LPC23xx definitions
#include "ARM_irq.h"                     // Interrupt controller definitions

// Set up external interrupt 2
void ExtInt2Init(unsigned char bMode, unsigned char bPositive) {

	// configure P2.12 as EINT2
	PINSEL4 &= ~(1UL << 25);
	PINSEL4 |= (1UL << 24);
	
	// writing a 1 bit sets edge-senstive mode
	EXTMODE |= bMode ? 0x4 : 0x0;
	
	// writing a 1 bit sets rising edge or positive level
	EXTPOLAR |= bPositive ? 0x4 : 0x0;
}

void ExtInt2PreFunc(int isr_vector_num, int isr_param) {
	if(EXTMODE & 0x4)
		EXTINT = 0x4;                        // Ack the interrupt in the external interrupt control block
}

// NOTE: We wait until the post func to set the EXTINT register in the case of level sensitive conifguration.  This way the user has a chance to reset the level in their VI.
void ExtInt2PostFunc(int isr_vector_num, int isr_param) {
	if(!(EXTMODE & 0x4))
		EXTINT = 0x4;					// Ack the interrupt in the external interrupt control block
	ARM_irq_ack(isr_vector_num);		// Ack the interrupt for the interrupt control block
}