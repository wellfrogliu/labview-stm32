/***************************************************************************************
	NI CONFIDENTIAL

	MCB2300_LCD.c - Wrapper layer around the LCD functions to display on  
					the character LCD on the MCB 2300 board.

	© 2003-2008 National Instruments Corporation.
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Embedded Development Module) and may not be copied, modified, or redistributed.  The code may
     only be used in accordance with the terms of the license agreement entered into between
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.
	
	Author: Andrew Dove
********************************************************************************************/

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include "CCGStrSupport.h"
#include "LCD.h"

#define BUFLEN	16 // number of characters on single line of LCD screen

static unsigned char bLCDInited = false;

static void MCB2300_LCDClearLine(int32 iFrom, int32 iLine) {
  int i;

  LCD_gotoxy( iFrom, iLine );
	for (i=iFrom-1;i<BUFLEN;i++) {
		LCD_putc( ' ' );
	}
}

void MCB2300_LCDPuts( VoidHand vhLine1, VoidHand vhLine2, int32 nErrIn, int32* pnErrOut) {
	PDAStrPtr strLine;
	int i;
	if (nErrIn) {
		if (vhLine1)
			PDAStrFree( vhLine1 );
		if (vhLine2)
			PDAStrFree( vhLine2 );
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (!bLCDInited) {
		if (vhLine1)
			PDAStrFree( vhLine1 );
		if (vhLine2)
			PDAStrFree( vhLine2 );
		if (pnErrOut) {
			*pnErrOut = k_LCD_NotInit;
		}
		return;
	}

  // Write line 1
	if (vhLine1) {
		strLine = (PDAStrPtr)vhLine1;
		LCD_gotoxy( 1, 2 );
		for (i=0;i<strLine->len;i++) {
			LCD_putc( strLine->str[i] );
		}
        MCB2300_LCDClearLine( strLine->len + 1, 2 );
		PDAStrFree( vhLine1 );
	}
  else {
    MCB2300_LCDClearLine( 1, 2 );   
  }

  // Write line 2
	if (vhLine2) {
		strLine = (PDAStrPtr)vhLine2;
		LCD_gotoxy( 1, 1 );
		for (i=0;i<strLine->len;i++) {
			LCD_putc( strLine->str[i] );
		}
        MCB2300_LCDClearLine( strLine->len + 1, 1 );
		PDAStrFree( vhLine2 );
	}
  else {
    MCB2300_LCDClearLine( 1, 1 );   
  }
	if (pnErrOut) {
		*pnErrOut = 0;
	}
	return;
}

void Simulator_LCDPuts( VoidHand vhLine1, VoidHand vhLine2, int32 nErrIn, int32* pnErrOut) {
	if (vhLine1) {
		PDAStrFree( vhLine1 );
	}
	if (vhLine2) {
		PDAStrFree( vhLine2 );
	}
	if (pnErrOut) {
		*pnErrOut = nErrIn;
	}
	return;
}

void MCB2300_LCDPutc( VoidHand  vhChar, int32 nErrIn, int32* pnErrOut) {
	PDAStrPtr strChar;
	if (nErrIn) {
		if (vhChar)
			PDAStrFree( vhChar );
		if (*pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (!bLCDInited) {
		if (vhChar)
			PDAStrFree( vhChar );
		if (pnErrOut) {
			*pnErrOut = k_LCD_NotInit;
		}
		return;
	}
	if (vhChar) {
		strChar = (PDAStrPtr)vhChar;
		strChar->str[strChar->len] = 0;
		LCD_putc( strChar->str[0] );
		PDAStrFree( vhChar );
	}
	if (pnErrOut) {
		*pnErrOut = 0;
	}
	return;
}

void Simulator_LCDPutc( VoidHand  vhChar, int32 nErrIn, int32* pnErrOut) {
	if (vhChar) {
		PDAStrFree( vhChar );
	}
	if (pnErrOut) {
		*pnErrOut = nErrIn;
	}
	return;
}

void MCB2300_LCDOn( int32 nErrIn, int32* pnErrOut) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (!bLCDInited) {
		if (pnErrOut) {
			*pnErrOut = k_LCD_NotInit;
		}
		return;
	}
	LCD_on( );
	if (pnErrOut) {
		*pnErrOut = 0;
	}

	return;
}

void Simulator_LCDOn( int32 nErrIn, int32* pnErrOut) {
	if (pnErrOut) {
		*pnErrOut = nErrIn;
	}
	return;
}

void MCB2300_LCDOff( int32 nErrIn, int32* pnErrOut) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (!bLCDInited) {
		if (pnErrOut) {
			*pnErrOut = k_LCD_NotInit;
		}
		return;
	}
	LCD_off( );
	if (pnErrOut) {
		*pnErrOut = 0;
	}

	return;
}

void Simulator_LCDOff( int32 nErrIn, int32* pnErrOut) {
	if (pnErrOut) {
		*pnErrOut = nErrIn;
	}
	return;
}


void MCB2300_LCDCurOff( int32 nErrIn, int32* pnErrOut) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (!bLCDInited) {
		if (pnErrOut) {
			*pnErrOut = k_LCD_NotInit;
		}
		return;
	}
	LCD_cur_off( );
	if (pnErrOut) {
		*pnErrOut = 0;
	}

	return;
}

void Simulator_LCDCurOff( int32 nErrIn, int32* pnErrOut) {
	if (pnErrOut) {
		*pnErrOut = nErrIn;
	}
	return;
}

void MCB2300_LCDCls( int32 nErrIn, int32* pnErrOut) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (!bLCDInited) {
		if (pnErrOut) {
			*pnErrOut = k_LCD_NotInit;
		}
		return;
	}
	LCD_cls( );
	if (pnErrOut) {
		*pnErrOut = 0;
	}

	return;
}

void Simulator_LCDCls( int32 nErrIn, int32* pnErrOut) {
	if (pnErrOut) {
		*pnErrOut = nErrIn;
	}
	return;
}

void MCB2300_LCDGotoXY( uInt32 nX, uInt32 nY, int32 nErrIn, int32* pnErrOut) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (nX < 1 || nX > 16 || nY < 1 || nY > 2) {
		if (pnErrOut) {
			*pnErrOut = k_XY_OutOfRange;
		}
		return;
	}
	if (!bLCDInited) {
		if (pnErrOut) {
			*pnErrOut = k_LCD_NotInit;
		}
		return;
	}
	LCD_gotoxy( nX, nY );
	if (pnErrOut) {
		*pnErrOut = 0;
	}

	return;
}
void Simulator_LCDGotoXY( uInt32 nX, uInt32 nY, int32 nErrIn, int32* pnErrOut) {
	if (pnErrOut) {
		*pnErrOut = nErrIn;
	}
	return;
}


void MCB2300_LCDInit( int32 nErrIn, int32* pnErrOut) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	LCD_init( );
	bLCDInited = true;
	if (pnErrOut) {
		*pnErrOut = 0;
	}

	return;
}

void Simulator_LCDInit( int32 nErrIn, int32* pnErrOut) {
	if (pnErrOut) {
		*pnErrOut = nErrIn;
	}
	return;
}


void MCB2300_LCDBarGraph( uInt32 nVal, uInt32 nSize, int32 nErrIn, int32* pnErrOut) {
	if (nErrIn) {
		if (pnErrOut) {
			*pnErrOut = nErrIn;
		}
		return;
	}
	if (nVal > 100) {
		if (pnErrOut) {
			*pnErrOut = k_Bar_OutOfRange;
		}
		return;
	}
	if (nSize < 1 || nSize > 16) {
		if (pnErrOut) {
			*pnErrOut = k_Bar_TooLarge;
		}
		return;
	}
	if (!bLCDInited) {
		if (pnErrOut) {
			*pnErrOut = k_LCD_NotInit;
		}
		return;
	}
	LCD_bargraph( nVal, nSize );
	if (pnErrOut) {
		*pnErrOut = 0;
	}

	return;
}


void Simulator_LCDBarGraph( uInt32 nVal, uInt32 nSize, int32 nErrIn, int32* pnErrOut) {
	if (pnErrOut) {
		*pnErrOut = nErrIn;
	}
	return;
}

