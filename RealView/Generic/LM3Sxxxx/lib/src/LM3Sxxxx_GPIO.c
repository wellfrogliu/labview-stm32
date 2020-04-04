//*****************************************************************************
//
// LM3Sxxxx_GPIO.c - Implements GPIO VI functions.
//
// Copyright (c) 2008 Luminary Micro, Inc.  All rights reserved.
// 
// Software License Agreement
// 
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
// 
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 114 of the Stellaris NI-LabVIEW support package.
//
//*****************************************************************************

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "hw_ints.h"
#include "gpio.h"
#include "rtl.h"

//
// Maps port numbers to GPIO port base addresses.
// Used by this function and by other functions.
//
unsigned long const g_ulGPIOPortMap[] =
{
    GPIO_PORTA_BASE, GPIO_PORTB_BASE, GPIO_PORTC_BASE, GPIO_PORTD_BASE,
    GPIO_PORTE_BASE, GPIO_PORTF_BASE, GPIO_PORTG_BASE, GPIO_PORTH_BASE
};

//
// Maps port numbers to the interrupt vector for the port.
// This array is used by other modules.
//
unsigned long const g_ulGPIOIntMap[] =
{
    INT_GPIOA, INT_GPIOB, INT_GPIOC, INT_GPIOD, INT_GPIOE, INT_GPIOF,
    INT_GPIOG, INT_GPIOH
};

//
// Keeps a bit for each port pin to see if it has been initted yet.
//
static unsigned char g_ucPortInit[8];
//
// Keeps track of whether pin is currently configured as read (0) or write (1)
//
static unsigned char g_ucPortReadWrite[8];

void
LM3Sxxxx_GPIO_Init(unsigned int uPort, unsigned char ucMask,
                   unsigned int uDirection)
{
	unsigned long ulBase;
	
	 //
     // Get the port base, based on the port number parameter
     //
     ulBase = g_ulGPIOPortMap[uPort];
	 
	 //
     // Configure the pin pads according to the specified type.
     //
     GPIODirModeSet(ulBase, ucMask, uDirection);

     //
     // Mark these pins as initted so they dont get initted again.
     //
	 g_ucPortInit[uPort] |= ucMask;
	 if(uDirection == GPIO_DIR_MODE_IN) {
		g_ucPortReadWrite[uPort] &= ~ucMask;
	}
	else {
		g_ucPortReadWrite[uPort] |= ucMask;
	}

}
				   
				   

//*****************************************************************************
//
// Read a GPIO port, with init
//
// uPort identifies the GPIO port.  0 means port A, 1 means B and so on.
// ucMask is a bit mask of the GPIO pins that are affected by this function.
// uType is the type of pin configuration, which should be one of the 3
//          valid input types using the values of the DriverLib calls
// pucState points to the storage for the port pins value
//
// This function is intended to be used with EIO.  It will read the value
// of specified pins of a GPIO port.  It will also initialize the
// configuration of those pins the first time it is called.
//
// The pin type is specified by the uType parameter.  This should be one
// of the DriverLib values GPIO_PIN_TYPE_STD, _WPU, or _WPD.  This value
// will be passed directly to the DriverLib call to configure the GPIO pin.
//
// The value of the pins is returned through the pucState parameter.
//
//*****************************************************************************
void
LM3Sxxxx_GPIO_Read_Port(unsigned int uPort, unsigned char ucMask,
                        unsigned int uType, unsigned char *pucState)
{
    unsigned long ulBase;

    //
    // Get the port base, based on the port number parameter
    //
    ulBase = g_ulGPIOPortMap[uPort];

    //
    // Check to see if all the pins are already initted
    //
    if(((g_ucPortInit[uPort] & ucMask) != ucMask) || ((~g_ucPortReadWrite[uPort] & ucMask) != ucMask))
    {
			LM3Sxxxx_GPIO_Init(uPort, ucMask, GPIO_DIR_MODE_IN);
            GPIOPadConfigSet(ulBase, ucMask, GPIO_STRENGTH_2MA, uType);

			// Short delay to allow GPIO to fully configure before reading input
			os_dly_wait(3);
    }

    //
    // Read the value of the specified pins and return it to the caller.
    //
    *pucState = GPIOPinRead(ulBase, ucMask);
}

//*****************************************************************************
//
// Write a GPIO port, with init
//
// uPort identifies the GPIO port.  0 means port A, 1 means B and so on.
// ucMask is a bit mask of the GPIO pins that are affected by this function.
// uType is the type of pin configuration, which should be one of the 3
//          valid input types using the values of the DriverLib calls
// uDrive is the drive strength of the pins, and should be a valid drive
//          strength value using the values of the DriverLib call
// ucState points to the value to write to the port
//
// This function is intended to be used with EIO.  It will write the value
// of specified pins to a GPIO port.  It will also initialize the
// configuration of those pins the first time it is called.
//
// The pin type is specified by the uType parameter.  This should be one
// of the DriverLib values GPIO_PIN_TYPE_STD, _WPU, or _WPD.  This value
// will be passed directly to the DriverLib call to configure the GPIO pin.
//
// The drive strength is specified by the uDrive parameter.  This should be
// one of the DriverLib values GPIO_STRENGTH_2MA, _4MA, _8MA, 8MA_SC.  This
// value will be passed directly to the DriverLib call.
//
//*****************************************************************************
void
LM3Sxxxx_GPIO_Write_Port(unsigned int uPort, unsigned char ucMask,
                         unsigned int uType, unsigned int uDrive,
                         unsigned char ucState)
{
    unsigned long ulBase;

    //
    // Get the port base, based on the port number parameter
    //
    ulBase = g_ulGPIOPortMap[uPort];

    //
    // Check to see if all the pins are already initted 
    //
    if(((g_ucPortInit[uPort] & ucMask) != ucMask) || ((g_ucPortReadWrite[uPort] & ucMask) != ucMask))
    {
        //
        // Configure the pin pads according to the specified type.
        //
		LM3Sxxxx_GPIO_Init(uPort, ucMask, GPIO_DIR_MODE_OUT);
        GPIOPadConfigSet(ulBase, ucMask, uDrive, uType);
    }

    //
    // Write the GPIO port value according to the input parameter and mask.
    //
    GPIOPinWrite(ulBase, ucMask, ucState);
}

//*****************************************************************************
//
// Read a GPIO pin, with init
//
// uPortPin identifies the GPIO port and pin.  The upper 4 bits identify
//          the port (A-H), and the lower 4 bits specify the pin number (0-7)
// uType is the type of pin configuration, which should be one of the 3
//          valid input types using the values of the DriverLib calls
// pucState points to the storage for the pin value, as a boolean
//
// This function is intended to be used with EIO.  It will read the value
// of 1 pin from a GPIO port.  It will also initialize the configuration of
// the pin the first time it is called.
//
// The pin type is specified by the uType parameter.  This should be one
// of the DriverLib values GPIO_PIN_TYPE_STD, _WPU, or _WPD.  This value
// will be passed directly to the DriverLib call to configure the GPIO pin.
//
// The value of the pin is returned through the pucState parameter, and is
// a boolean with a value of 1 or 0.
//
//*****************************************************************************
void
LM3Sxxxx_GPIO_Read_Pin(unsigned int uPortPin, unsigned int uType,
                       unsigned char *pucState)
{
    unsigned char ucRet;

    //
    // Call the read port function using a mask for just the single pin.
    //
    LM3Sxxxx_GPIO_Read_Port(uPortPin >> 4, 1 << (uPortPin & 7), uType, &ucRet);

    //
    // Convert the returned value into a boolean 1 or 0 (in case LV cares).
    //
    *pucState = ucRet ? 1 : 0;
}

//*****************************************************************************
//
// Write a GPIO pin, with init
//
// uPortPin identifies the GPIO port and pin.  The upper 4 bits identify
//          the port (A-H), and the lower 4 bits specify the pin number (0-7)
// uType is the type of pin configuration, which should be one of the 3
//          valid input types using the values of the DriverLib calls
// uDrive is the drive strength of the pin, and should be a valid drive
//          strength value using the values of the DriverLib call
// ucState points to the boolean state to set the pin
//
// This function is intended to be used with EIO.  It will write the value
// of 1 pin to a GPIO port.  It will also initialize the configuration of
// the pin the first time it is called.
//
// The pin type is specified by the uType parameter.  This should be one
// of the DriverLib values GPIO_PIN_TYPE_STD, _WPU, or _WPD.  This value
// will be passed directly to the DriverLib call to configure the GPIO pin.
//
// The drive strength is specified by the uDrive parameter.  This should be
// one of the DriverLib values GPIO_STRENGTH_2MA, _4MA, _8MA, 8MA_SC.  This
// value will be passed directly to the DriverLib call.
//
//*****************************************************************************
void
LM3Sxxxx_GPIO_Write_Pin(unsigned int uPortPin, unsigned int uType,
                        unsigned int uDrive, unsigned char ucState)
{
    //
    // Call the write port function using a mask for just the single pin.
    //
    LM3Sxxxx_GPIO_Write_Port(uPortPin >> 4, 1 << (uPortPin & 7), uType,
                             uDrive, ucState ? 0xFF : 0);
}

//*****************************************************************************
//
// Configure a GPIO port for input
//
// uPort identifies the GPIO port.  0 means port A, 1 means B and so on.
// ucMask is a bit mask of the GPIO pins that are affected by this function.
// uType is the type of pin configuration, which should be one of the 3
//          valid input types using the values of the DriverLib calls
//
// This function is intended to be used with EIO.  It will configure the
// GPIO port pins as specified by ucMask for input, according to the uType
// parameter.
//
// The pin type is specified by the uType parameter.  This should be one
// of the DriverLib values GPIO_PIN_TYPE_STD, _WPU, or _WPD.  This value
// will be passed directly to the DriverLib call to configure the GPIO pin.
//
//*****************************************************************************
void
LM3Sxxxx_GPIO_Read_Port_Config(unsigned int uPort, unsigned char ucMask,
                               unsigned int uType)
{
    unsigned long ulBase;

    //
    // Get the port base, based on the port number parameter
    //
    ulBase = g_ulGPIOPortMap[uPort];

    //
    // Configure the pin pads according to the specified type.
    //
    LM3Sxxxx_GPIO_Init(uPort, ucMask, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(ulBase, ucMask, GPIO_STRENGTH_2MA, uType);
}

//*****************************************************************************
//
// Configure a GPIO port for output
//
// uPort identifies the GPIO port.  0 means port A, 1 means B and so on.
// ucMask is a bit mask of the GPIO pins that are affected by this function.
// uType is the type of pin configuration, which should be one of the 3
//          valid input types using the values of the DriverLib calls
// uDrive is the drive strength of the pins, and should be a valid drive
//          strength value using the values of the DriverLib call
//
// This function is intended to be used with EIO.  It will configure the
// GPIO port pins as specified by ucMask for output, according to the uType
// and uDrive parameters.
//
// The pin type is specified by the uType parameter.  This should be one
// of the DriverLib values GPIO_PIN_TYPE_STD, _WPU, or _WPD.  This value
// will be passed directly to the DriverLib call to configure the GPIO pin.
//
// The drive strength is specified by the uDrive parameter.  This should be
// one of the DriverLib values GPIO_STRENGTH_2MA, _4MA, _8MA, 8MA_SC.  This
// value will be passed directly to the DriverLib call.
//
//*****************************************************************************
void
LM3Sxxxx_GPIO_Write_Port_Config(unsigned int uPort, unsigned char ucMask,
                                unsigned int uType, unsigned int uDrive)
{
    unsigned long ulBase;

    //
    // Get the port base, based on the port number parameter
    //
    ulBase = g_ulGPIOPortMap[uPort];

    //
    // Configure the pin pads according to the specified type.
    //
    LM3Sxxxx_GPIO_Init(uPort, ucMask, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(ulBase, ucMask, uDrive, uType);
}

//*****************************************************************************
//
// Configure a GPIO pin for input
//
// uPortPin identifies the GPIO port and pin.  The upper 4 bits identify
//          the port (A-H), and the lower 4 bits specify the pin number (0-7)
// uType is the type of pin configuration, which should be one of the 3
//          valid input types using the values of the DriverLib calls
//
// This function is intended to be used with EIO.  It will configure one
// GPIO pin for input, according to the uType parameter.
//
// The pin type is specified by the uType parameter.  This should be one
// of the DriverLib values GPIO_PIN_TYPE_STD, _WPU, or _WPD.  This value
// will be passed directly to the DriverLib call to configure the GPIO pin.
//
//*****************************************************************************
void
LM3Sxxxx_GPIO_Read_Pin_Config(unsigned int uPortPin, unsigned int uType)
{
    //
    // Call the port config function for the specific pin.
    //
    LM3Sxxxx_GPIO_Read_Port_Config(uPortPin >> 4, 1 << (uPortPin & 7), uType);
}

//*****************************************************************************
//
// Configure a GPIO pin for output
//
// uPortPin identifies the GPIO port and pin.  The upper 4 bits identify
//          the port (A-H), and the lower 4 bits specify the pin number (0-7)
// uType is the type of pin configuration, which should be one of the 3
//          valid input types using the values of the DriverLib calls
// uDrive is the drive strength of the pin, and should be a valid drive
//          strength value using the values of the DriverLib call
//
// This function is intended to be used with EIO.  It will configure one
// GPIO pin for output, according to the uType and uDrive parameters.
//
// The pin type is specified by the uType parameter.  This should be one
// of the DriverLib values GPIO_PIN_TYPE_STD, _WPU, or _WPD.  This value
// will be passed directly to the DriverLib call to configure the GPIO pin.
//
// The drive strength is specified by the uDrive parameter.  This should be
// one of the DriverLib values GPIO_STRENGTH_2MA, _4MA, _8MA, 8MA_SC.  This
// value will be passed directly to the DriverLib call.
//
//*****************************************************************************
void
LM3Sxxxx_GPIO_Write_Pin_Config(unsigned int uPortPin, unsigned int uType,
                               unsigned int uDrive)
{
    //
    // Call the port config function for the specific pin.
    //
    LM3Sxxxx_GPIO_Write_Port_Config(uPortPin >> 4, 1 << (uPortPin & 7),
                                    uType, uDrive);
}

#if 0
//*****************************************************************************
//
// Configures GPIO pins on a port.
//
// uPort identifies the GPIO port (0-7 --> A-H).
// uMask is the 8 bit mask of the bits to change.
// uType is the type of pin configuration.
// uDrive is the output drive strength.
// lErrIn is the input error value.
// plErrOut is the output error value.
//
// The GPIO port, A-H is specified by the uPort parameter.  Any or all of
// the 8 port bits can be configured by setting the bits of interest in the
// uMask parameter.
//
// uType is the pin configuration type (standard, open drain, etc) and the
// pin direction.  The values are the same as those used for the DriverLib
// GPIOPadConfigSet() call, except that the upper bit is used to indicate
// direction.  If the upper bit is a 1 then it is an output, otherwise input.
//
// The uDrive parameter is the output drive strength and uses the same
// values as the DriverLib call GPIOPadConfigSet().
//
// The lErrIn and plErrOut parameters are LabVIEW input and output errors.
//
//*****************************************************************************
void
LM3Sxxxx_GPIO_Configure(unsigned int uPort, unsigned int uMask,
                        unsigned int uType, unsigned int uDrive,
                        long lErrIn, long *plErrOut)
{
    unsigned long ulDir;
    unsigned long ulBase;

    //
    // NOTE: Assumes GPIO port has already been enabled.  This is taken
    // care of in TargetInit.c
    //

    //
    // Get the base address for the port.
    //
    ulBase = g_ulGPIOPortMap[uPort];

    //
    // Perform processing only if there is no error.
    //
    if(!lErrIn)
    {
        //
        // Determine if the pin type is input or output and set the direction.
        // The upper bit of the type is set for output.
        //
        ulDir = (uType & 0x80000000) ? GPIO_DIR_MODE_OUT : GPIO_DIR_MODE_IN;
        GPIODirModeSet(ulBase, uMask, ulDir);

        //
        // Configure the pad according to the specified type.
        // Ignore the upper bit which indicates direction.
        //
        GPIOPadConfigSet(ulBase, uMask, uDrive, uType & 0xFF);
    }

    //
    // Pass-through the input error to the output.
    //
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}

//*****************************************************************************
//
// Writes to GPIO pins on a port.
//
// uPort identifies the GPIO port (0-7 --> A-H).
// uMask is the 8 bit mask of the bits to change.
// uValue is the value of the bits to set.
// lErrIn is the input error value.
// plErrOut is the output error value.
//
// The GPIO port, A-H is specified by the Port parameter.  Any or all of
// the 8 port bits can be set by setting the bits of interest in the uMask
// parameter.
//
// The uValue parameter contains the value of the bits to write.  Only bits
// that have the equivalent mask bit set will be affected.  For example,
// to set port bit 2 to a 0 and port bit 3 to a 1, the uValue parameter
// should be 8, and the mask should be 0x0C (12d).  Any bits in the uValue
// parameter for which the equivalent mask bits are 0 will be unaffected
// in the GPIO port.
//
// The lErrIn and plErrOut parameters are LabVIEW input and output errors.
//
// The pins to set should be set as outputs prior to this call.
//
//*****************************************************************************
void
LM3Sxxxx_GPIO_Write(unsigned int uPort, unsigned int uMask,
                    unsigned int uValue, long lErrIn, long *plErrOut)
{
    //
    // Perform processing only if there is no error.
    //
    if(!lErrIn)
    {
        //
        // Set the GPIO outputs per parameters.
        // Equivalent DriverLib call:
        //  GPIOPinWrite(ulGpioPort[uPort], uMask, uValue);
        //
        HWREG(g_ulGPIOPortMap[uPort] + GPIO_O_DATA + (uMask << 2)) = uValue;
    }

    //
    // Pass-through the input error to the output.
    //
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}

//*****************************************************************************
//
// Reads from GPIO pins on a port.
//
// The uPort parameter identifies the GPIO port (0-7 --> A-H).
// The mask is the 8 bit mask of the bits to read.
//
// The GPIO port, A-H is specified by the Port parameter.  Any or all of
// the bits can be read by setting the bits of interest in the Mask
// parameter.
//
// For example, to read port bit 0 and 1, then bits 0 and 1 in the mask
// should be set (value 3).
//
// The return Value contains the value of the bits that were read.  For
// example, if bits 0 and 1 were set in the Mask, then bits 0 and 1 in the
// return value will contain the value of those bits, while the remaining
// bits in Value will be 0.
//
// The pins specified in the mask parameter will be set as inputs.
//
// No error checking is done on the inputs.  At the moment we assume
// this is only being called from well behaved code (the EIO model VI).
//
//*****************************************************************************
void
LM3Sxxxx_GPIO_Read(unsigned int uPort, unsigned int uMask,
                   unsigned char *pucValue, long lErrIn, long *plErrOut)
{
    //
    // Perform processing only if there is no error.
    //
    if(!lErrIn)
    {
        //
        // Read the specified GPIO pins and return the value to the caller.
        // Equivalent DriverLib call:
        //  *pucValue = (unsigned char)GPIOPinRead(ulGpioPort[uPort], uMask);
        *pucValue = (unsigned char)
                    HWREG(g_ulGPIOPortMap[uPort] + GPIO_O_DATA + (uMask << 2));
    }

    //
    // Pass-through the input error to the output.
    //
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}
#endif
