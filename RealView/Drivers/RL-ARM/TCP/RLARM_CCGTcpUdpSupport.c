/***************************************************************************************
	NI CONFIDENTIAL

	CCGTcpUdpSupport.c - description

	© 2003-2008 National Instruments Corporation.
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Microprocessor SDK) and may not be copied, modified, or redistributed.  The code may
     only be used in accordance with the terms of the license agreement entered into between 
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.
	
	Author: Miko Hadikusuma
          Richard Sewell 
********************************************************************************************/

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include "CCGConfig.h"

#if DEBUG_LOG > 0
extern U16 os_time;
#endif

#ifndef CStatic /* {RFD} Unimplemented for static */
#if TCPUDPSupport

#if defined(_Include_CallByRef) || defined(_Include_VariableSupport) || defined(_Include_UDPConnWrite)
#define _TCP_Needed_By_Others // defined by features that require TCP support
#endif

#include "CCGCommsSupport.h"
#include "CCGDataSupport.h"            
#include "CCGWinsock.h"
#include "RLARM_TCPWrapper.h"

#define pStr ((PDAStrPtr)address)
#define pIPStr ((PDAStrPtr)vhIPString)

#define kMaxDomainNameLen	16

#if defined(_Include_TCPCreateListener) || defined(_Include_Everything)

Boolean TCPCreateListener(uInt32 netAddrIn,
						  uInt16 port,              // input REQUIRED
						  String serviceNameIn,		// input
						  Cluster vherrin,          // input
						  NetConnRefNum* listIdOut, // output
						  uInt16 * portOut,
						  Cluster vherrout)         // output
{
	int e = 0;
	SOCKET wsock = INVALID_SOCKET;
	Boolean rVal;
	NCRefNum* pNCRef = NULL;
  BOOL res = __FALSE; 

	if (serviceNameIn)
		PDAStrFree(serviceNameIn);
		
	// no outputs, return immediately
	if (!listIdOut && !vherrout && vherrin)
	{
		PDAClusterFree(vherrin, ErrorClusterDataType);
			
		return true;
	}
		
	if (portOut) *portOut = 0;
	
	// allocate list id out
	if (listIdOut)
	{
		*listIdOut = NetConnRefNumNewEmpty(TCPRefNum);
		if (!(*listIdOut))
			return false;
	}

	// copy error in to out
	rVal = CopyErrorInToOut(vherrin, vherrout);
	if (!rVal) 
	{
		// there is no previous error
    /* Here is where we do the work. In RTX, a 'Listener' is a server socket */
    wsock = RTX_tcp_open_socket (TCP_TYPE_SERVER, /* Type      */
                                0,               /* TOS bits  */
                                5000,            /* Timeout   */
                                1522);           /* Buf size (enough for 1 packet) */      

    /* Start listening on TCP port 'port' */
    if (wsock) 
      res = RTX_tcp_listen (wsock, port);

		if (!res) {      
			// there is an error
      int32 errCode;
      if (wsock) {
        RTX_tcp_close_socket(wsock);  			
        errCode = WSAETIMEDOUT;
      }		
      else {        
        errCode = WSAEOUTOFMEM;
      }
      if (vherrout) SendError(vherrout, errCode, _LVT(""));
			return true;
		}

		// get the valid refnum			
		if(listIdOut){
			pNCRef = *listIdOut;
			if(!pNCRef){
				SetError(kPDATCPSupportModNum, kBadLock );
				return false;
			}
			pNCRef->refcnt = 1; 	
			pNCRef->type = TCPRefNum;
			pNCRef->rnum = wsock;
			if (portOut) *portOut = port;
		}
	}
	// there is a previous error, don't do anything else
	return true;
}
#endif


#if (TCPUDPSupport && defined(_TCP_Needed_By_Others)) || defined(_Include_TCPOpenConnection) || defined(_Include_Everything)
// caller has to delete the returned text ptr
static TextPtr NIServiceLocatorUrlToURI(String serviceName)
{
	TextPtr pUrlOut = NULL;
	int32 i = 0, sz = 0;
	while (i < 2) {
		TextPtr pTemp = ((PDAStrPtr)serviceName)->str;
		TextPtr pEnd = pTemp + ((PDAStrPtr)serviceName)->len;
		if (i > 0) {
			pUrlOut = MemPtrNew(sz + 1);
			if (!pUrlOut) {
				break;
			}
		}
		for (; pTemp != pEnd; ++pTemp) {
			if (LVIsAlpha(*pTemp) || LVIsDigit(*pTemp)
				|| *pTemp == '-' || *pTemp == '_' || *pTemp == '.'
				|| *pTemp == '!' || *pTemp == '~' || *pTemp == '*'
				|| *pTemp== '\'' || *pTemp == '(' || *pTemp == ')') {
				if (i == 0) {
					sz++;
				}
				else {
					*pUrlOut = *pTemp;
					++pUrlOut;
				}
			}
			else {
				if (i == 0) {
					sz += 3;
				}
				else {
					StrPrintF(pUrlOut, _LVT("%%%2X"), *pTemp);
					pUrlOut += 3;
				}
			}
		}
		i++;
	}

	*pUrlOut = 0; // end with NULL
	pUrlOut -= sz;
	return pUrlOut;
}

// inputs to this function MUST NOT be NULL
// service example: "National Instruments/LabVIEW/MPU/8.6/VariableService"
Err NIServiceLocatorGetServicePort(String address, String serviceName, int32 nTimeout, uInt16* pPortOut)
{
    BOOL bConnected;
	Err err = kNoErr;
	uInt32 nStartMs = LVGetTicks();
	SocketStatus wsockStat;
	SOCKET wsock = INVALID_SOCKET;
    uInt32 ipAddr;
	U8 sockState;
	
	DEBUG_LOG3(("NIServiceLocatorGetServicePort begin - %d\n", os_time));
	//err = SetSocketNonBlockingMode(wsock);
	
	if (err) {
		goto niServiceLocatorGetServicePortOut;
	}
	// connect to NI Service Locator
    wsock = RTX_tcp_open_socket (TCP_TYPE_CLIENT, /* Type      */
                                0,               /* TOS bits  */
                                nTimeout,         /* Timeout   */
                                1522);           /* Buf size  */ 


    RTX_get_host_by_name (pStr->str, (U8*)&ipAddr);
    bConnected = RTX_tcp_connect (wsock, (U8 *)&ipAddr, 3580, 0);

	// so loop until connection is made
	while(bConnected){
		// Wait then see if we are connected		
		if ((sockState = RTX_tcp_get_state(wsock)) != TCP_STATE_CONNECT) {  			
			if ((nTimeout >= 0) && ((LVGetTicks() - nStartMs) > (uInt32)nTimeout)) {
				err = WSAETIMEDOUT;
				goto niServiceLocatorGetServicePortOut;
			}
			WaitNextMultiple(55, NULL);
		}
		else {
			// We are connected!
			break;
		}
	}
	
	// query port number
	if (bConnected){
		U16 nBytes = 0;
		U16 nRecvd = 0;
		TextPtr buf = NULL;
		TextPtr pServiceName = NIServiceLocatorUrlToURI(serviceName);
		if (!pServiceName) {
			err = kMemErr;
			goto niServiceLocatorGetServicePortOut;
		}
		buf = MemPtrNew(256);
		if(!buf){
			err = kMemErr;
			goto niServiceLocatorGetServicePortOut;
		}
		StrCopy(buf, _LVT("GET /"));
		StrCat(buf, pServiceName);
		StrCat(buf, _LVT(" HTTP/1.0\r\n\r\n"));
		MemPtrFree(pServiceName);
		// send command
		nBytes = StrLen(buf);
		if(RTX_tcp_send(wsock, buf, &nBytes, nTimeout) != __TRUE || nBytes != StrLen(buf)){
			err = WSAETIMEDOUT;
			if (buf) MemPtrFree(buf);
			goto niServiceLocatorGetServicePortOut;
		}
		// receive service information
		buf[0] = 0;
		while(1) {
			TextPtr pTemp;
			nBytes = 256 - nRecvd;
			if(RTX_tcp_recv(wsock, buf + nRecvd, &nBytes) != __TRUE){
				if (buf) MemPtrFree(buf);
				goto niServiceLocatorGetServicePortOut;
			}
			nRecvd = nBytes;
			// parse the information
			if (nBytes) {
				pTemp = StrChr(buf, '=');
				if (pTemp) {
					pTemp++;
					*pPortOut = StrAToI(pTemp);
				}
				else {
					err = kServiceNotFound;
				}
				if (buf) MemPtrFree(buf);
				break;
			}
			if ((nTimeout >= 0) && ((LVGetTicks() - nStartMs) > (uInt32)nTimeout)) {
				err = WSAETIMEDOUT;
				if (buf) MemPtrFree(buf);
				goto niServiceLocatorGetServicePortOut;
			}
			else
				WaitNextMultiple(55, NULL);
		}
	}
	else
      err = WSAEOUTOFMEM;

niServiceLocatorGetServicePortOut:
	RTX_tcp_close_socket(wsock);
	DEBUG_LOG3(("NIServiceLocatorGetServicePort end - %d\n", os_time));
	return err;
}

#endif

#if (TCPUDPSupport && defined(_Include_CallByRef)) || defined(_Include_TCPOpenConnection) || defined(_Include_Everything)

/** TCPOpenConnection() - 
	Opens an TCP connection.
	\param address remote service name, must NOT be NULL
	\param port 
	\param pTimeout pointer to timeout
	\param dtTimeout timeout data type, default: int32
	\param vherrin error in
	\param connIdOut pointer to connection id out
	\param vherrout pointer to error out
	\return eRunStatus
*/
eRunStatus TCPOpenConnection(String address,      // input - REQUIRED
							 void* pPort,                       // input - REQUIRED
							 DataType dtPort,                   // default: uInt16
							 void* pTimeout,                    // input
							 DataType dtTimeout,                // default: int32
							 Cluster vherrin,                   // input
							 NetConnRefNum* connIdOut,          // output
							 Cluster vherrout,                  // output
							 Boolean bRunToFinish,              // run to finish?
							 Boolean* bFirstRun,                // first run?
							 int32* pRefN,                      // saved refnum
							 uInt32* pStartMs,                  // saved start ms
							 int32* pSavedTimeout               // saved timeout value
							)
	
{
	eRunStatus runStat = eFinished;
	Boolean rVal = FALSE;
	int err = 0;
	uInt16 port = 0;
	int32 timeout = 60000;	// default timeout is 60s
	int32 portTimeout = 1000;
	uInt32 retryCounter;
	int32 curMs = 0;
	SOCKET wsock = INVALID_SOCKET;
	NCRefNum* pNCRef = NULL;
  U32 ipAddr;
		
	if (*bFirstRun)
	{
		*bFirstRun = FALSE;

		// copy error in to out
		rVal = CopyErrorInToOut(vherrin, vherrout);

		// get inputs
		if (pPort) {
			if (IsString(dtPort)) { // get service port from NI Service Locator
				err = NIServiceLocatorGetServicePort(address, *((String*)pPort), portTimeout, &port);
				for(retryCounter=0; retryCounter<4 && err; retryCounter++) // retry up to 4 times
					err = NIServiceLocatorGetServicePort(address, *((String*)pPort), portTimeout, &port);
					
				PDAStrFree(*((String*)pPort));
			}
			else {
				port = (uInt16)PtrToULong(pPort, dtPort);
			}
		}

		if (pTimeout) {
			timeout = LVPtrToLong(pTimeout, dtTimeout);
		}

		// allocate output
		if (connIdOut) {
			*connIdOut = NetConnRefNumNewEmpty(TCPRefNum);
			if (!(*connIdOut)) {
				runStat = eFail;
				goto openOut;
			}
		}

		// if a previous error, don't do anything else
		if (rVal) {
			goto openOut;
		}

    if (timeout == 0) timeout = 5000;
    wsock = RTX_tcp_open_socket (TCP_TYPE_CLIENT, /* Type      */
                                0,               /* TOS bits  */
                                timeout,         /* Timeout   */
                                1522);           /* Buf size  */ 



    RTX_get_host_by_name (pStr->str, (U8*)&ipAddr);
    RTX_tcp_connect (wsock, (U8 *)&ipAddr, port, 0);

			
		if (!wsock) {
      err = WSAEOUTOFMEM;
			goto openOut;
    }

		*pRefN = wsock;

		// copy refnum to connection id out
		if (connIdOut) {
			pNCRef = (NCRefNum*)(*connIdOut);
			if(!pNCRef){
				SetError(kPDATCPSupportModNum, kBadLock );
				return eFail;
			}
			pNCRef->rnum = wsock;
		}

		if (pSavedTimeout)
			*pSavedTimeout = timeout;

		*pStartMs = LVGetTicks();
	}
	else // not first run
	{
		wsock = *pRefN;

		if (pSavedTimeout)
			timeout = *pSavedTimeout;
	}

	// so loop until connection is made
	while(1)
	{
    // Wait then see if we are connected		
    if (RTX_tcp_get_state(wsock) != TCP_STATE_CONNECT) {  			
  		if ((timeout >= 0) && ((LVGetTicks() - *pStartMs) > (uInt32)timeout)) {
  			err = WSAETIMEDOUT;
  			goto openOut;
  		}
  
  		if (bRunToFinish)
  			WaitNextMultiple(55, NULL);
  		else
  			return eNotFinished;
    }
    else {
      // We are connected!
      break;
    }
	}

openOut:
	if (err) {
		if (vherrout)
			SendError(vherrout, err, _LVT(""));

		if (wsock != INVALID_SOCKET) {
      RTX_tcp_close_socket(wsock);
			wsock  = INVALID_SOCKET;
		}
	}

  // Free the address input
	PDAStrFree(address);
	address = NULL;

	return runStat;
}

#endif



#if defined(_Include_TCPNetToIPString) || defined(_Include_UDPConnWrite) || defined(_Include_CallByRef) || defined(_Include_Everything)
Boolean TCPNetToIPString(uInt32 netAddy, Boolean bDotNotation, VoidHand *vhIPString) {	
	// initialize output
	if (vhIPString) {
		*vhIPString = NULL;
	}
		
	if(vhIPString) {
		char* cp = MemPtrNew(kMaxDomainNameLen);

   	if(!cp) {
  		SetError(kPDATCPSupportModNum, kMemFail);
  		return false;
  	}

		netAddy = FIX_INT(netAddy);

		if (bDotNotation) {
      RTX_inet_ntoa((const U8 *)&netAddy, cp);
		} 
    else if(0 == netAddy) {
			RTX_inet_get_hostname(cp, kMaxDomainNameLen);
		}
		else {
#if 0 // TCPNet does not have reverse DNS lookup yet
			struct hostent* pHostent = 
			gethostbyaddr((char*)&netAddy, sizeof(uInt32), AF_INET);
			if (pHostent)
				strIP = (char*)pHostent->h_name;
#else
      //cp[0] = 0;
      // ... but we can return the local host name
      RTX_inet_get_hostname(cp, kMaxDomainNameLen);      
#endif
		}

		*vhIPString = PDAStrNewFromStr(cp);
		MemPtrFree(cp);
	}

	if (vhIPString && !*vhIPString) {
		*vhIPString = PDAStrNew(NULL);
		if (*vhIPString == NULL) {
			return false;
		}
	}
	return true;
}
#endif

#if defined(_Include_TCPIPStringToNet) || defined(_Include_Everything)
Boolean TCPIPStringToNet(VoidHand vhIPString,  uInt32 *netAddy){
	char* cp = NULL;
		

	if (pIPStr && pIPStr->len) {
		cp = MemPtrNew(pIPStr->len + 1);
		if(!cp) {
			SetError(kPDATCPSupportModNum, kMemFail);
			return false;
		}
		MemMove(cp, pIPStr->str, pIPStr->len);
		cp[pIPStr->len] = 0;

		RTX_get_host_by_name (cp, (U8 *)netAddy);
	}
	else {
		// if empty string, return the board IP address
		RTX_inet_get_ipaddr((U8*)netAddy);
	}

	// Byte swap the address (necessary because LV treats it as a U32, RTX as a char[])
	*netAddy = FIX_INT(*netAddy); 

	if (cp)
		MemPtrFree(cp);

	if (vhIPString) {	
		PDAStrFree(vhIPString);
	}
	return true;
}
#endif

#if defined(_Include_UDPOpenConnection) || defined(_Include_Everything)
Boolean UDPOpenConnection( uInt16 port,   //input REQUIRED
               String serviceNameIn,
						   Cluster vherrin,           //input
						   NetConnRefNum* connIdOut,  //output
						   uInt16* pPortOut,		  //output
						   Cluster vherrout ) {       //output
	SOCKET wsock = INVALID_SOCKET;
	Boolean rVal;
	NCRefNum* pNCRef = NULL;
  U8 udpOptions = 0;
  BOOL res = __FALSE;
	if (serviceNameIn)
		PDAStrFree(serviceNameIn);
		
	// copy error in to out
	rVal = CopyErrorInToOut(vherrin, vherrout);
		
	// init output
	if (pPortOut)
		*pPortOut = 0;

	// allocate output
	if (connIdOut) {
		*connIdOut = NetConnRefNumNewEmpty(UDPRefNum);
		if (!(*connIdOut)) {
			SetError(kPDATCPSupportModNum, kMemFail);
			return false;
		}
	}

	if (!rVal )  // if a previous error, don't do anything else
	{
    /* Here is where we do the work. In RTX, a 'Listener' is a server socket */
    #if RLARM_UDP_CHECKSUMS
      udpOptions = UDP_OPT_SEND_CS |UDP_OPT_CHK_CS;
    #endif

    wsock = RTX_udp_open_socket (0,              /* TOS bits  */
                                udpOptions,     /* Options   */
                                1522);          /* Buf size  (enough for one packet) */      

    /* Start listening on UDP port 'port' */
    if (wsock)
      res = RTX_udp_listen (wsock, port);
     
		if(res && connIdOut){
			pNCRef = *connIdOut;
			if(!pNCRef){
				SetError(kPDATCPSupportModNum, kBadLock );
				return false;
			}
			pNCRef->refcnt = 1; 	
			pNCRef->type = UDPRefNum;
			pNCRef->rnum = wsock;
			if (pPortOut)
				*pPortOut = getUDPLocalPort(wsock);
		}

		if (!res) {
			int32 errCode;
			if (!wsock) {
				errCode = WSAEOUTOFMEM;
			}
		    else {
		        RTX_udp_close_socket(wsock);
		        wsock  = INVALID_SOCKET;
		        errCode = WSAETIMEDOUT;				
			}
			if (vherrout) SendError(vherrout, WSAETIMEDOUT, _LVT(""));
		}    
	}
	else
	{
		// there is a previous error, deallocate all inputs
		return true;
	}
	return true;
}
#endif


#if defined(_Include_UDPConnRead) || defined(_Include_Everything)

eRunStatus UDPConnRead(VoidHand connIdIn, 
					   int32* pBytesToRead, 
					   DataType dtBytesToRead, 
					   int32* pTimeout, 
					   DataType dtTimeout, 
					   Cluster vherrin, 
					   uInt16* pPortOut,  
					   uInt32* pAddrOut, 
					   NetConnRefNum* connIdOut, 
					   VoidHand* dataOut, 
					   Cluster vherrout,
					   Boolean bRunToFinish,
					   Boolean* bFirstRun,
					   int32* pRefN,                    // conn id
					   uInt32* pStartMs,                // start ms
					   uInt32* pMaxBytes,               // max bytes to read
					   int32* pSavedTimeout             // saved timeout value
					  ) {
	int err = 0;
	Boolean rVal = FALSE;
	int32 nMaxBytes = 548;
	int32 timeout = 25000;
	SOCKET wsock = INVALID_SOCKET;
	NCRefNumPtr pNCRef = NULL;
  U16 pktSize;
  VoidHand vhOut;
		
	if (*bFirstRun)
	{
		*bFirstRun = FALSE;
			
		// deallocate all inputs and allocate all outputs
		pNCRef = (NCRefNum*)connIdIn;
		if (!pNCRef) {
			SetError(kPDANetConnSupportModNum, kBadLock );
			return eFail;	
		}
		wsock = pNCRef->rnum;
		pNCRef = NULL;

		if (pBytesToRead) nMaxBytes = LVPtrToLong(pBytesToRead, dtBytesToRead);			
		if (pTimeout) timeout = LVPtrToLong(pTimeout, dtTimeout);

		// copy error in to out
		rVal = CopyErrorInToOut(vherrin, vherrout);
			
		if (pPortOut) *pPortOut = 0;
		if (pAddrOut) *pAddrOut = 0;

		if (connIdOut) {
			*connIdOut = NetConnRefNumNew(connIdIn);
			if (!(*connIdOut))
				return eFail;
		}

		NetConnRefNumFree(connIdIn);

		if (dataOut) {
			*dataOut = PDAStrNew(NULL);
			if (!(*dataOut))
				return eFail;
		}

		// if there is a previous error or zero byte to read,
		// don't do anything else
		if (rVal || !nMaxBytes)
			return eFinished;

		*pRefN = wsock;
		*pMaxBytes = nMaxBytes;

		if (pSavedTimeout) *pSavedTimeout = timeout;

		*pStartMs = LVGetTicks();
	}
	else // not first run
	{
		wsock = *pRefN;
		nMaxBytes = *pMaxBytes;

		if (pSavedTimeout) timeout = *pSavedTimeout;
	}
		
	// loop until reading is done (reading is done the moment a packet is received)
	while(1) {
		// RTX_udp_rx_bytes will return 0 if there is no packet to read
    pktSize = RTX_udp_rx_bytes (wsock);
    if (pktSize > 0) {
      /* We have got a packet available in the RLARM ciruclar buffer
         If the user requested nMaxBytes is less that the packet size, we can
         only return nMaxBytes, otherwise we can return the entire packet
         Also, if the output is not wired (dataOut == NULL), we still need to clear the packet
       */
      if (dataOut) {
        // Work out the packet size to read
        pktSize = (pktSize < nMaxBytes) ? pktSize : nMaxBytes;
        
        /* Allocate the LabVIEW string output buffer, and read the packet to it - we have to go to the
           low level string functions to do this... */ 
        PDAStrFree(*dataOut);
        *dataOut = PDAStrNew(NULL);                                   //  new output string
        if (!PDAStrResize(dataOut, pktSize + sizeof(PDAStr))) {
          return eFail;                                               // Allocate/size the output string (Check allocation succeeded)
        }
        vhOut = *dataOut;              			
        // Read the packet into the string buffer
        if (!RTX_udp_recv (wsock, (U8 *)((PDAStrPtr)vhOut)->str, &pktSize, (U8 *)pAddrOut, (U16 *)pPortOut)) {
          err = WSAENOTSOCK;
        }
      }
      else {
        // Discard the packet
        pktSize = 0;
        if (!RTX_udp_recv (wsock, (U8 *)((PDAStrPtr)vhOut)->str, &pktSize, (U8 *)pAddrOut, (U16 *)pPortOut)) {
          err = WSAENOTSOCK;
        }
      }
      goto udpReadOut;
    }
			
		if ((timeout >= 0) && ((LVGetTicks() - *pStartMs) > (uInt32)timeout)) {
			err = WSAETIMEDOUT;
			goto udpReadOut;
		}
			
		if (bRunToFinish)
			WaitNextMultiple(55, NULL);
		else
			return eNotFinished;
	}

udpReadOut:
	if (err && vherrout)
		SendError(vherrout, err, _LVT(""));

  if (!err) {
    // Byte swap the address (necessary because LV treats it as a U32, RTX as a char[])
    *pAddrOut = FIX_INT(*pAddrOut);               
  }

	return eFinished;
}
#endif

#if (TCPUDPSupport && defined(_Include_CallByRef)) || defined(_Include_UDPConnWrite) || defined(_Include_Everything)
eRunStatus UDPConnWrite(NetConnRefNum connIdIn, 
						VoidHand dataIn, 
						void* pPort,
						DataType dtPort,
						void* pAddress,
						DataType dtAddress, 
						Cluster vherrin, 
						NetConnRefNum* connIdOut,  
						Cluster vherrout,
						Boolean bRunToFinish,
						Boolean* bFirstRun,
						int32* pRefN,
						uInt32* pStartMs) {
	int err = 0;
	Boolean rVal = FALSE;
	int32 timeout = 15000;
	uInt32 address = 0;
	uInt16 port = 0;
	SOCKET wsock = INVALID_SOCKET;
	NCRefNumPtr pNCRef = NULL;
		

	if (*bFirstRun)
	{
		*bFirstRun = FALSE;

		pNCRef = (NCRefNum*)connIdIn;
		if (!pNCRef)
		{
			SetError(kPDANetConnSupportModNum, kBadLock );
			return eFail;	
		}
		wsock = pNCRef->rnum;
		pNCRef = NULL;

		if (pAddress) address = PtrToULong(pAddress, dtAddress);

		if (pPort) {
			if (IsString(dtPort)) { 
				String sAddress;
				TCPNetToIPString(address,TRUE, &sAddress );
				err = NIServiceLocatorGetServicePort(sAddress, *((String*)pPort), timeout, &port);
				PDAStrFree(*((String*)pPort));
				PDAStrFree(sAddress);
			}
			else {
				port = (uInt16)PtrToULong(pPort, dtPort);
			}
		}	

		// allocate connection id out
		if (connIdOut) {
			*connIdOut = NetConnRefNumNew(connIdIn);
			if (!(*connIdOut))
				return eFail;
		}

		NetConnRefNumFree(connIdIn);

		// copy error in to out
		rVal = CopyErrorInToOut(vherrin, vherrout);

		if (rVal || !((PDAStrPtr)dataIn) || !((PDAStrPtr)dataIn)->len) {
			// there is a previous error or no data in, 
			// don't do anything else
			if (dataIn) {
				PDAStrFree(dataIn);
			}

			return eFinished;
		}

		*pRefN = wsock;
		*pStartMs = LVGetTicks();
	}

  /* In other targets, this is a loop, but in RTX we know the send
     will only take one call to RTX_udp_send() */
	{
    U16 nBytes;
    char* pTemp;
    BOOL sendRes;

		wsock = *pRefN;

    pTemp = (char*)((PDAStrPtr)dataIn)->str;
    nBytes = ((PDAStrPtr)dataIn)->len * sizeof(TextChar);

    // Byte swap the address (necessary because LV treats it as a U32, RTX as a char[])
    address = FIX_INT(address);
    sendRes = RTX_udp_send (wsock, (U8 *)&address, port, pTemp, &nBytes);
  
		if (sendRes == __FALSE) err = WSAETIMEDOUT;
	}

udpWriteOut:
	if (dataIn) {
		PDAStrFree(dataIn);
		dataIn = NULL;
	}

	if (err && vherrout)
		SendError(vherrout, err, _LVT(""));

	return eFinished;
}
#endif

#endif // TCPUDPSupport
#endif /* {RFD} Unimplemented for static */

