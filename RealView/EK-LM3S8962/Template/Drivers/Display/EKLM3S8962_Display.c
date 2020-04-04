//*****************************************************************************
//
// EKLM3S8962_Display.c - C implementation of Display VIs for the 8962
//                        eval board.
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
// This is part of revision 61 of the Stellaris NI-LabVIEW support package.
//
//*****************************************************************************

#define CStatic_AllocEnable

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include "CCGDataSupport.h"
#include "hw_memmap.h"
#include "hw_ssi.h"

typedef unsigned char tBoolean;
#include "ssi.h"

#include "hw_types.h"

#include "grlib.h"
#define strIn (*(PDAStrPtr *)&vhIn)

#define FONT_CHOICE             &g_sFontCmss14
#define ERRCODE_NOT_INIT        k_OLED_NotInit
#define ERRCODE_NULL_POINTER    2

static tContext Context;
static unsigned char bInit = 0;
static unsigned int frontColor = 15790320; // set default foreground color to gray15
static unsigned int backColor = 0;  // default BG color to black

/*
 * Need to figure out a way to select a font from VI
 * without needing to link all the other fonts in here.
 */
#if 0
static
tFont *FontTable[][] =
{
    {
        &g_sFontCm12,   &g_sFontCm12b,   &g_sFontCm12i,
        &g_sFontCmss12, &g_sFontCmss12b, &g_sFontCmss12i,
        &g_sFontCmsc12, &g_sFontCm12scb, &g_sFontCmsc12i
    },
    {
        &g_sFontCm14,   &g_sFontCm14b,   &g_sFontCm14i,
        &g_sFontCmss14, &g_sFontCmss14b, &g_sFontCmss14i,
        &g_sFontCmsc14, &g_sFontCmsc14b, &g_sFontCmsc14i
    },
    {
        &g_sFontCm16,   &g_sFontCm16b,   &g_sFontCm16i,
        &g_sFontCmss16, &g_sFontCmss16b, &g_sFontCmss16i,
        &g_sFontCmsc16, &g_sFontCmsc16b, &g_sFontCmsc16i
    },
    {
        &g_sFontCm18,   &g_sFontCm18b,   &g_sFontCm18i,
        &g_sFontCmss18, &g_sFontCmss18b, &g_sFontCmss18i,
        &g_sFontCmsc18, &g_sFontCmsc18b, &g_sFontCmsc18i
    },
    {
        &g_sFontCm20,   &g_sFontCm20b,   &g_sFontCm20i,
        &g_sFontCmss20, &g_sFontCmss20b, &g_sFontCmss20i,
        &g_sFontCmsc20, &g_sFontCmsc20b, &g_sFontCmsc20i
    }
};

void
EKLM3S8962_Display_Set_Font(unsigned long ulFontTypeIdx,
                            unsigned long ulFontSizeIdx)
{
    GrContextFontSet(&Context, FontTable[ulFontSizeIdx][ulFontTypeIdx]);
}
#endif


void
EKLM3S8962_Display_Verify_SPI()
{
 	unsigned long ulConfig;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);  // ensure the peripheral is enabled before attempting to read registers
	
	ulConfig = HWREG(SSI0_BASE + SSI_O_CR0);

	// verify that polarity and phase are set correctly for OLED communication
	if(!(ulConfig & (1 << 6)) || (ulConfig & (1 << 7)))
	{
		SSIDisable(SSI0_BASE);
	 	SSIConfig(SSI0_BASE, SSI_FRF_MOTO_MODE_2, SSI_MODE_MASTER, 1000000, 8);
	}
	SSIEnable(SSI0_BASE);
}

void
EKLM3S8962_Display_Init(long lErrIn, long *plErrOut)
{
    /*
     * If there is no error input, then perform the operation.
     */
    if(!lErrIn)
    {
        /*
         * Initialize the display hardware, then get the
         * display context and set a default font and foreground color.
         */
        DpyInit();
        GrContextInit(&Context);
        GrContextFontSet(&Context, FONT_CHOICE);
		GrContextForegroundSet(&Context, frontColor); 
        bInit = 1;
    }

    /*
     * Pass through the error code.
     */
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}

void
EKLM3S8962_Display_Set_Clip_Region(long lXMin, long lYMin,
                                   long lXMax, long lYMax,
                                   long lErrIn, long *plErrOut)
{
    /*
     * If there is no error input, then perform the operation.
     */
    if(!lErrIn)
    {
        /*
         * If the display has not been initialized, then indicate
         * an error and return.
         */
        if(!bInit)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NOT_INIT;
            }
            return;
        }

		EKLM3S8962_Display_Verify_SPI();

        /*
         * Set the clip region.
         */
        GrContextClipRegionSet(&Context, lXMin, lYMin, lXMax, lYMax);
    }

    /*
     * Pass through the error code.
     */
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}

void
EKLM3S8962_Display_Set_Foreground(unsigned long ulColor,
                                  long lErrIn, long *plErrOut)
{
    if(!lErrIn)
    {
        if(!bInit)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NOT_INIT;
            }
            return;
        }

		EKLM3S8962_Display_Verify_SPI();
		frontColor = ulColor;
        GrContextForegroundSet(&Context, frontColor);
    }
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}

void
EKLM3S8962_Display_Set_Background(unsigned long ulColor,
                                  long lErrIn, long *plErrOut)
{
    if(!lErrIn)
    {
        if(!bInit)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NOT_INIT;
            }
            return;
        }

		EKLM3S8962_Display_Verify_SPI();
		backColor = ulColor;
        GrContextBackgroundSet(&Context, backColor);
    }
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}

void
EKLM3S8962_Display_Draw_Pixel(long lX, long lY, unsigned long bUpdate,
                              long lErrIn, long *plErrOut)
{
    if(!lErrIn)
    {
        if(!bInit)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NOT_INIT;
            }
            return;
        }

		EKLM3S8962_Display_Verify_SPI();
        GrPixelDraw(&Context, lX, lY);

        if(bUpdate)
        {
            GrFlush(&Context);
        }
    }
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}

void
EKLM3S8962_Display_Draw_Circle(long lX, long lY, long lRadius,
                               unsigned long bFilled,
                               unsigned long bUpdate,
                               long lErrIn, long *plErrOut)
{
    if(!lErrIn)
    {
        if(!bInit)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NOT_INIT;
            }
            return;
        }

		EKLM3S8962_Display_Verify_SPI();

        if(bFilled)
        {
            GrCircleFill(&Context, lX, lY, lRadius);
        }
        else
        {
            GrCircleDraw(&Context, lX, lY, lRadius);
        }

        if(bUpdate)
        {
            GrFlush(&Context);
        }
    }
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}

void
EKLM3S8962_Display_Draw_Line(long lX1, long lY1, long lX2, long lY2,
                             unsigned long bUpdate,
                             long lErrIn, long *plErrOut)
{
    if(!lErrIn)
    {
        if(!bInit)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NOT_INIT;
            }
            return;
        }

		EKLM3S8962_Display_Verify_SPI();
        GrLineDraw(&Context, lX1, lY1, lX2, lY2);

        if(bUpdate)
        {
            GrFlush(&Context);
        }
    }
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}

void
EKLM3S8962_Display_Draw_Rect(short sXMin, short sYMin,
                             short sXMax, short sYMax,
                             unsigned long bFilled,
                             unsigned long bUpdate,
                             long lErrIn, long *plErrOut)
{
    tRectangle Rect;

    if(!lErrIn)
    {
        if(!bInit)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NOT_INIT;
            }
            return;
        }

		EKLM3S8962_Display_Verify_SPI();

        Rect.sXMin = sXMin;
        Rect.sYMin = sYMin;
        Rect.sXMax = sXMax;
        Rect.sYMax = sYMax;

        if(bFilled)
        {
            GrRectFill(&Context, &Rect);
        }
        else
        {
            GrRectDraw(&Context, &Rect);
        }

        if(bUpdate)
        {
            GrFlush(&Context);
        }
    }
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}

void
EKLM3S8962_Display_Draw_String(VoidHand vhIn, long lX, long lY,
                               unsigned long bOpaque, unsigned long bCentered,
                               unsigned long bUpdate,
                               long lErrIn, long *plErrOut)
{

    if(!lErrIn)
    {
        if(!vhIn)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NULL_POINTER;
            }
            return;
        }
        if(!bInit)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NOT_INIT;
            }
            PDAStrFree( vhIn );
            return;
        }

		EKLM3S8962_Display_Verify_SPI();

        if(bCentered)
        {
            GrStringDrawCentered(&Context, (char *)strIn->str, strIn->len,
                                 lX, lY, bOpaque);
        }
        else
        {
            GrStringDraw(&Context, (char *)strIn->str, strIn->len,
                         lX, lY, bOpaque);
        }

        if(bUpdate)
        {
            GrFlush(&Context);
        }
    }
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
    if(vhIn)
    {
        PDAStrFree( vhIn );
    }
}

void
EKLM3S8962_Display_Get_String_Width(VoidHand vhIn, long *plWidth,
                                    long lErrIn, long *plErrOut)
{
    long lWidth;

    if(!lErrIn)
    {
        if(!vhIn)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NULL_POINTER;
            }
            return;
        }
        if(!bInit)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NOT_INIT;
            }
            PDAStrFree( vhIn );
            return;
        }

		EKLM3S8962_Display_Verify_SPI();
        lWidth = GrStringWidth(&Context, (char *)strIn->str, strIn->len);

        if(plWidth)
        {
            *plWidth = lWidth;
        }
    }
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
    if(vhIn)
    {
        PDAStrFree( vhIn );
    }
}

void
EKLM3S8962_Display_Update(long lErrIn, long *plErrOut)

{
    if(!lErrIn)
    {
        if(!bInit)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NOT_INIT;
            }
            return;
        }

		EKLM3S8962_Display_Verify_SPI();
        GrFlush(&Context);

    }
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}

void
EKLM3S8962_Display_Clear_Screen(unsigned long bUpdate, long lErrIn, long *plErrOut)
{
    tRectangle Rect;

    if(!lErrIn)
    {
        if(!bInit)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NOT_INIT;
            }
            return;
        }

		EKLM3S8962_Display_Verify_SPI();
		
		GrContextForegroundSet(&Context, backColor);  // set FG color to match BG color
		
		// set rectangle dimension to max size
        Rect.sXMin = 0;
        Rect.sYMin = 0;
        Rect.sXMax = 127;
        Rect.sYMax = 95;

        GrRectFill(&Context, &Rect);

        if(bUpdate)
        {
            GrFlush(&Context);
        }
		
		GrContextForegroundSet(&Context, frontColor);  // revert color
    }
    if(plErrOut)
    {
        *plErrOut = lErrIn;
    }
}
