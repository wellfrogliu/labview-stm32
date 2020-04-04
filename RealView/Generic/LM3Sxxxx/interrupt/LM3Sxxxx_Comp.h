//*****************************************************************************
//
// LM3Sxxxx_Comp.h - Prototypes for PWM managed interrupts
//
//
//*****************************************************************************

#ifndef __LM3Sxxxx_COMP_H__
#define __LM3Sxxxx_COMP_H__

#include "arm_irq.h"

//*****************************************************************************
//
// Function prototypes
//
//*****************************************************************************
extern lv_irq_fp LM3Sxxxx_Comp0HandlerP;
extern void LM3Sxxxx_Comp0IntInit(unsigned int ulPinFlag, unsigned int ulVoltage,
								  unsigned int ulIntFlag, unsigned int ulInvert);

#endif
