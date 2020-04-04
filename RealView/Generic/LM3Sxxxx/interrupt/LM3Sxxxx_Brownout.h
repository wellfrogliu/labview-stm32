//*****************************************************************************
//
// LM3Sxxxx_Brownout.h - Prototypes for sysctl managed interrupts
//
//
//*****************************************************************************

#ifndef __LM3Sxxxx_BROWNOUT_H__
#define __LM3Sxxxx_BROWNOUT_H__

#include "arm_irq.h"

//*****************************************************************************
//
// Function prototypes
//
//*****************************************************************************
extern lv_irq_fp LM3Sxxxx_SysCtlHandlerP;
extern void LM3Sxxxx_BrownoutIntInit();

#endif
