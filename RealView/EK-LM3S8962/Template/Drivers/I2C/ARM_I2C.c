/*----------------------------------------------------------------------------
 * Name:    ARM_I2C.c
 * Purpose: LM3Sxxxx I2C driver
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
#include "ARM_I2C.h"

#include <LM3Sxxxx.h>


#define I2C_INTERFACE_IN_USE(i2cIf) ((I2C_MIN_INTERFACE <= i2cIfNr) && (I2C_MAX_INTERFACE >= i2cIfNr))

#define I2C_MIN_INTERFACE   0
#define I2C_MAX_INTERFACE   0

/*----------------------------------------------------------------------------
  I2C default sttings
 *---------------------------------------------------------------------------*/
#define I2C_CLOCK_RATE                 100               // in kHz
#define I2C_SLAVE_ADDRESS             0x50
#define I2C_ADDRESS_SIZE          I2C_7BIT

#define I2C_REFERENCE_CLEARED            0
#define I2C_REFERENCE_CREATED            1


/*----------------------------------------------------------------------------
  structure to administrate I2C
 *---------------------------------------------------------------------------*/
typedef struct {
                    int   reference;            // = portNumber + 1
           unsigned int   status;               // opened or closed
                    int   interface;            // I2c interface
           unsigned int   slaveAddress;         // slave device address
           unsigned int   addressSize;          // 7bit or 10bit address
           unsigned int   clockRate;            // clock rate in kHz

           unsigned long  ulI2CPeriph;          // I2C peripheral
           unsigned long  ulI2CBase;            // base addr of I2C
           unsigned long  ulGPIOPeriph;         // GPIO needed for I2C
           unsigned long  ulGPIOPortBase;       // base addr of GPIO
           unsigned long  ulGPIOPins;           // pins needed for I2C
} I2C_INFO;

static I2C_INFO i2c_Dev[I2C_MAX_INTERFACE - I2C_MIN_INTERFACE + 1] = {
#if I2C_INTERFACE_IN_USE(0)
  { 1,
    I2C_REFERENCE_CLEARED, 0, I2C_SLAVE_ADDRESS, I2C_ADDRESS_SIZE, I2C_CLOCK_RATE,

    SYSCTL_PERIPH_I2C0, I2C0_MASTER_BASE,
    SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE, (GPIO_PIN_2 | GPIO_PIN_3)
  },
#endif /* I2C_INTERFACE_IN_USE(0) */
};


/*----------------------------------------------------------------------------
  Function Prototypes
 *---------------------------------------------------------------------------*/
static void          i2cWaitForComplete(I2C_INFO *pI2cDev);
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

  // Check the interface
  if ( (interface < I2C_MIN_INTERFACE) ||
       (interface > I2C_MAX_INTERFACE)   ) {
    *pErrOut = I2C_INTERFACE_NOTKNOWN_ERR;
    return;
  }

  // check the address size
  if (addressSize != I2C_7BIT ){
    *pErrOut = I2C_ADDRESSSIZE_NOTSUPPORTED_ERR;
    return;
  }

  // set a success error code
  // get a pointer to the I2C instance
  *pErrOut = I2C_OK;
  pI2cDev = &i2c_Dev[(interface - I2C_MIN_INTERFACE)];

  // Enable the I2C and associated GPIO peripherals.
  // Configure the GPIO pins for I2C use.
  SysCtlPeripheralEnable(pI2cDev->ulI2CPeriph);
  SysCtlPeripheralEnable(pI2cDev->ulGPIOPeriph);
  GPIOPinTypeI2C(pI2cDev->ulGPIOPortBase, pI2cDev->ulGPIOPins);

  // Initialize the I2C master
  I2CMasterInitExpClk(pI2cDev->ulI2CBase, SysCtlClockGet(),
                      clockRate == 400 ? 1 : 0);
  I2CMasterEnable(pI2cDev->ulI2CBase);

  // save the instance data
  pI2cDev->status        = I2C_REFERENCE_CREATED;
  pI2cDev->interface     = interface;
  pI2cDev->slaveAddress  = slaveAddress & 0x7F;        // maximum is 7bit
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

  unsigned int i     = 0;
  unsigned int wrLen = pDataWr->len;
  unsigned int rdLen = bytesToRead;
  unsigned int waitCount;

  nReferenceIn -= 1;                // reference is starting from 1

  // check the I2C reference
  if ( (nReferenceIn < (I2C_MIN_INTERFACE)) ||
       (nReferenceIn > (I2C_MAX_INTERFACE))   ) {
    *pErrOut = I2C_REFERENCE_NOTKNOWN_ERR;
    return;
  }

  // get pointer to the I2C instance
  *pErrOut = I2C_OK;
  pI2cDev = &i2c_Dev[(nReferenceIn - I2C_MIN_INTERFACE)];

  // make sure this instance has been initialized
  if (pI2cDev->status != I2C_REFERENCE_CREATED) {
    *pErrOut = I2C_REFERENCE_NOTCREATED_ERR;
    return;
  }

  // wait for free bus
  waitCount = 10;
  while (I2CMasterBusBusy(pI2cDev->ulI2CBase)) {
    if (waitCount-- == 0) {
      *pErrOut = I2C_DEVICE_ERR;        // use timeout on the wait
      return;
    }
    i2cDelay(5);                        // wait until bus not busy
  }

  //////////////////
  // WRITE
  //
  // check if there are any bytes to write
  if(wrLen)
  {
      i = 0;
      // Set the slave address for write
      I2CMasterSlaveAddrSet(pI2cDev->ulI2CBase, pI2cDev->slaveAddress, 0);

      // write the first byte
      I2CMasterDataPut(pI2cDev->ulI2CBase, pDataWr->data[i++]);

      // if there is only one byte to write, then perform a single send
      if(wrLen == 1)
      {
          I2CMasterControl(pI2cDev->ulI2CBase, I2C_MASTER_CMD_SINGLE_SEND);
          // wait for the write to complete
          i2cWaitForComplete(pI2cDev);
      }

      // otherwise there are multiple bytes to write
      else
      {
          // send the first byte of a burst, and wait for it to complete
          I2CMasterControl(pI2cDev->ulI2CBase, I2C_MASTER_CMD_BURST_SEND_START);
          wrLen--;
          i2cWaitForComplete(pI2cDev);

          // enter a loop to write the remaining bytes
          while(wrLen)
          {
              // write the next byte
              I2CMasterDataPut(pI2cDev->ulI2CBase, pDataWr->data[i++]);
              // if there is only 1 left, then this is the last byte
              if(wrLen == 1)
              {
                  I2CMasterControl(pI2cDev->ulI2CBase,
                                   I2C_MASTER_CMD_BURST_SEND_FINISH);
              }
              // or else there are multiple bytes remaining, so just continue
              else
              {
                  I2CMasterControl(pI2cDev->ulI2CBase,
                                   I2C_MASTER_CMD_BURST_SEND_CONT);
              }
              // wait for the write to complete
              i2cWaitForComplete(pI2cDev);
              wrLen--;
          }
      }
  }

  //////////////////
  // READ
  //
  // check if there are any bytes to read
  if(rdLen)
  {
      i = 0;
      // Set the slave address for read
      I2CMasterSlaveAddrSet(pI2cDev->ulI2CBase, pI2cDev->slaveAddress, 1);

      // if there is only one byte to read, then perform a single read
      if(rdLen == 1)
      {
          I2CMasterControl(pI2cDev->ulI2CBase, I2C_MASTER_CMD_SINGLE_RECEIVE);
          i2cWaitForComplete(pI2cDev);
      }

      // otherwise there are multiple bytes to read
      else
      {
          // command the first byte of a burst, and wait for it to complete
          I2CMasterControl(pI2cDev->ulI2CBase, I2C_MASTER_CMD_BURST_RECEIVE_START);
          i2cWaitForComplete(pI2cDev);
          rdLen--;

          // enter a loop to write the remaining bytes
          while(rdLen)
          {
              // read the next byte
              pDataRd->data[i++] = (unsigned char)I2CMasterDataGet(pI2cDev->ulI2CBase);
              // if there is only 1 left, then this is the last byte
              if(rdLen == 1)
              {
                  I2CMasterControl(pI2cDev->ulI2CBase,
                                   I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
              }
              // or else there are multiple bytes remaining, so just continue
              else
              {
                  I2CMasterControl(pI2cDev->ulI2CBase,
                                   I2C_MASTER_CMD_BURST_RECEIVE_CONT);
              }
              // wait for the read to complete
              i2cWaitForComplete(pI2cDev);
              rdLen--;
          }
      }

      // read the last (or only) byte
      pDataRd->data[i++] = (unsigned char)I2CMasterDataGet(pI2cDev->ulI2CBase);
  }
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

  // disable the I2C block
  I2CMasterDisable(pI2cDev->ulI2CBase);
  SysCtlPeripheralDisable(pI2cDev->ulGPIOPeriph);

  // also consider setting pins back to GPIO

  pI2cDev->status = I2C_REFERENCE_CLEARED;

}


/*----------------------------------------------------------------------------
  I2C lowlevel functions
 *---------------------------------------------------------------------------*/
static void i2cWaitForComplete(I2C_INFO *pI2cDev) {
    while (I2CMasterBusy(pI2cDev->ulI2CBase)) {
    }
}

static void i2cDelay (unsigned int ms) {
  os_dly_wait (ms);                                      // delay in msec
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

