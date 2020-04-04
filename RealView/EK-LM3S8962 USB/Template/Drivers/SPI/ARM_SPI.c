/*----------------------------------------------------------------------------
 * Name:    ARM_SPI.c
 * Purpose: LM3Sxxxx SPI driver
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
#include "ARM_SPI.h"
#include "LVFuncsUsed.h"
#include <LM3Sxxxx.h>

extern unsigned long g_ulGPIOPortMap[];

#define SPI_INTERFACE_IN_USE(spiIf) ((SPI_MIN_INTERFACE <= spiIfNr) && (SPI_MAX_INTERFACE >= spiIfNr))

#define SPI_MIN_INTERFACE   0
#define SPI_MAX_INTERFACE   0

#ifdef _Include_ARM_SPI_Functions

/*----------------------------------------------------------------------------
  SPI default settings
 *---------------------------------------------------------------------------*/
#define SPI_CLOCK_RATE          1000                     // in kHz
#define SPI_CLOCK_POLARITY      SPI_CLOCK_POLARITY_IDLELOW
#define SPI_CLOCK_PHASE         SPI_CLOCK_PHASE_FIRSTEDGE

#define SPI_REFERENCE_CLEARED   0
#define SPI_REFERENCE_CREATED   1

#define SPI_INT_PRIORITY        4       // 0-7, lower number is higher prio

/*----------------------------------------------------------------------------
  structure to administrate SPI
 *---------------------------------------------------------------------------*/
typedef struct {
                    int   reference;        // = portNumber + 1
           unsigned int   status;           // opened or closed
           unsigned int   interface;
		   unsigned int   size;
           unsigned int   clockRate;
           unsigned int   clockPolarity;
           unsigned int   clockPhase;

           unsigned int   wrLen;            // count of writes remaining
           unsigned int   rdLen;            // count of reads remaining
           unsigned int   wrExtra;          // count of dummy bytes to write
           unsigned int   rdExtra;          // count of dummy reads

           OS_SEM         semISR;           // interrupt handler semaphore
           OS_SEM         semInstance;      // instance semaphore

           unsigned long  ulSSIPeriph;      // SSI peripheral
           unsigned long  ulSSIBase;        // base address of SSI periph
           unsigned long  ulSSIInt;         // interrupt number for SSI
           unsigned long  ulGPIOPortBase;   // base address of GPIO for SSI
           unsigned long  ulGPIOPins;       // pins needed for SSI
           unsigned long  ulFssPin;         // pin used for Fss signal
           unsigned long  ulCSPortBase;     // GPIO port used for CS
           unsigned long  ulCSPinMask;      // GPIO pin used for CS

           unsigned char *pRdBuf;           // buffer for storing read data
           unsigned char *pWrBuf;           // buffer containing write data
} SPI_INFO;

static SPI_INFO spi_Dev[SPI_MAX_INTERFACE - SPI_MIN_INTERFACE + 1] = {
#if SPI_INTERFACE_IN_USE(0)
  { 1,
    SPI_REFERENCE_CLEARED, 0, SPI_CLOCK_RATE,
    SPI_CLOCK_POLARITY_IDLELOW, SPI_CLOCK_PHASE_FIRSTEDGE,

    0, 0, 0, 0,	0,

    {0, 0}, {0, 0},

    SYSCTL_PERIPH_SSI0, SSI0_BASE, INT_SSI0,
    GPIO_PORTA_BASE, (GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5), GPIO_PIN_3,
    0, 0
  },
#endif /* SPI_INTERFACE_IN_USE(0) */
};

/*----------------------------------------------------------------------------
  Maps the phase and clock settings to the DriverLib SSI mode equivalents
 *---------------------------------------------------------------------------*/
static unsigned long ulProtocol[4] =
{
   SSI_FRF_MOTO_MODE_0, SSI_FRF_MOTO_MODE_1,
   SSI_FRF_MOTO_MODE_2, SSI_FRF_MOTO_MODE_3
};

/*----------------------------------------------------------------------------
  SPI create a reference
  polarity - 0 means first edge, 1 means second edge
  phase - 0 idle clock is low, 1 idle clock is high

  interface parameter is used to specify the peripheral, and a GPIO
  chip select, if needed.  If no GPIO is specified, then the normal Fss
  signal is used.
  The interface parameter is arranged by bytes as follows:
  [07:00] interface number (only 0 is valid now)
  [11:08] GPIO pin number (0-7)
  [15:12] GPIO port number (1-7, where 1=A, etc)
 *---------------------------------------------------------------------------*/
void ARM_SPI_CreateConfigurationReference (
                         int    interface,
				unsigned int	size,
                unsigned int    clockRate,
                unsigned int    clockPolarity,
                unsigned int    clockPhase,
                         int*  pReferenceOut,
                         int*  pErrOut) {
  SPI_INFO *pSpiDev;
  unsigned int uProtIdx;
  unsigned long ulCS;

  if ( ((interface & 0xff) < SPI_MIN_INTERFACE) ||
       ((interface & 0xff) > SPI_MAX_INTERFACE)   ) {
    *pErrOut = SPI_CHIPSELECT_NOTKNOWN_ERR;         // check the device number
    return;
  }

  if ( (clockPolarity != SPI_CLOCK_POLARITY_IDLELOW ) &&
       (clockPolarity != SPI_CLOCK_POLARITY_IDLEHIGH)   ){
    *pErrOut = SPI_CLKPOLARITY_NOTSUPPORTED_ERR;    // check the clock polarity
    return;
  }

  if ( (clockPhase != SPI_CLOCK_POLARITY_IDLELOW ) &&
       (clockPhase != SPI_CLOCK_POLARITY_IDLEHIGH)   ){
    *pErrOut = SPI_CLKPHASE_NOTSUPPORTED_ERR;       // check the clock phase
    return;
  }

  // get pointer to the SPI instance
  pSpiDev = &spi_Dev[((interface & 0xff) - SPI_MIN_INTERFACE)];

  // see if a chip select GPIO is specified
  if(interface & 0xFF00)
  {
      ulCS = (interface & 0xF000) >> 12;
      pSpiDev->ulCSPortBase = g_ulGPIOPortMap[ulCS - 1];
      ulCS = (interface & 0xF00) >> 8;
      pSpiDev->ulCSPinMask = 1 << ulCS;
  }
  else
  {
      pSpiDev->ulCSPortBase = 0;
  }

  // critical section - dont let any other task in here
  tsk_lock();

  // Need to create the semaphores if not created yet.
  if(pSpiDev->semISR[0] == 0)
  {
      // initialize the instance and ISR semaphore
      os_sem_init(pSpiDev->semISR, 0);
      os_sem_init(pSpiDev->semInstance, 1);
  }

  // exit the critical section
  tsk_unlock();

  // get the instance semaphore so no other task can use this now
  // use 2 second timeout
  if(os_sem_wait(pSpiDev->semInstance, 2000) == OS_R_TMO)
  {
      // if it timed out, then something is wrong
      *pErrOut = SPI_DEVICE_ERR;
      return;
  }

  // figure out the SSI mode to use based on phase and polarity
  uProtIdx = (clockPhase ? 1 : 0) | (clockPolarity ? 2 : 0);

  // Enable the SSI peripheral and set up GPIO pins.
  // GPIO periph is not enabled here because all are enabled at sys init
  SysCtlPeripheralEnable(pSpiDev->ulSSIPeriph);
  GPIOPinTypeSSI(pSpiDev->ulGPIOPortBase, pSpiDev->ulGPIOPins);

  // set up the CS pin, or the Fss signal
  if(pSpiDev->ulCSPortBase)
  {
      // assume port is already enabled
      GPIOPinTypeGPIOOutput(pSpiDev->ulCSPortBase, pSpiDev->ulCSPinMask);
      GPIOPinWrite(pSpiDev->ulCSPortBase, pSpiDev->ulCSPinMask,
                   pSpiDev->ulCSPinMask);   // drive high
  }
  else
  {
      GPIOPinTypeSSI(pSpiDev->ulGPIOPortBase, pSpiDev->ulFssPin);
  }

  // Initialize the SSI peripheral and configure it
  SSIConfigSetExpClk(pSpiDev->ulSSIBase, SysCtlClockGet(),
                     ulProtocol[uProtIdx], SSI_MODE_MASTER,
                     clockRate * 1000, size);
  SSIEnable(pSpiDev->ulSSIBase);

  // configure the peripheral interrupt
  SSIIntDisable(pSpiDev->ulSSIBase, SSI_TXFF | SSI_RXFF | SSI_RXTO);
  IntPrioritySet(pSpiDev->ulSSIInt, SPI_INT_PRIORITY);
  IntEnable(pSpiDev->ulSSIInt);

  // save instance details
  pSpiDev->status = SPI_REFERENCE_CREATED;
  pSpiDev->interface = interface;	 
  pSpiDev->size = size;
  pSpiDev->clockRate = clockRate;
  pSpiDev->clockPolarity = clockPolarity;
  pSpiDev->clockPhase = clockPhase;
  *pReferenceOut = pSpiDev->reference;
  *pErrOut = SPI_OK;

  // post the instance semaphore to unlock access
  os_sem_send(pSpiDev->semInstance);
}

/*----------------------------------------------------------------------------
  SPI perform a write/read access
 *---------------------------------------------------------------------------*/
void ARM_SPI_WriteRead (
                         int   nReferenceIn,
                    T_bArray*  pDataWr,
				unsigned int   size,
                unsigned int   elementsToRead,
                    T_bArray*  pDataRd,
                         int*  pErrOut) {

  SPI_INFO     *pSpiDev;

  unsigned int spiStatus;
  unsigned int i     = 0;
  unsigned int wrLen = pDataWr->len;
  unsigned int rdLen = elementsToRead;
  unsigned long spiData;

  nReferenceIn -= 1;                        // reference is starting from 1

  // check the reference input
  if ( (nReferenceIn < (SPI_MIN_INTERFACE)) ||
       (nReferenceIn > (SPI_MAX_INTERFACE))   ) {
    *pErrOut = SPI_REFERENCE_NOTKNOWN_ERR;
    return;
  }

  // set the success error code
  // get a pointer to the SPI instance to use
  *pErrOut = SPI_OK;
  pSpiDev = &spi_Dev[(nReferenceIn - SPI_MIN_INTERFACE)];

  // make sure this SPI instance has been initialized already
  if (pSpiDev->status != SPI_REFERENCE_CREATED) {
    *pErrOut = SPI_REFERENCE_NOTCREATED_ERR;
    return;
  }
  
  // make sure the SPI read/write size matches the size when this SPI reference was created
  if (pSpiDev->size != size) {
    *pErrOut = SPI_SIZE_MISMATCH_ERR;
    return;
  }

  // get the instance semaphore so no other task can use this now
  // use 2 second timeout
  if(os_sem_wait(pSpiDev->semInstance, 2000) == OS_R_TMO)
  {
      // if it timed out, then something is wrong
      *pErrOut = SPI_DEVICE_ERR;
      return;
  }

  // flush any data remaining in the read FIFO
  while(SSIDataGetNonBlocking(pSpiDev->ulSSIBase, &spiData))
  {
  }

  // compute the number of real and dummy bytes to read and write
  pSpiDev->wrLen = pDataWr->len;
  pSpiDev->rdLen = elementsToRead;
  if(pSpiDev->wrLen > pSpiDev->rdLen)
  {
      pSpiDev->rdExtra = pSpiDev->wrLen - pSpiDev->rdLen;
      pSpiDev->wrExtra = 0;
  }
  else
  {
      pSpiDev->wrExtra = pSpiDev->rdLen - pSpiDev->wrLen;
      pSpiDev->rdExtra = 0;
  }

  // set the buffer pointers for read and write
  pSpiDev->pRdBuf = pDataRd->data;
  pSpiDev->pWrBuf = pDataWr->data;

  // if CS signalling is used, then drive CS pin low
  if(pSpiDev->ulCSPortBase)
  {
      GPIOPinWrite(pSpiDev->ulCSPortBase, pSpiDev->ulCSPinMask, 0);
  }

  // enable the interrupt handler, this should start the transfer running
  SSIIntEnable(pSpiDev->ulSSIBase, SSI_TXFF | SSI_RXFF | SSI_RXTO);

  // block on semaphore, waiting for ISR to complete, wait forever
  os_sem_wait(pSpiDev->semISR, 0xffff);

  // if CS signalling is used, drive the CS pin high
  if(pSpiDev->ulCSPortBase)
  {
      GPIOPinWrite(pSpiDev->ulCSPortBase, pSpiDev->ulCSPinMask,
                   pSpiDev->ulCSPinMask);
  }

  // load the count of bytes read, just use the specified value
  pDataRd->len = elementsToRead;

  // post the instance semaphore to unlock access
  os_sem_send(pSpiDev->semInstance);
}


/*----------------------------------------------------------------------------
  SPI close a  reference
 *---------------------------------------------------------------------------*/
void ARM_SPI_CloseReference (
                         int   nReferenceIn,
                         int*  pErrOut) {
  SPI_INFO *pSpiDev;

  nReferenceIn -= 1;                    // reference is starting from 1

  // check the reference input
  if ( (nReferenceIn < (SPI_MIN_INTERFACE)) ||
       (nReferenceIn > (SPI_MAX_INTERFACE))   ) {
    *pErrOut = SPI_REFERENCE_NOTKNOWN_ERR;
    return;
  }

  *pErrOut = SPI_OK;
  pSpiDev = &spi_Dev[(nReferenceIn - SPI_MIN_INTERFACE)];

  // make sure this SPI instance has been initialized already
  if (pSpiDev->status != SPI_REFERENCE_CREATED) {
    *pErrOut = SPI_REFERENCE_NOTCREATED_ERR;
    return;
  }

  // get the instance semaphore so no other task can use this now
  // use 2 second timeout
  if(os_sem_wait(pSpiDev->semInstance, 2000) == OS_R_TMO)
  {
      // if it timed out, then something is wrong
      *pErrOut = SPI_DEVICE_ERR;
      return;
  }

  // disable the SSI block
  SSIDisable(pSpiDev->ulSSIBase);
  SysCtlPeripheralDisable(pSpiDev->ulSSIPeriph);

  pSpiDev->ulCSPortBase = 0;
  pSpiDev->status   = SPI_REFERENCE_CLEARED;

  // post the instance semaphore to unlock access
  os_sem_send(pSpiDev->semInstance);
}
#endif // _Include_ARM_SPI_Functions

/*----------------------------------------------------------------------------
  Process the SSI peripheral interrupt

  An interrupt handler is used to process the SPI transactions.  This
  avoids the need for status polling, allowing the rest of the system to
  continue processing.  The initiating thread will remain blocked until
  the transfer is complete, but any other threads will continue to run.

  It is the nature of SPI that the same number of bytes are clocked in
  and out.  Therefore, even if the number of bytes desired to read and
  write are different, the physical interface must clock the same number
  in each direction.  The interrupt handler uses "extra" byte counts for
  read and write to balance out the transaction.  The following instance
  value must be set before this interrupt handler can be activated:

  - wrLen - the number of bytes to write from the write buffer
  - wrExtra - the balancing number of additional dummy bytes to write
  - rdLen - the number of bytes to read and store in the read buffer
  - rdExtra - the balancing number of dummy reads
  - pWrBuf - the write buffer, containing the data to write
  - pRdBuf - the read buffer, where the read data will be stored

  The main thread code activates the interrupt handler by setting up the
  above parameters, and then enabling the SSI interrupts.  It should also
  take the semISR semaphore to ensure that no other thread will try to
  use the int handler before this one is done.

  This ISR will continue to read and write bytes on the interface until
  all the counts are satisfied.  When this happens, the SSI interrupts are
  disabled, and the semaphore is posted, allowing the thread to resume.
 *---------------------------------------------------------------------------*/
void
SSI0IntHandler(void)
{
#ifdef _Include_ARM_SPI_Functions

    SPI_INFO *pSpiDev;
    unsigned long ulData;
    unsigned long ulStatus;

    //
    // Get instance pointer
    //
    pSpiDev = &spi_Dev[0];

    //
    // Get SSI interrupt status
    //
    ulStatus = SSIIntStatus(pSpiDev->ulSSIBase, 1);

    //
    // Process SSI RX condition
    //
    if(ulStatus & (SSI_RXFF | SSI_RXTO))
    {
        //
        // Read data as long as there is anything in the FIFO
        //
        while(SSIDataGetNonBlocking(pSpiDev->ulSSIBase, &ulData))
        {
            //
            // If there are still bytes to read, then store them in the
            // read buffer.
            //
            if(pSpiDev->rdLen)
            {
				if(pSpiDev->size == 8) {
					*(pSpiDev->pRdBuf++) = (unsigned char)ulData;
				}
				else {
					*(((unsigned short *)pSpiDev->pRdBuf)++) = (unsigned short)ulData;
					
				}
				pSpiDev->rdLen--;
            }

            //
            // Otherwise, if there are still extra "dummy" bytes to
            // read, then throw those away but decrement the counter
            //
            else if(pSpiDev->rdExtra)
            {
                pSpiDev->rdExtra--;
            }
        }
    }

    //
    // Process the SSI TX condition
    //
    if(ulStatus & SSI_TXFF)
    {
        //
        // Loop as long as there are real bytes, or dummy bytes to transmit
        //
        while(pSpiDev->wrLen || pSpiDev->wrExtra)
        {
            //
            // Load the real or dummy byte
            //
			if (pSpiDev->size == 8) {
				ulData = pSpiDev->wrLen ? *pSpiDev->pWrBuf : 0;
			}
			else {
				ulData = pSpiDev->wrLen ? *((unsigned short *)pSpiDev->pWrBuf) : 0;
			}

            //
            // Attempt to write the byte.  If FIFO is full then break
            // out of this loop
            //
            if(!SSIDataPutNonBlocking(pSpiDev->ulSSIBase, ulData))
            {
                break;
            }

            //
            // If there are still real bytes to write, then adjust the count
            // and advance the buffer pointer
            //
            if(pSpiDev->wrLen)
            {
                pSpiDev->wrLen--;
				if (pSpiDev->size == 8) {
					pSpiDev->pWrBuf++;
				}
				else {
					((unsigned short *)pSpiDev->pWrBuf)++;
				}
				
            }

            //
            // If there are only dummy bytes left to write, then just
            // adjust the counter
            //
            else if(pSpiDev->wrExtra)
            {
                pSpiDev->wrExtra--;
            }
        }

        //
        // When there are no more bytes to write at all, then disable
        // the TX interrupt
        //
        if((pSpiDev->wrLen + pSpiDev->wrExtra) == 0)
        {
            SSIIntDisable(pSpiDev->ulSSIBase, SSI_TXFF);
        }
    }

    //
    // If all of the real and dummy byte counters are now 0, then we
    // are done with the SSI transfer.  Disable all the SSI interrupts
    // and post the ISR semaphore so the blocked thread can proceed
    //
    if((pSpiDev->rdLen + pSpiDev->rdExtra + pSpiDev->wrLen + pSpiDev->wrExtra)
            == 0)
    {
        SSIIntDisable(pSpiDev->ulSSIBase, (SSI_TXFF | SSI_RXFF | SSI_RXTO));
        isr_sem_send(pSpiDev->semISR);
    }

#endif // _Include_ARM_SPI_Functions
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

