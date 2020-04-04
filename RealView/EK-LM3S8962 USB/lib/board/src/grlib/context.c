//*****************************************************************************
//
// context.c - Routines for handling drawing contexts.
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
//! Initializes a drawing context.
//!
//! \param pContext is a pointer to the drawing context to initialize.
//! \param pDisplay is a pointer to the tDisplayInfo structure that describes
//! the display driver to use.
//!
//! This function initializes a drawing context, preparing it for use.  The
//! provided display driver will be used for all subsequent graphics
//! operations, and the default clipping region will be set to the extent of
//! the screen.
//!
//! \return None.
//
//*****************************************************************************
void
GrContextInit(tContext *pContext)
{
    //
    // Set the size of the context.
    //
    pContext->lSize = sizeof(tContext);

    //
    // Initialize the extent of the clipping region to the extents of the
    // screen.
    //
    pContext->sClipRegion.sXMin = 0;
    pContext->sClipRegion.sYMin = 0;
    pContext->sClipRegion.sXMax = g_ulDpyWidth - 1;
    pContext->sClipRegion.sYMax = g_ulDpyHeight - 1;

    //
    // Provide a default color and font.
    //
    pContext->ulForeground = 0;
    pContext->pFont = 0;
}

//*****************************************************************************
//
//! Sets the extents of the clipping region.
//!
//! \param pContext is a pointer to the drawing context to use.
//! \param lXMin is the X coordinate of the left side of the clipping region.
//! \param lYMin is the Y coordinate of the top side of the clipping region.
//! \param lXMax is the X coordinate of the right side of the clipping region.
//! \param lYMax is the Y coordinate of the bottom side of the clipping region.
//!
//! This function sets the extents of the clipping region.  The clipping region
//! is not allowed to exceed the extents of the screen, but may be a portion of
//! the screen.
//!
//! The supplied coordinate are inclusive; \e lXMin of 1 and \e lXMax of 1 will
//! define a clipping region that will display only the pixels in the X = 1
//! column.  A consequence of this is that the clipping region must contain
//! at least one row and one column.
//!
//! \return None.
//
//*****************************************************************************
void
GrContextClipRegionSet(tContext *pContext, long lXMin, long lYMin, long lXMax,
                       long lYMax)
{
    long lTemp;

    //
    // Swap the X coordinates if they were specified in the wrong order.
    //
    if(lXMin > lXMax)
    {
        lTemp = lXMin;
        lXMin = lXMax;
        lXMax = lTemp;
    }

    //
    // Swap the Y coordinates if they were specified in the wrong order.
    //
    if(lYMin > lYMax)
    {
        lTemp = lYMin;
        lYMin = lYMax;
        lYMax = lTemp;
    }

    //
    // Set the extents of the clipping region, forcing them to reside within
    // the extents of the screen.
    //
    pContext->sClipRegion.sXMin = ((lXMin < 0) ? 0 :
                                   ((lXMin >= g_ulDpyWidth) ?
                                    (g_ulDpyWidth - 1) : lXMin));
    pContext->sClipRegion.sYMin = ((lYMin < 0) ? 0 :
                                   ((lYMin >= g_ulDpyHeight) ?
                                    (g_ulDpyHeight - 1) : lYMin));
    pContext->sClipRegion.sXMax = ((lXMax < 0) ? 0 :
                                   ((lXMax >= g_ulDpyWidth) ?
                                    (g_ulDpyWidth - 1) : lXMax));
    pContext->sClipRegion.sYMax = ((lYMax < 0) ? 0 :
                                   ((lYMax >= g_ulDpyHeight) ?
                                    (g_ulDpyHeight - 1) : lYMax));
}
