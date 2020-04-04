/*----------------------------------------------------------------------------
 * Name:    ARM_SPIWrapper.h
 * Purpose: Template generic / LPC24xx I2C driver defines for LabVIEW
 * Version: V1.00
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#ifndef __ARM_SPIWRAPPER_H
#define __ARM_SPIWRAPPER_H

#include "LVSysIncludes.h"
#include "LVDefs.h"
#include "LVConfig.h"

/*----------------------------------------------------------------------------
 SPI interface related prototypes
 *---------------------------------------------------------------------------*/
extern void ARM_SPI_CreateConfigurationReferenceW (
               int16       chipSelect,
               uInt16      clockRate,
               int32       clockPolarity,
               int32       clockPhase,
               int32       nErrIn,
               VoidHand*   pReferenceOut,
               int32*      pnErrOut);

extern void ARM_SPI_WriteReadW (
               VoidHand    pReferenceIn,
               VoidHand    pDataWr,
               int16       bytesToRead,
               int32       nErrIn,
               VoidHand*   pDataRd,
               VoidHand*   pReferenceOut,
               int32*      pnErrOut);

extern void ARM_SPI_CloseReferenceW (
               VoidHand    pReferenceIn,
               int32       nErrIn,
               int32*      pnErrOut);

#endif // __ARM_SPIWRAPPER_H
