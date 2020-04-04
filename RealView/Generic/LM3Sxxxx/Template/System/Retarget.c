/******************************************************************************/
/* Retarget.c: ITM retargetting for LabVIEW                                   */
/******************************************************************************/
/*                                                                            */                           
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2008 Keil - An ARM Company. All rights reserved.             */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include "LVConfig.h"             // LabVIEW configuration

#ifndef _Include_FileSupport
 #define __IO_RETARGET_FOPEN_ETC_ENABLED    0
#endif

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
// <e0>Enable ITM Terminal
//   <o1.0>Echo Characters Automatically<i>Echoes back characters typed in the serial window
//          <i>Default: Off
#ifndef __ITM_TERM_SUPPORT
 #define __ITM_TERM_SUPPORT   1
#endif

/* Echo characters back to the serial window automatically */
#ifndef __ITM_TERM_ECHO
 #define __ITM_TERM_ECHO       0
#endif
// </e>

// <e0>I/O Retargeting
// <i>Enable I/O retargeting
#ifndef __IO_RETARGET_ENABLED
 #define __IO_RETARGET_ENABLED  1
#endif

/* Possible stream sources/destinations */
#define __IO_RETARGET_STREAM_NONE           0
#define __IO_RETARGET_STREAM_RTA            1
#define __IO_RETARGET_STREAM_COM_0          2
#define __IO_RETARGET_STREAM_COM_1          3

// <o0>STDIO Routing (for printf(), scanf(), etc) <i>Select the route for standard input/output, and standard error
//   <0=> None
//   <1=> ITM Terminal
//   <3=> COM Port 1
#ifndef __IO_RETARGET_STDIO_ROUTE
 #define __IO_RETARGET_STDIO_ROUTE          1
#endif


// <o0.0>External file operations (eg RTL Flash File System)
// <i>Enable if __fopen(), __fclose(), __write(), __read(), __setfpos(), __flushbuf() and __get_flen() are defined
#ifndef __IO_RETARGET_FOPEN_ETC_ENABLED
 #define __IO_RETARGET_FOPEN_ETC_ENABLED    1
#endif

// </e>

// <<< end of configuration section >>>

/*----------------------------------------------------------------------------
 *      RTX User configuration part END
 *---------------------------------------------------------------------------*/


/*==============================================================================
   Helper MACROs
==============================================================================*/

// IO Retargetting is redirected to a useful location
#define __IO_RETARGET_STDIO_IN_USE  ((__IO_RETARGET_STDIO_ROUTE == __IO_RETARGET_STREAM_COM_1) || (__IO_RETARGET_STDIO_ROUTE == __IO_RETARGET_STREAM_RTA))


/*==============================================================================
   Checks for valid ITM Terminal and retargetting configurations
==============================================================================*/

/* If ITM Terminal is defined, Retargetting must also be defined */
#if (__ITM_TERM_SUPPORT == 1)
  #if (__IO_RETARGET_ENABLED == 0)
    #error ITM Terminal Support is enabled, but I/O Retargetting is not. I/O Retargetting is required to support the ITM Terminal. Enable I/O Retargetting or disable ITM Terminal support.
  #endif
#endif

/* If STDIO route is to ITM, then ITM Terminal must be defined */
#if ((__IO_RETARGET_ENABLED) && (__IO_RETARGET_STDIO_ROUTE == __IO_RETARGET_STREAM_RTA))
  #if (__ITM_TERM_SUPPORT == 0)
    #error STDIO is redirected to the Real-Time Agent Terminal, but Real-Time Agent Terminal Support is not enabled. Enable Real-Time Agent Terminal support, or redirect STDIO elsewhere.
  #endif
#endif


/*==============================================================================
   ITM Terminal Code
==============================================================================*/

#if (__ITM_TERM_SUPPORT == 1)

#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

/* Function prototypes */
unsigned int ITM_TermGetChar (void);
unsigned int ITM_TermPutChar (unsigned int ch);

/*
 *  Get a character from the terminal. This function blocks until a
 *  character is available
 *    Parameters:      None
 *    Return Value:    Character read from terminal
 */
/* NOT YET SUPPORTED */
unsigned int ITM_TermGetChar (void) {
  int ch = 0;
  return (ch);
}

/*
 *  Write a character to the terminal
 *    Parameters:      ch:    Character to write to the terminal
 *    Return Value:    Character written to the terminal
 */
unsigned int ITM_TermPutChar (unsigned int ch) {

  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return (ch);
}


#endif /* #if (__ITM_TERM_SUPPORT == 1) */




/*==============================================================================
   I/O Retargetting Code
==============================================================================*/

#if (__IO_RETARGET_ENABLED)

#include <stdio.h>
#include <string.h>
#include <rt_misc.h>
#include <rt_sys.h>

// Serial routines
extern int sendchar (int ch);
extern int getkey (void);

#pragma import(__use_no_semihosting_swi)

/* Standard IO device handles - arbitrary, but any real filesystem handles must be
   less than 0x8000. */
#define STDIN     0x8001
#define STDOUT    0x8002
#define STDERR    0x8003
#define ITMDBGIN  0x8004
#define ITMDBGOUT 0x8005

struct __FILE { 
  int handle; 
};

/*--------------------------- _ttywrch --------------------------------------*/

void _ttywrch (int ch) {
  ITM_TermPutChar (ch);
}


#ifdef __MICROLIB             // MicroLIB only has stdio stream, no other file IO

int fputc(int ch, FILE *f) {
  int retVal;
  #if (__IO_RETARGET_STDIO_IN_USE)        
    #if (__IO_RETARGET_STDIO_ROUTE == __IO_RETARGET_STREAM_COM_1)
      retVal = sendchar(ch);
    #elif (__IO_RETARGET_STDIO_ROUTE == __IO_RETARGET_STREAM_RTA)
      retVal = ITM_TermPutChar(ch);
    #endif
  #else /* #if (__IO_RETARGET_STDIO_IN_USE) */
    retVal = ch;        // If STDIO sent to nowhere, just return 'ch'
  #endif  
  return (retVal);
}

int fgetc (FILE *f) {
  int retVal;
  #if (__IO_RETARGET_STDIO_IN_USE)        
    #if (__IO_RETARGET_STDIO_ROUTE == __IO_RETARGET_STREAM_COM_1)
      retVal = getkey();
    #elif (__IO_RETARGET_STDIO_ROUTE == __IO_RETARGET_STREAM_RTA)
      retVal = ITM_TermGetChar();
    #endif
  #else /* #if (__IO_RETARGET_STDIO_IN_USE) */
    retVal = (-1);      // If STDIO sent to nowhere, just return '-1'
  #endif
  return (retVal);
}

int __backspace(FILE *f) {
  return (0);
}

#else /* #ifdef __MICROLIB */ 

/* Standard IO device name defines. */
const char __stdin_name[]  = "STDIN";
const char __stdout_name[] = "STDOUT";
const char __stderr_name[] = "STDERR";

#if (__IO_RETARGET_FOPEN_ETC_ENABLED)
  #include <file_config.h>    // Header file containing definitions for... 
  /* __fopen, __fclose, __write, __flushbuf, __read, __setfpos, __get_flen */
#endif

/*--------------------------- _sys_open -------------------------------------*/

FILEHANDLE _sys_open (const char *name, int openmode) {
  /* Register standard Input Output devices. */
  if (strcmp(name, "STDIN") == 0) {
    return (STDIN);
  }
  if (strcmp(name, "STDOUT") == 0) {
    return (STDOUT);     
  }
  if (strcmp(name, "STDERR") == 0) {      
    return (STDERR);
  }
  
  /* Register Real-Time Agent debug */
  if (strcmp(name, "ITMDBGIN") == 0) {
    return (ITMDBGIN);
  }
  if (strcmp(name, "ITMDBGOUT") == 0) {
    return (ITMDBGOUT);
  }

  #if (__IO_RETARGET_FOPEN_ETC_ENABLED)
    return (__fopen (name, openmode));
  #else
    return (0);
  #endif
}

/*--------------------------- _sys_close ------------------------------------*/

int _sys_close (FILEHANDLE fh) {
  if (fh > 0x8000) {
    return (0);
  }
  #if (__IO_RETARGET_FOPEN_ETC_ENABLED)
    return (__fclose (fh));
  #else
    return (0);
  #endif
}

/*--------------------------- _sys_write ------------------------------------*/

int _sys_write (FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode) {  
  int retVal = (-1);

  switch (fh) {
    case STDOUT:
    case STDERR:
      #if (__IO_RETARGET_STDIO_IN_USE)        
        #if (__IO_RETARGET_STDIO_ROUTE == __IO_RETARGET_STREAM_COM_1)
          for (  ; len; len--) {
            sendchar(*buf++);
          }
          retVal = 0;
        #elif (__IO_RETARGET_STDIO_ROUTE == __IO_RETARGET_STREAM_RTA)
          for (  ; len; len--) {
            ITM_TermPutChar(*buf++);
          }
          retVal = 0;
        #endif
      #else /* #if (__IO_RETARGET_STDIO_IN_USE) */
        retVal = 0;         // If STDIO sent to nowhere, just return OK
      #endif
    break;

    case ITMDBGOUT:
      for (  ; len; len--) {
        ITM_TermPutChar(*buf++);
      }
      retVal = 0;
    break;

    default:
      #if (__IO_RETARGET_FOPEN_ETC_ENABLED)
        retVal = __write(fh, buf, len);
      #endif
      ;
  }

  return (retVal);
}

/*--------------------------- _sys_read -------------------------------------*/

int _sys_read (FILEHANDLE fh, unsigned char *buf, unsigned len, int mode) {
  int retVal = (-1);

  switch (fh) {
    case STDIN:
      /* only read one byte at a time */
      #if (__IO_RETARGET_STDIO_IN_USE)        
        #if (__IO_RETARGET_STDIO_ROUTE == __IO_RETARGET_STREAM_COM_1)
          if (len) {
            *buf++ = getkey();
          }
          retVal = (len-1);
        #elif (__IO_RETARGET_STDIO_ROUTE == __IO_RETARGET_STREAM_RTA)
          if (len) {
            *buf++ = ITM_TermGetChar();
          }
          retVal = (len-1);
        #endif
      #else /* #if (__IO_RETARGET_STDIO_IN_USE) */
        retVal = 0;         // If STDIO sent to nowhere, just return OK
      #endif
    break;

    case ITMDBGIN:
      /* only read one byte at a time */
      if (len) {
        *buf++ = ITM_TermGetChar();
      }   
      retVal = (len-1);
    break;

    default:
      #if (__IO_RETARGET_FOPEN_ETC_ENABLED)
        retVal = __read(fh, buf, len);
      #endif
      ;
  }

  return (retVal);
}

/*--------------------------- _sys_istty ------------------------------------*/

int _sys_istty (FILEHANDLE fh) {
  if (fh > 0x8000) {
    return (1);
  }
  return (0);
}

/*--------------------------- _sys_seek -------------------------------------*/

int _sys_seek (FILEHANDLE fh, long pos) {
  if (fh > 0x8000) {
    return (0);
  }
  
  #if (__IO_RETARGET_FOPEN_ETC_ENABLED)
    return (__setfpos (fh, pos));
  #else
    return (-1);
  #endif
}

/*--------------------------- _sys_ensure -----------------------------------*/

int _sys_ensure (FILEHANDLE fh) {
  if (fh > 0x8000) {
    return (-1);
  }

  #if (__IO_RETARGET_FOPEN_ETC_ENABLED)
    return (__flushbuf (fh));
  #else
    return (-1);
  #endif
}

/*--------------------------- _sys_flen -------------------------------------*/

long _sys_flen (FILEHANDLE fh) {
  if (fh > 0x8000) {
    return (0);
  }

  #if (__IO_RETARGET_FOPEN_ETC_ENABLED)
    return (__get_flen (fh));
  #else
    return (0);
  #endif
}

/*--------------------------- _sys_tmpnam -----------------------------------*/

int _sys_tmpnam (char *name, int sig, unsigned maxlen) {
  return (1);
}

/*--------------------------- _sys_command_string ---------------------------*/

char *_sys_command_string (char *cmd, int len) {
  return (cmd);
}


#endif /* #ifdef __MICROLIB */

/*--------------------------- _sys_exit -------------------------------------*/

void _sys_exit (int return_code) {
  /* Endless loop. */
  while (1);
}

#endif /* #if (__IO_RETARGET_ENABLED) */


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
