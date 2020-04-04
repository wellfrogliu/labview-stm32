//*****************************************************************************
//
// rit-graphics.c - A simple display driver implementation allowing use
// of the Luminary Graphics Library on evaluation kits equipped with
// RIT 128x64 displays.
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
// This is part of revision 26 of the Stellaris NI-LabVIEW support package.
//
//*****************************************************************************
#include "hw_types.h"
#include "sysctl.h"
#include "grlib/grlib.h"
#include "rit128x96x4.h"

//*****************************************************************************
//
//! \addtogroup ek_fury_rit_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Screen dimension variables required by the graphics library.
//
//*****************************************************************************
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 96
const unsigned long g_ulDpyWidth = SCREEN_WIDTH;
const unsigned long g_ulDpyHeight = SCREEN_HEIGHT;

#define BYTES_PER_LINE ((SCREEN_WIDTH)/2)

//*****************************************************************************
//
// Frame buffer for the screen. We use a 4bpp representation to allow 16 colors
// or levels, hence the division by two.
//
//*****************************************************************************
static unsigned char g_pucFrame[BYTES_PER_LINE * SCREEN_HEIGHT];

//*****************************************************************************
//
//! Initializes the display driver.
//!
//! This function initializes the RIT 128x96 display preparing it to display
//! data.
//!
//! \return None.
//
//*****************************************************************************
void
DpyInit(void)
{
    //
    // Using the Osram display
    //
    RIT128x96x4Init(1000000);
}

//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is required with the RIT display since we use a local frame buffer and
//! this function acts as the trigger to refresh the actual display from
//! the local buffer.
//!
//! \return None.
//
//*****************************************************************************
void
DpyFlush(void)
{
    RIT128x96x4ImageDraw(g_pucFrame, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param psRect is a pointer to the structure describing the rectangle.
//! \param ulValue is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (i.e. both sXMin and sXMax are
//! drawn, along with sYMin and sYMax).
//!
//! \return None.
//
//*****************************************************************************
void
DpyRectFill(const tRectangle *psRect, unsigned long ulValue)
{
    long lY;

    //
    // Draw the rectangle as a series of horizontal lines.
    //
    for(lY = psRect->sYMin; lY <= psRect->sYMax; lY++)
    {
        DpyLineDrawH(psRect->sXMin, psRect->sXMax, lY, ulValue);
    }
}

//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param lX is the X coordinate of the line.
//! \param lY1 is the Y coordinate of the start of the line.
//! \param lY2 is the Y coordinate of the end of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void
DpyLineDrawV(long lX, long lY1, long lY2, unsigned long ulValue)
{
    long lY;

    //
    // Draw a vertical line as a series of points
    //
    for(lY = lY1; lY <= lY2; lY++)
    {
        DpyPixelDraw(lX, lY, ulValue);
    }
}

//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param lX1 is the X coordinate of the start of the line.
//! \param lX2 is the X coordinate of the end of the line.
//! \param lY is the Y coordinate of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void
DpyLineDrawH(long lX1, long lX2, long lY, unsigned long ulValue)
{
    long lX;

    //
    // Handle any odd pixel at the left edge of the line.
    //
    if(lX1 & 1)
    {
        lX = lX1 / 2;
        g_pucFrame[(lY * BYTES_PER_LINE) + lX] =
            ((g_pucFrame[(lY * BYTES_PER_LINE) + lX] & 0xf0) |
             (ulValue & 0xf));
        //
        // We've drawn the first pixel so adjust the start point.
        //
        lX1++;

    }

    //
    // Handle any odd pixel at the right edge of the line (remembering
    // that the line includes both end pixels)
    //
    if((lX2 > lX1) && !(lX2 & 1))
    {
        lX = lX2 / 2;
        g_pucFrame[(lY * BYTES_PER_LINE) + lX] =
            ((g_pucFrame[(lY * BYTES_PER_LINE) + lX] & 0x0F) |
             ((ulValue & 0xf) << 4));
        //
        // We've drawn the last pixel so adjust the end point.
        //
        lX2--;
    }

    //
    // Fix up the level parameter since we will be writing bytes from here on.
    //
    ulValue = (ulValue & 0xF);
    ulValue |= (ulValue << 4);

    //
    // Now fill in the middle segment that we know occupies entire bytes.
    //
    for(lX = lX1; lX <= lX2; lX += 2)
    {
        g_pucFrame[(lY * BYTES_PER_LINE) + (lX / 2)] =
            (unsigned char)ulValue;
    }
}

//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param lX is the X coordinate of the pixel.
//! \param lY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void
DpyPixelDraw(long lX, long lY, unsigned long ulLevel)
{
    if(lX & 1)
    {
        g_pucFrame[(lY * BYTES_PER_LINE) + (lX / 2)] =
            ((g_pucFrame[(lY * BYTES_PER_LINE) + (lX / 2)] & 0xf0) |
             (ulLevel & 0xf));
    }
    else
    {
        g_pucFrame[(lY * BYTES_PER_LINE) + (lX / 2)] =
            ((g_pucFrame[(lY * BYTES_PER_LINE) + (lX / 2)] & 0x0f) |
             ((ulLevel & 0xf) << 4));
    }
}

//*****************************************************************************
//
//! Translates a 24 bit RGB color into a level suitable for this display.
//
//! \param ulValue is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function is used to map a display-independent 24 bit color value
//! into a color value suitable for writing to the current screen's display
//! buffer. Since the RIT display supports only 16 grey levels, we perform a
//! very simple conversion from color to grey by summing the red, blue and
//! twice the green component from the RGB color passed and dividing the result
//! by 64 to yield a value in the range 0 - 15.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
unsigned long
DpyColorTranslate(unsigned long ulValue)
{
    unsigned long ulLevel;

    //
    // This display offers 16 grey levels so, obviously, we have a hard time
    // showing a 24 bit RGB color. As a quick translation, we merely add the
    // R, B and twice the G component then divide down to create a value in
    // the range 0-15.
    ulLevel = ((ulValue & ClrRedMask) >> ClrRedShift) +
              ((ulValue & ClrBlueMask) >> ClrBlueShift) +
              ((ulValue & ClrGreenMask) >> ClrGreenShift) * 2;

    //
    // The previous line results in a value in the range [0-1020]. Shift
    // right by 6 bits to get this into the range [0-15]
    ulLevel >>= 6;

    //
    // We really don't want all really dark colors to map to black so ensure
    // that anything that results in black translates to level 1 unless the
    // color passed was actually 0.
    //
    if(ulValue && !ulLevel)
    {
        ulLevel = 1;
    }

    //
    // Pass back the translated color.
    //
    return(ulLevel);
}

//*****************************************************************************
//
//! Turns on the backlight (dummy function).
//!
//! This display does not require a backlight so this function is stubbed
//! merely to keep applications using the graphics library from failing to
//! link.
//!
//! \return None.
//
//*****************************************************************************
void
DpyBacklightOn(void)
{
    //
    // No backlight on this display.
    //
}

//*****************************************************************************
//
//! Turns off the backlight (dummy function).
//!
//! This display does not require a backlight so this function is stubbed
//! merely to keep applications using the graphics library from failing to
//! link.
//!
//! \return None.
//
//*****************************************************************************
void
DpyBacklightOff(void)
{
    //
    // No backlight on this display.
    //
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
