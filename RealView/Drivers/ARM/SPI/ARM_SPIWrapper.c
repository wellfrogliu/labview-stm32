/*----------------------------------------------------------------------------
 * Name:    ARM_SPIWrapper.c
 * Purpose: Template generic / LPC24xx SPI driver for LabVIEW
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
 
#include "ARM_SPI.h"
 
#include "LVSysIncludes.h"
#include "LVCCG.h"
#include "CCGArrSupport.h"

#ifdef _Include_ARM_SPI_Functions

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
static void cnvErrSPI2LV (int32 nErrIn, int32* pnErrOut) {

  if (pnErrOut) {
    switch (nErrIn) {
      case SPI_OK:
      case SPI_INTERFACE_ERR:
	  	*pnErrOut = nErrIn;
		break;
      case SPI_CHIPSELECT_NOTKNOWN_ERR:
	  	*pnErrOut = k_SPI_ChipSelect_NotKnown;
		break;
      case SPI_DEVICE_ERR:
	  	*pnErrOut = k_SPI_Device_Err;
		break;
      case SPI_CLKPOLARITY_NOTSUPPORTED_ERR:
	  	*pnErrOut = k_SPI_ClkPolarity_NotSupported;
		break;
      case SPI_CLKPHASE_NOTSUPPORTED_ERR:
	  	*pnErrOut = k_SPI_ClkPhase_NotSupported;
		break;
      case SPI_REFERENCE_NOTKNOWN_ERR:
      case SPI_REFERENCE_NOTCREATED_ERR:
	  	*pnErrOut = k_SPI_Reference_NotKnown;
		break;
      case SPI_GENERAL_ERR:
        *pnErrOut = nErrIn;
        break;

      default:
        *pnErrOut = SPI_GENERAL_ERR;
        break;

    } // end switch
  } // end then
}

/*----------------------------------------------------------------------------
  SPI create a reference
 *---------------------------------------------------------------------------*/
void ARM_SPI_CreateConfigurationReferenceW (
               int16       chipSelect,
               uInt16      clockRate,
               int32       clockPolarity,
               int32       clockPhase,
               int32       nErrIn,
               VoidHand*   pReferenceOut,
               int32*      pnErrOut) {

  int      spiErr    = SPI_GENERAL_ERR;

  if (cpyErrIn2Out(nErrIn, pnErrOut))  goto end;           // check for existing error

  ARM_SPI_CreateConfigurationReference (
                (unsigned int) chipSelect,
                (unsigned int) clockRate,
                (unsigned int) clockPolarity,   
                (unsigned int) clockPhase,
                        (int*) pReferenceOut,
                              &spiErr);

end:
  if (!nErrIn) {
    cnvErrSPI2LV (spiErr, pnErrOut);                       // convert I2C error
  }
}

/*----------------------------------------------------------------------------
  SPI perform a write/read access
 *---------------------------------------------------------------------------*/
void ARM_SPI_WriteReadW (
               VoidHand    pReferenceIn,
               VoidHand    pDataWr,
               int16       bytesToRead,
               int32       nErrIn,
               VoidHand*   pDataRd,
               VoidHand*   pReferenceOut,
               int32*      pnErrOut) {

  int      spiErr    = SPI_GENERAL_ERR;
  T_bArray spiDataWr = {0,0};
  T_bArray spiDataRd = {0,0};

  cpyRevIn2Out((int32)pReferenceIn, (int32*)pReferenceOut);// copy the reference
  if (cpyErrIn2Out(nErrIn, pnErrOut))  goto end;           // check for existing error
  if ((!pDataRd) && (bytesToRead > 0)) goto end;           // check read data 

  if(pDataWr)
  {
  spiDataWr.data = PDAArrGetData (pDataWr, 0);             // set pointer to write data
  spiDataWr.len = (unsigned int)PDAArrNElems(pDataWr);     // set write length
  }

  if (bytesToRead > 0) {                                   // check if bytes must be red
    *pDataRd = PDAArrNew1D(bytesToRead, uCharDataType);    // create an array for read data
	if (!(*pDataRd)) goto end;                             // failed to allocate read data
    spiDataRd.data = PDAArrGetData (*pDataRd, 0);
    spiDataRd.len  = (unsigned int)PDAArrNElems(*pDataRd); // just to check if the array is allocated
  }

  ARM_SPI_WriteRead (
                         (int)  pReferenceIn,              // ?? should be (int*) but it is (int)
                               &spiDataWr,
                (unsigned int)  bytesToRead,
                               &spiDataRd,
                               &spiErr);

end:
  if (!nErrIn) {
    cnvErrSPI2LV (spiErr, pnErrOut);                       // convert I2C error
  }
  if (pDataWr) {
    PDAArrFree(pDataWr);                                   // free write data Array
  }

}


/*----------------------------------------------------------------------------
  SPI close a  reference
 *---------------------------------------------------------------------------*/
void ARM_SPI_CloseReferenceW (
               VoidHand    pReferenceIn,
               int32       nErrIn,
               int32*      pnErrOut) {
  
  int      spiErr    = SPI_GENERAL_ERR;

  if (cpyErrIn2Out(nErrIn, pnErrOut))  goto end;           // check for existing error

  ARM_SPI_CloseReference (
                        (int)  pReferenceIn,	           // ?? should be (int*) but it is (int)
                              &spiErr);

end:
  if (!nErrIn) {
    cnvErrSPI2LV (spiErr, pnErrOut);                       // convert SPI error
  }
}

#endif // _Include_ARM_SPI_Functions
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
