/***************************************************************
 * © Copyright 2009 by National Instruments Corp.
 * All rights reserved.
 * Author: Nick Van Sickel
***************************************************************/

#include <LPC23xx.H>                    // LPC23xx definitions
#include "ARM_irq.h"                     // Interrupt controller definitions

// Set up external interrupt 3
void ExtInt3Init(unsigned char bMode, unsigned char bPositive) {

	// configure P2.13 as EINT0
	PINSEL4 &= ~(1UL << 27);
	PINSEL4 |= (1UL << 26);
	
	// writing a 1 bit sets edge-senstive mode
	EXTMODE |= bMode ? 0x8 : 0x0;
	
	// writing a 1 bit sets rising edge or positive level
	EXTPOLAR |= bPositive ? 0x8 : 0x0;
}

void ExtInt3PreFunc(int isr_vector_num, int isr_param) {
	if(EXTMODE & 0x8)
		EXTINT = 0x8;                        // Ack the interrupt in the external interrupt control block
}

// NOTE: We wait until the post func to set the EXTINT register in the case of level sensitive conifguration.  This way the user has a chance to reset the level in their VI.
void ExtInt3PostFunc(int isr_vector_num, int isr_param) {
	if(!(EXTMODE & 0x8))
		EXTINT = 0x8;					// Ack the interrupt in the external interrupt control block
	ARM_irq_ack(isr_vector_num);		// Ack the interrupt for the interrupt control block
}