/*----------------------------------------------------------------------------
 *      Name:    ARM_Serial.c
 *      Purpose: For Luminary Micro LM3SXXXX microcontrollers
 *               adapted from Template generic driver for LabVIEW
 *      Version: v1.00
 *      Note(s):
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2007 Keil Software.
 *---------------------------------------------------------------------------*/

/* NOTES:
 * - define LM_USE_INT_REGISTER to allow for dynamic registration of
 *   interrupt handlers.  Otherwise static registration is assumed.
 * - this module is written for the LM3S8962, but actually should work
 *   with any Stellaris part.  The 8962 has only 2 serial ports, so that
 *   is all that is enabled here.  But support is added for a third
 *   serial port in case a part with 3 UARTs is used. */

#define PART_LM3S8962  // TODO: This needs to be defined at the project level

#include "LVConfig.h"

#include "ARM_Serial.h"
#include "pin_map.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "sysctl.h"
#include "interrupt.h"
#include "uart.h"
#include "gpio.h"

#define SERIAL_PORT_IN_USE(port)    ((SER_MINPORT <= port) && (SER_MAXPORT >= port)) 
 
/* Set minimum and maximum port numbers available 
   NOTE: Setting this to only the required ports will reduce RAM and code size    */   
#define SER_MINPORT   0
#define SER_MAXPORT   1 /* 8962 has 2 serial ports, some parts have 3 */

#define SER_IN_BUF_SIZE   64              /* Input buffer in bytes (power 2) */
#define SER_OUT_BUF_SIZE  0              /* Output buffer in bytes (power 2) */

#define SER_PORTCLOSED    0
#define SER_PORTOPENED    1

#define SER_OK                  1
#define SER_GENERAL_ERR         0
#define SER_PORTNOTKNOWN_ERR  (-1)
#define SER_PORTNOTOPENED_ERR (-1)
#define SER_DEVNOTKNOWN_ERR   (-1)

/* Buffer masks */
#define SER_IN_MASK           (SER_IN_BUF_SIZE-1ul)
#define SER_OUT_MASK          (SER_OUT_BUF_SIZE-1ul)

/* Buffer read / write macros */
#define IN_BUF_RESET(serDev)        (serDev->ser_in.rd_idx = serDev->ser_in.wr_idx = 0)
#define IN_BUF_WR(serDev, dataIn)   (serDev->ser_in.data[SER_IN_MASK & serDev->ser_in.wr_idx++] = (dataIn))
#define IN_BUF_RD(serDev)           (serDev->ser_in.data[SER_IN_MASK & serDev->ser_in.rd_idx++])   
#define IN_BUF_EMPTY(serDev)        (serDev->ser_in.rd_idx == serDev->ser_in.wr_idx)
#define IN_BUF_FULL(serDev)         (serDev->ser_in.rd_idx == serDev->ser_in.wr_idx+1)
#define IN_BUF_COUNT(serDev)        (SER_IN_MASK & (serDev->ser_in.wr_idx - serDev->ser_in.rd_idx))

// Serial input buffer
typedef struct __ser_in_t {
  volatile unsigned char data[SER_IN_BUF_SIZE];
  volatile unsigned int wr_idx;
  volatile unsigned int rd_idx;
} ser_in_t;


/*----------------------------------------------------------------------------
  structure to administrate the UART
 *---------------------------------------------------------------------------*/
typedef void (*ser_irq_fp)(void);           // RV irq function pointer typedef

#if SERIAL_PORT_IN_USE(0)
#ifdef LM_USE_INT_REGISTER
static
#endif
void ser_irq_0 (void);                      // RV irq function prototype for port 0
#endif /* SERIAL_PORT_IN_USE(0) */

#if SERIAL_PORT_IN_USE(1)
#ifdef LM_USE_INT_REGISTER
static
#endif
void ser_irq_1 (void);                      // RV irq function prototype for port 1
#endif /* SERIAL_PORT_IN_USE(1) */

#if SERIAL_PORT_IN_USE(2)
#ifdef LM_USE_INT_REGISTER
static
#endif
void ser_irq_2 (void);                      // RV irq function prototype for port 2
#endif /* SERIAL_PORT_IN_USE(2) */

/* USER CODE =====================================================================*/
/* Add / Remove function protoypes for any extra port interrupts */   
/* END USER CODE ================================================================ */  

typedef struct {
    int             serialDeviceNumber;     // = portNumber + 1 (not really)
    unsigned int    status;
    unsigned long   baudRate;
    unsigned int    dataBits;
    unsigned int    stopBits;
    unsigned int    parity;

    /* LM specific */
    unsigned long   uartBase;
    unsigned long   interrupt;
    unsigned long   periphUart;
    unsigned long   periphGpio;
    unsigned long   port;
    unsigned long   pins;

    /* leftover, probably not needed */
    unsigned long   txFifoEmpty;        // 0x000(0000000X) Indicates transmit fifo is empty and needs to be kickstarted
  
    ser_irq_fp      ser_irq;            // Serial IRQ function
    ser_in_t        ser_in;
} SER_INFO;

static SER_INFO ser_Dev[SER_MAXPORT - SER_MINPORT + 1] = {
#if SERIAL_PORT_IN_USE(0)
  { 1, 
    SER_PORTCLOSED, 9600, 8, 0, 0, 

    UART0_BASE,
    INT_UART0,
    SYSCTL_PERIPH_UART0,
    SYSCTL_PERIPH_GPIOA,
    U0TX_PORT,
    U0TX_PIN | U0RX_PIN,

    1,
    ser_irq_0                                             /* IRQ function */
  },
#endif /* SERIAL_PORT_IN_USE(0) */

#if SERIAL_PORT_IN_USE(1) 
  { 2, 
    SER_PORTCLOSED, 9600, 8, 0, 0, 
    
    UART1_BASE,
    INT_UART1,
    SYSCTL_PERIPH_UART1,
    SYSCTL_PERIPH_GPIOD,
    U1TX_PORT,
    U1TX_PIN | U1RX_PIN,

    1,
    ser_irq_1                                             /* IRQ function */    
  },
#endif /* SERIAL_PORT_IN_USE(1) */

/* The LM3S8962 only has 2 serial ports so this is an extra.  It is added
 * here in case this code is used with a different part that has 3 serial
 * ports. */

#if SERIAL_PORT_IN_USE(2) 
  { 3, 
    SER_PORTCLOSED, 9600, 8, 0, 0, 
    
    UART2_BASE,
    INT_UART2,
    SYSCTL_PERIPH_UART2,
    SYSCTL_PERIPH_GPIOG,
    U2TX_PORT,
    U2TX_PIN | U2RX_PIN,

    1,
    ser_irq_2                                             /* IRQ function */    
  },
#endif /* SERIAL_PORT_IN_USE(2) */
};

/*----------------------------------------------------------------------------
  Function Prototypes
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  open the UART
 *---------------------------------------------------------------------------*/
int ARM_Serial_OpenPort (int port, int *serDevNo) {
  SER_INFO *pSerDev;

  if ( (port < SER_MINPORT) ||                      /* check the port number */
       (port > SER_MAXPORT)   ) {
    return (SER_PORTNOTKNOWN_ERR);
  }

  /* get handy pointer to serial port info */
  pSerDev = &ser_Dev[port - SER_MINPORT];

  /* enable the GPIO port and the UART, then enable the UART pins */
  SysCtlPeripheralEnable(pSerDev->periphGpio);
  SysCtlPeripheralEnable(pSerDev->periphUart);
  GPIOPinTypeUART(pSerDev->port, pSerDev->pins);
  UARTEnable(pSerDev->uartBase);

  /* set port status indicator and return "handle" to caller */
  pSerDev->status = SER_PORTOPENED;
  *serDevNo = pSerDev->serialDeviceNumber;

  return (SER_OK);
}

/*----------------------------------------------------------------------------
  close the UART
 *---------------------------------------------------------------------------*/
int ARM_Serial_ClosePort (int serDevNo) {
  SER_INFO *pSerDev;

  serDevNo--;
  if ( (serDevNo < (SER_MINPORT)) ||              /* check the serial device number */
       (serDevNo > (SER_MAXPORT))   ) {
    return (SER_DEVNOTKNOWN_ERR);
  }

  /* get pointer to serial port info */
  pSerDev = &ser_Dev[(serDevNo - SER_MINPORT)];

  /* Turn off the uart generating interrupts, and then disable the uart.
   * Disable the uart interrupt on the interrupt controller. */
  UARTIntDisable(pSerDev->uartBase, UART_INT_RT | UART_INT_RX | UART_INT_TX);
  UARTDisable(pSerDev->uartBase);
#ifdef LM_USE_INT_REGISTER
  UARTIntUnregister(pSerDev->uartBase);
#else
  IntDisable(pSerDev->interrupt);
#endif

  /* Turn the peripheral off since it will no longer be used, but
   * leave the GPIO port enabled because some other peripheral might
   * be using it.  Set the peripheral pins to be inputs. */
  SysCtlPeripheralDisable(pSerDev->periphUart);
  GPIOPinTypeGPIOInput(pSerDev->port, pSerDev->pins);

  /* Indicate the port is closed. */
  pSerDev->status   = SER_PORTCLOSED;
  
  return (SER_OK);
}

/*----------------------------------------------------------------------------
  initialize the UART
 *---------------------------------------------------------------------------*/
int ARM_Serial_InitPort (int serDevNo, unsigned long baudrate, unsigned int  databits,
                                unsigned int  parity,   unsigned int  stopbits) {
  SER_INFO *pSerDev;
  unsigned long config = 0;

  serDevNo--;
  if ( (serDevNo < (SER_MINPORT)) ||     /* check the serial device number */
       (serDevNo > (SER_MAXPORT))   ) {
    return (SER_DEVNOTKNOWN_ERR);
  }

  pSerDev = &ser_Dev[(serDevNo - SER_MINPORT)];

  switch (databits) {
    case 5:                              /* 5 Data bits   */
      config |= UART_CONFIG_WLEN_5;
    break;
    case 6:                              /* 6 Data bits   */
      config |= UART_CONFIG_WLEN_6;
    break;
    case 7:                              /* 7 Data bits   */
      config |= UART_CONFIG_WLEN_7;
    break;
    case 8:                              /* 8 Data bits   */
    default:
      config |= UART_CONFIG_WLEN_8;
    break;
  }

  switch (stopbits) {
    case 1:                              /* 1,5 Stop bits */
    case 2:                              /* 2   Stop bits */
      config |= UART_CONFIG_STOP_TWO;
    break;
    case 0:                              /* 1   Stop bit  */
    default:
      config |= UART_CONFIG_STOP_ONE;
    break;
  }

  switch (parity) {
    case 1:                              /* Parity Odd    */
      config |= UART_CONFIG_PAR_ODD;
    break;
    case 2:                              /* Parity Even   */
      config |= UART_CONFIG_PAR_EVEN;
    break;
    case 3:                              /* Parity Mark   */
      config |= UART_CONFIG_PAR_ONE;
    break;
    case 4:                              /* Parity Space  */
      config |= UART_CONFIG_PAR_ZERO;
    break;
    case 0:                              /* Parity None   */
    default:
      config |= UART_CONFIG_PAR_NONE;
    break;
  }

  pSerDev->baudRate = baudrate;
  pSerDev->dataBits = databits;
  pSerDev->stopBits = stopbits;
  pSerDev->parity   = parity;
  
  IN_BUF_RESET(pSerDev);

  /* configure the uart parameters */
  UARTConfigSetExpClk(pSerDev->uartBase, SysCtlClockGet(),
                      baudrate, config);
  pSerDev->txFifoEmpty = 1;                         /* TX fifo is empty                 */

  /* Enable or install the UART interrupt handler, then enable the
   * specific interrupts on the UART. */
#ifdef LM_USE_INT_REGISTER
  UARTIntRegister(pSerDev->uartBase, pSerDev->ser_irq);
#else
  IntEnable(pSerDev->interrupt);
#endif
  UARTIntEnable(pSerDev->uartBase, UART_INT_RT | UART_INT_RX | UART_INT_TX);

  /* TODO: do we need to set interrupt priority other than default? */
  /* TODO: do we really need to enable the TX interrupt?  it does not
   * appear to actually be used for anything here */

  return (SER_OK);
}

/*----------------------------------------------------------------------------
  read data from UART
 *---------------------------------------------------------------------------*/
int ARM_Serial_Read (int serDevNo, char *buffer, const int *length) {
  SER_INFO *pSerDev;
  int bytesToRead, bytesRead;
  
  serDevNo--;
  if ( (serDevNo < (SER_MINPORT)) ||     /* check the serial device number */
       (serDevNo > (SER_MAXPORT))   ) {
    return (SER_DEVNOTKNOWN_ERR);
  }
  pSerDev = &ser_Dev[(serDevNo - SER_MINPORT)];

  // Read *length bytes, block if *bytes are not avaialable
  bytesToRead = *length;
  bytesToRead = (bytesToRead < (*length)) ? bytesToRead : (*length);
  bytesRead = bytesToRead;

  while (bytesToRead--) {
    while (IN_BUF_EMPTY(pSerDev));               /* Block until data is available if none */
    *buffer++ = IN_BUF_RD(pSerDev);
  }
  return (bytesRead);  
}

/*----------------------------------------------------------------------------
  write data to the UART
 *---------------------------------------------------------------------------*/
int ARM_Serial_Write (int serDevNo, const char *buffer, int *length) {
  SER_INFO *pSerDev;
  int bytesToWrite, bytesWritten;

  serDevNo--;
  if ( (serDevNo < (SER_MINPORT)) ||     /* check the serial device number */
       (serDevNo > (SER_MAXPORT))   ) {
    return (SER_DEVNOTKNOWN_ERR);
  }
  pSerDev = &ser_Dev[(serDevNo - SER_MINPORT)];

  // Write *length bytes
  // always writes all the bytes requested
  bytesToWrite = *length;
  bytesWritten = bytesToWrite;

  /* While there are still any bytes to write, copy to UART output.
   * This will be a blocking call, but the UART has a FIFO so it will only
   * block if the FIFO is full */
  while (bytesToWrite) {
    UARTCharPut(pSerDev->uartBase, *buffer++);
    bytesToWrite--;
  }

  /* TODO: can caller handle only partial transfer?
   * TODO: this could be rewritten to be slightly more effecient
   * by using direct register access instead of DL call
   * TODO: ignoring txFifoEmpty flag because I think it serves no
   * purpose here */

  return (bytesWritten); 
}

/*----------------------------------------------------------------------------
  check if character(s) are available at the serial interface
 *---------------------------------------------------------------------------*/
int ARM_Serial_AvailChar (int serDevNo, int *availChar) {
  SER_INFO *pSerDev;

  serDevNo--;
  if ( (serDevNo < (SER_MINPORT)) ||     /* check the serial device number */
       (serDevNo > (SER_MAXPORT))   ) {
    return (SER_DEVNOTKNOWN_ERR);
  }
  pSerDev = &ser_Dev[(serDevNo - SER_MINPORT)];

  *availChar = IN_BUF_COUNT(pSerDev);

  return (SER_OK);
}

#if SERIAL_PORT_IN_USE(0)
/*----------------------------------------------------------------------------
  UART 0 interrupt
 *---------------------------------------------------------------------------*/
#ifdef LM_USE_INT_REGISTER
static
#endif
void ser_irq_0 (void) {
  SER_INFO *pSerDev = &ser_Dev[0 - SER_MINPORT];
  unsigned long status;

  /* get the interrupt status and clear the pending ints */
  status = UARTIntStatus(pSerDev->uartBase, 1);
  UARTIntClear(pSerDev->uartBase, status);

  /* If TX interrupt, just set the empty flag */
  if(status & UART_INT_TX) {
    pSerDev->txFifoEmpty = 1;
  }

  /* For RX interrupt, read as many characters as are available in the UART */
  if(status & (UART_INT_RX | UART_INT_RT)) {
    while(UARTCharsAvail(pSerDev->uartBase)) {
        IN_BUF_WR(pSerDev, UARTCharGetNonBlocking(pSerDev->uartBase));
    }
  } 

  /* TODO: could use direct register access for more effeciency */
  /* TODO: could really get rid of TX case completely as txFifoFlag
   * is really not used for anything */
}

#else
/* If static int vectors are used, then the vector table may have a
 * reference to this function, even if it is not used.  So create a
 * dummy to keep the linker happy */
void ser_irq_0(void) {}
#endif /* SERIAL_PORT_IN_USE(0) */


#if SERIAL_PORT_IN_USE(1)
/*----------------------------------------------------------------------------
  UART 1 interrupt
 *---------------------------------------------------------------------------*/
#ifdef LM_USE_INT_REGISTER
static
#endif
void ser_irq_1 (void) {
  SER_INFO *pSerDev = &ser_Dev[1 - SER_MINPORT];
  unsigned long status;

  /* get the interrupt status and clear the pending ints */
  status = UARTIntStatus(pSerDev->uartBase, 1);
  UARTIntClear(pSerDev->uartBase, status);

  /* If TX interrupt, just set the empty flag */
  if(status & UART_INT_TX) {
    pSerDev->txFifoEmpty = 1;
  }

  /* For RX interrupt, read as many characters as are available in the UART */
  if(status & (UART_INT_RX | UART_INT_RT)) {
    while(UARTCharsAvail(pSerDev->uartBase)) {
        IN_BUF_WR(pSerDev, UARTCharGetNonBlocking(pSerDev->uartBase));
    }
  } 

  /* TODO: could use direct register access for more effeciency */
  /* TODO: could really get rid of TX case completely as txFifoFlag
   * is really not used for anything */
}

#else
/* If static int vectors are used, then the vector table may have a
 * reference to this function, even if it is not used.  So create a
 * dummy to keep the linker happy */
void ser_irq_1(void) {}
#endif /* SERIAL_PORT_IN_USE(1) */


#if SERIAL_PORT_IN_USE(2)
/*----------------------------------------------------------------------------
  UART 2 interrupt
 *---------------------------------------------------------------------------*/
#ifdef LM_USE_INT_REGISTER
static
#endif
void ser_irq_2 (void) {
  SER_INFO *pSerDev = &ser_Dev[2 - SER_MINPORT];
  unsigned long status;

  /* get the interrupt status and clear the pending ints */
  status = UARTIntStatus(pSerDev->uartBase, 1);
  UARTIntClear(pSerDev->uartBase, status);

  /* If TX interrupt, just set the empty flag */
  if(status & UART_INT_TX) {
    pSerDev->txFifoEmpty = 1;
  }

  /* For RX interrupt, read as many characters as are available in the UART */
  if(status & (UART_INT_RX | UART_INT_RT)) {
    while(UARTCharsAvail(pSerDev->uartBase)) {
        IN_BUF_WR(pSerDev, UARTCharGetNonBlocking(pSerDev->uartBase));
    }
  } 

  /* TODO: could use direct register access for more effeciency */
  /* TODO: could really get rid of TX case completely as txFifoFlag
   * is really not used for anything */
}

#else
/* If static int vectors are used, then the vector table may have a
 * reference to this function, even if it is not used.  So create a
 * dummy to keep the linker happy */
void ser_irq_2(void) {}
#endif /* SERIAL_PORT_IN_USE(2) */


/* USER CODE =====================================================================*/
/* Add / Remove interrupt code for any extra ports.
   NOTE: if multiple ports are used (or a vectored interrupt controller is not
         available), speed may be sacrificed for code speed by having one
         interrupt handler only, that checks for which port caused the 
         interrupt                                                                */   
/* END USER CODE ================================================================ */

/*----------------------------------------------------------------------------
  The following functions are used by the printf / scanf if stdout is
  redirected to the UART. They do not need to be modified
 *---------------------------------------------------------------------------*/

#ifndef UART_PRINTF_BAUDRATE 
#define UART_PRINTF_BAUDRATE 115200
#endif

#ifndef UART_PRINTF_PORT 
#define UART_PRINTF_PORT 1
#endif

#if ((UART_PRINTF_PORT != 0) && (UART_PRINTF_PORT != 1))
  #error "UART_PRINTF_PORT must be defined as either 0 or 1!"
#endif

static int printfDevNo;

void init_serial (void)  {               /* Initialize Serial Interface       */

  ARM_Serial_OpenPort (UART_PRINTF_PORT, &printfDevNo);
  ARM_Serial_InitPort(printfDevNo,
               UART_PRINTF_BAUDRATE,
               8,   /* 8 data bits */ 
               0,   /* No partity */ 
               0);  /* 1 stop bit */
}

/* implementation of putchar (also used by printf function to output data)    */
#define CR     0x0D
int sendchar (int ch)  {                 /* Write character to UART    */
  int length = 1;
  int cr = CR;

  if (ch == '\n')  {
    ARM_Serial_Write (printfDevNo, (const char *)&cr, &length);
  }
  ARM_Serial_Write (printfDevNo, (const char *)&ch, &length);


  return (ch);
}


int getkey (void)  {                     /* Read character from UART   */
  int availChar = 0;
  int length = 1;
  char data;

  do {
    ARM_Serial_AvailChar(printfDevNo, &availChar);
  } while (!availChar);

  ARM_Serial_Read (printfDevNo, &data, &length);

  return (data);
}


