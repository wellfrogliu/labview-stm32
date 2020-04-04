/***************************************************************************************
  NI CONFIDENTIAL

  CCGNetConnSupport.c - description

  © 2003-2006 National Instruments Corporation.  
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Embedded Development Module) and may not be copied, modified, or redistributed.  The code may 
     only be used in accordance with the terms of the license agreement entered into between 
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.

  Author: Richard Sewell (Keil - An ARM Company)
********************************************************************************************/


#define pStr ((PDAStrPtr)dataIn)

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include "CCGCommsSupport.h"
#include "CCGDataSupport.h"
#ifndef CStatic /* {RFD} Unimplemented for static */

#include "CCGWinsock.h"
#include "CCGConfig.h"

#if SocketSupport

#include "RLARM_TCPWrapper.h"

#if  defined(_Include_NetConnWaitOnListener) || defined(_Include_Everything)

/** NetConnWaitOnListener() - 
  Waits for an accepted network connection based on ncType. 
*/
eRunStatus NetConnWaitOnListener(NetConnRefNum listIdIn,          // input REQUIRED
                                 NCRefNumKind ncType,             // net conn refnum type
                                 void* pTimeout,                  // input
                                 DataType dtTimeout,              // default: int32
                                 Boolean bResolveRemoteAddress,
                                 Cluster vherrin,                 // input
                                 String* remoteLsap,              // output
                                 void* remotePort,                // output
                                 NetConnRefNum* listIdOut,        // output
                                 NetConnRefNum* connIdOut,        // output
                                 Cluster vherrout,                // output
                                 Boolean bRunToFinish,
                                 Boolean* bFirstRun,
                                 int32* pRefN,                    // refnum
                                 uInt32* pStartMs,                // start time
                                 int32* pSavedTimeout             // saved timeout value
                                 )
{
  int err = 0;
  Boolean rVal = FALSE;
  int32 timeout = -1;
  SOCKET wsock = INVALID_SOCKET;
  SOCKADDR_IN remoteTcpAddr;
  NCRefNumPtr pNCRef = NULL;
  String* remoteAddr = remoteLsap;
  U16 lRemotePort;
  U32 lRemoteIp;

  // Early return - we only support TCP here at present
  if (ncType != TCPRefNum) return eFinished;
    
  // if run for the first time, set up wait on listener
  if (*bFirstRun) {
    *bFirstRun = FALSE;

    // get inputs and allocate all outputs
    pNCRef = (NCRefNumPtr)listIdIn;
    if (!pNCRef) {
      SetError(kPDANetConnSupportModNum, kBadLock );
      return eFail; 
    }
    wsock = pNCRef->rnum;
    pNCRef = NULL;

    if (pTimeout) {
      timeout = LVPtrToLong(pTimeout, dtTimeout);
    }

    if (remotePort) {
      *((uInt16*)remotePort) = 0;
    }

    if (remoteAddr) {
      *remoteAddr = PDAStrNew(NULL);
      if (!(*remoteAddr))
        return eFail;
    }
      
    if (connIdOut) {
      *connIdOut = NetConnRefNumNew(listIdIn);
      if (!(*connIdOut))
        return eFail;
      pNCRef = (NCRefNum*)(*connIdOut);
      if (!pNCRef)
      {
        SetError(kPDANetConnSupportModNum, kBadLock );
        return eFail; 
      }
      pNCRef->rnum = INVALID_SOCKET;
      pNCRef = NULL;
    }

    if (listIdOut) {
      *listIdOut = NetConnRefNumNew(listIdIn);
      if (!(*listIdOut))
        return eFail; 
    }

    NetConnRefNumFree(listIdIn);

    // copy error in to out
    rVal = CopyErrorInToOut(vherrin, vherrout);
      
    // if a previous error, don't do anything else
    if (rVal)
      return eFinished;

    *pRefN = wsock;

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


  // loop until we are connected to a client
  while(1) {
    if (RTX_tcp_get_state(wsock) != TCP_STATE_CONNECT) {
      if ((timeout >= 0) && ((LVGetTicks() - *pStartMs) > (uInt32)timeout)) {
        err = WSAETIMEDOUT;
        break;
      }
        
      if (bRunToFinish == 0x01) {
        WaitNextMultiple(55, NULL);
      }
      else {
        return eNotFinished;      
      }
    }
    else {
      // We are connected! - call the winsock create to allocate LV buffering
      break;
    }
  }

  // Set error return if an error occurred
  if (err) {
    if (vherrout)
      SendError(vherrout, err, _LVT(""));
  }
  else {
    // copy refnum to connection id out
    if (connIdOut) {
      pNCRef = (NCRefNum*)(*connIdOut);
      if (!pNCRef)
      {
        SetError(kPDANetConnSupportModNum, kBadLock );
        return eFail; 
      }
      pNCRef->rnum = wsock;
      pNCRef = NULL;
    }

    // get the remote address and port
    RTX_inet_get_addr (wsock, NULL, NULL, (U8 *)&lRemoteIp, &lRemotePort);

    // set output port
    if (remotePort) *((uInt16*)remotePort) = lRemotePort;

    // convert the remote address to a string as set it
    if (remoteAddr) {
      char sBuf[18];
            
      #if 0   // RTX stack does not support reverse DNS
			//if (bResolveRemoteAddress) {        
				//struct hostent* pHostent = gethostbyaddr((char*)&(remoteTcpAddr.sin_addr), sizeof(uInt32), AF_INET);
				//if (pHostent)
				//	pRemoteAddr = pHostent->h_name;
			//}
			//else {			
      //	pRemoteAddr = inet_ntoa(remoteTcpAddr.sin_addr);
      //}
      #else        
        RTX_inet_ntoa((const U8*)&lRemoteIp, sBuf);
      #endif // #if 0 (no RDNS)
      
      // Byte swap the address (necessary because LV treats it as a U32, RTX as a char[])
      lRemoteIp = FIX_INT(lRemoteIp);
			
			PDAStrFree(*remoteAddr);
			*remoteAddr = NULL;
			*remoteAddr = PDAStrNewFromStr((TextPtr)(sBuf));
			if (!(*remoteAddr))
				return eFail;
    }
  }
    
  return eFinished;
}

#endif // #if  defined(_Include_NetConnWaitOnListener) || defined(_Include_Everything)

#if (TCPUDPSupport && defined(_Include_CallByRef)) || defined(_Include_NetConnCloseConnection) || defined(_Include_Everything)

/** NetConnCloseConnection() - 
  Close a network connection based on ncType.
*/
Boolean NetConnCloseConnection(NetConnRefNum connIdIn,  // input - REQUIRED
                               NCRefNumKind ncType,
                               Cluster vherrin, 
                               Cluster vherrout,
                               NetConnRefNum* connIdOut)
{
  Boolean rVal = FALSE;
  SOCKET wsock = INVALID_SOCKET;
  NCRefNumPtr pNCRefN;

  // Early return - we only support TCP/UDP here at present
  if ((ncType != TCPRefNum) && (ncType != UDPRefNum)) return eFinished;

  pNCRefN = (NCRefNum*)connIdIn;

  if (!pNCRefN)
  {
    SetError(kPDANetConnSupportModNum, kBadLock );
    return false; 
  }
    
  // copy error in to out
  rVal = CopyErrorInToOut(vherrin, vherrout);

  wsock = pNCRefN->rnum;
    
  // allocate connection id out
  if (connIdOut) {
    *connIdOut = NetConnRefNumNew(connIdIn);
    if (!(*connIdOut))
      return false;
  }
    
  NetConnRefNumFree(connIdIn);
  pNCRefN = NULL;

  switch (ncType) {
    case UDPRefNum:
      RTX_udp_close_socket(wsock);
      break;

    case TCPRefNum:
      RTX_tcp_close_socket(wsock);
      break;

    default:
      ; // Should never happen
  } 

  return true;
}

#endif // #if (TCPUDPSupport && defined(_Include_CallByRef)) || defined(_Include_NetConnCloseConnection) || defined(_Include_Everything)

#if (TCPUDPSupport && defined(_Include_CallByRef)) || defined(_Include_NetConnRead) || defined(_Include_Everything)

/** read modes */
typedef enum
{
  kStdMode, ///< standard mode
  kBuffMode,  ///< buffered mode
  kCRLFMode,  ///< CRLF mode
  kImmMode    ///< immediate mode
} ReadMode;


/** NetConnRead() - 
  Read number of bytes from a network connection based on ncType.
*/
extern eRunStatus NetConnRead(NetConnRefNum connIdIn,
                              NCRefNumKind ncType,
                              void* pReadMode,          /* uInt32 */
                              DataType dtReadMode,
                              void* pBytesToRead,       /* uInt32 */
                              DataType dtBytesToRead,
                              void* pTimeout,           /* int32  */
                              DataType dtTimeout, 
                              Cluster vherrin,
                              NetConnRefNum* connIdOut, 
                              String* dataOut, 
                              Cluster vherrout,
                              Boolean bRunToFinish,
                              Boolean* bFirstRun,
                              int32* pRefN,
                              uInt32* pStartMs,
                              uInt32* pMode,
                              uInt32* pBytes,
                              int32* pSavedTimeout
                              )
{
  int err = 0;
  Boolean rVal = FALSE;
  uInt32 readMode = 0;
  U16 nBytesToRead = 0;
  int32 timeout = 25000;
  SOCKET wsock = INVALID_SOCKET;
  U16 pktSize;
  uInt32 nCRLFendIndex = 0;
  NCRefNumPtr pNCRef = NULL;
  PDAStrPtr vhOut;

  // Early return - we only support TCP here at present
  if (ncType != TCPRefNum) return eFinished;

  if (*bFirstRun)
  {
    *bFirstRun = FALSE;
      
    // deallocate all inputs and allocate all outputs
    pNCRef = (NCRefNum*)connIdIn;
    if (!pNCRef)
    {
      SetError(kPDANetConnSupportModNum, kBadLock );
      return eFail; 
    }
    wsock = pNCRef->rnum;
    pNCRef = NULL;

    if (pReadMode) {
      readMode = PtrToULong(pReadMode, dtReadMode);
    }
      
    if (pBytesToRead) {
      int32 nLToRead = LVPtrToLong(pBytesToRead, dtBytesToRead);
      if (nLToRead > 0) {
        nBytesToRead = (uInt32)nLToRead;
      }
    }
      
    if (pTimeout) {
      timeout = LVPtrToLong(pTimeout, dtTimeout);
    }

    // copy error in to out
    rVal = CopyErrorInToOut(vherrin, vherrout);
      
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
    if (rVal || !nBytesToRead)
      return eFinished;

    *pRefN = wsock;
    *pMode = readMode;
    *pBytes = nBytesToRead;

    if (pSavedTimeout)
      *pSavedTimeout = timeout;

    *pStartMs = LVGetTicks();
  }
  else // not first run
  {
    wsock = *pRefN;
    readMode = *pMode;
    nBytesToRead = *pBytes;

    if (pSavedTimeout)
      timeout = *pSavedTimeout;
  }
    
  // loop until reading is done
  while(1)
  {
    // Peek the data stream buffer to see if we can get the required data
    // RTX_udp_rx_bytes will return 0 if there is no packet to read
    pktSize = RTX_tcp_rx_bytes (wsock);
    if (pktSize > 0) {
      switch (readMode) {
        case kImmMode:
          if (pktSize) {
            goto readOut;
          }
          break;
  
        case kStdMode:
        case kBuffMode:
        default:
          if (pktSize >= nBytesToRead) {
            goto readOut;
          }
          break;
  
        case kCRLFMode:
          nCRLFendIndex = RTX_tcp_rx_crlf (wsock, nBytesToRead);
          if (nCRLFendIndex) {
            goto readOut;
          }
  
          // if number of bytes requested has been reached
          // and no CRLF pair is found, return timeout error
          if (pktSize >= nBytesToRead && nCRLFendIndex == 0) {
            err = WSAETIMEDOUT;
            goto readOut;
          }
          break;
      } // switch (readMode)
    } // if (pktSize > 0)
      
    if ((timeout >= 0) && ((LVGetTicks() - *pStartMs) > (uInt32)timeout)) {
      err = WSAETIMEDOUT;
      goto readOut;
    }
      
    if (bRunToFinish)
      WaitNextMultiple(55, NULL);
    else
      return eNotFinished;
  }

readOut:
  if (err && vherrout)
    SendError(vherrout, err, _LVT("SocketRead error"));

  if (dataOut && (pktSize > 0)) {
    // copy the data from the circular buffer
    switch (readMode) {
      case kImmMode:
      case kStdMode:
      default:
        nBytesToRead = (nBytesToRead <= pktSize) ? 
                 nBytesToRead : pktSize;
        break;
      case kBuffMode:
        nBytesToRead = (nBytesToRead <= pktSize) ?
                 nBytesToRead : 0;
        break;
      case kCRLFMode:
        nBytesToRead = nCRLFendIndex ? nCRLFendIndex + 1 : 0;
        break;
    }

    if (nBytesToRead) {
      /* Allocate the LabVIEW string output buffer, and read the packet to it - we have to go to the
           low level string functions to do this... */ 
      PDAStrFree(*dataOut);      
      *dataOut = PDAStrNew(NULL);                                   //  new output string
      if (!PDAStrResize(dataOut, nBytesToRead + sizeof(PDAStr))) {
        return eFail;                                                 // Allocate/size the output string (Check allocation succeeded)
      }

      // Read the packet into the string buffer
      vhOut = *dataOut;              			
      if (!RTX_tcp_recv (wsock, (U8 *)((PDAStrPtr)vhOut)->str, &nBytesToRead)) {
        err = WSAENOTSOCK;
      }
    }

    if (!(*dataOut))
      return eFail;
  }

  return eFinished;
}
#endif // #if (TCPUDPSupport && defined(_Include_CallByRef)) || defined(_Include_NetConnRead) || defined(_Include_Everything)

#if (TCPUDPSupport && defined(_Include_CallByRef)) || defined(_Include_NetConnWrite) || defined(_Include_Everything)

/** NetConnWrite() - 
  Writes data to a network connection based on ncType.
*/
extern eRunStatus NetConnWrite(NetConnRefNum connIdIn,
                               NCRefNumKind ncType,
                               String dataIn, 
                               void* pTimeout,            /* int32 */
                               DataType dtTimeout,
                               Cluster vherrin,
                               NetConnRefNum* connIdOut,
                               uInt32* bytesWritten, 
                               Cluster vherrout, 
                               Boolean bRunToFinish, 
                               Boolean* bFirstRun,
                               int32* pRefN,              // conn id
                               uInt32* pStartMs,          // start ms
                               int32* pSavedTimeout       // saved timeout value
                               )
{
  int err = 0;
  Boolean rVal = FALSE;
  int32 timeout = 25000;
  SOCKET wsock = INVALID_SOCKET;
  NCRefNumPtr pNCRef = NULL;

  // Early return - we only support TCP here at present
  if (ncType != TCPRefNum) return eFinished;

    
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

    if (pTimeout) {
      timeout = LVPtrToLong(pTimeout, dtTimeout);
    }

    // allocate connection id out
    if (connIdOut) {
      *connIdOut = NetConnRefNumNew(connIdIn);
      if (!(*connIdOut))
        return eFail;
    }

    NetConnRefNumFree(connIdIn);

    if (bytesWritten)
      *bytesWritten = 0;

    // copy error in to out
    rVal = CopyErrorInToOut(vherrin, vherrout);

    if (rVal || !pStr || !pStr->len) {
      // there is a previous error or no data in, 
      // don't do anything else
      if (dataIn) {
        PDAStrFree(dataIn);
      }

      return eFinished;
    }

    *pRefN = wsock;

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

  /* In other targets, this is a loop, but in RTX we know the send
     will only take one call to RTX_tcp_send() */
  {   
    U16 nBytes;
    char* pTemp;
    BOOL sendRes;

    pTemp = (char*)pStr->str;
    nBytes = pStr->len * sizeof(TextChar);

    sendRes = RTX_tcp_send (wsock, pTemp, &nBytes, timeout);

    if (bytesWritten) *bytesWritten = nBytes;      
		if (sendRes == __FALSE) err = WSAETIMEDOUT;
  }

writeOut:
  if (dataIn) {
    PDAStrFree(dataIn);
    dataIn = NULL;
  }

  if (err && vherrout)
    SendError(vherrout, err, _LVT(""));

  return eFinished;
}

#endif // #if (TCPUDPSupport && defined(_Include_CallByRef)) || defined(_Include_NetConnWrite) || defined(_Include_Everything)
#endif // #if SocketSupport
#endif /* {RFD} Unimplemented for static */

#if  defined(_Include_AllFlatten) || defined(_Include_Everything)
Boolean PDANCRefNumFlatten(VoidHand *str, int32 *pos, VoidHand data, DataType dt, int32 *offset,
               Boolean header, uInt16 byteOrder, Cluster vpErrIn, Cluster vpErrOut, Boolean bCvt70)
{
  NCRefNumPtr nPtr;
  if (!str) {
    if (CopyErrorInToOut( vpErrIn, vpErrOut )) {
      return true;
    }
  }
  else if (vpErrOut)  {
    // second pass  - if error on first pass do not continue, error out already set
    Boolean error;
    PDAFUNC(PDAClusterGetElemByPos)( vpErrOut, ErrorClusterDataType, 0L, (void *)&error, BooleanDataType FUNCSTATICARGS1(NULL));
    if (error) {
      return true;
    }
  }

  if(!(nPtr = (NCRefNumPtr)data))
  {
    SetError(kPDANetConnSupportModNum, kBadLock );
    return false;
  }


  if (byteOrder != 0) {
    if (vpErrOut)
      SendError(vpErrOut, mgArgErr, "Prepend Array = FALSE not supported in PDA Flatten");
    return true;
  }

  if(!PDAIntFlatten(str,pos,&(nPtr->rnum),LongDataType,offset, header, byteOrder, NULL, NULL, bCvt70)) //check if (nPtr->rnum) is to be used instead of &(nPtr->rnum)
    return false;
  return true;
}
#endif

#if defined(_Include_AllUnflatten) || defined(_Include_Everything)
Boolean PDANCRefNumUnflatten(VoidHand str,VoidHand *data, int32 *offset, DataType dt, VoidHand* dataLeft,
               Boolean header, uInt16 byteOrder, Cluster vpErrIn, Cluster vpErrOut)
{
  NCRefNumPtr nPtr;
  

  if (CopyErrorInToOut( vpErrIn, vpErrOut )) {
    return true;
  }

  if (byteOrder != 0) {
    if (vpErrOut)
      SendError(vpErrOut, mgArgErr, "Prepend Array = FALSE not supported in PDA UnFlatten");
    return true;
  }

  if (data) {
    *data = NetConnRefNumNewEmpty(GenericRefNum);
    if (! *data) {
      SetError(kPDANetConnSupportModNum, kMemFail );
      return false;
    }
    if(!(nPtr = (NCRefNumPtr)(*data)))
    {
      SetError(kPDANetConnSupportModNum, kBadLock );
      return false;
    }
    nPtr->refcnt = 1;
    if (!PDAIntUnflatten(str,&(nPtr->rnum),offset,LongDataType, dataLeft, header, byteOrder, NULL, NULL))
      return false;
  }

  return true;
} 
#endif

