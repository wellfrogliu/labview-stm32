/*----------------------------------------------------------------------------
 *      Name:    ARM_irq.c
 *      Purpose: interrupt controller handling implementation for LPC23xx
 *      Version: V1.00
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2007 Keil Software.
 *---------------------------------------------------------------------------*/
 
#include "ARM_irq.h"
#include <LPC23xx.H>                      // LPC23xx definitions

#if defined(_Include_LVISRSupport)
#include "LVCGenIntInit.h"
#endif

// Interrupt controller defines
#define pVICVectAddrBase    (volatile unsigned long *)(VIC_BASE_ADDR + 0x100)
#define pVICVectCntlBase    (volatile unsigned long *)(VIC_BASE_ADDR + 0x200)

#define MAX_INTERRUPTS      32            // Maximum interrupt vectors
#define ARM_IRQ_0            (1UL << 0)    // Watchdog Interrupt (WDINT)
#define ARM_IRQ_1            (1UL << 1)    // Software Interrupt Only
#define ARM_IRQ_2            (1UL << 2)    // ARM Core (DbgCommRx)
#define ARM_IRQ_3            (1UL << 3)    // ARM Core (DbgCommTx)
#define ARM_IRQ_4            (1UL << 4)    // TIMER0
#define ARM_IRQ_5            (1UL << 5)    // TIMER1
#define ARM_IRQ_6            (1UL << 6)    // UART0
#define ARM_IRQ_7            (1UL << 7)    // UART1
#define ARM_IRQ_8            (1UL << 8)    // PWM0, PWM1
#define ARM_IRQ_9            (1UL << 9)    // I2C0
#define ARM_IRQ_10           (1UL << 10)   // SPI0, SSP0
#define ARM_IRQ_11           (1UL << 11)   // SSP1 
#define ARM_IRQ_12           (1UL << 12)   // PLL 
#define ARM_IRQ_13           (1UL << 13)   // RTC 
#define ARM_IRQ_14           (1UL << 14)   // External Interrupt 0
#define ARM_IRQ_15           (1UL << 15)   // External Interrupt 1
#define ARM_IRQ_16           (1UL << 16)   // External Interrupt 2
#define ARM_IRQ_17           (1UL << 17)   // External Interrupt 3
#define ARM_IRQ_18           (1UL << 18)   // ADC0
#define ARM_IRQ_19           (1UL << 19)   // I2C1
#define ARM_IRQ_20           (1UL << 20)   // BOD
#define ARM_IRQ_21           (1UL << 21)   // Ethernet
#define ARM_IRQ_22           (1UL << 22)   // USB
#define ARM_IRQ_23           (1UL << 23)   // CAN
#define ARM_IRQ_24           (1UL << 24)   // SD / MMC
#define ARM_IRQ_25           (1UL << 25)   // GP DMA
#define ARM_IRQ_26           (1UL << 26)   // TIMER2
#define ARM_IRQ_27           (1UL << 27)   // TIMER3
#define ARM_IRQ_28           (1UL << 28)   // UART2
#define ARM_IRQ_29           (1UL << 29)   // UART3
#define ARM_IRQ_30           (1UL << 30)   // I2C2 
#define ARM_IRQ_31           (1UL << 31)   // I2S 
                  
#ifndef ARM_IRQ_AVAILABLE                         
#define ARM_IRQ_AVAILABLE    (ARM_IRQ_0  | \
                             /* In use by RTX ARM_IRQ_1  |*/ \
                             /* In use by RTX ARM_IRQ_2  |*/ \
                             /* In use by RTX ARM_IRQ_3  |*/ \
                             /* In use by RTX ARM_IRQ_4  |*/ \
                             ARM_IRQ_5  | \
                             ARM_IRQ_6  | \
                             ARM_IRQ_7  | \
                             ARM_IRQ_8  | \
                             ARM_IRQ_9  | \
                             ARM_IRQ_10 | \
                             ARM_IRQ_11 | \
                             ARM_IRQ_12 | \
                             ARM_IRQ_13 | \
                             ARM_IRQ_14 | \
                             ARM_IRQ_15 | \
                             ARM_IRQ_16 | \
                             ARM_IRQ_17 | \
                             ARM_IRQ_18 | \
                             ARM_IRQ_19 | \
                             ARM_IRQ_20 | \
                             ARM_IRQ_21 | \
                             ARM_IRQ_22 | \
                             ARM_IRQ_23 | \
                             ARM_IRQ_24 | \
                             ARM_IRQ_25 | \
                             ARM_IRQ_26 | \
                             ARM_IRQ_27 | \
                             ARM_IRQ_28 | \
                             ARM_IRQ_29 | \
                             ARM_IRQ_30 | \
                             ARM_IRQ_31)
#endif /* #ifndef ARM_IRQ_AVAILABLE */

#define RESERVED_IRQ        0xFFFFFFFF    // Inidcates an IRQ is reserved for use directly by a driver

typedef __irq void (*ARM_irq_fp)(void);    // RV irq function call prototype


typedef struct __ISRInfo {
  lv_irq_fp isr_startFunc;
	lv_irq_fp isr_runFunc;
  lv_irq_fp isr_endFunc;
	int isr_param;
  int isr_vector_num;
} ISRInfo;

ISRInfo gISRInfo[MAX_INTERRUPTS];


// Interrupt vector functions
#define ARM_irq(int_vector)      ISRInfo *pIsr = &gISRInfo[int_vector];                                                \
                                if (pIsr->isr_startFunc) pIsr->isr_startFunc(pIsr->isr_vector_num, pIsr->isr_param);  \
                                pIsr->isr_runFunc(pIsr->isr_vector_num, pIsr->isr_param);                             \
                                if (pIsr->isr_endFunc) pIsr->isr_endFunc(pIsr->isr_vector_num, pIsr->isr_param);

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_0)
static __irq void ARM_irq_0(void) { ARM_irq(0); }   
#else
#define ARM_irq_0    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_1)
static __irq void ARM_irq_1(void) { ARM_irq(1); }
#else
#define ARM_irq_1    0   
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_2)
static __irq void ARM_irq_2(void) { ARM_irq(2); }
#else
#define ARM_irq_2    0   
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_3)
static __irq void ARM_irq_3(void) { ARM_irq(3); }
#else
#define ARM_irq_3    0   
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_4)
static __irq void ARM_irq_4(void) { ARM_irq(4); }
#else
#define ARM_irq_4    0   
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_5)
static __irq void ARM_irq_5(void) { ARM_irq(5); }
#else
#define ARM_irq_5    0  
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_6)
static __irq void ARM_irq_6(void) { ARM_irq(6); }  
#else
#define ARM_irq_6    0 
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_7)
static __irq void ARM_irq_7(void) { ARM_irq(7); }
#else
#define ARM_irq_7    0  
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_8)
static __irq void ARM_irq_8(void) { ARM_irq(8); }
#else
#define ARM_irq_8    0   
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_9)
static __irq void ARM_irq_9(void) { ARM_irq(9); }  
#else
#define ARM_irq_9    0 
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_10)
static __irq void ARM_irq_10(void) { ARM_irq(10); }  
#else
#define ARM_irq_10    0 
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_11)
static __irq void ARM_irq_11(void) { ARM_irq(11); }  
#else
#define ARM_irq_11    0 
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_12)
static __irq void ARM_irq_12(void) { ARM_irq(12); }  
#else
#define ARM_irq_12    0 
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_13)
static __irq void ARM_irq_13(void) { ARM_irq(13); }   
#else
#define ARM_irq_13    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_14)
static __irq void ARM_irq_14(void) { ARM_irq(14); }  
#else
#define ARM_irq_14    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_15)
static __irq void ARM_irq_15(void) { ARM_irq(15); }   
#else
#define ARM_irq_15    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_16)
static __irq void ARM_irq_16(void) { ARM_irq(16); }   
#else
#define ARM_irq_16    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_17)
static __irq void ARM_irq_17(void) { ARM_irq(17); }  
#else
#define ARM_irq_17    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_18)
static __irq void ARM_irq_18(void) { ARM_irq(18); }   
#else
#define ARM_irq_18    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_19)
static __irq void ARM_irq_19(void) { ARM_irq(19); }  
#else
#define ARM_irq_19    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_20)
static __irq void ARM_irq_20(void) { ARM_irq(20); }  
#else
#define ARM_irq_20    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_21)
static __irq void ARM_irq_21(void) { ARM_irq(21); }  
#else
#define ARM_irq_21    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_22)
static __irq void ARM_irq_22(void) { ARM_irq(22); }  
#else
#define ARM_irq_22    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_23)
static __irq void ARM_irq_23(void) { ARM_irq(23); } 
#else
#define ARM_irq_23    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_24)
static __irq void ARM_irq_24(void) { ARM_irq(24); }  
#else
#define ARM_irq_24    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_25)
static __irq void ARM_irq_25(void) { ARM_irq(25); }  
#else
#define ARM_irq_25    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_26)
static __irq void ARM_irq_26(void) { ARM_irq(26); } 
#else
#define ARM_irq_26    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_27)
static __irq void ARM_irq_27(void) { ARM_irq(27); }   
#else
#define ARM_irq_27    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_28)
static __irq void ARM_irq_28(void) { ARM_irq(28); }   
#else
#define ARM_irq_28    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_29)
static __irq void ARM_irq_29(void) { ARM_irq(29); }  
#else
#define ARM_irq_29    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_30)
static __irq void ARM_irq_30(void) { ARM_irq(30); }  
#else
#define ARM_irq_30    0
#endif

#if (ARM_IRQ_AVAILABLE & ARM_IRQ_31)
static __irq void ARM_irq_31(void) { ARM_irq(31); }  
#else
#define ARM_irq_31    0
#endif

/* Interrupt vector functions LUT */
static const ARM_irq_fp ARM_isr_vector_func[] = {
  ARM_irq_0,
  ARM_irq_1,
  ARM_irq_2,
  ARM_irq_3,
  ARM_irq_4,
  ARM_irq_5,
  ARM_irq_6,
  ARM_irq_7,
  ARM_irq_8,
  ARM_irq_9,
  ARM_irq_10,
  ARM_irq_11,
  ARM_irq_12,
  ARM_irq_13,
  ARM_irq_14,
  ARM_irq_15,
  ARM_irq_16,
  ARM_irq_17,
  ARM_irq_18,
  ARM_irq_19,
  ARM_irq_20,
  ARM_irq_21,
  ARM_irq_22,
  ARM_irq_23,
  ARM_irq_24,
  ARM_irq_25,
  ARM_irq_26,
  ARM_irq_27,
  ARM_irq_28,
  ARM_irq_29,
  ARM_irq_30,
  ARM_irq_31
};


// Function Prototypes


/* Initialise the interrupt controller */
int ARM_irq_init(void) {
  int i;
  
	for (i = 0; i < MAX_INTERRUPTS; i++) {
    if (ARM_IRQ_AVAILABLE & (1UL << i)) {
      gISRInfo[i].isr_runFunc = 0;
      gISRInfo[i].isr_param = 0;
      gISRInfo[i].isr_vector_num = 0;
    }
    else {
      gISRInfo[i].isr_vector_num = RESERVED_IRQ;  
    }
	}

  return 1;
}

/* Uninitialise the interrupt controller */
int ARM_irq_uninit(void) {
  int i;
  
	for (i = 0; i < MAX_INTERRUPTS; i++) {
    ARM_irq_unregister(i);
    gISRInfo[i].isr_vector_num = RESERVED_IRQ;
	}

  return 1;
}

/* Register an interrupt with interrupt controller */
int  ARM_irq_register(int isr_vector_num,
                        int isr_param,
                        int isr_priority,
                        lv_irq_fp isr_startFunc,
                        lv_irq_fp isr_runFunc,                                                
                        lv_irq_fp isr_endFunc) {
  ISRInfo *pIsr = &gISRInfo[isr_vector_num];
  volatile unsigned long *pVICVectAddr = pVICVectAddrBase + isr_vector_num;
  volatile unsigned long *pVICVectCtrl = pVICVectCntlBase + isr_vector_num;  
  int retVal = 0;
    
  if (isr_vector_num < MAX_INTERRUPTS) {
    if (!pIsr->isr_vector_num) {
      if (isr_runFunc) {
      
        // Hardare interaction
        *pVICVectAddr = (unsigned long)ARM_isr_vector_func[isr_vector_num];
        *pVICVectCtrl = isr_priority;
      
        pIsr->isr_startFunc = (lv_irq_fp)isr_startFunc;
        pIsr->isr_runFunc = (lv_irq_fp)isr_runFunc;
        pIsr->isr_endFunc = (lv_irq_fp)isr_endFunc;
        pIsr->isr_param = isr_param;
        pIsr->isr_vector_num = isr_vector_num;
        retVal = 1;      
      }
    }
  }  

  return (retVal);
}
             
/* Unregister an interrupt from the interrupt controller */              
int  ARM_irq_unregister(int isr_vector_num) {
  ISRInfo *pIsr = &gISRInfo[isr_vector_num];
  volatile unsigned long *pVICVectAddr = pVICVectAddrBase + isr_vector_num;
  volatile unsigned long *pVICVectCtrl = pVICVectCntlBase + isr_vector_num;
  int retVal = 0;
    
  if (isr_vector_num < MAX_INTERRUPTS) {
    if ((pIsr->isr_vector_num == isr_vector_num)) {
      ARM_irq_disable(isr_vector_num);
    
      // Hardare interaction
      *pVICVectAddr = (unsigned long)0;
      *pVICVectCtrl = 0;
    
      pIsr->isr_startFunc = 0;
      pIsr->isr_runFunc = 0;
      pIsr->isr_endFunc = 0;
      pIsr->isr_param = 0;
      pIsr->isr_vector_num = 0;
      retVal = 1;
    }
  }  

  return (retVal);
}

/* Enable an interrupt */
void ARM_irq_enable (int isr_vector_num) {
  ISRInfo *pIsr = &gISRInfo[isr_vector_num];
  
  if (pIsr->isr_vector_num == isr_vector_num) {
    VICIntEnable = 1 << isr_vector_num;
  }
}

/* Disable an interrupt */
void ARM_irq_disable (int isr_vector_num) {
  ISRInfo *pIsr = &gISRInfo[isr_vector_num];
  
  if (pIsr->isr_vector_num == isr_vector_num) {
    VICIntEnClr = 1 << isr_vector_num;
  }
}

/* Acknowledge an interrupt */
void ARM_irq_ack (int isr_vector_num) {
  ISRInfo *pIsr = &gISRInfo[isr_vector_num];
  
  if (pIsr->isr_vector_num == isr_vector_num) {
    VICVectAddr = 0;
  }
}



