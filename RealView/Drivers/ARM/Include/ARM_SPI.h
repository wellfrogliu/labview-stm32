/*----------------------------------------------------------------------------
 * Name:    ARM_SPI.h
 * Purpose: Template generic / LPC24xx SPI driver
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

#ifndef __ARM_SPI_H
#define __ARM_SPI_H


/*----------------------------------------------------------------------------
 Types to pass data 
 *---------------------------------------------------------------------------*/
 typedef struct __bArray {
  unsigned int  len;
  unsigned char* data;
} T_bArray;
  

/*----------------------------------------------------------------------------
 SPI Clock data
 *---------------------------------------------------------------------------*/
#define SPI_CLOCK_POLARITY_IDLELOW      0
#define SPI_CLOCK_POLARITY_IDLEHIGH     1
#define SPI_CLOCK_PHASE_FIRSTEDGE       0
#define SPI_CLOCK_PHASE_SECONDEDGE      1


/*----------------------------------------------------------------------------
 SPI error codes
 *---------------------------------------------------------------------------*/
#define SPI_OK                             0
#define SPI_GENERAL_ERR                  (-1)
#define SPI_INTERFACE_ERR                (-2)
#define SPI_CHIPSELECT_NOTKNOWN_ERR      (-3)
#define SPI_DEVICE_ERR                   (-4)
#define SPI_CLKPOLARITY_NOTSUPPORTED_ERR (-5)
#define SPI_CLKPHASE_NOTSUPPORTED_ERR    (-6)
#define SPI_REFERENCE_NOTKNOWN_ERR       (-7)
#define SPI_REFERENCE_NOTCREATED_ERR     (-8)


/*----------------------------------------------------------------------------
 SPI interface related prototypes
 *---------------------------------------------------------------------------*/
extern void ARM_SPI_CreateConfigurationReference (
                         int    chipSelect,
                unsigned int    clockRate,
                unsigned int    clockPolarity,
                unsigned int    clockPhase,
                         int*  pReferenceOut,
                         int*  pErrOut);

extern void ARM_SPI_WriteRead (
                         int   nReferenceIn,
                    T_bArray*  pDataWr,
                unsigned int   bytesToRead,
                    T_bArray*  pDataRd,
                         int*  pErrOut);

extern void ARM_SPI_CloseReference (
                         int   nReferenceIn,
                         int*  pErrOut);

#endif // __ARM_SPI_H
