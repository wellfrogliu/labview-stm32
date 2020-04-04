/*----------------------------------------------------------------------------
 * Name:    ARM_I2C.h
 * Purpose: Template generic / LPC24xx I2C driver
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

#ifndef __ARM_I2C_H
#define __ARM_I2C_H

 
/*----------------------------------------------------------------------------
 Types to pass data 
 *---------------------------------------------------------------------------*/
 typedef struct __bArray {
  unsigned int  len;
  unsigned char* data;
} T_bArray;
  

/*----------------------------------------------------------------------------
 I2C address size
 *---------------------------------------------------------------------------*/
#define I2C_7BIT                           0
#define I2C_10BIT                          1


/*----------------------------------------------------------------------------
 I2C error codes
 *---------------------------------------------------------------------------*/
#define I2C_OK                             0
#define I2C_GENERAL_ERR                  (-1)
#define I2C_INTERFACE_ERR                (-2)
#define I2C_INTERFACE_NOTKNOWN_ERR       (-3)
#define I2C_DEVICE_ERR                   (-4)
#define I2C_ADDRESSSIZE_NOTSUPPORTED_ERR (-5)
#define I2C_REFERENCE_NOTKNOWN_ERR       (-6)
#define I2C_REFERENCE_NOTCREATED_ERR     (-7)


/*----------------------------------------------------------------------------
 I2C interface related prototypes
 *---------------------------------------------------------------------------*/
extern void ARM_I2C_CreateConfigurationReference (
                         int    interface,
                unsigned int    slaveAddress,
                unsigned int    addressSize,   
                unsigned int    clockRate,
                         int*  pReferenceOut,
                         int*  pErrOut);

extern void ARM_I2C_WriteRead (
                         int   nReferenceIn,
                    T_bArray*  pDataWr,
                unsigned int   bytesToRead,
                    T_bArray*  pDataRd,
                         int*  pErrOut);

extern void ARM_I2C_CloseReference (
                         int   nReferenceIn,
                         int*  pErrOut);

#endif // __ARM_I2C_H
