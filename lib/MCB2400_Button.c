/***************************************************************************************
	NI CONFIDENTIAL

	MCB2400_Button.c - Function to detect if the MCB2400 buttons are pressed - no debounce.
	                   This includes one button on EINT0 as well as 4 buttons on the I2C.

	© 2003-2008 National Instruments Corporation.
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Embedded Development Module) and may not be copied, modified, or redistributed.  The code may
     only be used in accordance with the terms of the license agreement entered into between
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.
	
	Author: Nick Van Sickel
********************************************************************************************/
#ifndef MCB2400_BUTTON_C
#define MCB2400_BUTTON_C

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include "CCGArrSupport.h"
#include <LPC23xx.H>                    /* LPC23xx/LPC24xx definitions                */

/* Push Button Definitions */
#define S2     0x00000400  /* P2.10 */

//I2C definitions
#define I2C_AA      0x00000004
#define I2C_SI      0x00000008
#define I2C_STO     0x00000010
#define I2C_STA     0x00000020
#define I2C_I2EN    0x00000040 

extern void MCB2400_LED_Init (void);

 // Function that indicates which buttons are pressed
void MCB2400_IsButtonPressed(int32 nErrIn, int32* pnErrOut, uInt8 button, unsigned char *pbPressed) 
{
  	if (nErrIn) 
	{
		if (pnErrOut) 
		{
			*pnErrOut = nErrIn;
		}
		return;
	}


	if(button > 4)
	{
		if (pnErrOut)
		{
		 	*pnErrOut = 1;
		}
		return;
	}

	MCB2400_LED_Init ();

  	I20CONCLR =  I2C_AA | I2C_SI | I2C_STA | I2C_STO;
  	I20CONSET =  I2C_STA;
  	while (!(I20CONSET & I2C_SI));        // Wait for START
	                     
	I20DAT    =  0xC0;   //C0 = write operation
  	I20CONCLR =  I2C_SI | I2C_STA;
  	while (!(I20CONSET & I2C_SI));        // Wait for write command send
	 
	I20DAT    =  0x00;  //00 = no auto-increment, read device pins 0-7
	I20CONCLR =  I2C_SI;
  	while (!(I20CONSET & I2C_SI));        // Wait for control send 

  	I20CONSET =  I2C_STA;
	I20CONCLR =  I2C_SI;
  	while (!(I20CONSET & I2C_SI));        // Wait for RE-START 
	                 
  	I20DAT    =  0xC1;   //C1 = read operation
  	I20CONCLR =  I2C_SI | I2C_STA;
  	while (!(I20CONSET & I2C_SI));        // Wait for read command send

	I20CONCLR =  I2C_SI;
  	while (!(I20CONSET & I2C_SI));        // Wait for DATA send 
  	
	//set up 5th switch on P2.10
    PINSEL4 &= ~(3 << (10*2));   
	PINSEL10 = 0;
	FIO2MASK &= ~(1 << 10);  
	FIO2DIR &= ~(1 << 10); 
	
	switch(button) {
	
	case 0:
		 *pbPressed = ((I20DAT & 0x01) == 0);
		 break;
	case 1:
		 *pbPressed = ((I20DAT & 0x02) == 0);
		 break;
	case 2:
		 *pbPressed = ((I20DAT & 0x04) == 0);
		 break;
	case 3:
		 *pbPressed = ((I20DAT & 0x08) == 0);
		 break;
	case 4:               
		 *pbPressed = ((FIO2PIN & S2) == 0);              
		 break;
    default:
		 if (pnErrOut) {
		 	*pnErrOut = 1;
		 }
		 break;
    }
              
  	I20CONSET =  I2C_STO;
	I20CONCLR =  I2C_SI;
  	while (I20CONSET & I2C_STO);          // Wait for STOP 
  
  	if (pnErrOut) {
		*pnErrOut = 0;
	}
}
#endif

