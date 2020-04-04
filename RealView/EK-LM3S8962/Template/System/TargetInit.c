/******************************************************************************/
/* Init.c: Target specific initialisation / shutdown                          */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2007 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include "LVSysIncludes.h"
#include "LVDefs.h"
#include "LVConfig.h"

#include "RLARM_TCPWrapper.h"

#include <LM3Sxxxx.H>

extern void init_serial (void);

#if (SocketSupport)
extern int RTX_tcp_uninit (void);
#endif

#if (UsesSerialDebugger)
  extern int gDbgSerialPort;
#endif

#if 0
/*
 * These arrays map integers to GPIO port addresses and int vectors.  It is
 * used from several places to look up GPIO ports based on an index.
 */
unsigned long g_ulGPIOPortMap[] =
{
    GPIO_PORTA_BASE, GPIO_PORTB_BASE, GPIO_PORTC_BASE, GPIO_PORTD_BASE,
    GPIO_PORTE_BASE, GPIO_PORTF_BASE, GPIO_PORTG_BASE
};

unsigned long g_ulGPIOIntMap[] =
{
    INT_GPIOA, INT_GPIOB, INT_GPIOC, INT_GPIOD, INT_GPIOE, INT_GPIOF, INT_GPIOG
};
#endif

void ARM_TargetInit(void) {
    /* Enable all the GPIO ports here rather than in bits and
     * pieces elsewhere throughout the support code.
     * NOTE: _SET2_ is deprecated.  They need to be changed to
     *       _RCGC2_ whenever the Keil tree is updated with more
     *       recent DriverLib.
     */
    HWREG(SYSCTL_RCGC2) |= SYSCTL_SET2_GPIOA | SYSCTL_SET2_GPIOB |
                           SYSCTL_SET2_GPIOC | SYSCTL_SET2_GPIOD |
                           SYSCTL_SET2_GPIOE | SYSCTL_SET2_GPIOF |
                           SYSCTL_SET2_GPIOG;

    /*
     * Set the SVC interrupt priority higher than SysTick, which is
     * set to 7 in RTX.
     */
    IntPrioritySet(FAULT_SVCALL, 6 << 5);

  #if (SocketSupport)
    #if (UsesTCPDebugger)
        //DebugSetIP(ipaddr);
    #endif

    RTX_tcp_init ();
  #endif

  /* Set the Serial debug port */
  #if (UsesSerialDebugger)
    gDbgSerialPort = LV_DBUG_UART_PORT;
  #endif

  /* Initialise RTA redirected serial port */
  #if (__IO_RETARGET_STDIO_ROUTE != __IO_RETARGET_STREAM_RTA)
    init_serial();              // Initialise the RTA Retargetting serial port
  #endif
}

void ARM_TargetUnInit(void) {
  #if (SocketSupport)
    RTX_tcp_uninit ();
  #endif
}
