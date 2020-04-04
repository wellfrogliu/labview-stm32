//*****************************************************************************
//
// LM3Sxxxx_Watchdog.h - Prototypes for Watchdog managed interrupts
//
//
//*****************************************************************************

#ifndef __LM3Sxxxx_Watchdog_H__
#define __LM3Sxxxx_Watchdog_H__

#include "arm_irq.h"

//*****************************************************************************
//
// Function prototypes
//
//*****************************************************************************
extern lv_irq_fp LM3Sxxxx_WatchdogHandlerP;
extern void LM3Sxxxx_WatchdogIntInit(unsigned char bLock, unsigned char ucUnits, unsigned int ulCount);

#endif
