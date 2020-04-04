/*----------------------------------------------------------------------------
 *      Name:    ARM_Serial.c
 *      Purpose: Template generic / LPC23xx UART ('serial') driver for LabVIEW
 *      Version: v1.00
 *      Note(s):
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2007 Keil - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/
 
#define LPC23XX_SAMPLE_DRIVER        1
#define SERIAL_PORT_IN_USE(port)    ((SER_MINPORT <= port) && (SER_MAXPORT >= port)) 
 
 
/* USER CODE =====================================================================*/
/* Add the correct header file for your selected CPU */ 
#if (LPC23XX_SAMPLE_DRIVER == 1)
/* <LPC2378 Example Code> */
#include <LPC23xx.H>                             /* LPC23xx definitions */
#else
#include <???.h>
#endif
/* END USER CODE ================================================================ */

#include "LVConfig.h"
#include "ARM_Serial.h"

/* USER CODE =====================================================================*/
/* Set minimum and maximum port numbers available 
   NOTE: Setting this to only the required ports will reduce RAM and code size    */   
#if (LPC23XX_SAMPLE_DRIVER == 1)
/* <LPC2378 Example Code> */
#define SER_MINPORT   0
#define SER_MAXPORT   1  /* Change this to 3 to enable the extra serial ports 2 and 3*/
#else
#define SER_MINPORT   ???
#define SER_MAXPORT   ???
#endif
/* END USER CODE ================================================================ */  


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
typedef __irq void (*ser_irq_fp)(void);           // RV irq function pointer typedef

#if SERIAL_PORT_IN_USE(0)
static __irq void ser_irq_0 (void);               // RV irq function prototype for port 0
#endif /* SERIAL_PORT_IN_USE(0) */

#if SERIAL_PORT_IN_USE(1)
static __irq void ser_irq_1 (void);               // RV irq function prototype for port 1
#endif /* SERIAL_PORT_IN_USE(1) */

#if SERIAL_PORT_IN_USE(2)
static __irq void ser_irq_2 (void);               // RV irq function prototype for port 2
#endif /* SERIAL_PORT_IN_USE(2) */

#if SERIAL_PORT_IN_USE(3)
static __irq void ser_irq_3 (void);               // RV irq function prototype for port 3
#endif /* SERIAL_PORT_IN_USE(3) */

/* USER CODE =====================================================================*/
/* Add / Remove function protoypes for any extra port interrupts */   
/* END USER CODE ================================================================ */  

typedef struct {
           int   serialDeviceNumber;       // = portNumber + 1
           unsigned int   status;
           unsigned long  baudRate;
           unsigned int   dataBits;
           unsigned int   stopBits;
           unsigned int   parity;
           
/* USER CODE =====================================================================*/
/* Add the correct defines for the registers within your UART. Also add 
   any other port specific variables that are required */
#if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */   
  volatile unsigned long *rbr;
  volatile unsigned long *thr;
  volatile unsigned long *dll;
  volatile unsigned long *dlm;
  volatile unsigned long *lcr;
  volatile unsigned long *lsr;
  volatile unsigned long *fdr;
  volatile unsigned long *fcr;
  volatile unsigned long *iir;
  volatile unsigned long *ier;
  volatile unsigned long *irqAddr;
  volatile unsigned long *irqCntl;  
  unsigned long          ser_irq_mask;              // Serial IRQ enable / disable bit mask
  unsigned long          txFifoEmpty      :1;       // 0x000(0000000X) Indicates transmit fifo is empty and needs to be kickstarted
  unsigned long          unused_flags     :31;      // 0xXXX(XXXXXXX0) Unused 
#else
  /* <your code>  */
#endif  
/* END USER CODE ================================================================ */
  
  ser_irq_fp             ser_irq;                   // Serial IRQ function
  ser_in_t               ser_in;
} SER_INFO;

static SER_INFO ser_Dev[SER_MAXPORT - SER_MINPORT + 1] = {
#if SERIAL_PORT_IN_USE(0)
  { 1, 
    SER_PORTCLOSED, 9600, 8, 0, 0, 

/* USER CODE =====================================================================*/
/* Add the correct register addresses within your first UART. Also add 
   any other variable initialisation required */
#if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */    
    (volatile unsigned long *)(UART0_BASE_ADDR + 0x00),   /* RBR */
    (volatile unsigned long *)(UART0_BASE_ADDR + 0x00),   /* THR */
    (volatile unsigned long *)(UART0_BASE_ADDR + 0x00),   /* DLL */
    (volatile unsigned long *)(UART0_BASE_ADDR + 0x04),   /* DLM */
    (volatile unsigned long *)(UART0_BASE_ADDR + 0x0C),   /* LCR */
    (volatile unsigned long *)(UART0_BASE_ADDR + 0x14),   /* LSR */
    (volatile unsigned long *)(UART0_BASE_ADDR + 0x28),   /* FDR */
    (volatile unsigned long *)(UART0_BASE_ADDR + 0x08),   /* FCR */
    (volatile unsigned long *)(UART0_BASE_ADDR + 0x08),   /* IIR */
    (volatile unsigned long *)(UART0_BASE_ADDR + 0x04),   /* IER */
    (volatile unsigned long *)(VIC_BASE_ADDR + 0x118),    /* IRQ Addr */
    (volatile unsigned long *)(VIC_BASE_ADDR + 0x218),    /* IRQ Cntl */
    0x00000040,                                           /* IRQ mask bit */
    1,                                                    /* txFifoEmpty */
    0,                                                    /* unused_flags */
#else
  /* <your code>  */
#endif    
/* END USER CODE ================================================================ */    
    
    ser_irq_0                                             /* IRQ function */
  },
#endif /* SERIAL_PORT_IN_USE(0) */

#if SERIAL_PORT_IN_USE(1) 
  { 2, 
    SER_PORTCLOSED, 9600, 8, 0, 0, 
    
/* USER CODE =====================================================================*/
/* Add the correct register addresses within your first UART. Also add 
   any other variable initialisation required */
#if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */   
    (volatile unsigned long *)(UART1_BASE_ADDR + 0x00),   /* RBR */
    (volatile unsigned long *)(UART1_BASE_ADDR + 0x00),   /* THR */
    (volatile unsigned long *)(UART1_BASE_ADDR + 0x00),   /* DLL */
    (volatile unsigned long *)(UART1_BASE_ADDR + 0x04),   /* DLM */
    (volatile unsigned long *)(UART1_BASE_ADDR + 0x0C),   /* LCR */
    (volatile unsigned long *)(UART1_BASE_ADDR + 0x14),   /* LSR */
    (volatile unsigned long *)(UART1_BASE_ADDR + 0x28),   /* FDR */
    (volatile unsigned long *)(UART1_BASE_ADDR + 0x08),   /* FCR */
    (volatile unsigned long *)(UART1_BASE_ADDR + 0x08),   /* IIR */
    (volatile unsigned long *)(UART1_BASE_ADDR + 0x04),   /* IER */
    (volatile unsigned long *)(VIC_BASE_ADDR + 0x11C),    /* IRQ Addr */
    (volatile unsigned long *)(VIC_BASE_ADDR + 0x21C),    /* IRQ Cntl */
    0x00000080,                                           /* IRQ mask bit */
    1,                                                    /* txFifoEmpty */
    0,                                                    /* unused_flags */
#else
  /* <your code>  */
#endif    
/* END USER CODE ================================================================ */    
    
    ser_irq_1                                             /* IRQ function */    
  },
#endif /* SERIAL_PORT_IN_USE(1) */

#if SERIAL_PORT_IN_USE(2) 
  { 3, 
    SER_PORTCLOSED, 9600, 8, 0, 0, 
    
/* USER CODE =====================================================================*/
/* Add the correct register addresses within your first UART. Also add 
   any other variable initialisation required */
#if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */   
    (volatile unsigned long *)(UART2_BASE_ADDR + 0x00),   /* RBR */
    (volatile unsigned long *)(UART2_BASE_ADDR + 0x00),   /* THR */
    (volatile unsigned long *)(UART2_BASE_ADDR + 0x00),   /* DLL */
    (volatile unsigned long *)(UART2_BASE_ADDR + 0x04),   /* DLM */
    (volatile unsigned long *)(UART2_BASE_ADDR + 0x0C),   /* LCR */
    (volatile unsigned long *)(UART2_BASE_ADDR + 0x14),   /* LSR */
    (volatile unsigned long *)(UART2_BASE_ADDR + 0x28),   /* FDR */
    (volatile unsigned long *)(UART2_BASE_ADDR + 0x08),   /* FCR */
    (volatile unsigned long *)(UART2_BASE_ADDR + 0x08),   /* IIR */
    (volatile unsigned long *)(UART2_BASE_ADDR + 0x04),   /* IER */
    (volatile unsigned long *)(VIC_BASE_ADDR + 0x170),    /* IRQ Addr */
    (volatile unsigned long *)(VIC_BASE_ADDR + 0x270),    /* IRQ Cntl */
    0x10000000,                                           /* IRQ mask bit */
    1,                                                    /* txFifoEmpty */
    0,                                                    /* unused_flags */
#else
  /* <your code>  */
#endif    
/* END USER CODE ================================================================ */    
    
    ser_irq_2                                             /* IRQ function */    
  },
#endif /* SERIAL_PORT_IN_USE(2) */

#if SERIAL_PORT_IN_USE(3)  
  { 4, 
    SER_PORTCLOSED, 9600, 8, 0, 0, 
    
/* USER CODE =====================================================================*/
/* Add the correct register addresses within your first UART. Also add 
   any other variable initialisation required */
#if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */   
    (volatile unsigned long *)(UART3_BASE_ADDR + 0x00),   /* RBR */
    (volatile unsigned long *)(UART3_BASE_ADDR + 0x00),   /* THR */
    (volatile unsigned long *)(UART3_BASE_ADDR + 0x00),   /* DLL */
    (volatile unsigned long *)(UART3_BASE_ADDR + 0x04),   /* DLM */
    (volatile unsigned long *)(UART3_BASE_ADDR + 0x0C),   /* LCR */
    (volatile unsigned long *)(UART3_BASE_ADDR + 0x14),   /* LSR */
    (volatile unsigned long *)(UART3_BASE_ADDR + 0x28),   /* FDR */
    (volatile unsigned long *)(UART3_BASE_ADDR + 0x08),   /* FCR */
    (volatile unsigned long *)(UART3_BASE_ADDR + 0x08),   /* IIR */
    (volatile unsigned long *)(UART3_BASE_ADDR + 0x04),   /* IER */
    (volatile unsigned long *)(VIC_BASE_ADDR + 0x174),    /* IRQ Addr */
    (volatile unsigned long *)(VIC_BASE_ADDR + 0x274),    /* IRQ Cntl */
    0x20000000,                                           /* IRQ mask bit */
    1,                                                    /* txFifoEmpty */
    0,                                                    /* unused_flags */
#else
  /* <your code>  */
#endif    
/* END USER CODE ================================================================ */    
    
    ser_irq_3                                             /* IRQ function */    
  } 
#endif /* SERIAL_PORT_IN_USE(3) */
/* USER CODE =====================================================================*/
/* Add / remove code for any extra port initialisation */   
/* END USER CODE ================================================================ */  
};

/*----------------------------------------------------------------------------
  Function Prototypes
 *---------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------
  open the UART
 *---------------------------------------------------------------------------*/
int ARM_Serial_OpenPort (int port, int *serDevNo) {

  if ( (port < SER_MINPORT) ||                      /* check the port number */
       (port > SER_MAXPORT)   ) {
    return (SER_PORTNOTKNOWN_ERR);
  }

  switch (port) {
    #if SERIAL_PORT_IN_USE(0)
    case 0:
      /* USER CODE =====================================================================*/
      /* Add code to initialise the pinout hardware for UART 0 */
      #if (LPC23XX_SAMPLE_DRIVER == 1)
      /* <LPC2378 Example Code> */     
      PINSEL0 |= 0x00000050;                           /* Enable TxD0 and RxD0 */
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */  
      break;
    #endif
    
    #if SERIAL_PORT_IN_USE(1)
    case 1:
      /* USER CODE =====================================================================*/
      /* Add code to initialise the pinout hardware for UART 1 */
      #if (LPC23XX_SAMPLE_DRIVER == 1)
      /* <LPC2378 Example Code> */     
      PINSEL7 |= 0x0000000F;                           /* Enable TxD1 and RxD1 */
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */      
      break;
    #endif
    
    #if SERIAL_PORT_IN_USE(2)      
    case 2:
      /* USER CODE =====================================================================*/
      /* Add code to initialise the pinout hardware for UART 2 */
      #if (LPC23XX_SAMPLE_DRIVER == 1)
      /* <LPC2378 Example Code> */     
      PINSEL0 |= 0x00500000;                           /* Enable TxD2 and RxD2 */
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */  
      break;
    #endif
    
    #if SERIAL_PORT_IN_USE(3)
    case 3:
      /* USER CODE =====================================================================*/
      /* Add code to initialise the pinout hardware for UART 3 */
      #if (LPC23XX_SAMPLE_DRIVER == 1)
      /* <LPC2378 Example Code> */     
      PINSEL0 |= 0x0000000A;                                    /* Enable TxD3 and RxD3 */
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */      
      break;      
    #endif
      
    default:
      ; // Should never happen (protected by port check above)
  };

  ser_Dev[port - SER_MINPORT].status = SER_PORTOPENED;
  *serDevNo = ser_Dev[port - SER_MINPORT].serialDeviceNumber;

  return (SER_OK);
}

/*----------------------------------------------------------------------------
  close the UART
 *---------------------------------------------------------------------------*/
int ARM_Serial_ClosePort (int serDevNo) {
  SER_INFO *pSerDev;

  serDevNo -= 1;                                  /* Convert the serDevNo to the port number */
  if ( (serDevNo < (SER_MINPORT)) ||              /* check the serial device number */
       (serDevNo > (SER_MAXPORT))   ) {
    return (SER_DEVNOTKNOWN_ERR);
  }

  pSerDev = &ser_Dev[(serDevNo - SER_MINPORT)];

  while(!pSerDev->txFifoEmpty) 
    os_dly_wait(10);                  /* wait for fifo to empty before closing port */

  switch (serDevNo - SER_MINPORT) {
    #if SERIAL_PORT_IN_USE(0)
    case 0:
      /* USER CODE =====================================================================*/
      /* Uninitialise the pinout hardware for UART 0 */
      #if (LPC23XX_SAMPLE_DRIVER == 1)
      /* <LPC2378 Example Code> */     
      PINSEL0 &= 0xFFFFFFAF;                          /* Disable TxD0 and RxD0 */
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */  
      break;
    #endif
    
    #if SERIAL_PORT_IN_USE(1)
    case 1:
      /* USER CODE =====================================================================*/
      /* Uninitialise the pinout hardware for UART 1 */
      #if (LPC23XX_SAMPLE_DRIVER == 1)
      /* <LPC2378 Example Code> */     
      PINSEL7 &= ~0x0000000F;                         /* Disable TxD1 and RxD1 */
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */      
      break;
    #endif
    
    #if SERIAL_PORT_IN_USE(2)
    case 2:
      /* USER CODE =====================================================================*/
      /* Uninitialise the pinout hardware for UART 2 */
      #if (LPC23XX_SAMPLE_DRIVER == 1)
      /* <LPC2378 Example Code> */     
      PINSEL0 &= 0xFFAFFFFF;                          /* Disable TxD2 and RxD2 */
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */      
      break;
    #endif
    
    #if SERIAL_PORT_IN_USE(3)
    case 3:
      /* USER CODE =====================================================================*/
      /* Uninitialise the pinout hardware for UART 3 */
      #if (LPC23XX_SAMPLE_DRIVER == 1)
      /* <LPC2378 Example Code> */     
      PINSEL0 &= 0xFFFFFFF5;                          /* Disable TxD3 and RxD3 */
      #else
      /* <your code>  */
      #endif
      /* END USER CODE ================================================================ */      
      break;
    #endif
    
    default:
      ; // Should never happen (protected by port check above)  
  }

  /* USER CODE =====================================================================*/
  /* Disable the interrupt in the VIC and UART controllers */
  #if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */     
  *(pSerDev->ier) = 0x00;                           /* Disable UART interrupts   */
  VICIntEnClr = pSerDev->ser_irq_mask;              /* Disable VIC interrupt     */
  #else
  /* <your code>  */
  #endif
  /* END USER CODE ================================================================ */      

  pSerDev->status   = SER_PORTCLOSED;
  
  return (SER_OK);
}

/*----------------------------------------------------------------------------
  initialize the UART
 *---------------------------------------------------------------------------*/
int ARM_Serial_InitPort (int serDevNo, unsigned long baudrate, unsigned int  databits,
                                unsigned int  parity,   unsigned int  stopbits) {

  unsigned char lcr_p, lcr_s, lcr_d;
  unsigned long dll;
  SER_INFO *pSerDev;

  serDevNo -= 1;                         /* Convert the serDevNo to the port number */
  if ( (serDevNo < (SER_MINPORT)) ||     /* check the serial device number */
       (serDevNo > (SER_MAXPORT))   ) {
    return (SER_DEVNOTKNOWN_ERR);
  }

  pSerDev = &ser_Dev[(serDevNo - SER_MINPORT)];

  switch (databits) {
    case 5:                              /* 5 Data bits   */
      lcr_d = 0x00;
    break;
    case 6:                              /* 6 Data bits   */
      lcr_d = 0x01;
    break;
    case 7:                              /* 7 Data bits   */
      lcr_d = 0x02;
    break;
    case 8:                              /* 8 Data bits   */
    default:
      lcr_d = 0x03;
    break;
  }

  switch (stopbits) {
    case 1:                              /* 1,5 Stop bits */
    case 2:                              /* 2   Stop bits */
      lcr_s = 0x04;
    break;
    case 0:                              /* 1   Stop bit  */
    default:
      lcr_s = 0x00;
    break;
  }

  switch (parity) {
    case 1:                              /* Parity Odd    */
      lcr_p = 0x08;
    break;
    case 2:                              /* Parity Even   */
      lcr_p = 0x18;
    break;
    case 3:                              /* Parity Mark   */
      lcr_p = 0x28;
    break;
    case 4:                              /* Parity Space  */
      lcr_p = 0x38;
    break;
    case 0:                              /* Parity None   */
    default:
      lcr_p = 0x00;
    break;
  }

  pSerDev->baudRate = baudrate;
  pSerDev->dataBits = databits;
  pSerDev->stopBits = stopbits;
  pSerDev->parity   = parity;
  
  IN_BUF_RESET(pSerDev);
  
  /* USER CODE =====================================================================*/
  /* Configure the UART hardware with the specified data for baud rate / etc  */
  #if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */  
  dll = ((UART_CLK / baudrate) / 16UL);

  *(pSerDev->fdr) = 0;                              /* Fractional divider not used      */
  *(pSerDev->lcr) = 0x80 | lcr_d | lcr_p | lcr_s;   /* Data bits, Parity,   Stop bit    */
  *(pSerDev->dll) = dll;                            /* Baud Rate @ 14,4 MHZ PCLK        */
  *(pSerDev->dlm) = (dll >> 8);                     /* High divisor latch               */
  *(pSerDev->lcr) = 0x00 | lcr_d | lcr_p | lcr_s;   /* DLAB = 0                         */
  *(pSerDev->fcr) = 0x87;                           /* Enable and reset FIFOs (irq=8 Chr in RxFIFO) */
  *(pSerDev->ier) = 0x03;                           /* Enable TX/RX interrupts          */
  pSerDev->txFifoEmpty = 1;                         /* TX fifo is empty                 */
  #else
  /* <your code>  */
  #endif
  /* END USER CODE ================================================================ */   

  /* USER CODE =====================================================================*/
  /* Set up and enable the UART interrupt in the interrupt controller */
  #if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */  
  *(pSerDev->irqAddr) = (unsigned long)pSerDev->ser_irq;  /* Set interrupt function           */
  *(pSerDev->irqCntl) = 0x0F;                             /* Set interrupt priority           */
  VICIntEnable = pSerDev->ser_irq_mask;                   /* Enable interrupt */ 
  #else
  /* <your code>  */
  #endif
  /* END USER CODE ================================================================ */   

  return (SER_OK);
}

/*----------------------------------------------------------------------------
  read data from UART
 *---------------------------------------------------------------------------*/
int ARM_Serial_Read (int serDevNo, char *buffer, const int *length) {
  SER_INFO *pSerDev;
  int bytesToRead;
  
  serDevNo -= 1;                         /* Convert the serDevNo to the port number */
  if ( (serDevNo < (SER_MINPORT)) ||     /* check the serial device number */
       (serDevNo > (SER_MAXPORT))   ) {
    return (SER_DEVNOTKNOWN_ERR);
  }
  pSerDev = &ser_Dev[(serDevNo - SER_MINPORT)];

  bytesToRead = *length;
  while (bytesToRead--) {
    while (IN_BUF_EMPTY(pSerDev));       /* Block until data is available if none */
    *buffer++ = IN_BUF_RD(pSerDev);
  }
  return (*length);  
}

/*----------------------------------------------------------------------------
  write data to the UART
 *---------------------------------------------------------------------------*/
int ARM_Serial_Write (int serDevNo, const char *buffer, int *length) {
  SER_INFO *pSerDev;
  int i, bytesToWrite, bytesWritten;

  serDevNo -= 1;                         /* Convert the serDevNo to the port number */
  if ( (serDevNo < (SER_MINPORT)) ||     /* check the serial device number */
       (serDevNo > (SER_MAXPORT))   ) {
    return (SER_DEVNOTKNOWN_ERR);
  }
  pSerDev = &ser_Dev[(serDevNo - SER_MINPORT)];

  // Write *length bytes
  bytesToWrite = *length;
  bytesWritten = bytesToWrite;

  /* USER CODE =====================================================================*/
  /* Write the data to the transmit section of the UART hardware */
  #if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */  
  /* Lets just use the 16 byte hardware FIFO to buffer outgoing data */
  while (bytesToWrite) {
    if (pSerDev->txFifoEmpty) {
      for (i=16; (bytesToWrite && i);i--) {
        pSerDev->txFifoEmpty = 0;
        *(pSerDev->thr) = *buffer++;                // Write to the Tx FIFO
        bytesToWrite--;
      }      
    }
    // Else: Wait for the fifo to be empty
  }
  #else
  /* <your code>  */
  #endif
  /* END USER CODE ================================================================ */     

  return (bytesWritten); 
}

/*----------------------------------------------------------------------------
  check if character(s) are available at the serial interface
 *---------------------------------------------------------------------------*/
int ARM_Serial_AvailChar (int serDevNo, int *availChar) {
  SER_INFO *pSerDev;

  serDevNo -= 1;                         /* Convert the serDevNo to the port number */
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
static __irq void ser_irq_0 (void) {
  SER_INFO *pSerDev = &ser_Dev[0 - SER_MINPORT];
  
  /* USER CODE =====================================================================*/
  /* Handle serial interrupts, including writing received data to the 
     receive data register for UART 0 */
  #if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */  
  volatile unsigned long iir;
  
  iir = *(pSerDev->iir);
   
  if ((iir & 0x4) || (iir & 0xC)) {                           // RDA or CTI pending
    while (*(pSerDev->lsr) & 0x01) {                          // Rx FIFO is not empty
      IN_BUF_WR(pSerDev, *(pSerDev->rbr));                    // Read Rx FIFO to buffer  
    }
  }
  if ((iir & 0x2)) {                                          // TXMIS pending
    pSerDev->txFifoEmpty = 1;
  }

  VICVectAddr = 0;
  #else
  /* <your code>  */
  #endif
  /* END USER CODE ================================================================ */     
}

#endif /* SERIAL_PORT_IN_USE(0) */


#if SERIAL_PORT_IN_USE(1)
/*----------------------------------------------------------------------------
  UART 1 interrupt
 *---------------------------------------------------------------------------*/
static __irq void ser_irq_1 (void) { 
  SER_INFO *pSerDev = &ser_Dev[1 - SER_MINPORT];
  
  /* USER CODE =====================================================================*/
  /* Handle serial interrupts, including writing received data to the 
     receive data register for UART 1 */
  #if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */  
  volatile unsigned long iir;
  
  iir = *(pSerDev->iir);
   
  if ((iir & 0x4) || (iir & 0xC)) {                           // RDA or CTI pending
    while (*(pSerDev->lsr) & 0x01) {                          // Rx FIFO is not empty
      IN_BUF_WR(pSerDev, *(pSerDev->rbr));                    // Read Rx FIFO to buffer  
    }
  }
  if ((iir & 0x2)) {                                          // TXMIS pending
    pSerDev->txFifoEmpty = 1;
  }

  VICVectAddr = 0;
  #else
  /* <your code>  */
  #endif
  /* END USER CODE ================================================================ */     
}

#endif /* SERIAL_PORT_IN_USE(1) */


#if SERIAL_PORT_IN_USE(2)
/*----------------------------------------------------------------------------
  UART 2 interrupt
 *---------------------------------------------------------------------------*/
static __irq void ser_irq_2 (void) { 
  SER_INFO *pSerDev = &ser_Dev[2 - SER_MINPORT];
  
  /* USER CODE =====================================================================*/
  /* Handle serial interrupts, including writing received data to the 
     receive data register for UART 2 */
  #if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */  
  volatile unsigned long iir;
  
  iir = *(pSerDev->iir);
   
  if ((iir & 0x4) || (iir & 0xC)) {                           // RDA or CTI pending
    while (*(pSerDev->lsr) & 0x01) {                          // Rx FIFO is not empty
      IN_BUF_WR(pSerDev, *(pSerDev->rbr));                    // Read Rx FIFO to buffer  
    }
  }
  if ((iir & 0x2)) {                                          // TXMIS pending
    pSerDev->txFifoEmpty = 1;
  }

  VICVectAddr = 0;
  #else
  /* <your code>  */
  #endif
  /* END USER CODE ================================================================ */     
}

#endif /* SERIAL_PORT_IN_USE(2) */


#if SERIAL_PORT_IN_USE(3)
/*----------------------------------------------------------------------------
  UART 3 interrupt
 *---------------------------------------------------------------------------*/
static __irq void ser_irq_3 (void) { 
  SER_INFO *pSerDev = &ser_Dev[3 - SER_MINPORT];
  
  /* USER CODE =====================================================================*/
  /* Handle serial interrupts, including writing received data to the 
     receive data register for UART 3 */
  #if (LPC23XX_SAMPLE_DRIVER == 1)
  /* <LPC2378 Example Code> */  
  volatile unsigned long iir;
  
  iir = *(pSerDev->iir);
   
  if ((iir & 0x4) || (iir & 0xC)) {                           // RDA or CTI pending
    while (*(pSerDev->lsr) & 0x01) {                          // Rx FIFO is not empty
      IN_BUF_WR(pSerDev, *(pSerDev->rbr));                    // Read Rx FIFO to buffer  
    }
  }
  if ((iir & 0x2)) {                                          // TXMIS pending
    pSerDev->txFifoEmpty = 1;
  }

  VICVectAddr = 0;
  #else
  /* <your code>  */
  #endif
  /* END USER CODE ================================================================ */     
}
#endif /* SERIAL_PORT_IN_USE(3) */


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


