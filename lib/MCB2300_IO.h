#ifndef _MCB2300_IO
#define _MCB2300_IO

extern void MCB2300_AIInit(int32 nErrIn, uInt8 nChanEnable, int32* pnErrOut);
extern void MCB2300_AnalogIn(uInt8 nChan, int32 nErrIn, int32 *pnValue, Boolean *pbOverrun, int32* pnErrOut);
extern void MCB2300_AnalogInScan(int32 nErrIn, PDAArrPtr *arrValues, PDAArrPtr *arrOverruns, int32* pnErrOut);

extern void MCB2300_ButtonInit(int32 nErrIn, int32* pnErrOut);
extern void MCB2300_IsButtonPressed(int32 nErrIn, int32* pnErrOut, unsigned char *pbPressed);

extern void MCB2300_LEDOn (unsigned int num, int32 nErrIn, int32* pnErrOut);
extern void MCB2300_LEDOff (unsigned int num, int32 nErrIn, int32* pnErrOut);
extern void MCB2300_LEDSet(unsigned int value, int32 nErrIn, int32* pnErrOut);

extern void MCB2300_DigitalOutput( int32 nErrIn, int32* pnErrOut, int32 port, int32 pin, unsigned char cValue );
extern void MCB2300_DigitalInput( int32 nErrIn, int32* pnErrOut, int32 port, int32 pin, unsigned char *pcValue );
extern void MCB2300_DigitalPortOutput32( int32 nErrIn, int32* pnErrOut, int32 port, unsigned long nValue );
extern void MCB2300_DigitalPortOutput16( int32 nErrIn, int32* pnErrOut, int32 port, unsigned char bUpper, unsigned short sValue );
extern void MCB2300_DigitalPortOutput8( int32 nErrIn, int32* pnErrOut, int32 port, unsigned int nBytePosition, unsigned char cValue );
extern void MCB2300_DigitalPortInput32( int32 nErrIn, int32* pnErrOut, int32 port, unsigned long *pnValue );
extern void MCB2300_DigitalPortInput16( int32 nErrIn, int32* pnErrOut, int32 port, unsigned short bUpper, unsigned short *psValue );
extern void MCB2300_DigitalPortInput8( int32 nErrIn, int32* pnErrOut, int32 port, unsigned int nBytePosition, unsigned char *pcValue );

extern void MCB2300_PWMOut( uInt8 nChan, 
					uInt8 nOffset, 
					Boolean bDoubleEdge, 
					Boolean bPolarity, 
					uInt8 nDuration, 
					int32 nErrIn, 
					int32 *pnErrOut );
extern void MCB2300_PWMInit(int32 nCycleTime, int32 nErrIn, int32* pnErrOut);

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
#endif /* Simulator */
#endif
