/******************************************************************************/
/* LVARMLib.C: LabVIEW Specific ARM C Library implementations                 */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2007 Keil Software. All rights reserved.                     */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#ifndef _LVARMLIB_H_
#define _LVARMLIB_H_



void *RTX_malloc (unsigned int size);
void *RTX_calloc (unsigned int nelements, unsigned int bytes);
void *RTX_realloc (void *pointer, unsigned int size);
void RTX_free (void *pointer);


#endif /* _LVARMLIB_H_ */






