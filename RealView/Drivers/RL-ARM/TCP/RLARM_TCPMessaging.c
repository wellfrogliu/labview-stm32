/***************************************************************************************
	NI CONFIDENTIAL

	TCPMessaging_rtx.c - socket interface for the debugger for RL-ARM TCP/IP stack targets

	© 2003-2006 National Instruments Corporation.  
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Embedded Development Module) and may not be copied, modified, or redistributed.  The code may 
     only be used in accordance with the terms of the license agreement entered into between 
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.
	
	Author: Richard Sewell 
********************************************************************************************/

/*
	NI CONFIDENTIAL
	TCPMessaging.c - socket interface for the debugger for RL-ARM TCP/IP stack targets

	© Copyright 2002 by National Instruments.
	© Copyright 2007-2008 by KEIL - An ARM Company.
	All rights reserved.
	
	Andrew Callan
  Richard Sewell
*/
#ifdef CStatic
#undef CStatic
#endif

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include "CCGConfig.h"
#if SocketSupport
#if (defined(_Include_PDA_DEBUG) || defined(_Include_Everything)) && UsesTCPDebugger

#include "RLARM_TCPWrapper.h"
#include "TCPMessaging.h"
#include "Debug.h"

#define kTCPDebuggerPort 4434

static SOCKET dbgSock = INVALID_SOCKET;
static int32 gHostIp = 0;


void DebugSetIP(uInt32 ipaddr) {
	if(ipaddr) {
		gHostIp = ToBigEndL(ipaddr);
	}
}

		  
eRunStatus CCGDebugTCPPrepareConnection() {	
#if !ReverseTCPConnection
	DEBUG_LOG3(("CCGDebugTCPPrepareConnection begin\n"));

	dbgSock = RTX_tcp_open_socket (TCP_TYPE_SERVER, 0, 5000, 1500);
	if (dbgSock) {
		/* Start listening on TCP port kTCPDebuggerPort */
		RTX_tcp_listen (dbgSock, kTCPDebuggerPort);
	}

	DEBUG_LOG3(("CCGDebugTCPPrepareConnection end\n"));
#endif // !ReverseTCPConnection
	return eFinished;
}

eRunStatus CCGDebugTCPReleaseConnection() {
	DEBUG_LOG3(("CCGDebugReleaseConnection begin\n"));
	// Not required for RL-ARM stack
	DEBUG_LOG3(("CCGDebugReleaseConnection end\n"));
	return eFinished;
}

eRunStatus CCGDebugTCPConnect(uInt32 ipAddr) {
#if !ReverseTCPConnection
	DEBUG_LOG3(("CCGDebugTCPConnect begin\n"));
	// Not required for RL-ARM stack
	connectionStat = pdaConnected;
	DEBUG_LOG3(("CCGDebugTCPConnect end\n"));
#else // !ReverseTCPConnection
	U8   tcpstate;
    BOOL bConnected;
	uInt32 nStartMs = LVGetTicks();
	const uInt32 nTimeout = 1000;

	DEBUG_LOG3(("CCGDebugTCPConnect begin\n"));
	connectionStat = pdaDisconnected;
    dbgSock = RTX_tcp_open_socket (TCP_TYPE_CLIENT, /* Type      */
                                0,                  /* TOS bits  */
                                5000,               /* Timeout   */
                                1500);              /* Buf size  */ 

    RTX_get_host_by_name (TCP_DEBUG_HOST, (U8*)&ipAddr);
    bConnected = RTX_tcp_connect (dbgSock, (U8 *)&ipAddr, kTCPDebuggerPort, 0);
	
	if(!dbgSock)
        return eFail;

	// so loop until connection is made
	while(bConnected){
		// Wait then see if we are connected		
		tcpstate = RTX_tcp_get_state(dbgSock);
		if (tcpstate == TCP_STATE_CONNECT) {
			// We are connected!
			connectionStat = pdaConnected;
			return eFinished;
		}
		else if (nTimeout && ((LVGetTicks() - nStartMs) > nTimeout)){
			RTX_tcp_close_socket(dbgSock);
			dbgSock = INVALID_SOCKET;
			return eNotFinished;
		}
		else {
			SysTaskDelay(1); // pump messages
		}
	}
	DEBUG_LOG3(("CCGDebugTCPConnect end\n"));

#endif // !ReverseTCPConnection
	return eFinished;
}


eRunStatus CCGDebugTCPDisconnect() {  
#if !ReverseTCPConnection
	DEBUG_LOG3(("CCGDebugTCPDisconnect begin\n"));
	// Not required for RL-ARM stack
	DEBUG_LOG3(("CCGDebugTCPDisconnect end\n"));
#else // !ReverseTCPConnection
	DEBUG_LOG3(("CCGDebugTCPDisconnect begin\n"));
	RTX_tcp_close_socket(dbgSock);
	dbgSock = INVALID_SOCKET;
	DEBUG_LOG3(("CCGDebugTCPDisconnect end\n"));
#endif // !ReverseTCPConnection
	return eFinished;
}

eRunStatus CCGDebugTCPWrite(const char* pc, uInt32 sz) {
	U16 nBytes = (U16)sz;
  DEBUG_LOG3(("CCGDebugTCPWrite begin\n"));
	
  if (RTX_tcp_send (dbgSock, (U8 *)pc, &nBytes, 10000) != __TRUE) {
    return eFail;
  }

  DEBUG_LOG3(("CCGDebugTCPWrite end\n"));

	return eFinished;
}

eRunStatus CCGDebugTCPRead(char **pc, uInt32 reqBytes, uInt32 *actualBytes) {
	DEBUG_LOG3(("CCGDebugTCPRead begin\n"));

	if(dbgSock == INVALID_SOCKET || !pc || !(*pc = (char *)MemPtrNew(reqBytes))) {
		return eFail;
  }

  RTX_tcp_recv (dbgSock, *pc, (U16 *)&reqBytes);
  *actualBytes = reqBytes; 

  DEBUG_LOG3(("CCGDebugTCPRead end\n"));

	return eFinished;
}

eRunStatus CCGDebugTCPBytes(uInt32* bytes) {
	DEBUG_LOG3(("CCGDebugTCPBytes begin\n"));
	if(dbgSock == INVALID_SOCKET)
		return eFail;

	*bytes = RTX_tcp_rx_bytes(dbgSock);

	DEBUG_LOG3(("CCGDebugTCPBytes end\n"));
	return eFinished;
}

/*===================================================================*/
// Helper functions
/*===================================================================*/


#endif // (defined(_Include_PDA_DEBUG) || defined(_Include_Everything)) && UsesTCPDebugger

#endif // SocketSupport
