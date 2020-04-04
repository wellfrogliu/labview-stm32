/***************************************************************************************
	LVDefs_plat.h - Header file of basic constants and macros needed by the C Generator
					to run on Windows.

	© 2003-2008 National Instruments Corporation.
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Microprocessor SDK) and may not be copied, modified, or redistributed.  The code may
     only be used in accordance with the terms of the license agreement entered into between
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.

	Author: Andrew Dove
********************************************************************************************/
#ifndef _LVDefs_plat_h
#define _LVDefs_plat_h

/*
 *
 * Basic types
 *
 */

// Integers
typedef signed char int8;
typedef unsigned char uInt8;
typedef signed short int16;
typedef unsigned short uInt16;
typedef signed long int32;
typedef unsigned long uInt32;
typedef long long int64;
typedef unsigned long long uInt64;

// Floating-point numbers
// typedef float float32;
// typedef double float64;

// Fixed-point numbers
typedef uInt8  uFixedPoint8;
typedef uInt16 uFixedPoint16;
typedef uInt32 uFixedPoint32;
typedef int8   sFixedPoint8;
typedef int16  sFixedPoint16;
typedef int32  sFixedPoint32;

/* set error string verbosity level 
choose from
CCGERRSUPP_NONE
CCGERRSUPP_SHORT
CCGERRSUPP_VERBOSE 
use one of the first two to make your program smaller */
#define CCGERRSUPP_LEVEL CCGERRSUPP_VERBOSE
/* compiler definition to get function name (valid only within a function) */
#define CCGERRSUPP_FUNCNAME __FUNCTION__

// Boolean
typedef unsigned char Boolean;
#ifndef _extcode_H
#define FALSE				0
#define TRUE				1
#endif
#define false				((Boolean)0)
#define true				((Boolean)1)

// Characters and strings
typedef signed char TextChar;
typedef signed char* TextPtr;
typedef unsigned char OSTextChar;
typedef unsigned char* OSTextPtr;

// Pointers
typedef void* VoidPtr;

/*
 *
 * Floating point infinity and not-a-number constants
 *
 */
#define DoubleLVInfinity	{{0x00000000, 0x7ff00000}}
#define FloatLVInfinity		{{0x7f800000}}
#define DoubleLVNotANumber	{{0xffffffff, 0xffffffff}}
#define FloatLVNotANumber	{{0xffffffff}}

// #define isinf(x) (((x) == Inf) || ((x) == -Inf))
#define isPosInf(x) ((x) == Inf)
#define isNegInf(x) ((x) == -Inf)
// #define isnan _isnan

/*
 *
 * Floating-point number rounding
 *
 */
#define round(x) (((x)<0)?floor(x):ceil(x))

/*
 *
 * Random number generation
 *
 */
#define SysRandom(x) rand()
#define sysRandomMax RAND_MAX
#define LV_SRAND_INIT() // Seeding function.  Unimplemented in this example project.


/*
 *
 * String and character functions
 *
 */

// String manipulation
#define StrCopy(x, y) strcpy(x, y)
#define StrCat(x, y) strcat(x, y)
#define StrStr(x, y) strstr(x, y)
#define StrChr(x, y) strchr(x, y)
#define StrNCopy strncpy
#define StrNCat strncat

// String length
#define StrLen(x) strlen(x)

// Return type of StrLen
typedef signed long PDAStrLen_t;

// String comparison
#define StrCompare(x, y) strcmp(x, y)
#define StrCaselessCompare(x, y) _stricmp(x, y)
#define StrNCompare strncmp
#define StrNCaselessCompare(x, y, n) _strnicmp(x, y, n)

// String conversion
#define StrIToA(x, y) _itoa(y, x, 10)
#define StrAToI(x) atoi(x)

// String formatting
#define platform_printf	printf
#define StrPrintF sprintf(x)
#define StrNPrintF _snprintf(x)

// Character comparison
#define LVIsHex(c) ((((c) >= '0') && ((c) <= '9')) || (((c) >= 'a') && ((c) <= 'f')) || (((c) >= 'A') && ((c) <= 'F')))
#define LVIsDigit(c) iswdigit((unsigned short)c)
#define LVIsSpace(c) iswspace((unsigned short)c)
#define LVIsAlpha(c) isalpha((unsigned short)c)
#define LVIsPunct(c) ispunct((unsigned short)c)

// End-of-line character
#define SSEOL	"\r\n"

/*
 *
 * Memory handling
 *
 */

#define MemSet(x, y, z) memset(x, (int)z, (size_t)y)
#define MemMove(x, y, z) memmove(x, y, z)
#define MemCmp(x, y, z) memcmp(x, y, z)

#include "LVDefs_misc.h"

#endif /* don't add code after this */
