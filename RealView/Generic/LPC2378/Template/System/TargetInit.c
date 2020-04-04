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

extern void init_serial (void);

#if (SocketSupport)
extern int RTX_tcp_uninit (void);
#endif

#if (UsesSerialDebugger)
  extern int gDbgSerialPort;
#endif

void ARM_TargetInit(void) {
  #if (SocketSupport)
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

  #ifdef _Include_FileSupport
  	finit();
  #endif
}

void ARM_TargetUnInit(void) {
  #if (SocketSupport)
    RTX_tcp_uninit ();
  #endif
}
