/*----------------------------------------------------------------------------
 * Name:    ARM_I2C.c
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

#include <RTL.h>


#define LPC24XX_SAMPLE_DRIVER        1
#define I2C_INTERFACE_IN_USE(i2cIf) ((I2C_MIN_INTERFACE <= i2cIfNr) && (I2C_MAX_INTERFACE >= i2cIfNr)) 
 
 
/* USER CODE =====================================================================*/
/* Add the correct header file for your selected CPU */ 
#if (LPC24XX_SAMPLE_DRIVER == 1)
  /* <LPC2468 Example Code> */    
#include <LPC23xx.H>                             /* LPC23xx definitions */
#else
#include <???.h>
#endif
/* END USER CODE ================================================================ */


#include "ARM_I2C.h"

/* USER CODE =====================================================================*/
/* Set minimum and maximum port numbers available 
   NOTE: Setting this to only the required ports will reduce RAM and code size    */   
#if (LPC24XX_SAMPLE_DRIVER == 1)
  /* <LPC2468 Example Code> */    
#define I2C_MIN_INTERFACE   0
#define I2C_MAX_INTERFACE   2
#else
#define I2C_MIN_INTERFACE   ???
#define I2C_MAX_INTERFACE   ???
#endif
/* END USER CODE ================================================================ */

/*----------------------------------------------------------------------------
  I2C default sttings
 *---------------------------------------------------------------------------*/
#ifndef I2C_CLK
#define I2C_CLK  12000000UL                              // 48MHz / 4
#endif  

#define I2C_CLOCK_RATE                 100               // in kHz
#define I2C_SLAVE_ADDRESS             0x50
#define I2C_ADDRESS_SIZE          I2C_7BIT

#define I2C_REFERENCE_CLEARED            0
#define I2C_REFERENCE_CREATED            1


/*----------------------------------------------------------------------------
  structure to administrate I2C
 *---------------------------------------------------------------------------*/
typedef struct {
                    int   reference;                     // = portNumber + 1
           unsigned int   status;                        // opened or closed
                    int   interface;                     // I2c interface
           unsigned int   slaveAddress;                  // slave device address
           unsigned int   addressSize;                   // 7bit or 10bit address   
           unsigned int   clockRate;                     // clock rate in kHz
           
/* USER CODE =====================================================================*/
/* Add the correct defines for the I2C registers. Also add 
   any other port specific variables that are required */
#if (LPC24XX_SAMPLE_DRIVER == 1)
  /* <LPC2468 Example Code> */   
  volatile unsigned long *i2conset;
  volatile unsigned long *i2stat;
  volatile unsigned long *i2dat;
  volatile unsigned long *i2sclh;
  volatile unsigned long *i2scll;
  volatile unsigned long *i2conclr;
#else
  /* <your code>  */
#endif  
/* END USER CODE ================================================================ */
  
} I2C_INFO;

static I2C_INFO i2c_Dev[I2C_MAX_INTERFACE - I2C_MIN_INTERFACE + 1] = {
#if I2C_INTERFACE_IN_USE(0)
  { 1, 
    I2C_REFERENCE_CLEARED, 0, I2C_SLAVE_ADDRESS, I2C_ADDRESS_SIZE, I2C_CLOCK_RATE, 

/* USER CODE =====================================================================*/
/* Add the correct register addresses within your first I2C port. Also add 
   any other variable initialisation required */
#if (LPC24XX_SAMPLE_DRIVER == 1)
  /* <LPC2468 Example Code> */    
    (volatile unsigned long *)(I2C0_BASE_ADDR + 0x00),   // I20CONSET
    (volatile unsigned long *)(I2C0_BASE_ADDR + 0x04),   // I20STAT
    (volatile unsigned long *)(I2C0_BASE_ADDR + 0x08),   // I20DAT
    (volatile unsigned long *)(I2C0_BASE_ADDR + 0x10),   // I20SCLH
    (volatile unsigned long *)(I2C0_BASE_ADDR + 0x14),   // I20SCLL
    (volatile unsigned long *)(I2C0_BASE_ADDR + 0x18)    // I20CONCLR
#else
  /* <your code>  */
#endif    
/* END USER CODE ================================================================ */    
    
  },
#endif /* I2C_INTERFACE_IN_USE(0) */

#if I2C_INTERFACE_IN_USE(1) 
  { 2, 
    I2C_REFERENCE_CLEARED, 0, I2C_SLAVE_ADDRESS, I2C_ADDRESS_SIZE, I2C_CLOCK_RATE, 

/* USER CODE =====================================================================*/
/* Add the correct register addresses within your first I2C port. Also add 
   any other variable initialisation required */
#if (LPC24XX_SAMPLE_DRIVER == 1)
  /* <LPC2468 Example Code> */    
    (volatile unsigned long *)(I2C1_BASE_ADDR + 0x00),   // I21CONSET
    (volatile unsigned long *)(I2C1_BASE_ADDR + 0x04),   // I21STAT
    (volatile unsigned long *)(I2C1_BASE_ADDR + 0x08),   // I21DAT
    (volatile unsigned long *)(I2C1_BASE_ADDR + 0x10),   // I21SCLH
    (volatile unsigned long *)(I2C1_BASE_ADDR + 0x14),   // I21SCLL
    (volatile unsigned long *)(I2C1_BASE_ADDR + 0x18)    // I21CONCLR
#else
  /* <your code>  */
#endif    
/* END USER CODE ================================================================ */    
    
  },
#endif /* I2C_INTERFACE_IN_USE(1) */

#if I2C_INTERFACE_IN_USE(2) 
  { 3, 
    I2C_REFERENCE_CLEARED, 0, I2C_SLAVE_ADDRESS, I2C_ADDRESS_SIZE, I2C_CLOCK_RATE, 

/* USER CODE =====================================================================*/
/* Add the correct register addresses within your first I2C port. Also add 
   any other variable initialisation required */
#if (LPC24XX_SAMPLE_DRIVER == 1)
  /* <LPC2468 Example Code> */    
    (volatile unsigned long *)(I2C2_BASE_ADDR + 0x00),   // I22CONSET
    (volatile unsigned long *)(I2C2_BASE_ADDR + 0x04),   // I22STAT
    (volatile unsigned long *)(I2C2_BASE_ADDR + 0x08),   // I22DAT
    (volatile unsigned long *)(I2C2_BASE_ADDR + 0x10),   // I22SCLH
    (volatile unsigned long *)(I2C2_BASE_ADDR + 0x14),   // I22SCLL
    (volatile unsigned long *)(I2C2_BASE_ADDR + 0x18)    // I22CONCLR
#else
  /* <your code>  */
#endif    
/* END USER CODE ================================================================ */    
    
  },
#endif /* I2C_INTERFACE_IN_USE(2) */

/* USER CODE =====================================================================*/
/* Add / remove code for any extra port initialisation */   
/* END USER CODE ================================================================ */  
};



/*----------------------------------------------------------------------------
  I2C Bit definitions
 *---------------------------------------------------------------------------*/
#define I2C_AA      0x00000004
#define I2C_SI      0x00000008
#define I2C_STO     0x00000010
#define I2C_STA     0x00000020
#define I2C_I2EN    0x00000040

/*----------------------------------------------------------------------------
  I2C Status register definitions
 *---------------------------------------------------------------------------*/
/* Master */
#define I2CSR_START              0x08
#define I2CSR_REP_START          0x10
/* Master Transmitter */
#define I2CSR_MT_SLA_ACK         0x18
#define I2CSR_MT_SLA_NACK        0x20
#define I2CSR_MT_DATA_ACK        0x28
#define I2CSR_MT_DATA_NACK       0x30
#define I2CSR_MT_ARB_LOST        0x38
/* Master Receiver */
#define I2CSR_MR_ARB_LOST        0x38
#define I2CSR_MR_SLA_ACK         0x40
#define I2CSR_MR_SLA_NACK        0x48
#define I2CSR_MR_DATA_ACK        0x50
#define I2CSR_MR_DATA_NACK       0x58

#define I2CSR_NO_INFO            0xF8

// return values
#define I2C_ERROR_NODEV          0x01


/*----------------------------------------------------------------------------
  Function Prototypes
 *---------------------------------------------------------------------------*/
static void          i2cSendStart      (I2C_INFO *pI2cDev);
static void          i2cSendStop       (I2C_INFO *pI2cDev);
static void          i2cWaitForComplete(I2C_INFO *pI2cDev);
static void          i2cSendByte       (I2C_INFO *pI2cDev, unsigned char data);
static void          i2cReceiveByte    (I2C_INFO *pI2cDev, unsigned char ackFlag);
static unsigned char i2cGetReceivedByte(I2C_INFO *pI2cDev);
static unsigned char i2cGetStatus      (I2C_INFO *pI2cDev);
static void          i2cDelay          (unsigned int);          // delay in ms


/*----------------------------------------------------------------------------
  I2C create a reference
 *---------------------------------------------------------------------------*/
void ARM_I2C_CreateConfigurationReference (
                         int    interface,
                unsigned int    slaveAddress,
                unsigned int    addressSize,   
                unsigned int    clockRate,
                         int*  pReferenceOut,
                         int*  pErrOut) {

  I2C_INFO      *pI2cDev;
  unsigned short scl;

  if ( (interface < I2C_MIN_INTERFACE) ||                // check the I2C interface
       (interface > I2C_MAX_INTERFACE)   ) {
    *pErrOut = I2C_INTERFACE_NOTKNOWN_ERR;
    return;
  }

  if ( (addressSize != I2C_7BIT ) &&
       (addressSize != I2C_10BIT)   ){                   // check the I2C address size
    *pErrOut = I2C_ADDRESSSIZE_NOTSUPPORTED_ERR;
    return;
  }

  *pErrOut = I2C_OK;
  pI2cDev = &i2c_Dev[(interface - I2C_MIN_INTERFACE)];

  switch (interface) {
    #if I2C_INTERFACE_IN_USE(0)
    case 0:
      /* USER CODE =====================================================================*/
      /* Add code to initialise the pinout hardware for I2C 0 */
      #if (LPC24XX_SAMPLE_DRIVER == 1)
      /* <LPC2468 Example Code> */     
      PCONP   |=  (1 << 7);                              // enable power contol for I2C0
      
      PINSEL1  &= ~0x03C00000;                           // I2C0 pins SDA (P0.27) and SCL (P0.28)
      PINSEL1  |=  0x01400000;

      scl = I2C_CLK / (clockRate * 1000UL) / 2;          // calculate clock speed      
      *pI2cDev->i2scll   =  scl;                         // Setup I2C clock speed
      *pI2cDev->i2sclh   =  scl;

      *pI2cDev->i2conclr = I2C_AA  | I2C_SI |            // Clear I2C state machine 
                         I2C_STA | I2C_I2EN;

      *pI2cDev->i2conset =  I2C_I2EN;                    // enable I2C0
    #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */  
      break;
    #endif /* I2C_INTERFACE_IN_USE(0) */
    
    #if I2C_INTERFACE_IN_USE(1) 
    case 1:
      /* USER CODE =====================================================================*/
      /* Add code to initialise the pinout hardware for I2C 0 */
      #if (LPC24XX_SAMPLE_DRIVER == 1)
      /* <LPC2468 Example Code> */     
      PCONP   |=  (1 << 19);                             // enable power contol for I2C1
      
      PINSEL1  &= ~0x0000000F;                           // I2C1 pins SDA (P0.0) and SCL (P0.1)
      PINSEL1  |=  0x0000000F;

      scl = I2C_CLK / (clockRate * 1000UL) / 2;          // calculate clock speed      
      *pI2cDev->i2scll   =  scl;                         // Setup I2C clock speed
      *pI2cDev->i2sclh   =  scl;

      *pI2cDev->i2conclr = I2C_AA  | I2C_SI |            // Clear I2C state machine 
                         I2C_STA | I2C_I2EN;

      *pI2cDev->i2conset =  I2C_I2EN;                    // enable I2C1

    #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */  
      break;
    #endif /* I2C_INTERFACE_IN_USE(1) */

    #if I2C_INTERFACE_IN_USE(2) 
    case 2:
      /* USER CODE =====================================================================*/
      /* Add code to initialise the pinout hardware for I2C 0 */
      #if (LPC24XX_SAMPLE_DRIVER == 1)
      /* <LPC2468 Example Code> */     
      PCONP   |=  (1 << 26);                             // enable power contol for I2C2
      
      PINSEL1  &= ~0x00F00000;                           // I2C2 pins SDA (P0.10) and SCL (P0.11)
      PINSEL1  |=  0x00A00000;

      scl = I2C_CLK / (clockRate * 1000UL) / 2;          // calculate clock speed      
      *pI2cDev->i2scll   =  scl;                         // Setup I2C clock speed
      *pI2cDev->i2sclh   =  scl;

      *pI2cDev->i2conclr = I2C_AA  | I2C_SI |            // Clear I2C state machine 
                         I2C_STA | I2C_I2EN;

      *pI2cDev->i2conset =  I2C_I2EN;                    // enable I2C2
    #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */  
      break;
    #endif /* I2C_INTERFACE_IN_USE(1) */

    default:
      ; // Should never happen (protected by port check above)
  };

  pI2cDev->status        = I2C_REFERENCE_CREATED;
  pI2cDev->interface     = interface;
  pI2cDev->slaveAddress  = slaveAddress & 0x3FF;         // maximum is 10bit
  pI2cDev->addressSize   = addressSize;   
  pI2cDev->clockRate     = clockRate;

  *pReferenceOut = pI2cDev->reference;

}

/*----------------------------------------------------------------------------
  I2C perform a write/read access
 *---------------------------------------------------------------------------*/
void ARM_I2C_WriteRead (
                         int   nReferenceIn,
                    T_bArray*  pDataWr,
                unsigned int   bytesToRead,
                    T_bArray*  pDataRd,
                         int*  pErrOut) {

  I2C_INFO     *pI2cDev;
  unsigned char i2cSlaveAdrBH, i2cSlaveAdrBL;            // High / Low part of slave address

  unsigned int i     = 0;
  unsigned int wrLen = pDataWr->len;
  unsigned int rdLen = bytesToRead;

  nReferenceIn -= 1;                                     // reference is starting from 1

  if ( (nReferenceIn < (I2C_MIN_INTERFACE)) ||           // check the I2C reference
       (nReferenceIn > (I2C_MAX_INTERFACE))   ) {
    *pErrOut = I2C_REFERENCE_NOTKNOWN_ERR;
    return;
  }

  *pErrOut = I2C_OK;
  pI2cDev = &i2c_Dev[(nReferenceIn - I2C_MIN_INTERFACE)];

  if (pI2cDev->status != I2C_REFERENCE_CREATED) {
    *pErrOut = I2C_REFERENCE_NOTCREATED_ERR;
    return;
  }

  i2cSlaveAdrBH = (pI2cDev->slaveAddress >> 8) & 0x03;   // high part of slave address
  i2cSlaveAdrBL = (pI2cDev->slaveAddress     ) & 0xFF;   // low  part of slave address

  // first write data
  if(wrLen > 0)
  {
  i2cSendStart(pI2cDev);                                 // send start condition
  i2cWaitForComplete(pI2cDev);
  *pI2cDev->i2conclr = I2C_STA;

  if (pI2cDev->addressSize == I2C_7BIT) {
    i2cSendByte(pI2cDev, ((i2cSlaveAdrBL << 1) & 0xFE));        // send device address with write
    i2cWaitForComplete(pI2cDev);
  }
  else {
    i2cSendByte(pI2cDev, ((i2cSlaveAdrBH << 1) | 0xF0)); // send device address high part with write
    i2cWaitForComplete(pI2cDev);
    i2cSendByte(pI2cDev, i2cSlaveAdrBL);                 // send device address low  part
    i2cWaitForComplete(pI2cDev);
  }

  if (i2cGetStatus(pI2cDev) == I2CSR_MT_SLA_ACK)         // check if device is present and live
  {
    while(wrLen) {                                       // send data
      i2cSendByte(pI2cDev, pDataWr->data[i++]);
      i2cWaitForComplete(pI2cDev);
      wrLen--;
    }
  }
  else {                                                 // device did not ACK it's address, data will not be transferred
    i2cSendStop(pI2cDev);                                // transmit stop condition
    *pErrOut = I2C_DEVICE_ERR;
    return;
  }

	
  if (bytesToRead == 0) {
    i2cSendStop(pI2cDev);                                // transmit stop condition

    // needs optimization: wait only if data is written and not only address
    i2cDelay (10);                                       // wait until data is written

    return;
  }
	
  }

  // second part receive data

  i2cSendStart(pI2cDev);                                 // send start condition
  i2cWaitForComplete(pI2cDev);
  *pI2cDev->i2conclr = I2C_STA;

  if (pI2cDev->addressSize == I2C_7BIT) {
    i2cSendByte(pI2cDev, ((i2cSlaveAdrBL << 1) | 0x01));        // send device address with read
    i2cWaitForComplete(pI2cDev);
  }
  else {
	i2cSendByte(pI2cDev, ((i2cSlaveAdrBH << 1) | 0xF1)); // send device address high part with read
    i2cWaitForComplete(pI2cDev);
    i2cSendByte(pI2cDev, i2cSlaveAdrBL);                 // send device address low  part
    i2cWaitForComplete(pI2cDev);
  }

  if (i2cGetStatus(pI2cDev) == I2CSR_MR_SLA_ACK)         // check if device is present and live
  {
    i = 0;
    while(rdLen > 1) {                                   // accept receive data and ack it
      i2cReceiveByte(pI2cDev, 1);                        // acknowledeg received byte
      i2cWaitForComplete(pI2cDev);
      pDataRd->data[i++] = i2cGetReceivedByte(pI2cDev);  // read received byte
      rdLen--;
    }

    i2cReceiveByte(pI2cDev, 0);                          // accept receive data and nack it (last-byte signal)
    i2cWaitForComplete(pI2cDev);
    pDataRd->data[i++] = i2cGetReceivedByte(pI2cDev);    // read received byte
    pDataRd->len = i;                                    // number of bytes red
  }
  else {                                                 // device did not ACK it's address, data will not be transferred
    i2cSendStop(pI2cDev);                                // transmit stop condition
    *pErrOut = I2C_DEVICE_ERR;
    return;
  }

  i2cSendStop(pI2cDev);                                  // transmit stop condition

}


/*----------------------------------------------------------------------------
  I2C close a  reference
 *---------------------------------------------------------------------------*/
void ARM_I2C_CloseReference (
                         int   nReferenceIn,
                         int*  pErrOut) {
  I2C_INFO *pI2cDev;

  nReferenceIn -= 1;                                     // reference is starting from 1

  if ( (nReferenceIn < (I2C_MIN_INTERFACE)) ||           // check the I2C reference
       (nReferenceIn > (I2C_MAX_INTERFACE))   ) {
    *pErrOut = I2C_REFERENCE_NOTKNOWN_ERR;
    return;
  }

  *pErrOut = I2C_OK;
  pI2cDev = &i2c_Dev[(nReferenceIn - I2C_MIN_INTERFACE)];

  switch (pI2cDev->reference) {
    #if I2C_INTERFACE_IN_USE(0)
    case 0:
      /* USER CODE =====================================================================*/
      /* Uninitialise the pinout hardware for UART 0 */
      #if (LPC24XX_SAMPLE_DRIVER == 1)
      /* <LPC2468 Example Code> */     
      PCONP   &= ~(1 << 7);                              // disable power contol for I2C0
      PINSEL1  &= ~0x03C00000;                           // clear pin settings
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */  
      break;
    #endif /* I2C_INTERFACE_IN_USE(0) */
    
    #if I2C_INTERFACE_IN_USE(1) 
    case 1:
      /* USER CODE =====================================================================*/
      /* Uninitialise the pinout hardware for UART 0 */
      #if (LPC24XX_SAMPLE_DRIVER == 1)
      /* <LPC2468 Example Code> */     
      PCONP   &= ~(1 << 19);                              // disable power contol for I2C1
      PINSEL1  &= ~0x0000000F;                           // clear pin settings
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */  
      break;
    #endif /* I2C_INTERFACE_IN_USE(1) */

    #if I2C_INTERFACE_IN_USE(2) 
    case 2:
      /* USER CODE =====================================================================*/
      /* Uninitialise the pinout hardware for UART 0 */
      #if (LPC24XX_SAMPLE_DRIVER == 1)
      /* <LPC2468 Example Code> */     
      PCONP   &= ~(1 << 26);                             // disable power contol for I2C2
      PINSEL1  &= ~0x00F00000;                           // clear pin settings
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */  
      break;
    #endif /* I2C_INTERFACE_IN_USE(2) */

    default:
      ; // Should never happen (protected by reference check above)  
  }

  /* USER CODE =====================================================================*/
  #if (LPC24XX_SAMPLE_DRIVER == 1)
  /* <LPC2468 Example Code> */     
  #else
  /* <your code>  */
  #endif
  /* END USER CODE ================================================================ */      

  pI2cDev->status = I2C_REFERENCE_CLEARED;
  
}


/*----------------------------------------------------------------------------
  I2C lowlevel functions
 *---------------------------------------------------------------------------*/
static void i2cSendStart(I2C_INFO *pI2cDev) {
  *pI2cDev->i2conclr = I2C_SI;                           // clear SI bit to begin transfer
  *pI2cDev->i2conset = I2C_STA;                          // transmit start condition
}

static void i2cSendStop(I2C_INFO *pI2cDev) {
  *pI2cDev->i2conset = I2C_STO;                          // transmit stop condition
  *pI2cDev->i2conclr = I2C_SI;                           // clear SI bit to begin transfer
}

static void i2cWaitForComplete(I2C_INFO *pI2cDev) {
  while (!(*pI2cDev->i2conset & I2C_SI));
}

static void i2cSendByte(I2C_INFO *pI2cDev, unsigned char data) {
  *pI2cDev->i2dat    = data;                             // save data into data register
  *pI2cDev->i2conclr = I2C_SI;                           // clear SI bit to begin transfer
}

static void i2cReceiveByte(I2C_INFO *pI2cDev, unsigned char ackFlag) {
  if (ackFlag) {                                         // begin receive over i2c
    *pI2cDev->i2conset = I2C_AA;                         // ackFlag = TRUE: ACK the recevied data
  }
  else {
    *pI2cDev->i2conclr = I2C_AA;                         // ackFlag = FALSE: NACK the recevied data
  }
  *pI2cDev->i2conclr = I2C_SI;                           // clear SI bit to begin transfer
}

static unsigned char i2cGetReceivedByte(I2C_INFO *pI2cDev) {
  return *pI2cDev->i2dat;                                // return received data byte
}

static unsigned char i2cGetStatus(I2C_INFO *pI2cDev) {
  return *pI2cDev->i2stat;                               // return satus byte
}

static void i2cDelay (unsigned int ms) {
  os_dly_wait (ms);                                      // delay in msec
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/







