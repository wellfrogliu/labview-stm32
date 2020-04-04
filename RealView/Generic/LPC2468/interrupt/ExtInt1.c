/***************************************************************
 * © Copyright 2009 by National Instruments Corp.
 * All rights reserved.
 * Author: Nick Van Sickel
***************************************************************/

#include <LPC23xx.H>                    // LPC23xx definitions
#include "ARM_irq.h"                     // Interrupt controller definitions


// Set up external interrupt 1
void ExtInt1Init(unsigned char bMode, unsigned char bPositive) {

	// configure P2.11 as EINT1
	PINSEL4 &= ~(1UL << 23);
	PINSEL4 |= (1UL << 22);
	
	// writing a 1 bit sets edge-senstive mode
	EXTMODE |= bMode ? 0x2 : 0x0;
	
	// writing a 1 bit sets rising edge or positive level
	EXTPOLAR |= bPositive ? 0x2 : 0x0;
}

void ExtInt1PreFunc(int isr_vector_num, int isr_param) {
	if(EXTMODE & 0x2)
		EXTINT = 0x2;                        // Ack the interrupt in the external interrupt control block
}

// NOTE: We wait until the post func to set the EXTINT register in the case of a level sensitive conifguration.  This way the user has a chance to reset the level in their VI.
void ExtInt1PostFunc(int isr_vector_num, int isr_param) {
	if(!(EXTMODE & 0x2))
		EXTINT = 0x2;					 // Ack the interrupt in the external interrupt control block
	ARM_irq_ack(isr_vector_num);		// Ack the interrupt for the interrupt control block
}