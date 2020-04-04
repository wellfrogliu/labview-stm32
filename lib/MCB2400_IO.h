#ifndef _MCB2400_IO
#define _MCB2400_IO
#include "LVSysIncludes.h"
#include "LVCCG.h"
#include <LPC23xx.H>                    /* LPC23xx/LPC24xx definitions                */

extern void MCB2400_AIInit(int32 nErrIn, uInt8 nChanEnable, int32* pnErrOut);
extern void MCB2400_AnalogIn(uInt8 nChan, int32 nErrIn, uInt16 *pnValue, Boolean *pbOverrun, int32* pnErrOut);
extern void MCB2400_AnalogInScan(int32 nErrIn, PDAArrPtr *arrValues, PDAArrPtr *arrOverruns, int32* pnErrOut);

extern void MCB2400_AnalogOut( uInt16 nValue, int32 nErrIn, int32 *pnErrOut );

extern void MCB2400_LEDInit (void);
extern void MCB2400_LEDOn (uInt8 num, int32 nErrIn, int32* pnErrOut);
extern void MCB2400_LEDOff (uInt8 num, int32 nErrIn, int32* pnErrOut);
extern void MCB2400_LEDSet(uInt8 value, int32 nErrIn, int32* pnErrOut);

extern void MCB2400_IsButtonPressed(int32 nErrIn, int32* pnErrOut, uInt8 button, unsigned char *pbPressed);
extern void MCB2400_ButtonsPressed(int32 nErrIn, int32* pnErrOut, VoidHand *pbPressed);


extern void MCB2400_DigitalOutput( int32 nErrIn, int32* pnErrOut, uInt8 port, int32 pin, uInt8 cValue );
extern void MCB2400_DigitalInput( int32 nErrIn, int32* pnErrOut, uInt8 port, int32 pin, uInt8 *pcValue );
extern void MCB2400_DigitalPortOutput32( int32 nErrIn, int32* pnErrOut, uInt8 port, uInt32 nValue );
extern void MCB2400_DigitalPortOutput16( int32 nErrIn, int32* pnErrOut, uInt8 port, Boolean bUpper, uInt16 sValue );
extern void MCB2400_DigitalPortOutput8( int32 nErrIn, int32* pnErrOut, uInt8 port, uInt8 nBytePosition, uInt8 cValue );
extern void MCB2400_DigitalPortInput32( int32 nErrIn, int32* pnErrOut, uInt8 port, uInt32 *pnValue );
extern void MCB2400_DigitalPortInput16( int32 nErrIn, int32* pnErrOut, uInt8 port, Boolean bUpper, uInt16 *psValue );
extern void MCB2400_DigitalPortInput8( int32 nErrIn, int32* pnErrOut, uInt8 port, uInt8 nBytePosition, uInt8 *pcValue );

extern void MCB2400_PWM0Out( uInt8 nChan, 
					uInt8 nOffset, 
					Boolean bDoubleEdge, 
					Boolean bPolarity, 
					uInt8 nDuration, 
					int32 nErrIn, 
					int32 *pnErrOut );
extern void MCB2400_PWM0Init(int32 nCycleTime, int32 nErrIn, int32* pnErrOut);
extern void MCB2400_PWM1Out( uInt8 nChan, 
					uInt8 nOffset, 
					Boolean bDoubleEdge, 
					Boolean bPolarity, 
					uInt8 nDuration, 
					int32 nErrIn, 
					int32 *pnErrOut );
extern void MCB2400_PWM1Init(int32 nCycleTime, int32 nErrIn, int32* pnErrOut);
void MCB2400_PWMInit(uInt8 select, int32 nCycleTime, int32 nErrIn, int32* pnErrOut);
void MCB2400_PWMOut( uInt8 select, 
					uInt8 nChan, 
					uInt8 nOffset, 
					Boolean bDoubleEdge, 
					Boolean bPolarity, 
					uInt8 nDuration, 
					int32 nErrIn, 
					int32 *pnErrOut );

/*
#if defined(Simulator) && !defined(_LIB)
extern void Simulator_LCDPuts( VoidHand vhLine1, VoidHand vhLine2, int32 nErrIn, int32* pnErrOut);
extern void Simulator_LCDPutc( VoidHand  vhChar, int32 nErrIn, int32* pnErrOut);
extern void Simulator_LCDOn( int32 nErrIn, int32* pnErrOut);
extern void Simulator_LCDOff( int32 nErrIn, int32* pnErrOut);
extern void Simulator_LCDCurOff( int32 nErrIn, int32* pnErrOut);
extern void Simulator_LCDCls( int32 nErrIn, int32* pnErrOut);
extern void Simulator_LCDGotoXY( uInt32 nX, uInt32 nY, int32 nErrIn, int32* pnErrOut);
extern void Simulator_LCDInit( int32 nErrIn, int32* pnErrOut);
extern void Simulator_LCDBarGraph( uInt32 nVal, uInt32 nSize, int32 nErrIn, int32* pnErrOut);
#define MCB2300_LCDPuts Simulator_LCDPuts
#define MCB2300_LCDPutc Simulator_LCDPutc
#define MCB2300_LCDOn Simulator_LCDOn
#define MCB2300_LCDOff Simulator_LCDOff
#define MCB2300_LCDCurOff Simulator_LCDCurOff
#define MCB2300_LCDCls Simulator_LCDCls
#define MCB2300_LCDGotoXY Simulator_LCDGotoXY
#define MCB2300_LCDInit Simulator_LCDInit
#define MCB2300_LCDBarGraph Simulator_LCDBarGraph
#else
extern void MCB2300_LCDPuts( VoidHand vhLine1, VoidHand vhLine2, int32 nErrIn, int32* pnErrOut);
extern void MCB2300_LCDPutc( VoidHand  vhChar, int32 nErrIn, int32* pnErrOut);
extern void MCB2300_LCDOn( int32 nErrIn, int32* pnErrOut);
extern void MCB2300_LCDOff( int32 nErrIn, int32* pnErrOut);
extern void MCB2300_LCDCurOff( int32 nErrIn, int32* pnErrOut);
extern void MCB2300_LCDCls( int32 nErrIn, int32* pnErrOut);
extern void MCB2300_LCDGotoXY( uInt32 nX, uInt32 nY, int32 nErrIn, int32* pnErrOut);
extern void MCB2300_LCDInit( int32 nErrIn, int32* pnErrOut);
extern void MCB2300_LCDBarGraph( uInt32 nVal, uInt32 nSize, int32 nErrIn, int32* pnErrOut);
#endif // Simulator */

#endif
