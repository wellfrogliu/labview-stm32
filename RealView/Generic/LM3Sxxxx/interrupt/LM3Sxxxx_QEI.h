//*****************************************************************************
//
// LM3Sxxxx_PWM0.h - Prototypes for PWM managed interrupts
//
//
//*****************************************************************************

#ifndef __LM3Sxxxx_QEI_H__
#define __LM3Sxxxx_QEI_H__

#include "arm_irq.h"

//*****************************************************************************
//
// Function prototypes
//
//*****************************************************************************
extern lv_irq_fp LM3Sxxxx_QEI0HandlerP;
extern lv_irq_fp LM3Sxxxx_QEI1HandlerP;
extern void LM3Sxxxx_QEIIntInit(unsigned int ulChannel, unsigned int ulIntFlags, unsigned char bQuadMode, unsigned char bSwap,
					unsigned char bCapAB, unsigned int ulMaxPosition, unsigned char bResetIdx, unsigned char bVelEnable, 
					unsigned int ulPreDiv, unsigned int ulPeriod);

#endif
