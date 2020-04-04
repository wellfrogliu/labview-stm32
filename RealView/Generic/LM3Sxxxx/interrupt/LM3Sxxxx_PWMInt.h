//*****************************************************************************
//
// LM3Sxxxx_PWM0.h - Prototypes for PWM managed interrupts
//
//
//*****************************************************************************

#ifndef __LM3Sxxxx_PWM0_H__
#define __LM3Sxxxx_PWM0_H__

#include "arm_irq.h"

//*****************************************************************************
//
// Function prototypes
//
//*****************************************************************************
extern lv_irq_fp LM3Sxxxx_PWM0HandlerP;
extern lv_irq_fp LM3Sxxxx_PWM1HandlerP;
extern lv_irq_fp LM3Sxxxx_PWM2HandlerP;
void LM3Sxxxx_PWMIntInit(unsigned int uChannelMask, unsigned int uFreq, 
					unsigned int uFlags, unsigned int uDivider, unsigned int uFlags0, 
					unsigned int uFlags1, unsigned int uWidth0, unsigned int uType0, 
					unsigned int uWidth1, unsigned int uType1, unsigned int ulIntFlags);

#endif
