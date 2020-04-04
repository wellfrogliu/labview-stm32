//*****************************************************************************
//
// string.c - Routines for drawing text.
//
// Copyright (c) 2007-2008 Luminary Micro, Inc.  All rights reserved.
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

#include "grlib.h"

//*****************************************************************************
//
//! Determines the width of a string.
//!
//! \param pContext is a pointer to the drawing context to use.
//! \param pcString is the string in question.
//! \param ulLength is the length of the string.
//!
//! This function determines the width of a string (or portion of the string)
//! when drawn with a particular font.  The \e ulLength parameter allows a
//! portion of the string to be examined without having to insert a NULL
//! character at the stopping point (would not be possible if the string was
//! located in flash).
//!
//! \return Returns the width of the string in pixels.
//
//*****************************************************************************
long
GrStringWidth(const tContext *pContext, const char *pcString,
              unsigned long ulLength)
{
    const unsigned short *pusOffset;
    const unsigned char *pucData;
    long lWidth;

    //
    // Get some pointers to relevant information in the font to make things
    // easier, and give the compiler a hint about extraneous loads that it can
    // avoid.
    //
    pucData = pContext->pFont->pucData;
    pusOffset = pContext->pFont->pusOffset;

    //
    // Loop through the characters in the string.
    //
    for(lWidth = 0; *pcString && ulLength; pcString++, ulLength--)
    {
       //
        // Get a pointer to the font data for the next character from the
        // string.  If there is not a glyph for the next character, replace it
        // with a ".".
        //
        if((*pcString >= ' ') && (*pcString <= '~'))
        {
            //
            // Add the width of this character as drawn with the given font.
            //
            lWidth += pucData[pusOffset[*pcString - ' '] + 1];
        }
        else
        {
            //
            // This character does not exist in the font so replace it with
            // a '.' instead.  This matches the approach taken in GrStringDraw
            // and ensures that the width returned here represents the
            // rendered dimension of the string.
            //
            lWidth += pucData[pusOffset['.' - ' '] + 1];
        }
    }

    //
    // Return the width of the string.
    //
    return(lWidth);
}

//*****************************************************************************
//
//! Draws a string.
//!
//! \param pContext is a pointer to the drawing context to use.
//! \param pcString is a pointer to the string to be drawn.
//! \param ulLength is the number of characters from the string that should be
//! drawn on the screen.
//! \param lX is the X coordinate of the upper left corner of the string
//! position on the screen.
//! \param lY is the Y coordinate of the upper left corner of the string
//! position on the screen.
//! \param bOpaque is true of the background of each character should be drawn
//! and false if it should not (leaving the background as is).
//!
//! This function draws a string of test on the screen.  The \e ulLength
//! parameter allows a portion of the string to be examined without having to
//! insert a NULL character at the stopping point (which would not be possible
//! if the string was located in flash).
//!
//! \return None.
//
//*****************************************************************************
void
GrStringDraw(const tContext *pContext, const char *pcString,
             unsigned long ulLength, long lX, long lY, unsigned long bOpaque)
{
    long lIdx, lX0, lY0, lCount, lOff, lOn;
    const unsigned char *pucData;
    tContext sCon;

    //
    // Copy the drawing context into a local structure that can be modified.
    //
    sCon = *pContext;

    //
    // Loop through the characters in the string.
    //
    while(*pcString && ulLength--)
    {
        //
        // Stop drawing the string if the right edge of the clipping region has
        // been exceeded.
        //
        if(lX > sCon.sClipRegion.sXMax)
        {
            break;
        }

        //
        // Get a pointer to the font data for the next character from the
        // string.  If there is not a glyph for the next character, replace it
        // with a ".".
        //
        if((*pcString >= ' ') && (*pcString <= '~'))
        {
            pucData = (sCon.pFont->pucData +
                       sCon.pFont->pusOffset[*pcString++ - ' ']);
        }
        else
        {
            pucData = (sCon.pFont->pucData +
                       sCon.pFont->pusOffset['.' - ' ']);
            pcString++;
        }

        //
        // See if the entire character is to the left of the clipping region.
        //
        if((lX + pucData[1]) < sCon.sClipRegion.sXMin)
        {
            //
            // Increment the X coordinate by the width of the character.
            //
            lX += pucData[1];

            //
            // Go to the next character in the string.
            //
            continue;
        }

        //
        // Loop through the bytes in the encoded data for this glyph.
        //
        for(lIdx = 2, lX0 = 0, lY0 = 0; lIdx < pucData[0]; )
        {
            //
            // See if the bottom of the clipping region has been exceeded.
            //
            if((lY + lY0) > sCon.sClipRegion.sYMax)
            {
                //
                // Stop drawing this character.
                //
                break;
            }

            //
            // See if this is a byte that encodes some on and off pixels.
            //
            if(pucData[lIdx])
            {
                //
                // Extract the number of off pixels.
                //
                lOff = (pucData[lIdx] >> 4) & 15;

                //
                // Extract the number of on pixels.
                //
                lOn = pucData[lIdx] & 15;

                //
                // Skip past this encoded byte.
                //
                lIdx++;
            }

            //
            // Otherwise, see if this is a repeated on pixel byte.
            //
            else if(pucData[lIdx + 1] & 0x80)
            {
                //
                // There are no off pixels in this encoding.
                //
                lOff = 0;

                //
                // Extract the number of on pixels.
                //
                lOn = (pucData[lIdx + 1] & 0x7f) * 8;

                //
                // Skip past these two encoded bytes.
                //
                lIdx += 2;
            }

            //
            // Otherwise, this is a repeated off pixel byte.
            //
            else
            {
                //
                // Extract the number of off pixels.
                //
                lOff = pucData[lIdx + 1] * 8;

                //
                // There are no on pixels in this encoding.
                //
                lOn = 0;

                //
                // Skip past these two encoded bytes.
                //
                lIdx += 2;
            }

            //
            // Loop while there are any on pixels.
            //
            while(lOff)
            {
                //
                // See if the bottom of the clipping region has been exceeded.
                //
                if((lY + lY0) > sCon.sClipRegion.sYMax)
                {
                    //
                    // Ignore the remainder of the on pixels.
                    //
                    break;
                }

                //
                // See if there is more than one on pixel that will fit onto
                // the current row.
                //
                if((lOff > 1) && ((lX0 + 1) < pucData[1]))
                {
                    //
                    // Determine the number of on pixels that will fit on this
                    // row.
                    //
                    lCount = (((lX0 + lOff) > pucData[1]) ? pucData[1] - lX0 :
                              lOff);

                    //
                    // If this row is within the clipping region, draw a
                    // horizontal line that corresponds to the sequence of on
                    // pixels.
                    //
                    if(((lY + lY0) >= sCon.sClipRegion.sYMin) && bOpaque)
                    {
                        sCon.ulForeground = pContext->ulBackground;
                        GrLineDrawH(&sCon, lX + lX0, lX + lX0 + lCount - 1,
                                    lY + lY0);
                    }

                    //
                    // Decrement the count of on pixels by the number on this
                    // row.
                    //
                    lOff -= lCount;

                    //
                    // Increment the X offset by the number of on pixels.
                    //
                    lX0 += lCount;
                }

                //
                // Otherwise, there is only a single on pixel that can be
                // drawn.
                //
                else
                {
                    //
                    // If this pixel is within the clipping region, then draw
                    // it.
                    //
                    if(((lX + lX0) >= sCon.sClipRegion.sXMin) &&
                       ((lX + lX0) <= sCon.sClipRegion.sXMax) &&
                       ((lY + lY0) >= sCon.sClipRegion.sYMin) && bOpaque)
                    {
                        DpyPixelDraw(lX + lX0, lY + lY0,
                                     pContext->ulBackground);
                    }

                    //
                    // Decrement the count of on pixels.
                    //
                    lOff--;

                    //
                    // Increment the X offset.
                    //
                    lX0++;
                }

                //
                // See if the X offset has reached the right side of the
                // character glyph.
                //
                if(lX0 == pucData[1])
                {
                    //
                    // Increment the Y offset.
                    //
                    lY0++;

                    //
                    // Reset the X offset to the left side of the character
                    // glyph.
                    //
                    lX0 = 0;
                }
            }

            //
            // Loop while there are any on pixels.
            //
            while(lOn)
            {
                //
                // See if the bottom of the clipping region has been exceeded.
                //
                if((lY + lY0) > sCon.sClipRegion.sYMax)
                {
                    //
                    // Ignore the remainder of the on pixels.
                    //
                    break;
                }

                //
                // See if there is more than one on pixel that will fit onto
                // the current row.
                //
                if((lOn > 1) && ((lX0 + 1) < pucData[1]))
                {
                    //
                    // Determine the number of on pixels that will fit on this
                    // row.
                    //
                    lCount = (((lX0 + lOn) > pucData[1]) ? pucData[1] - lX0 :
                              lOn);

                    //
                    // If this row is within the clipping region, draw a
                    // horizontal line that corresponds to the sequence of on
                    // pixels.
                    //
                    if((lY + lY0) >= sCon.sClipRegion.sYMin)
                    {
                        sCon.ulForeground = pContext->ulForeground;
                        GrLineDrawH(&sCon, lX + lX0, lX + lX0 + lCount - 1,
                                    lY + lY0);
                    }

                    //
                    // Decrement the count of on pixels by the number on this
                    // row.
                    //
                    lOn -= lCount;

                    //
                    // Increment the X offset by the number of on pixels.
                    //
                    lX0 += lCount;
                }

                //
                // Otherwise, there is only a single on pixel that can be
                // drawn.
                //
                else
                {
                    //
                    // If this pixel is within the clipping region, then draw
                    // it.
                    //
                    if(((lX + lX0) >= sCon.sClipRegion.sXMin) &&
                       ((lX + lX0) <= sCon.sClipRegion.sXMax) &&
                       ((lY + lY0) >= sCon.sClipRegion.sYMin))
                    {
                        DpyPixelDraw(lX + lX0, lY + lY0,
                                     pContext->ulForeground);
                    }

                    //
                    // Decrement the count of on pixels.
                    //
                    lOn--;

                    //
                    // Increment the X offset.
                    //
                    lX0++;
                }

                //
                // See if the X offset has reached the right side of the
                // character glyph.
                //
                if(lX0 == pucData[1])
                {
                    //
                    // Increment the Y offset.
                    //
                    lY0++;

                    //
                    // Reset the X offset to the left side of the character
                    // glyph.
                    //
                    lX0 = 0;
                }
            }
        }

        //
        // Increment the X coordinate by the width of the character.
        //
        lX += pucData[1];
    }
}
