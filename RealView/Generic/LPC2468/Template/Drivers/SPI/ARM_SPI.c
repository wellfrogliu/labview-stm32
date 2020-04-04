/*----------------------------------------------------------------------------
 * Name:    ARM_SPI.c
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

#include <RTL.h>

 
#define LPC24XX_SAMPLE_DRIVER        1
#define SPI_INTERFACE_IN_USE(spiIf) ((SPI_MIN_INTERFACE <= spiIfNr) && (SPI_MAX_INTERFACE >= spiIfNr)) 
 
 
/* USER CODE =====================================================================*/
/* Add the correct header file for your selected CPU */ 
#if (LPC24XX_SAMPLE_DRIVER == 1)
  /* <LPC2468 Example Code> */    
#include <LPC23xx.H>                             /* LPC23xx definitions */
#else
#include <???.h>
#endif
/* END USER CODE ================================================================ */


#include "ARM_SPI.h"

/* USER CODE =====================================================================*/
/* Set minimum and maximum port numbers available 
   NOTE: Setting this to only the required ports will reduce RAM and code size    */   
#if (LPC24XX_SAMPLE_DRIVER == 1)
  /* <LPC2468 Example Code> */    
#define SPI_MIN_INTERFACE   0
#define SPI_MAX_INTERFACE   0
#else
#define SPI_MIN_INTERFACE   ???
#define SPI_MAX_INTERFACE   ???
#endif
/* END USER CODE ================================================================ */


/*----------------------------------------------------------------------------
  SPI default sttings
 *---------------------------------------------------------------------------*/
#ifndef SPI_CLK
#define SPI_CLK  12000000UL          /* 48MHz / 4 */
#endif  

#define SPI_CLOCK_RATE          1000                     // in kHz
#define SPI_CLOCK_POLARITY      SPI_CLOCK_POLARITY_IDLELOW
#define SPI_CLOCK_PHASE         SPI_CLOCK_PHASE_FIRSTEDGE

#define SPI_REFERENCE_CLEARED   0
#define SPI_REFERENCE_CREATED   1



/*----------------------------------------------------------------------------
  structure to administrate SPI
 *---------------------------------------------------------------------------*/
typedef struct {
                    int   reference;       // = portNumber + 1
           unsigned int   status;          // opened or closed
           unsigned int   chipSelect;
           unsigned int   clockRate;
           unsigned int   clockPolarity;
           unsigned int   clockPhase;
           
/* USER CODE =====================================================================*/
/* Add the correct defines for the SPI registers. Also add 
   any other port specific variables that are required */
#if (LPC24XX_SAMPLE_DRIVER == 1)
  /* <LPC2468 Example Code> */   
  volatile unsigned long *spsr;
  volatile unsigned long *spdr;
#else
  /* <your code>  */
#endif  
/* END USER CODE ================================================================ */
  
} SPI_INFO;

static SPI_INFO spi_Dev[SPI_MAX_INTERFACE - SPI_MIN_INTERFACE + 1] = {
#if SPI_INTERFACE_IN_USE(0)
  { 1, 
    SPI_REFERENCE_CLEARED, 0, SPI_CLOCK_RATE, SPI_CLOCK_POLARITY_IDLELOW, SPI_CLOCK_PHASE_FIRSTEDGE, 

/* USER CODE =====================================================================*/
/* Add the correct register addresses within your first SPI port. Also add 
   any other variable initialisation required */
#if (LPC24XX_SAMPLE_DRIVER == 1)
  /* <LPC2468 Example Code> */   
    (volatile unsigned long *)(SPI0_BASE_ADDR + 0x04),   // S0SPSR
    (volatile unsigned long *)(SPI0_BASE_ADDR + 0x08),   // S0SPDR
#else
  /* <your code>  */
#endif    
/* END USER CODE ================================================================ */    
    
  },
#endif /* SPI_INTERFACE_IN_USE(0) */

/* USER CODE =====================================================================*/
/* Add / remove code for any extra port initialisation */   
/* END USER CODE ================================================================ */  
};

/*----------------------------------------------------------------------------
  Function Prototypes
 *---------------------------------------------------------------------------*/
static void          spiWaitForComplete(SPI_INFO *pSpiDev);
static void          spiSendByte       (SPI_INFO *pSpiDev, unsigned char data);
static unsigned char spiGetReceivedByte(SPI_INFO *pSpiDev);
static unsigned int  spicGetStatus     (SPI_INFO *pSpiDev);

/*----------------------------------------------------------------------------
  SPI create a reference
 *---------------------------------------------------------------------------*/
void ARM_SPI_CreateConfigurationReference (
                         int    chipSelect,
                unsigned int    clockRate,
                unsigned int    clockPolarity,
                unsigned int    clockPhase,
                         int*  pReferenceOut,
                         int*  pErrOut) {

  if ( (chipSelect < SPI_MIN_INTERFACE) ||
       (chipSelect > SPI_MAX_INTERFACE)   ) {
    *pErrOut = SPI_CHIPSELECT_NOTKNOWN_ERR;              // check the device number
    return;
  }

  if ( (clockPolarity != SPI_CLOCK_POLARITY_IDLELOW ) &&
       (clockPolarity != SPI_CLOCK_POLARITY_IDLEHIGH)   ){ 
    *pErrOut = SPI_CLKPOLARITY_NOTSUPPORTED_ERR;         // check the clock polarity
    return;
  }

  if ( (clockPhase != SPI_CLOCK_POLARITY_IDLELOW ) &&
       (clockPhase != SPI_CLOCK_POLARITY_IDLEHIGH)   ){ 
    *pErrOut = SPI_CLKPHASE_NOTSUPPORTED_ERR;            // check the clock polarity
    return;
  }

  *pErrOut = SPI_OK;

  switch (chipSelect) {
    #if SPI_INTERFACE_IN_USE(0)
    case 0:
      /* USER CODE =====================================================================*/
      /* Add code to initialise the pinout hardware forSPI 0 */
      #if (LPC24XX_SAMPLE_DRIVER == 1)
      /* <LPC2468 Example Code> */     
      PCONP   |=  (1 << 8);                              // enable power for SPI
      PINSEL0 &= ~0xC0000000;                            // clear pin settings
      PINSEL1 &= ~0x0000003F;                            // clear pin settings
      PINSEL0 |=  0xC0000000;                            // P0.15 (SCK) is SPI pin
      PINSEL1 |=  0x0000003F;                            // P0.16..18 (MISO, MOSI, SSEL) are SPI pins.

      S0SPCCR = SPI_CLK / (clockRate * 1000UL);          // set clock rate
      S0SPCR =   (( clockPhase    & 1) << 3)             // set clock phase
             | ((~clockPolarity & 1) << 4)               // set clock polarity
             | (1 << 5)                                  // set master mode
             | (0 << 6)                                  // MSB trans. first         
             | (4 | (8 << 8));                           // set 8 bit data
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */  
      break;
    #endif /* SPI_INTERFACE_IN_USE(0) */
    
    default:
      ; // Should never happen (protected by port check above)
  };

  spi_Dev[chipSelect - SPI_MIN_INTERFACE].status        = SPI_REFERENCE_CREATED;
  spi_Dev[chipSelect - SPI_MIN_INTERFACE].chipSelect    = chipSelect;
  spi_Dev[chipSelect - SPI_MIN_INTERFACE].clockRate     = clockRate;
  spi_Dev[chipSelect - SPI_MIN_INTERFACE].clockPolarity = clockPolarity;
  spi_Dev[chipSelect - SPI_MIN_INTERFACE].clockPhase    = clockPhase;
  *pReferenceOut = spi_Dev[chipSelect - SPI_MIN_INTERFACE].reference;

}

/*----------------------------------------------------------------------------
  SPI perform a write/read access
 *---------------------------------------------------------------------------*/
void ARM_SPI_WriteRead (
                         int   nReferenceIn,
                    T_bArray*  pDataWr,
                unsigned int   bytesToRead,
                    T_bArray*  pDataRd,
                         int*  pErrOut) {

  SPI_INFO     *pSpiDev;

  unsigned int spiStatus;
  unsigned int i     = 0;
  unsigned int wrLen = pDataWr->len;
  unsigned int wr_reLen = bytesToRead;
  
  if(wrLen > wr_reLen)
  {
	wr_reLen = wrLen;
  }

  nReferenceIn -= 1;                                     // reference is starting from 1

  if ( (nReferenceIn < (SPI_MIN_INTERFACE)) ||           // check the I2C reference
       (nReferenceIn > (SPI_MAX_INTERFACE))   ) {
    *pErrOut = SPI_REFERENCE_NOTKNOWN_ERR;
    return;
  }

  *pErrOut = SPI_OK;
  pSpiDev = &spi_Dev[(nReferenceIn - SPI_MIN_INTERFACE)];

  if (pSpiDev->status != SPI_REFERENCE_CREATED) {
    *pErrOut = SPI_REFERENCE_NOTCREATED_ERR;
    return;
  }

  spiStatus = spicGetStatus(pSpiDev);                    // read the status
  while(wr_reLen) {                                         // send data
    if(i < wrLen)
	{
    spiSendByte(pSpiDev, pDataWr->data[i]);              // transmit byte
	}
	else
	{
		spiSendByte(pSpiDev, (unsigned char) 0);             //send dummy data
	}
    spiWaitForComplete(pSpiDev);                         // wait until transfer complete
    spiStatus = spicGetStatus(pSpiDev);                  // read the status
    if (spiStatus != (1<<7)) {                           // check for errors
      *pErrOut = SPI_DEVICE_ERR;
      return;
    }
	if(i < bytesToRead)
	{
    pDataRd->data[i] = spiGetReceivedByte(pSpiDev);      // read received byte
		pDataRd->len = i;                                    // number of bytes read
	}
	else
	{
		spiGetReceivedByte(pSpiDev);      // read and toss received byte (to prevent overflow)
	}
    wr_reLen--;
    i++;
  }

}


/*----------------------------------------------------------------------------
  SPI close a  reference
 *---------------------------------------------------------------------------*/
void ARM_SPI_CloseReference (
                         int   nReferenceIn,
                         int*  pErrOut) {
  SPI_INFO *pSpiDev;

  nReferenceIn -= 1;                                     // reference is starting from 1

  if ( (nReferenceIn < (SPI_MIN_INTERFACE)) ||           // check the SPI reference
       (nReferenceIn > (SPI_MAX_INTERFACE))   ) {
    *pErrOut = SPI_REFERENCE_NOTKNOWN_ERR;
    return;
  }

  *pErrOut = SPI_OK;
  pSpiDev = &spi_Dev[(nReferenceIn - SPI_MIN_INTERFACE)];

  switch (pSpiDev->reference) {
    #if SPI_INTERFACE_IN_USE(0)
    case 0:
      /* USER CODE =====================================================================*/
      /* Uninitialise the pinout hardware for UART 0 */
      #if (LPC24XX_SAMPLE_DRIVER == 1)
      /* <LPC2468 Example Code> */     
      PCONP   &= ~(1 << 8);                              // disable power contol for SPI0
      PINSEL0 &= ~0xC0000000;  /* clear pin settings */
      PINSEL1 &= ~0x0000003F;  /* clear pin settings */
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */  
      break;
    #endif /* SPI_INTERFACE_IN_USE(0) */
    
    default:
      ; // Should never happen (protected by reference check above)  
  }

  /* USER CODE =====================================================================*/
  /* Disable the interrupt in the VIC and UART controllers */
  #if (LPC24XX_SAMPLE_DRIVER == 1)
  /* <LPC2468 Example Code> */     
  #else
  /* <your code>  */
  #endif
  /* END USER CODE ================================================================ */      

  pSpiDev->status   = SPI_REFERENCE_CLEARED;
  
}


/*----------------------------------------------------------------------------
  SPI lowlevel functions
 *---------------------------------------------------------------------------*/
static void spiWaitForComplete(SPI_INFO *pSpiDev) {
  while ((*pSpiDev->spsr & (1<<7)) == 0);                // wait until transfer complete
}

static void spiSendByte(SPI_INFO *pSpiDev, unsigned char data) {
  *pSpiDev->spdr = data;                                 // save data into data register
}

static unsigned char spiGetReceivedByte(SPI_INFO *pSpiDev) {
  return (unsigned char)*pSpiDev->spdr;                  // return received data byte
}

static unsigned int spicGetStatus(SPI_INFO *pSpiDev) {
  return *pSpiDev->spsr;                                 // return satus byte
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

