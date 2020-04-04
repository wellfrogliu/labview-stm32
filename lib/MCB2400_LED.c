/***************************************************************************************
	NI CONFIDENTIAL

	MCB2400_LED.c - Functions turn on or off LEDs. These LEDs display data sent over I2C.

	© 2003-2008 National Instruments Corporation.
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Embedded Development Module) and may not be copied, modified, or redistributed.  The code may
     only be used in accordance with the terms of the license agreement entered into between
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.
	
	Author: Nick Van Sickel
********************************************************************************************/
#ifndef MCB2400_LED_C
#define MCB2400_LED_C

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include <LPC23xx.H>                    // LPC23xx/LPC24xx definitions   
 #if defined(_Include_MCB2400_LEDFunctions) || defined(_Include_Everything)
//I2C defines
#define I2C_AA      0x00000004
#define I2C_SI      0x00000008
#define I2C_STO     0x00000010
#define I2C_STA     0x00000020
#define I2C_I2EN    0x00000040 

//static to keep track of LED value
static unsigned int leds = 0;
static unsigned char called = 0;


/* Function that initializes LEDs (also acts as init function for buttons 
   Note: called only once                   */
void MCB2400_LED_Init () {

  	if (!called)  //check if already enabled
	{

    leds = 0;

  	PCONP    |=  (1 << 7);       //Enable clock for I2C0                        

  	 //Initialize pins for SDA (P0.27) and SCL (P0.28) functions                
  	PINSEL1  &= ~0x03C00000;   
  	PINSEL1  |=  0x01400000;

  	 //Clear I2C state machine                                                  
  	I20CONCLR =  I2C_AA | I2C_SI | I2C_STA | I2C_I2EN;

  	// Setup I2C clock speed                                                    
  	I20SCLL   =  0x80;
  	I20SCLH   =  0x80;

	//Enable I2C
  	I20CONSET =  I2C_I2EN;
  	I20CONSET =  I2C_STO;

	called = 1;  //set flag
	}


} 

/* Function that turns on requested LED                                       */
void MCB2400_LEDOn (uInt8 num, int32 nErrIn, int32* pnErrOut) 
{
  	unsigned int val;
  	if (nErrIn) 
	{
		if (pnErrOut) 
		{
			*pnErrOut = nErrIn;
		}
		return;
	}

	//check if user OOB
	if( num > 4 )
	{
	  	if (pnErrOut) 
		{
			*pnErrOut = 1;
		}
		return;
	}

	val = (leds | (1 << num));

	MCB2400_LED_Init ();

	leds = val;

  	I20CONCLR =  I2C_AA | I2C_SI | I2C_STA | I2C_STO;
  	I20CONSET =  I2C_STA;
  	while (!(I20CONSET & I2C_SI));        // Wait for START                     
  	I20DAT    =  0xC0;
  	I20CONCLR =  I2C_SI | I2C_STA;
  	while (!(I20CONSET & I2C_SI));        // Wait for ADDRESS send             
  	I20DAT    =  0x18;
	I20CONCLR =  I2C_SI;
  	while (!(I20CONSET & I2C_SI));        // Wait for DATA send                 
  	I20DAT    =  (0x01 * ((val >> 0) & 1)) |
               (0x04 * ((val >> 1) & 1)) |
               (0x10 * ((val >> 2) & 1)) |
               (0x40 * ((val >> 3) & 1)) ;
	I20CONCLR =  I2C_SI;
  	while (!(I20CONSET & I2C_SI));        // Wait for DATA send                 
  	I20CONSET =  I2C_STO;
	I20CONCLR =  I2C_SI;
  	while (I20CONSET & I2C_STO);          // Wait for STOP 

    //set up GPIO to drive the 5th LED
	PINSEL4 &= ~(3 << (10*2));
	PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
	FIO2MASK &= ~(3 << 2*10);
	FIO2DIR |= (1 << 10);
	FIO2PIN &= ~(((val >> 4) & 1) << 10); 	 

  	if (pnErrOut) {
		*pnErrOut = 0;
	}                   
}

/* Function that turns off requested LED                                      */
void MCB2400_LEDOff (uInt8 num, int32 nErrIn, int32* pnErrOut) 
{
	unsigned int val;
	if (nErrIn) 
	{
		if (pnErrOut) 
		{
			*pnErrOut = nErrIn;
		}
		return;
	}

	if( num > 4 )
	{
	  	if (pnErrOut) 
		{
			*pnErrOut = 1;
		}
		return;
	}

	val = (leds & ~(1 << num));

    MCB2400_LED_Init ();    

	leds = val;

  	I20CONCLR =  I2C_AA | I2C_SI | I2C_STA | I2C_STO;
  	I20CONSET =  I2C_STA;
  	while (!(I20CONSET & I2C_SI));        // Wait for START
	                    
  	I20DAT    =  0xC0;
  	I20CONCLR =  I2C_SI | I2C_STA;
  	while (!(I20CONSET & I2C_SI));        // Wait for ADDRESS send
	             
  	I20DAT    =  0x18;
	I20CONCLR =  I2C_SI;
  	while (!(I20CONSET & I2C_SI));        // Wait for DATA send 
	               
  	I20DAT    =  (0x01 * ((val >> 0) & 1)) |
               (0x04 * ((val >> 1) & 1)) |
               (0x10 * ((val >> 2) & 1)) |
               (0x40 * ((val >> 3) & 1)) ;
	I20CONCLR =  I2C_SI;
  	while (!(I20CONSET & I2C_SI));        // Wait for DATA send
	                 
  	I20CONSET =  I2C_STO;
	I20CONCLR =  I2C_SI;
  	while (I20CONSET & I2C_STO);          // Wait for STOP     

    //set up GPIO to drive the 5th LED
	PINSEL4 &= ~(3 << (10*2));
	PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
	FIO2MASK &= ~(3 << 2*10);
	FIO2DIR |= (1 << 10);
	FIO2PIN |= ~(((val >> 4) & 1) << 10); 	 

	if (pnErrOut) 
	{
		*pnErrOut = 0;
	}
}


/* Function that outputs value to LEDs                                        */
void MCB2400_LEDSet(uInt8 value, int32 nErrIn, int32* pnErrOut) 
{
	unsigned int val;

 	if (nErrIn) 
	{
		if (pnErrOut) 
		{
			*pnErrOut = nErrIn;
		}
		return;
	}

	/*
	//removed since doesnt prevent an error, just annoys user..
	if(value > 0x1F)
	{
	  	if (pnErrOut) 
		{
			*pnErrOut = 1;
		}
		return;
	}
	*/

	val = value;

	MCB2400_LED_Init ();

	leds = val;

  	I20CONCLR =  I2C_AA | I2C_SI | I2C_STA | I2C_STO;
  	I20CONSET =  I2C_STA;
	while (!(I20CONSET & I2C_SI));        // Wait for START
	                     
  	I20DAT    =  0xC0;
  	I20CONCLR =  I2C_SI | I2C_STA;
  	while (!(I20CONSET & I2C_SI));        // Wait for ADDRESS send 
	             
  	I20DAT    =  0x08;
	I20CONCLR =  I2C_SI;
  	while (!(I20CONSET & I2C_SI));        // Wait for DATA send 
	               
  	I20DAT    =  (0x01 * ((val >> 0) & 1)) |
               (0x04 * ((val >> 1) & 1)) |
               (0x10 * ((val >> 2) & 1)) |
               (0x40 * ((val >> 3) & 1)) ;
	I20CONCLR =  I2C_SI;

  	while (!(I20CONSET & I2C_SI));        // Wait for DATA send  
	              
  	I20CONSET =  I2C_STO;
	I20CONCLR =  I2C_SI;
  	while (I20CONSET & I2C_STO);          // Wait for STOP

	 //set up GPIO to drive the 5th LED
	PINSEL4 &= ~(3 << (10*2));
	PINSEL10 = 0; // disable ETM interface if these pins are used as GPIO
	FIO2MASK &= ~(3 << 2*10);
	FIO2DIR |= (1 << 10);

	if((val >> 4) & 1)
	{
	 	FIO2PIN &= ~(1 << 10);	
	}
	else
	{
	 	FIO2PIN |= (1 << 10);
	}
	     
	if (pnErrOut) 
	{
		*pnErrOut = 0;
	}                
}

#endif
#endif
