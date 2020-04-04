/*----------------------------------------------------------------------------
 * Name:    ARM_I2CWrapper.c
 * Purpose: Template generic / LPC24xx I2C driver for LabVIEW
 * Version: v1.00
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
 
#include "ARM_I2C.h"
 
#include "LVSysIncludes.h"
#include "LVCCG.h"
#include "CCGArrSupport.h"

#ifdef _Include_ARM_I2C_Functions

/*----------------------------------------------------------------------------
  Copy In reverence to Out reverence
 *---------------------------------------------------------------------------*/
static void cpyRevIn2Out(int32 nReverenceIn, int32* pnReverenceOut) {

  if (pnReverenceOut) {
    *pnReverenceOut = nReverenceIn;
  }
}

/*----------------------------------------------------------------------------
  Copy In Error to Out Error
 *---------------------------------------------------------------------------*/
static Boolean cpyErrIn2Out(int32 nErrIn, int32* pnErrOut) {

  if (nErrIn) {                          // check for existing error 
    if (pnErrOut) {
      *pnErrOut = nErrIn;
    }
    return (true);
  }

  return (false);
}

/*----------------------------------------------------------------------------
  Convert Error to LV error code
 *---------------------------------------------------------------------------*/
static void cnvErrI2C2LV (int32 nErrIn, int32* pnErrOut) {

  if (pnErrOut) {
    switch (nErrIn) {
      case I2C_OK:
      case I2C_GENERAL_ERR:
      case I2C_INTERFACE_ERR:
		*pnErrOut = nErrIn;	 
		break;
	  case I2C_INTERFACE_NOTKNOWN_ERR:
		*pnErrOut = k_I2C_Interface_NotKnown;
		break;
      case I2C_DEVICE_ERR:
		*pnErrOut = k_I2C_Device_Err;
		break;
      case I2C_ADDRESSSIZE_NOTSUPPORTED_ERR:
		*pnErrOut = k_AddressSize_NotSupported;
		break;
      case I2C_REFERENCE_NOTKNOWN_ERR:
      case I2C_REFERENCE_NOTCREATED_ERR:
        *pnErrOut = k_I2C_Reference_NotKnown;
        break;

      default:
        *pnErrOut = I2C_GENERAL_ERR;
        break;

    } // end switch
  } // end then
}

/*----------------------------------------------------------------------------
  I2C create a reference
 *---------------------------------------------------------------------------*/
void ARM_I2C_CreateConfigurationReferenceW (
               uInt16      slaveAddress,
		       int32       addressSize,   
               uInt16      clockRate,
               int32       nErrIn,
               VoidHand*   pReferenceOut,
               int32*      pnErrOut) {

  int      i2cErr    = I2C_GENERAL_ERR;

  if (cpyErrIn2Out(nErrIn, pnErrOut))  goto end;           // check for existing error

  ARM_I2C_CreateConfigurationReference (
                               0,
                (unsigned int) slaveAddress,
                (unsigned int) addressSize,   
                (unsigned int) clockRate,
                        (int*) pReferenceOut,
                              &i2cErr);

end:
  if (!nErrIn) {
    cnvErrI2C2LV (i2cErr, pnErrOut);                       // convert I2C error
  }
}

/*----------------------------------------------------------------------------
  I2C perform a write/read access
 *---------------------------------------------------------------------------*/
void ARM_I2C_WriteReadW (
               VoidHand    pReferenceIn,                   // ?? but it is interpreted as int
               VoidHand    pDataWr,
               int16       bytesToRead,
               int32       nErrIn,
               VoidHand*   pDataRd,
               VoidHand*   pReferenceOut,
               int32*      pnErrOut) {

  int      i2cErr    = I2C_GENERAL_ERR;
  T_bArray i2cDataWr = {0,0};
  T_bArray i2cDataRd = {0,0};

  cpyRevIn2Out((int32)pReferenceIn, (int32*)pReferenceOut);// copy the reference
  if (cpyErrIn2Out(nErrIn, pnErrOut))  goto end;           // check for existing error
  if ((!pDataRd) && (bytesToRead > 0)) goto end;           // check read data 

  if(!pDataRd && !pDataWr){ 							   // why are we here??  nothing to do.
    i2cErr = I2C_OK;
    goto end;
  }
  
  if(pDataWr)
  {
	i2cDataWr.data = PDAArrGetData (pDataWr, 0);             // set pointer to write data
	i2cDataWr.len = (unsigned int)PDAArrNElems(pDataWr);     // set write length
  }

  if (bytesToRead > 0) {                                   // check if bytes must be red
    *pDataRd = PDAArrNew1D(bytesToRead, uCharDataType);    // create an array for read data
	if (!(*pDataRd)) goto end;                             // failed to allocate read data
    i2cDataRd.data = PDAArrGetData (*pDataRd, 0);
    i2cDataRd.len  = (unsigned int)PDAArrNElems(*pDataRd); // just to check if the array is allocated
  }

  ARM_I2C_WriteRead (
                         (int)  pReferenceIn,              // ?? should be (int*) but it is (int)
                               &i2cDataWr,
                (unsigned int)  bytesToRead,
                               &i2cDataRd,
                               &i2cErr);

end:
  if (!nErrIn) {
    cnvErrI2C2LV (i2cErr, pnErrOut);                       // convert I2C error
  }
  if (pDataWr) {
    PDAArrFree(pDataWr);                                   // free write data Array
  }

}


/*----------------------------------------------------------------------------
  I2C close a  reference
 *---------------------------------------------------------------------------*/
void ARM_I2C_CloseReferenceW (
               VoidHand    pReferenceIn,                   // ?? but it is interpreted as int
               int32       nErrIn,
               int32*      pnErrOut) {
  
  int      i2cErr    = I2C_GENERAL_ERR;

  if (cpyErrIn2Out(nErrIn, pnErrOut))  goto end;           // check for existing error

  ARM_I2C_CloseReference (
                        (int)  pReferenceIn,	           // ?? should be (int*) but it is (int)
                              &i2cErr);

end:
  if (!nErrIn) {
    cnvErrI2C2LV (i2cErr, pnErrOut);                       // convert I2C error
  }
}

#endif // _Include_ARM_I2C_Functions
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
