//*****************************************************************************
//
// LM3Sxxxx_ADC.h - Prototypes for ADC managed interrupts
//
//
//*****************************************************************************

#ifndef __LM3Sxxxx_ADC_H__
#define __LM3Sxxxx_ADC_H__

#include "arm_irq.h"

//*****************************************************************************
//
// Function prototypes
//
//*****************************************************************************
extern lv_irq_fp LM3Sxxxx_ADC0HandlerP;
extern lv_irq_fp LM3Sxxxx_ADC1HandlerP;
extern lv_irq_fp LM3Sxxxx_ADC2HandlerP;
extern void LM3Sxxxx_ADCIntInit(unsigned int sequence, unsigned int samples, unsigned int speed,
					unsigned int trigger, unsigned int pins, unsigned int priority, unsigned int differential);

#endif
