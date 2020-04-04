//*****************************************************************************
//
// LM3Sxxxx_PWM.c - Implements PWM VI functions
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
// This is part of revision 132 of the Stellaris NI-LabVIEW support package.
//
//*****************************************************************************

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_pwm.h"
#include "gpio.h"
#include "sysctl.h"
#include "pwm.h"
#include "lm3s_pins.h"

//
// Array to map PWM channel numbers to PWM generators.
//
static unsigned long const ulPWMGen[] =
{   PWM_GEN_0, PWM_GEN_0, PWM_GEN_1, PWM_GEN_1, PWM_GEN_2, PWM_GEN_2    };

//
// Maps the choice of PWM clock dividers
//
static unsigned long const ulPWMSysDiv[] =
{
    SYSCTL_PWMDIV_1,
    SYSCTL_PWMDIV_2,
    SYSCTL_PWMDIV_4,
    SYSCTL_PWMDIV_8,
    SYSCTL_PWMDIV_16,
    SYSCTL_PWMDIV_32,
    SYSCTL_PWMDIV_64
};

//
// Maps the PWM channel number to PWM output
//
static unsigned long const ulPWMOut[] =
{   PWM_OUT_0, PWM_OUT_1, PWM_OUT_2, PWM_OUT_3, PWM_OUT_4, PWM_OUT_5 };

static unsigned int bPwmOutInit = 0;
static unsigned int bPwmGenInit = 0;
static unsigned char bPwmFaultInit = 0;

void
LM3Sxxxx_PWM_Fault_Enable( unsigned int uChannelMask, unsigned int uEnableMask );

void
LM3Sxxxx_PWMGen_Initialize(unsigned int uChannel, unsigned int uFreq,
						   unsigned int uFlags, unsigned int uDivider)
{
		unsigned long ulTimeBase;
		unsigned long ulPwmGenBit = 1 << (uChannel >> 1);
		
		if(!(bPwmGenInit & ulPwmGenBit)) {
		
			bPwmGenInit |= ulPwmGenBit;

			//
			// enable the PWM peripheral, set the PWM system clock divider
			//
			SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
			SysCtlPWMClockSet(ulPWMSysDiv[uDivider]);
		
			//
			// set the counter mode based on left or center alignment
			//
			if(uFlags & 1)
			{
				//
				// center aligned - use up/down mode
				//
				PWMGenConfigure(PWM_BASE, ulPWMGen[uChannel],
                            PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC |
                            PWM_GEN_MODE_DBG_RUN);
			}
			else
			{
				//
				// left aligned, use count-down mode
				//
				PWMGenConfigure(PWM_BASE, ulPWMGen[uChannel],
                            PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC |
                            PWM_GEN_MODE_DBG_RUN);
			}
		
			//
			// find the system clock and adjust by the PWM divider
			//
			ulTimeBase = SysCtlClockGet() / (1 << uDivider);

			//
			// set the PWM frequency
			//
			PWMGenPeriodSet(PWM_BASE, ulPWMGen[uChannel], ulTimeBase / uFreq);
		}
}

void
LM3Sxxxx_PWMOut_Initialize(unsigned int uChannel, unsigned int uFlags)
{

		unsigned long ulPwmOutBit = 1 << uChannel;
		
		if(!(bPwmOutInit & ulPwmOutBit)) {
		
			bPwmOutInit |= ulPwmOutBit;
		
			//
			// set up the GPIO pin for the PWM output
			// The GPIO port was already enabled during system init
			//
			GPIOPinTypePWM(g_ulPWMPort[uChannel], g_ulPWMPin[uChannel]);

			//
			// set the output fault and invert properties according to the flags
			//
			//PWMOutputFault(PWM_BASE, ulPwmOutBit, (uFlags & 8) ? 1 : 0);
			LM3Sxxxx_PWM_Fault_Enable(ulPwmOutBit, ((uFlags & 8) ? 1 : 0 ) << uChannel );
			PWMOutputInvert(PWM_BASE, ulPwmOutBit, (uFlags & 4) ? 1 : 0);

			//
			// set the pulse width to 1 so the output is mostly "off".  If we set this to 0
			// then the output will go high
			//
			PWMPulseWidthSet(PWM_BASE, ulPWMOut[uChannel], 1);

			//
			// enable the PWM output
			//
			PWMOutputState(PWM_BASE, ulPwmOutBit, 1);

			//
			// enable the PWM generator
			//	
			PWMGenEnable(PWM_BASE, ulPWMGen[uChannel]);
		}
		
}

void
LM3Sxxxx_PWM_Init_Out(unsigned int uChannel, unsigned int uWidth,
                      unsigned int uFreq, unsigned int uFlags,
                      unsigned int uDivider)
{
	
	//
    // Initialize the PWM generator if it has not been initted before.
    //	
	LM3Sxxxx_PWMGen_Initialize(uChannel, uFreq, uFlags, uDivider);
	
	//
    // Initialize the PWM output if it has not been initted before.
    //
	LM3Sxxxx_PWMOut_Initialize(uChannel, uFlags);

	//
	// Prevent the width being set to 0. If set to 0, the output will go high.
	//
	if (uWidth == 0) {
	 	uWidth = 1;
	}
    //
    // Set the specified PWM pulse width
    //
    PWMPulseWidthSet(PWM_BASE, ulPWMOut[uChannel], uWidth);
}

// uChannel is PWM output
// uPeriod is period in ticks
void
LM3Sxxxx_PWM_Set_Period( unsigned int uChannel, unsigned int uPeriod )
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    PWMGenPeriodSet(PWM_BASE, ulPWMGen[uChannel], uPeriod);
}

// uChannel is PWM output
// uWidth is pulse width in cycles
void
LM3Sxxxx_PWM_Set_Width( unsigned int uChannel, unsigned int uWidth )
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);

	//
	// Prevent the width being set to 0. If set to 0, the output will go high.
	//
	if (uWidth == 0) {
	 	uWidth = 1;
	}

    PWMPulseWidthSet(PWM_BASE, ulPWMOut[uChannel], uWidth);
}

// uChannel is PWM output
// uDuty is duty cycle in 1/100 percent
void
LM3Sxxxx_PWM_Set_Duty( unsigned int uChannel, unsigned int uDuty )
{
    unsigned long ulWidth;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    //
    // compute the pulse width from the current period and the
    // duty cycle in percent
    //
    ulWidth = (PWMGenPeriodGet(PWM_BASE, ulPWMGen[uChannel]) * uDuty) / 10000;

	//
	// Prevent the width being set to 0. If set to 0, the output will go high.
	//
	if (ulWidth == 0) {
	 	ulWidth = 1;
	}

    //
    // set the new duty cycle
    //
    PWMPulseWidthSet(PWM_BASE, ulPWMOut[uChannel], ulWidth);
}

// uChannelMask is a bit mask of PWM outputs
// uEnableMask enables or disables channels in the mask
void
LM3Sxxxx_PWM_Set_State( unsigned int uChannelMask, unsigned int uEnableMask )
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    HWREG(PWM_BASE + PWM_O_ENABLE) = (HWREG(PWM_BASE + PWM_O_ENABLE) &
                                      ~uChannelMask) | uEnableMask;
}

// uChannel is the PWM channel
// bEnable enables or disable the sync update for the channel
void
LM3Sxxxx_PWM_Sync_Enable( unsigned int uChannel, unsigned int bEnable )
{
    unsigned long ulPwmGen = uChannel / 2;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    if(bEnable)
    {
        HWREG(PWM_BASE + PWM_O_0_CTL + (PWM_GEN_0 * ulPwmGen)) |=
             (PWM_X_CTL_CMPBUPD | PWM_X_CTL_CMPAUPD | PWM_X_CTL_LOADUPD);
    }
    else
    {
        HWREG(PWM_BASE + PWM_O_0_CTL + (PWM_GEN_0 * ulPwmGen)) &=
             ~(PWM_X_CTL_CMPBUPD | PWM_X_CTL_CMPAUPD | PWM_X_CTL_LOADUPD);
    }
}

// uChannelMask is a bit mask of PWM outputs
void
LM3Sxxxx_PWM_Sync_Update( unsigned int uChannelMask)
{
    unsigned long ulGenBits;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    ulGenBits = ((uChannelMask & 0x03) ? PWM_GEN_0_BIT : 0) |
                ((uChannelMask & 0x0c) ? PWM_GEN_1_BIT : 0) |
                ((uChannelMask & 0x30) ? PWM_GEN_2_BIT : 0);

    PWMSyncUpdate(PWM_BASE, ulGenBits);
}

// uChannelMask is a bit mask of PWM outputs
// uEnableMask enables or disable fault control for the channel
void
LM3Sxxxx_PWM_Fault_Enable( unsigned int uChannelMask, unsigned int uEnableMask )
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    if(!bPwmFaultInit)
    {
        bPwmFaultInit = 1;
        GPIODirModeSet(g_ulPWMFaultPort, g_ulPWMFaultPin, GPIO_DIR_MODE_HW);
        GPIOPadConfigSet(g_ulPWMFaultPort, g_ulPWMFaultPin, GPIO_STRENGTH_2MA,
                         GPIO_PIN_TYPE_STD_WPD);
    }
    HWREG(PWM_BASE + PWM_O_FAULT) = (HWREG(PWM_BASE + PWM_O_FAULT) &
                                     ~uChannelMask) | uEnableMask;
}

// uChannelMask is a bit mask of PWM outputs
// uEnableMask enables or disable inversion for the outputs
void
LM3Sxxxx_PWM_Invert_Enable( unsigned int uChannelMask, unsigned int uEnableMask )
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    HWREG(PWM_BASE + PWM_O_INVERT) = (HWREG(PWM_BASE + PWM_O_INVERT) &
                                      ~uChannelMask) | uEnableMask;
}

// uChannel is the PWM output
// bEnable enables or disable debug stop for the output
void
LM3Sxxxx_PWM_Debug_Stop( unsigned int uChannel, unsigned int bEnable)
{
    unsigned long ulPwmGen = uChannel / 2;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    if(bEnable)
    {
        HWREG(PWM_BASE + PWM_O_0_CTL + (PWM_GEN_0 * ulPwmGen)) &=
              ~PWM_X_CTL_DEBUG;
    }
    else
    {
        HWREG(PWM_BASE + PWM_O_0_CTL + (PWM_GEN_0 * ulPwmGen)) |=
              PWM_X_CTL_DEBUG;
    }
}

// pucStatus points to fault status return
void
LM3Sxxxx_PWM_Fault_Status( unsigned char *pucStatus )
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    if(!bPwmFaultInit)
    {
        bPwmFaultInit = 1;
        GPIODirModeSet(g_ulPWMFaultPort, g_ulPWMFaultPin, GPIO_DIR_MODE_HW);
        GPIOPadConfigSet(g_ulPWMFaultPort, g_ulPWMFaultPin, GPIO_STRENGTH_2MA,
                         GPIO_PIN_TYPE_STD_WPD);
    }
    if(pucStatus)
    {
        if(HWREG(PWM_BASE + PWM_O_STATUS) & PWM_STATUS_FAULT0)
        {
            *pucStatus = 1;
        }
        else
        {
            *pucStatus = 0;
        }
    }
}

// uChannel is PWM output
// bEnable is a flag to enable dead band control
// uRise is rise delay in ticks, -1 mean do not set
// uFall is fall delay in ticks, -1 mean do not set
void
LM3Sxxxx_PWM_Set_Dead_Band( unsigned int uChannel, unsigned int bEnable,
                            unsigned int uRise, unsigned int uFall)
{
    unsigned long ulPwmGen = uChannel / 2;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    if((int)uRise != -1)
    {
        HWREG((PWM_BASE + PWM_O_0_DBRISE) + (PWM_GEN_0 * ulPwmGen)) = uRise;
    }

    if((int)uFall != -1)
    {
        HWREG((PWM_BASE + PWM_O_0_DBFALL) + (PWM_GEN_0 * ulPwmGen)) = uFall;
    }

	// if bEnable == -1, then do not enable or disable
    if(bEnable == 1)
    {
        HWREG(PWM_BASE + PWM_O_0_DBCTL + (PWM_GEN_0 * ulPwmGen)) |=
              PWM_X_DBCTL_ENABLE;
    }
    else if(bEnable == 0)
    {
        HWREG(PWM_BASE + PWM_O_0_DBCTL + (PWM_GEN_0 * ulPwmGen)) &=
              ~PWM_X_DBCTL_ENABLE;
    }
}

// uChannelMask is a bit mask of PWM outputs to sync time base
void
LM3Sxxxx_PWM_Sync_Time_Base( unsigned int uChannelMask )
{
    unsigned long ulGenBits;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    ulGenBits = ((uChannelMask & 0x03) ? PWM_GEN_0_BIT : 0) |
                ((uChannelMask & 0x0c) ? PWM_GEN_1_BIT : 0) |
                ((uChannelMask & 0x30) ? PWM_GEN_2_BIT : 0);

    PWMSyncTimeBase(PWM_BASE, ulGenBits);
}

// uChannel is PWM output
// uCountMode is the PWM gen counting mode; 0=down, 1=up/down
void
LM3Sxxxx_PWM_Set_Count_Mode( unsigned int uChannel, unsigned int uCountMode)
{
    unsigned long ulPwmGen = uChannel / 2;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    if(uCountMode == 1)
    {
        HWREG(PWM_BASE + PWM_O_0_CTL + (PWM_GEN_0 * ulPwmGen)) |=
              PWM_X_CTL_MODE;
    }
    else
    {
        HWREG(PWM_BASE + PWM_O_0_CTL + (PWM_GEN_0 * ulPwmGen)) &=
              ~PWM_X_CTL_MODE;
    }
}

#if 0
//*****************************************************************************
//
// This VI is used to initialize a PWM output channel.
//
// The PWM channel ID identifies the PWM output to configure: (0-5).
//
// The PWM divider is used to predivide the system clock for the PWM
// generator time base.  The value is the power of 2 of the divider
// value.  The valid choices for divider are 1, 2, ... 64 in powers of 2.
// The valid parameter input values are 0, 1, 2 ... 6.
//
// The frequency is the PWM frequency, in Hz, of the output.
//
// The flags indicate alignment and output control.
// Bits 1:0 of the flags represent the output pulse alignment:
//  0 - left, 1 - centered.
// Bit 2 of the flags represents output inversion.
// Bit 3 of the flags represents disable on fault.
//
// No error checking is done on the inputs.  At the moment we assume
// this is only being called from well behaved code (the EIO model VI).
//
//*****************************************************************************
void
LM3Sxxxx_PWM_Init_FN(unsigned int uChannel, unsigned int uFreq,
                     unsigned int uDivider, unsigned int uFlags)
{
    unsigned long ulTimeBase;
    unsigned long ulPwmOutBit = 1 << uChannel;

    //
    // enable the PWM peripheral, set the PWM system clock divider
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    SysCtlPWMClockSet(ulPWMSysDiv[uDivider]);

    //
    // set up the GPIO pin for the PWM output
    // The GPIO port was already enabled during system init
    //
    GPIOPinTypePWM(ulPWMPort[uChannel], ulPWMPin[uChannel]);

    //
    // find the system clock and adjust by the PWM divider
    //
    ulTimeBase = SysCtlClockGet() / (1 << uDivider);

    //
    // set the PWM frequency
    //
    PWMGenPeriodSet(PWM_BASE, ulPWMGen[uChannel], ulTimeBase / uFreq);

    //
    // set the output fault and invert properties according to the flags
    //
    PWMOutputFault(PWM_BASE, ulPwmOutBit, (uFlags & 8));
    PWMOutputInvert(PWM_BASE, ulPwmOutBit, (uFlags & 4));

    //
    // set the counter mode based on left or center alignment
    //
    if(uFlags & 1)
    {
        //
        // center aligned - use up/down mode
        //
        PWMGenConfigure(PWM_BASE, ulPWMGen[uChannel],
                        PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC |
                        PWM_GEN_MODE_DBG_STOP);
    }
    else
    {
        //
        // left aligned, use count-down mode
        //
        PWMGenConfigure(PWM_BASE, ulPWMGen[uChannel],
                        PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC |
                        PWM_GEN_MODE_DBG_STOP);
    }

    //
    // set the pulse width to 0 so the output is "off"
    //
    PWMPulseWidthSet(PWM_BASE, ulPWMOut[uChannel], 0);

    //
    // enable the PWM output
    //
    PWMOutputState(PWM_BASE, ulPwmOutBit, 1);

    //
    // enable the PWM generator
    //
    PWMGenEnable(PWM_BASE, ulPWMGen[uChannel]);
}

//*****************************************************************************
//
// This VI is used to set the duty cycle of a PWM output.
//
// The PWM channel ID identifies the PWM output to configure: (0-5).
// The duty cycle is in percent (0-100).
//
// No error checking is done on the inputs.  At the moment we assume
// this is only being called from well behaved code (the EIO model VI).
//
//*****************************************************************************
void
LM3Sxxxx_PWM_FN(unsigned int uChannel, unsigned int uDutyPct)
{
    unsigned long ulWidth; 

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
    //
    // compute the pulse width from the current period and the
    // duty cycle in percent
    //
    ulWidth = (PWMGenPeriodGet(PWM_BASE, ulPWMGen[uChannel]) * uDutyPct) / 100;

    //
    // set the new duty cycle
    //
    PWMPulseWidthSet(PWM_BASE, ulPWMOut[uChannel], ulWidth);
}
#endif
