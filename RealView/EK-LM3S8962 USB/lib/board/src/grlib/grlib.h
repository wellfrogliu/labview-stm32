//*****************************************************************************
//
// grlib.h - Prototypes for the low level primitives provided by the graphics
//           library.
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

#ifndef __GRLIB_H__
#define __GRLIB_H__

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//! This structure defines the extents of a rectangle.  All points greater than
//! or equal to the minimum and less than or equal to the maximum are part of
//! the rectangle.
//
//*****************************************************************************
typedef struct
{
    //
    //! The minimum X coordinate of the rectangle.
    //
    short sXMin;

    //
    //! The minimum Y coordinate of the rectangle.
    //
    short sYMin;

    //
    //! The maximum X coordinate of the rectangle.
    //
    short sXMax;

    //
    //! The maximum Y coordinate of the rectangle.
    //
    short sYMax;
}
tRectangle;

//*****************************************************************************
//
//! This structure describes a font used for drawing text onto the screen.  The
//! font data is compressed with a scheme treats the rows of the font glyph as
//! if they were connected side-by-side.  Therefore, pixels from the end of one
//! row can be combined with pixels from the beginning of the next row when
//! compressing.  The encoded format of font data is as follows:
//!
//! - The first byte of the encoding is the number of bytes within the encoding
//!   (including the size byte).
//!
//! - The second byte is the width of the character in pixels.
//!
//! - The remaining bytes describe the pixels within the glyph.
//!
//!   - A non-zero byte encodes up to 15 consecutive off pixels followed by up
//!     to 15 consecutive on pixels.  The upper nibble contains the number of
//!     off pixels and the lower nibble contains the number of on pixels.  So,
//!     for example, <tt>0x12</tt> means that there is a single off pixel
//1     followed by two on pixels.
//!
//!   - A zero byte indicates repeated pixels.  The next byte determines the
//!     size and type of the repeated pixels.  The lower seven bits of the next
//!     byte specifies the number of bytes in the literal encoding (referred to
//!     as N).  If the upper bit of the next byte is set, then there are N * 8
//!     on pixels.  If the upper bit is not set, then there are N * 8 off
//!     pixels.
//!
//! The repeated pixel encoding is very useful for the several rows worth of
//! off pixels that are present in many glyphs.
//!
//! As an example, consider the following font glyph (for a non-existant
//! character from a 16x8 font):
//!
//! \verbatim
//!     ................
//!     ................
//!     ................
//!     .......xx.......
//!     ......xxxx......
//!     ....xxxxxxxx....
//!     ....xxxxxxxx....
//!     ................
//! \endverbatim
//!
//! The compressed version of this glyph would be as follows, with an
//! explanation of each byte or byte sequence:
//!
//! - 0x08: There are 8 bytes in the encoding of this glyph, including this
//!         byte.
//!
//! - 0x10: This glyph is 16 pixels wide.
//!
//! - 0x00 0x06: The glyphs starts with 48 off pixels (6 bytes).
//!
//! - 0x72: The fourth row has seven off pixels followed by two on pixels.
//!
//! - 0xd4: The fourth row ends with 7 off pixels and the fifth row starts with
//!         6 off pixels, making 13 off pixels.  This is followed by 4 on
//!         pixels.
//!
//! - 0xa8: The fifth row ends with 6 off pixels, and the sixth row starts with
//!         4 off pixels, making 10 off pixels.  This is followed by 8 on
//!         pixels.
//!
//! - 0x88: The sixth row ends with 4 off pixels, and the seventh row starts
//!         with 4 off pixels, making 8 off pixels.  This is followed by 8 on
//!         pixels.
//!
//! The final 4 off pixels on the seventh row, and the 12 off pixels on the
//! eighth row, do not need to be encoded since there are no following on
//! pixels.  This results in an eight byte compressed font glyph, compared to
//! the sixteen bytes required to describe the uncompressed 16x8 array.
//!
//! While being simplistic, this encoding method provides very effective
//! results.  Since the ASCII character set has a small number of strokes per
//! character, the non-zero byte encoding format can effectively encode most
//! occurrences of non-zero pixels and the repeated pixel encoding format can
//! effectively encode the large run of zero pixels at the top of many glyphs.
//
//*****************************************************************************
typedef struct
{
    //
    //! The height of the character cell; this may be taller than the font data
    //! for the characters (to provide inter-line spacing).
    //
    char cHeight;

    //
    //! The offset between the top of the character cell and the baseline of
    //! the glyph.  The baseline is the bottom row of a capital letter, below
    //! which only the descenders of the lower case letters occur.
    //
    char cBaseline;

    //
    //! The offset within pucData to the data for each character in the font.
    //
    unsigned short pusOffset[95];

    //
    //! A pointer to the data for the font.
    //
    const unsigned char *pucData;
}
tFont;

//*****************************************************************************
//
//! This structure defines a drawing context to be used to draw onto the
//! screen.  Multiple drawing contexts may exist at any time.
//
//*****************************************************************************
typedef struct
{
    //
    //! The size of this structure.
    //
    long lSize;

    //
    //! The clipping region to be used when drawing onto the screen.
    //
    tRectangle sClipRegion;

    //
    //! The color used to draw primitives onto the screen.
    //
    unsigned long ulForeground;

    //
    //! The background color used to draw primitives onto the screen.
    //
    unsigned long ulBackground;

    //
    //! The font used to render text onto the screen.
    //
    const tFont *pFont;
}
tContext;

//*****************************************************************************
//
//! Sets the font to be used.
//!
//! \param pContext is a pointer to the drawing context to modify.
//! \param pFnt is a pointer to the font to be used.
//!
//! This function sets the font to be used for string drawing operations in the
//! specified drawing context.
//!
//! \return None.
//
//*****************************************************************************
#define GrContextFontSet(pContext, pFnt) \
        (pContext)->pFont = pFnt

//*****************************************************************************
//
//! Sets the foreground color to be used.
//!
//! \param pContext is a pointer to the drawing context to modify.
//! \param ulValue is the color to be used.
//!
//! This function sets the color to be used for drawing operations in the
//! specified drawing context.
//!
//! \return None.
//
//*****************************************************************************
#define GrContextForegroundSet(pContext, ulValue)             \
        (pContext)->ulForeground = DpyColorTranslate(ulValue)

//*****************************************************************************
//
//! Sets the background color to be used.
//!
//! \param pContext is a pointer to the drawing context to modify.
//! \param ulValue is the color to be used.
//!
//! This function sets the background color to be used for drawing operations
//! in the specified drawing context.
//!
//! \return None.
//
//*****************************************************************************
#define GrContextBackgroundSet(pContext, ulValue)             \
        (pContext)->ulBackground = DpyColorTranslate(ulValue)

//*****************************************************************************
//
//! Draws a pixel.
//!
//! \param pContext is a pointer to the drawing context to use.
//! \param lX is the X coordinate of the pixel.
//! \param lY is the Y coordinate of the pixel.
//!
//! This function draws a pixel.
//!
//! \return None.
//
//*****************************************************************************
#define GrPixelDraw(pContext, lX, lY)                       \
        if((lX >= (pContext)->sClipRegion.sXMin) &&         \
           (lX <= (pContext)->sClipRegion.sXMax) &&         \
           (lY >= (pContext)->sClipRegion.sYMin) &&         \
           (lY <= (pContext)->sClipRegion.sYMax))           \
        {                                                   \
            DpyPixelDraw(lX, lY, (pContext)->ulForeground); \
        }

//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pContext is a pointer to the drawing context to use.
//!
//! This function flushes any cached drawing operations.  For display drivers
//! that draw into a local frame buffer before writing to the actual display,
//! calling this function will cause the display to be updated to match the
//! contents of the local frame buffer.
//!
//! \return None.
//
//*****************************************************************************
#define GrFlush(pContext)       DpyFlush()

//*****************************************************************************
//
//! Draws a centered string.
//!
//! \param pContext is a pointer to the drawing context to use.
//! \param pcString is a pointer to the string to be drawn.
//! \param ulLength is the number of characters from the string that should be
//! drawn on the screen.
//! \param lX is the X coordinate of the center of the string position on the
//! screen.
//! \param lY is the Y coordinate of the center of the string position on the
//! screen.
//! \param bOpaque is true of the background of each character should be drawn
//! and false if it should not (leaving the background as is).
//!
//! This function draws a string of test on the screen centered upon the
//! provided position.  The \e ulLength parameter allows a portion of the
//! string to be examined without having to insert a NULL character at the
//! stopping point (which would not be possible if the string was located in
//! flash).
//!
//! \return None.
//
//*****************************************************************************
#define GrStringDrawCentered(pContext, pcString, ulLength, lX, lY, bOpaque) \
        do                                                                  \
        {                                                                   \
            char *pcStr = pcString;                                         \
                                                                            \
            GrStringDraw(pContext, pcStr, ulLength,                         \
                         (lX) -                                             \
                         (GrStringWidth(pContext, pcStr, ulLength) / 2),    \
                         (lY) - ((pContext)->pFont->cBaseline / 2),         \
                         bOpaque);                                          \
        }                                                                   \
        while(0)

//*****************************************************************************
//
// A set of color definitions.  This set is the subset of the X11 colors (from
// rgb.txt) that are supported by typical web browsers.
//
//*****************************************************************************
#define ClrAliceBlue            0x00F0F8FF
#define ClrAntiqueWhite         0x00FAEBD7
#define ClrAqua                 0x0000FFFF
#define ClrAquamarine           0x007FFFD4
#define ClrAzure                0x00F0FFFF
#define ClrBeige                0x00F5F5DC
#define ClrBisque               0x00FFE4C4
#define ClrBlack                0x00000000
#define ClrBlanchedAlmond       0x00FFEBCD
#define ClrBlue                 0x000000FF
#define ClrBlueViolet           0x008A2BE2
#define ClrBrown                0x00A52A2A
#define ClrBurlyWood            0x00DEB887
#define ClrCadetBlue            0x005F9EA0
#define ClrChartreuse           0x007FFF00
#define ClrChocolate            0x00D2691E
#define ClrCoral                0x00FF7F50
#define ClrCornflowerBlue       0x006495ED
#define ClrCornsilk             0x00FFF8DC
#define ClrCrimson              0x00DC143C
#define ClrCyan                 0x0000FFFF
#define ClrDarkBlue             0x0000008B
#define ClrDarkCyan             0x00008B8B
#define ClrDarkGoldenrod        0x00B8860B
#define ClrDarkGray             0x00A9A9A9
#define ClrDarkGreen            0x00006400
#define ClrDarkKhaki            0x00BDB76B
#define ClrDarkMagenta          0x008B008B
#define ClrDarkOliveGreen       0x00556B2F
#define ClrDarkOrange           0x00FF8C00
#define ClrDarkOrchid           0x009932CC
#define ClrDarkRed              0x008B0000
#define ClrDarkSalmon           0x00E9967A
#define ClrDarkSeaGreen         0x008FBC8F
#define ClrDarkSlateBlue        0x00483D8B
#define ClrDarkSlateGray        0x002F4F4F
#define ClrDarkTurquoise        0x0000CED1
#define ClrDarkViolet           0x009400D3
#define ClrDeepPink             0x00FF1493
#define ClrDeepSkyBlue          0x0000BFFF
#define ClrDimGray              0x00696969
#define ClrDodgerBlue           0x001E90FF
#define ClrFireBrick            0x00B22222
#define ClrFloralWhite          0x00FFFAF0
#define ClrForestGreen          0x00228B22
#define ClrFuchsia              0x00FF00FF
#define ClrGainsboro            0x00DCDCDC
#define ClrGhostWhite           0x00F8F8FF
#define ClrGold                 0x00FFD700
#define ClrGoldenrod            0x00DAA520
#define ClrGray                 0x00808080
#define ClrGreen                0x00008000
#define ClrGreenYellow          0x00ADFF2F
#define ClrHoneydew             0x00F0FFF0
#define ClrHotPink              0x00FF69B4
#define ClrIndianRed            0x00CD5C5C
#define ClrIndigo               0x004B0082
#define ClrIvory                0x00FFFFF0
#define ClrKhaki                0x00F0E68C
#define ClrLavender             0x00E6E6FA
#define ClrLavenderBlush        0x00FFF0F5
#define ClrLawnGreen            0x007CFC00
#define ClrLemonChiffon         0x00FFFACD
#define ClrLightBlue            0x00ADD8E6
#define ClrLightCoral           0x00F08080
#define ClrLightCyan            0x00E0FFFF
#define ClrLightGoldenrodYellow 0x00FAFAD2
#define ClrLightGreen           0x0090EE90
#define ClrLightGrey            0x00D3D3D3
#define ClrLightPink            0x00FFB6C1
#define ClrLightSalmon          0x00FFA07A
#define ClrLightSeaGreen        0x0020B2AA
#define ClrLightSkyBlue         0x0087CEFA
#define ClrLightSlateGray       0x00778899
#define ClrLightSteelBlue       0x00B0C4DE
#define ClrLightYellow          0x00FFFFE0
#define ClrLime                 0x0000FF00
#define ClrLimeGreen            0x0032CD32
#define ClrLinen                0x00FAF0E6
#define ClrMagenta              0x00FF00FF
#define ClrMaroon               0x00800000
#define ClrMediumAquamarine     0x0066CDAA
#define ClrMediumBlue           0x000000CD
#define ClrMediumOrchid         0x00BA55D3
#define ClrMediumPurple         0x009370DB
#define ClrMediumSeaGreen       0x003CB371
#define ClrMediumSlateBlue      0x007B68EE
#define ClrMediumSpringGreen    0x0000FA9A
#define ClrMediumTurquoise      0x0048D1CC
#define ClrMediumVioletRed      0x00C71585
#define ClrMidnightBlue         0x00191970
#define ClrMintCream            0x00F5FFFA
#define ClrMistyRose            0x00FFE4E1
#define ClrMoccasin             0x00FFE4B5
#define ClrNavajoWhite          0x00FFDEAD
#define ClrNavy                 0x00000080
#define ClrOldLace              0x00FDF5E6
#define ClrOlive                0x00808000
#define ClrOliveDrab            0x006B8E23
#define ClrOrange               0x00FFA500
#define ClrOrangeRed            0x00FF4500
#define ClrOrchid               0x00DA70D6
#define ClrPaleGoldenrod        0x00EEE8AA
#define ClrPaleGreen            0x0098FB98
#define ClrPaleTurquoise        0x00AFEEEE
#define ClrPaleVioletRed        0x00DB7093
#define ClrPapayaWhip           0x00FFEFD5
#define ClrPeachPuff            0x00FFDAB9
#define ClrPeru                 0x00CD853F
#define ClrPink                 0x00FFC0CB
#define ClrPlum                 0x00DDA0DD
#define ClrPowderBlue           0x00B0E0E6
#define ClrPurple               0x00800080
#define ClrRed                  0x00FF0000
#define ClrRosyBrown            0x00BC8F8F
#define ClrRoyalBlue            0x004169E1
#define ClrSaddleBrown          0x008B4513
#define ClrSalmon               0x00FA8072
#define ClrSandyBrown           0x00F4A460
#define ClrSeaGreen             0x002E8B57
#define ClrSeashell             0x00FFF5EE
#define ClrSienna               0x00A0522D
#define ClrSilver               0x00C0C0C0
#define ClrSkyBlue              0x0087CEEB
#define ClrSlateBlue            0x006A5ACD
#define ClrSlateGray            0x00708090
#define ClrSnow                 0x00FFFAFA
#define ClrSpringGreen          0x0000FF7F
#define ClrSteelBlue            0x004682B4
#define ClrTan                  0x00D2B48C
#define ClrTeal                 0x00008080
#define ClrThistle              0x00D8BFD8
#define ClrTomato               0x00FF6347
#define ClrTurquoise            0x0040E0D0
#define ClrViolet               0x00EE82EE
#define ClrWheat                0x00F5DEB3
#define ClrWhite                0x00FFFFFF
#define ClrWhiteSmoke           0x00F5F5F5
#define ClrYellow               0x00FFFF00
#define ClrYellowGreen          0x009ACD32

//*****************************************************************************
//
// Masks and shifts to aid in color format translation by drivers.
//
//*****************************************************************************
#define ClrRedMask              0x00FF0000
#define ClrRedShift             16
#define ClrGreenMask            0x0000FF00
#define ClrGreenShift           8
#define ClrBlueMask             0x000000FF
#define ClrBlueShift            0

//*****************************************************************************
//
// Prototypes for the predefined fonts in the graphics library.  ..Cm.. is the
// computer modern font, which is a serif font.  ..Cmsc.. is the computer
// modern small-caps font, which is also a serif font.  ..Cmss.. is the
// computer modern sans-serif font.
//
//*****************************************************************************
extern const tFont g_sFontCm12;
extern const tFont g_sFontCm12b;
extern const tFont g_sFontCm12i;
extern const tFont g_sFontCm14;
extern const tFont g_sFontCm14b;
extern const tFont g_sFontCm14i;
extern const tFont g_sFontCm16;
extern const tFont g_sFontCm16b;
extern const tFont g_sFontCm16i;
extern const tFont g_sFontCm18;
extern const tFont g_sFontCm18b;
extern const tFont g_sFontCm18i;
extern const tFont g_sFontCm20;
extern const tFont g_sFontCm20b;
extern const tFont g_sFontCm20i;
extern const tFont g_sFontCm22;
extern const tFont g_sFontCm22b;
extern const tFont g_sFontCm22i;
extern const tFont g_sFontCm24;
extern const tFont g_sFontCm24b;
extern const tFont g_sFontCm24i;
extern const tFont g_sFontCm26;
extern const tFont g_sFontCm26b;
extern const tFont g_sFontCm26i;
extern const tFont g_sFontCm28;
extern const tFont g_sFontCm28b;
extern const tFont g_sFontCm28i;
extern const tFont g_sFontCm30;
extern const tFont g_sFontCm30b;
extern const tFont g_sFontCm30i;
extern const tFont g_sFontCm32;
extern const tFont g_sFontCm32b;
extern const tFont g_sFontCm32i;
extern const tFont g_sFontCm34;
extern const tFont g_sFontCm34b;
extern const tFont g_sFontCm34i;
extern const tFont g_sFontCm36;
extern const tFont g_sFontCm36b;
extern const tFont g_sFontCm36i;
extern const tFont g_sFontCm38;
extern const tFont g_sFontCm38b;
extern const tFont g_sFontCm38i;
extern const tFont g_sFontCm40;
extern const tFont g_sFontCm40b;
extern const tFont g_sFontCm40i;
extern const tFont g_sFontCm42;
extern const tFont g_sFontCm42b;
extern const tFont g_sFontCm42i;
extern const tFont g_sFontCm44;
extern const tFont g_sFontCm44b;
extern const tFont g_sFontCm44i;
extern const tFont g_sFontCm46;
extern const tFont g_sFontCm46b;
extern const tFont g_sFontCm46i;
extern const tFont g_sFontCm48;
extern const tFont g_sFontCm48b;
extern const tFont g_sFontCm48i;
extern const tFont g_sFontCmsc12;
extern const tFont g_sFontCmsc14;
extern const tFont g_sFontCmsc16;
extern const tFont g_sFontCmsc18;
extern const tFont g_sFontCmsc20;
extern const tFont g_sFontCmsc22;
extern const tFont g_sFontCmsc24;
extern const tFont g_sFontCmsc26;
extern const tFont g_sFontCmsc28;
extern const tFont g_sFontCmsc30;
extern const tFont g_sFontCmsc32;
extern const tFont g_sFontCmsc34;
extern const tFont g_sFontCmsc36;
extern const tFont g_sFontCmsc38;
extern const tFont g_sFontCmsc40;
extern const tFont g_sFontCmsc42;
extern const tFont g_sFontCmsc44;
extern const tFont g_sFontCmsc46;
extern const tFont g_sFontCmsc48;
extern const tFont g_sFontCmss12;
extern const tFont g_sFontCmss12b;
extern const tFont g_sFontCmss12i;
extern const tFont g_sFontCmss14;
extern const tFont g_sFontCmss14b;
extern const tFont g_sFontCmss14i;
extern const tFont g_sFontCmss16;
extern const tFont g_sFontCmss16b;
extern const tFont g_sFontCmss16i;
extern const tFont g_sFontCmss18;
extern const tFont g_sFontCmss18b;
extern const tFont g_sFontCmss18i;
extern const tFont g_sFontCmss20;
extern const tFont g_sFontCmss20b;
extern const tFont g_sFontCmss20i;
extern const tFont g_sFontCmss22;
extern const tFont g_sFontCmss22b;
extern const tFont g_sFontCmss22i;
extern const tFont g_sFontCmss24;
extern const tFont g_sFontCmss24b;
extern const tFont g_sFontCmss24i;
extern const tFont g_sFontCmss26;
extern const tFont g_sFontCmss26b;
extern const tFont g_sFontCmss26i;
extern const tFont g_sFontCmss28;
extern const tFont g_sFontCmss28b;
extern const tFont g_sFontCmss28i;
extern const tFont g_sFontCmss30;
extern const tFont g_sFontCmss30b;
extern const tFont g_sFontCmss30i;
extern const tFont g_sFontCmss32;
extern const tFont g_sFontCmss32b;
extern const tFont g_sFontCmss32i;
extern const tFont g_sFontCmss34;
extern const tFont g_sFontCmss34b;
extern const tFont g_sFontCmss34i;
extern const tFont g_sFontCmss36;
extern const tFont g_sFontCmss36b;
extern const tFont g_sFontCmss36i;
extern const tFont g_sFontCmss38;
extern const tFont g_sFontCmss38b;
extern const tFont g_sFontCmss38i;
extern const tFont g_sFontCmss40;
extern const tFont g_sFontCmss40b;
extern const tFont g_sFontCmss40i;
extern const tFont g_sFontCmss42;
extern const tFont g_sFontCmss42b;
extern const tFont g_sFontCmss42i;
extern const tFont g_sFontCmss44;
extern const tFont g_sFontCmss44b;
extern const tFont g_sFontCmss44i;
extern const tFont g_sFontCmss46;
extern const tFont g_sFontCmss46b;
extern const tFont g_sFontCmss46i;
extern const tFont g_sFontCmss48;
extern const tFont g_sFontCmss48b;
extern const tFont g_sFontCmss48i;

//*****************************************************************************
//
// Prototypes for the low level graphics routines provided by the display
// driver.  Typically, these should not be directly called by an application or
// widget.
//
//*****************************************************************************
extern const unsigned long g_ulDpyWidth;
extern const unsigned long g_ulDpyHeight;
extern void DpyPixelDraw(long lX, long lY, unsigned long ulValue);
extern void DpyLineDrawH(long lX1, long lX2, long lY, unsigned long ulValue);
extern void DpyLineDrawV(long lX, long lY1, long lY2, unsigned long ulValue);
extern void DpyRectFill(const tRectangle *pRect, unsigned long ulValue);
extern unsigned long DpyColorTranslate(unsigned long ulValue);
extern void DpyFlush(void);

//*****************************************************************************
//
// Prototypes for the low level display driver routines that are directly
// accessible from application software. These functions are stubbed in
// cases where the display driver does not support or offer a given
// feature.
//
//*****************************************************************************
extern void DpyInit(void);
extern void DpyBacklightOn(void);
extern void DpyBacklightOff(void);

//*****************************************************************************
//
// Prototypes for the graphics library functions.
//
//*****************************************************************************
extern void GrCircleDraw(const tContext *pContext, long lX, long lY,
                         long lRadius);
extern void GrCircleFill(const tContext *pContext, long lX, long lY,
                         long lRadius);
extern void GrContextClipRegionSet(tContext *pContext, long lXMin, long lYMin,
                                   long lXMax, long lYMax);
extern void GrContextInit(tContext *pContext);
extern void GrLineDraw(const tContext *pContext, long lX1, long lY1, long lX2,
                       long lY2);
extern void GrLineDrawH(const tContext *pContext, long lX1, long lX2, long lY);
extern void GrLineDrawV(const tContext *pContext, long lX, long lY1, long lY2);
extern void GrRectDraw(const tContext *pContext, const tRectangle *pRect);
extern void GrRectFill(const tContext *pContext, const tRectangle *pRect);
extern void GrStringDraw(const tContext *pContext, const char *pcString,
                         unsigned long ulLength, long lX, long lY,
                         unsigned long bOpaque);
extern long GrStringWidth(const tContext *pContext, const char *pcString,
                          unsigned long ulLength);

#ifdef __cplusplus
}
#endif

#endif // __GRLIB_H__
