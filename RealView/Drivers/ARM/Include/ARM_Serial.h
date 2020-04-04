/*----------------------------------------------------------------------------
 *      Name:    serial.h
 *      Purpose: serial port handling
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
 Serial interface related prototypes
 *---------------------------------------------------------------------------*/
extern int  ARM_Serial_OpenPort  (int port, int *serDevNo);
extern int  ARM_Serial_ClosePort (int serDevNo); 
extern int  ARM_Serial_InitPort  (int serDevNo, unsigned long baudrate, unsigned int databits, unsigned int parity, unsigned int stopbits);
extern int  ARM_Serial_Read (int serDevNo, char *buffer, const int *length);
extern int  ARM_Serial_Write (int serDevNo, const char *buffer, int *length); 
extern int  ARM_Serial_AvailChar (int serDevNo, int *availChar);
