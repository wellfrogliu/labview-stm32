/*----------------------------------------------------------------------------
 *      Name:    RTX_LV_irq.h
 *      Purpose: interrupt controller handling API
 *      Version: V1.00
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2007 Keil Software.
 *---------------------------------------------------------------------------*/


 
/*----------------------------------------------------------------------------
 IRQ interface type definitions
 *---------------------------------------------------------------------------*/ 
typedef void (*lv_irq_fp)(int isr_vector_num, int isr_param);
  
/*----------------------------------------------------------------------------
 IRQ interface prototypes
 *---------------------------------------------------------------------------*/  
extern int ARM_irq_init(void);
extern int ARM_irq_uninit(void);
extern int ARM_irq_register(int isr_vector_num,
                            int isr_param,
                            int isr_priority,
                            lv_irq_fp isr_startFunc,
                            lv_irq_fp isr_runFunc,                                                
                            lv_irq_fp isr_endFunc);
extern int  ARM_irq_unregister(int isr_vector); 
extern void ARM_irq_enable (int isr_vector);
extern void ARM_irq_disable (int isr_vector);
extern void ARM_irq_ack (int isr_vector);
