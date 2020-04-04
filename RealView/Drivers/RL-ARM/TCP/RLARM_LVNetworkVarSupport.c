/***************************************************************************************
	NI CONFIDENTIAL

	LVNetworkVarSupport.c - description

	© 2003-2008 National Instruments Corporation.
     This code is for internal use only and may not be used outside of the LabVIEW embedded product
     (LabVIEW Microprocessor SDK) and may not be copied, modified, or redistributed.  The code may
     only be used in accordance with the terms of the license agreement entered into between
     National Instruments Corporation and the authorized user.  Your use of the code designates your
     acceptance of the terms of the license agreement.
	
	Author: Andrew Dove
	              Bill Pittman
********************************************************************************************/

#include "LVSysIncludes.h"
#include "LVCCG.h"

#if DEBUG_LOG > 0
extern U16 os_time;
#endif

#if defined(_Include_VariableSupport) || defined(_Include_VariableSingleProcessSupport) || defined(_Include_Everything)

#include "LVNetworkVarSupport.h"
#include "CCGDataSupport.h"
#include "LVTdp.h" // for waveform datatype flavors

#if defined(_Include_VariableSupport) || defined(_Include_Everything)
#ifndef CStatic // Unimplemented for static
#if SocketSupport

#include "CCGWinsock.h"
#include "CCGTcpUdpSupport.h"

#if BigEndian
#define kNetVarManagerServiceVersion 0x08602019 // 8.6d19
#else
#define kNetVarManagerServiceVersion 0x19206008 // 8.6d19
#endif // BigEndian

#define kErrVariableHasNoValue 0x8BBB0006 // LabVIEW:  (Hex 0x8BBB0006) The shared variable has no value.

#define kNetVarRead				0
#define	kNetVarWrite			1
#define	kNetVarWriteAsLvVariant	2
#define	kNetVarReadBuffered		3

#define IsNetVarReadMode(x) (x == kNetVarRead || x == kNetVarReadBuffered)

static TextPtr gStrHostIPAddress = NULL;
static SOCKET gConnSock = INVALID_SOCKET;

// this function should only be called from lvEmbeddedMain
void NetworkVariableSetHostIP(TextPtr pVarMgrIPAddressStr) {
	gStrHostIPAddress = pVarMgrIPAddressStr;
}

Err NetworkVariableStart() {
	Err err =  kNoErr;
	uInt16 nPort = 0;
	BOOL bConnected;
	uInt32 retryCounter;
	uInt32 nStartMs;
	const uInt32 ulPortTimeout = 1000; // up to 5x this value worst case (retries 4 times max)
	const uInt32 ulTimeout = 2000;
	SocketStatus wsockStat;
	PDAStrPtr pStrHostIPAddress = NULL;
    uInt32 ipAddr;
	
	if (gConnSock != INVALID_SOCKET) {
		goto netVarStartOut;
	}

	if (gStrHostIPAddress == NULL) {
		err = kInvalidInput;
		goto netVarStartOut;
	}
	else {
		// construct Variable Manager service name
		const TextPtr pServiceName = _LVT("National Instruments/LabVIEW/MPU/11.0/VariableService");
		PDAStrPtr pStrServiceName = MemPtrNew(sizeof(PDAStr) + StrLen(pServiceName));
		if (!pStrServiceName) {
			err = kMemErr;
			goto netVarStartOut;
		}
		pStrServiceName->refcnt = 1;
		pStrServiceName->staticStr = 0;
		pStrServiceName->len = StrLen(pServiceName);
		StrCopy(pStrServiceName->str, pServiceName);
		// construct Variable Manager IP address string
		pStrHostIPAddress = MemPtrNew(sizeof(PDAStr) + StrLen(gStrHostIPAddress));
		if (!pStrHostIPAddress) {
			MemPtrFree(pStrServiceName);
			err = kMemErr;
			goto netVarStartOut;
		}
		pStrHostIPAddress->refcnt = 1;
		pStrHostIPAddress->staticStr = 0;
		pStrHostIPAddress->len = StrLen(gStrHostIPAddress);
		StrCopy(pStrHostIPAddress->str, gStrHostIPAddress);

		err = NIServiceLocatorGetServicePort(pStrHostIPAddress, pStrServiceName, ulPortTimeout, &nPort);
		for(retryCounter=0; retryCounter<4 && err; retryCounter++) // retry up to 4 times
			err = NIServiceLocatorGetServicePort(pStrHostIPAddress, pStrServiceName, ulPortTimeout, &nPort);

		MemPtrFree(pStrServiceName);
	}

	if (err) {
		goto netVarStartOut;
	}
	nStartMs = LVGetTicks();

	// connect to NI Service Locator
    gConnSock = RTX_tcp_open_socket (TCP_TYPE_CLIENT, /* Type      */
                                0,                    /* TOS bits  */
                                ulTimeout,            /* Timeout   */
                                1522);                /* Buf size  */ 
								
    RTX_get_host_by_name (pStrHostIPAddress->str, (U8*)&ipAddr);
    bConnected = RTX_tcp_connect (gConnSock, (U8 *)&ipAddr, nPort, 0);
	
	if(!gConnSock)
        err = WSAEOUTOFMEM;

	// so loop until connection is made
	while(bConnected){
		// Wait then see if we are connected		
		if (RTX_tcp_get_state(gConnSock) != TCP_STATE_CONNECT) {  			
			if ((ulTimeout >= 0) && ((LVGetTicks() - nStartMs) > (uInt32)ulTimeout)) {
				err = WSAETIMEDOUT;
				goto netVarStartOut;
			}
			WaitNextMultiple(55, NULL);
		}
		else {
			// We are connected!
			break;
		}
	}
		
netVarStartOut:
	if (pStrHostIPAddress) {
		MemPtrFree(pStrHostIPAddress);
	}
	if (err && gConnSock != INVALID_SOCKET) {
		RTX_tcp_close_socket(gConnSock);
		gConnSock = INVALID_SOCKET;
	}

	return err;
}

void NetworkVariableCleanup() {
	if (gConnSock != INVALID_SOCKET) {
		RTX_tcp_close_socket(gConnSock);
		gConnSock = INVALID_SOCKET;
	}
	gStrHostIPAddress = NULL;
}

static void NetworkVariableReset() {
	if (gConnSock != INVALID_SOCKET) {
		RTX_tcp_close_socket(gConnSock);
		gConnSock = INVALID_SOCKET;
	}
}

// caller has to free returned string
static TextPtr NetworkVariableConstructConnectCommandString(NetworkVariableAttrs attrs, int8 nMode, Boolean bTimestamp, int32* pLenOut) {
	TextPtr pCommandStr;
	uInt32 ulIndex = sizeof(uInt32);
	int32 lVersion = kNetVarManagerServiceVersion;
	uInt8 nByte = StrLen(attrs.pUrl);
	uInt32 nSize = sizeof (uInt32)	// variable id
				 + sizeof(int32)	// version
				 + sizeof(int8)		// mode
				 + sizeof(uInt8)	// url len
				 + 4 + nByte;		// url string - "psp:" + url

	if (IsNetVarReadMode(nMode)) {
		nSize += sizeof(int8);		// timestamp inclusion
		nSize += sizeof(int8);		// buffering option
		nSize += sizeof(int32);		// buffer size limit
		nSize += sizeof(int32);		// buffer value limit
	}

	pCommandStr = MemPtrNew(nSize);
	if (!pCommandStr) {
		return NULL;
	}

	MemSet(pCommandStr, sizeof(uInt32), 0);
	MemMove(pCommandStr + ulIndex, &lVersion, sizeof(int32));
	ulIndex += sizeof(int32);
	pCommandStr[ulIndex] = nMode;
	ulIndex++;
	pCommandStr[ulIndex] = nByte + 4;
	ulIndex++;
	StrNCopy(pCommandStr + ulIndex, _LVT("psp:"), 4);
	ulIndex += 4;
	MemMove(pCommandStr + ulIndex, attrs.pUrl, nByte);

	if (IsNetVarReadMode(nMode)) {
		ulIndex += nByte;
		pCommandStr[ulIndex] = bTimestamp ? 1 : 0;
		ulIndex++;
		pCommandStr[ulIndex] = attrs.bBuffer ? 1 : 0;
		ulIndex++;
		MemMove(pCommandStr + ulIndex, &attrs.nBufferSizeLimit, sizeof(int32));
		Standardize(pCommandStr + ulIndex, int32DataType);
		ulIndex += sizeof(int32);
		MemMove(pCommandStr + ulIndex, &attrs.nBufferValueLimit, sizeof(int32));
		Standardize(pCommandStr + ulIndex, int32DataType);
	}

	*pLenOut = nSize;
	return pCommandStr;
}

static Err NetworkVariableConnect(NetworkVariableAttrs attrs, int8 nMode, Boolean bTimestamp,
								  Boolean* p_bErrStatusOut, int32* p_nErrCode, uInt32* pVarIdOut)	{
	Err err = kNoErr;
	int32 nLen;
	U16 len;
	const uInt32 ulTimeout = 10000;
	TextPtr pCommandString = NetworkVariableConstructConnectCommandString(attrs, nMode, bTimestamp, &nLen);

	DEBUG_LOG3(("NetworkVariableConnect begin - %d\n", os_time));

	if (!pCommandString) {
		return kMemErr;
	}
	len = nLen;
	RTX_tcp_send(gConnSock, pCommandString, &len, ulTimeout);
	MemPtrFree(pCommandString);

	// read error status
	len = sizeof(int8);
	RTX_tcp_recv_timeout(gConnSock, (char*)p_bErrStatusOut, &len, ulTimeout, &err);
	if (err) {
		return err;
	}

	// read error code
	len = sizeof(int32);
	RTX_tcp_recv_timeout(gConnSock, (char*)p_nErrCode, &len, ulTimeout, &err);
	if (err) {
		return err;
	}
	UnStandardize(p_nErrCode, int32DataType);

	if (!(*p_bErrStatusOut)) {
		// if there is no error, read variable id
		len = sizeof(uInt32);
		RTX_tcp_recv_timeout(gConnSock, (char*)pVarIdOut, &len, ulTimeout, &err);
	}

	DEBUG_LOG3(("NetworkVariableConnect end - %d\n", os_time));
	return err;
}

Boolean NetworkVariableRead(uInt32* pVarId,
							NetworkVariableAttrs attrs,
							Cluster errIn,
							void* pDataOut,
							DataType dtDataOut,
							Timestamp128* pTimestampOut,
							Cluster errOut)
{
	Boolean bPrevError =
#if defined(_Include_Cluster_Utils) || defined(_Include_Everything)
		CopyErrorInToOut(errIn, errOut);
#else
		false;
#endif // _Include_Cluster_Utils
	Err err = 0;
	Boolean bErrStatus = false;
	int32 nErrCode = 0;
	Boolean bTimestamp = (pTimestampOut != NULL) ? true : false;
	int8 nMode = kNetVarRead;

	DEBUG_LOG3(("NetworkVariableRead begin - %d\n", os_time));

	// init outputs
	InitOutput(pDataOut, dtDataOut);
	InitOutput(pTimestampOut, Timestamp128DataType);

	if (bPrevError)
		goto netVarReadOut;

	err = NetworkVariableStart();
	if (err) {
		goto netVarReadOut;
	}


	if (*pVarId == 0) {
		// no variable id, need to connect to variable manager
		err = NetworkVariableConnect(attrs, nMode, bTimestamp, &bErrStatus, &nErrCode, pVarId);
		if (err || bErrStatus) {
			goto netVarReadOut;
		}
	}

	// at this point, we have a valid variable id
	{
		// send a read request
		int32 nLen;
		U16 len;
		PDAStrPtr pData;
		const uInt32 ulTimeout = 10000;
		char buf[sizeof(uInt32) + sizeof(int8)];

		// send variable id
		memcpy(buf, pVarId, sizeof(uInt32));
		
		// send mode
		memcpy(buf+sizeof(uInt32), &nMode, sizeof(int8));
		
		len = sizeof(buf);
		if(RTX_tcp_send(gConnSock, buf, &len, ulTimeout) != __TRUE)
			err = WSAEOUTOFMEM;
		if (err) {
			goto netVarReadOut;
		}

		// read error status
		len = sizeof(int8);
		RTX_tcp_recv_timeout(gConnSock, (char*)&bErrStatus, &len, ulTimeout, &err);
		if (err) {
			goto netVarReadOut;
		}

		// read error code
		len = sizeof(int32);
		RTX_tcp_recv_timeout(gConnSock, (char*)&nErrCode, &len, ulTimeout, &err);
		if (err) {
			goto netVarReadOut;
		}
		UnStandardize(&nErrCode, int32DataType);

		// read data size
		len = sizeof(int32);
		RTX_tcp_recv_timeout(gConnSock, (char*)&nLen, &len, ulTimeout, &err);
		if (err) {
			goto netVarReadOut;
		}
		UnStandardize(&nLen, int32DataType);
		if (nLen <= 0) {
			goto netVarReadOut;
		}

		// read data
		pData = MemPtrNew(sizeof(PDAStr) + nLen);
		pData->refcnt = 1;
		pData->staticStr = 0;
		pData->len = 0;
		if (!pData) {
			err = kMemErr;
			goto netVarReadOut;
		}
		len = nLen;
		RTX_tcp_recv_timeout(gConnSock, pData->str, &len, ulTimeout, &err);

		if (!err && bTimestamp) {
			// read timestamp
			len = sizeof(Timestamp128);
			RTX_tcp_recv_timeout(gConnSock, (char*)pTimestampOut, &len, ulTimeout, &err);
		}

		if (!err) {
			// unflatten data
			if (pTimestampOut) {
				UnStandardize(pTimestampOut, Timestamp128DataType);
			}
			pData->len = nLen;
			nLen = 0;
			if (!UnFlattenData(pData, &nLen, NULL, dtDataOut, pDataOut, false)) {
				return false;
			}
		}
		MemPtrFree(pData);
	}

netVarReadOut:
	if (!bPrevError) {
		if (err) {
			*pVarId = 0; // there is a socket error, so reset connection and start over
			NetworkVariableReset();
			bErrStatus = true;
		}
		else if (nErrCode) {
			err = nErrCode;
			if (err == kInvalidInput) {
				*pVarId = 0; // error 1 returned by the connection manager means we have a stale refnum
			}
		}

#if defined(_Include_Cluster_Utils) || defined(_Include_Everything)
		if (err) {
			SendErrorExt(errOut, bErrStatus, err, _LVT("Shared Variable Read"));
		}
#endif // _Include_Cluster_Utils
	}

	AllocateOutputIfNeeded(pDataOut, dtDataOut);
	DEBUG_LOG3(("NetworkVariableRead end - %d\n", os_time));
	return true;
}

static Err NetworkVariableSendWriteCommand(uInt32 ulVarId, int8 nWriteMode, TextPtr pUrl, String strFlatData, int8* pErrStatusOut, int32* pErrCodeOut) {
	Err err = kNoErr;
	U16 len;
	const uInt32 ulTimeout = 10000;
	char buf[sizeof(uInt32) + sizeof(int8) + sizeof(int32)];
	int32 szData = ((PDAStrPtr)strFlatData)->len;
	
	// send variable id
	memcpy(buf, &ulVarId, sizeof(uInt32));

	// send mode
	memcpy(buf+sizeof(uInt32), &nWriteMode, sizeof(int8));

	// send data size
	Standardize(&szData, int32DataType);
	memcpy(buf+sizeof(uInt32)+sizeof(int8), &szData, sizeof(int32));
	
	len = sizeof(buf);
	if(RTX_tcp_send(gConnSock, buf, &len, ulTimeout) != __TRUE)
		err = WSAEOUTOFMEM;
	if (err) {
		return err;
	}

	// send data
	len = ((PDAStrPtr)strFlatData)->len;
	if(RTX_tcp_send(gConnSock, ((PDAStrPtr)strFlatData)->str, &len, ulTimeout) != __TRUE)
		err = WSAEOUTOFMEM;
	if (err) {
		return err;
	}
	
	// read error status
	len = sizeof(int8);
	RTX_tcp_recv_timeout(gConnSock, (char*)pErrStatusOut, &len, ulTimeout, &err);
	if (err) {
		return err;
	}

	// read error code
	len = sizeof(int32);
	RTX_tcp_recv_timeout(gConnSock, (char*)pErrCodeOut, &len, ulTimeout, &err);
	if (err) {
		return err;
	}
	UnStandardize(pErrCodeOut, int32DataType);
	return err;
}

#define NETVARWRITEASNATIVE 0

#if NETVARWRITEASNATIVE
// caller has to free pStrFlatDataOut
static Boolean NetworkVariableFlattenData(void* pDataIn, DataType dtDataIn, DataType dtVariable,
												String* pStrFlatDataOut, Err* pErrOut) {
	switch(GetType(dtVariable))
	{
		case BooleanDataType:
		case CharDataType:
		case UCharDataType:
		case Enum8DataType: {
			int8 nValue = (int8)LVPtrToLong(pDataIn, dtDataIn);
			if (!FlattenData(&nValue, dtVariable, pStrFlatDataOut, false))
				return false;
			}
			break;

		case UIntDataType:
		case IntDataType:
		case Enum16DataType: {
			int16 nValue = (int16)LVPtrToLong(pDataIn, dtDataIn);
			if (!FlattenData(&nValue, dtVariable, pStrFlatDataOut, false))
				return false;
			}
			break;

		case LongDataType:
		case ULongDataType:
		case Enum32DataType: {
			int32 nValue = (int32)LVPtrToLong(pDataIn, dtDataIn);
			if (!FlattenData(&nValue, dtVariable, pStrFlatDataOut, false))
				return false;
			}
			break;

#ifndef _Integer_Only
		case floatDataType: {
			float32 fValue = (float32)PtrToDouble(pDataIn, dtDataIn);
			if (!FlattenData(&fValue, dtVariable, pStrFlatDataOut, false))
				return false;
			}
			break;

		case doubleDataType: {
			float64 dblValue = PtrToDouble(pDataIn, dtDataIn);
			if (!FlattenData(&dblValue, dtVariable, pStrFlatDataOut, false))
				return false;
			}
			break;

#if defined(_Include_Complex) || defined(_Include_Everything)
		case cmplx64DataType: {
			cmplx64 cmplx64Value = PtrToCmplx64(pDataIn, dtDataIn);
			if (!FlattenData(&cmplx64Value, dtVariable, pStrFlatDataOut, false))
				return false;
			}
			break;

		case cmplx128DataType: {
			cmplx128 cmplx128Value = PtrToCmplx128(pDataIn, dtDataIn);
			if (!FlattenData(&cmplx128Value, dtVariable, pStrFlatDataOut, false))
				return false;
			}
			break;
#endif // _Include_Complex
#endif // _Integer_Only

		case ArrayDataType:
		case ClusterDataType:
		case BigClusterDataType:
			if (dtDataIn != dtVariable) {
				*pErrOut = kInvalidInput;
				break;
			}

		case Timestamp128DataType:
		case StringDataType: {
			if (!FlattenData(pDataIn, dtVariable, pStrFlatDataOut, false))
				return false;
			}
			break;

#if defined(_Include_Variant) || defined(_Include_Everything)
		case VariantDataType:
			if (IsVariant(dtDataIn)) {
				if (!FlattenData(pDataIn, dtVariable, pStrFlatDataOut, false))
					return false;
			}
			else {
				Variant lvVariant;
				if (PDAVariantNewFromData(pDataIn, dtDataIn, &lvVariant, dtVariable) == NULL)
					return false;
				if (!FlattenData(&lvVariant, dtVariable, pStrFlatDataOut, false))
					return false;
				PDAVariantFree(lvVariant);
			}
			break;
#endif // _Include_Variant

		default:
			*pErrOut = kInvalidInput;
			break;
	}
	return true;
}
#endif // NETVARWRITEASNATIVE

static Boolean NetworkVariableFlattenDataAsLvVariant(
								void* pDataIn, DataType dtDataIn, DataType dtVariable,
								String* pStrFlatDataOut, Err* pErrOut) {
	Variant lvVariant = NULL;
	switch(GetType(dtVariable))
	{
		case BooleanDataType:
		case CharDataType:
		case UCharDataType:
		case Enum8DataType: {
			int8 nValue = (int8)LVPtrToLong(pDataIn, dtDataIn);
			if (PDAVariantNewFromData(&nValue, dtVariable, &lvVariant, VariantDataType) == NULL)
				return false;
			}
			break;

		case UIntDataType:
		case IntDataType:
		case Enum16DataType: {
			int16 nValue = (int16)LVPtrToLong(pDataIn, dtDataIn);
			if (PDAVariantNewFromData(&nValue, dtVariable, &lvVariant, VariantDataType) == NULL)
				return false;
			}
			break;

		case LongDataType:
		case ULongDataType:
		case Enum32DataType: {
			int32 nValue = (int32)LVPtrToLong(pDataIn, dtDataIn);
			if (PDAVariantNewFromData(&nValue, dtVariable, &lvVariant, VariantDataType) == NULL)
				return false;
			}
			break;

#ifndef _Integer_Only
		case floatDataType: {
			float32 fValue = (float32)PtrToDouble(pDataIn, dtDataIn);
			if (PDAVariantNewFromData(&fValue, dtVariable, &lvVariant, VariantDataType) == NULL)
				return false;
			}
			break;

		case doubleDataType: {
			float64 dblValue = PtrToDouble(pDataIn, dtDataIn);
			if (PDAVariantNewFromData(&dblValue, dtVariable, &lvVariant, VariantDataType) == NULL)
				return false;
			}
			break;

#if defined(_Include_Complex) || defined(_Include_Everything)
		case cmplx64DataType: {
			cmplx64 cmplx64Value = PtrToCmplx64(pDataIn, dtDataIn);
			if (PDAVariantNewFromData(&cmplx64Value, dtVariable, &lvVariant, VariantDataType) == NULL)
				return false;
			}
			break;

		case cmplx128DataType: {
			cmplx128 cmplx128Value = PtrToCmplx128(pDataIn, dtDataIn);
			if (PDAVariantNewFromData(&cmplx128Value, dtVariable, &lvVariant, VariantDataType) == NULL)
				return false;
			}
			break;
#endif // _Include_Complex
#endif // _Integer_Only

		case ArrayDataType:
		case ClusterDataType:
		case BigClusterDataType:
			if (dtDataIn != dtVariable) {
				*pErrOut = kInvalidInput;
				break;
			}

		case Timestamp128DataType:
		case StringDataType: {
			if (PDAVariantNewFromData(pDataIn, dtVariable, &lvVariant, VariantDataType) == NULL)
				return false;
			}
			break;

#if defined(_Include_Variant) || defined(_Include_Everything)
		case VariantDataType:
			if (IsVariant(dtDataIn)) {
				if (!FlattenData(pDataIn, dtVariable, pStrFlatDataOut, false))
					return false;
			}
			else {
				if (PDAVariantNewFromData(pDataIn, dtDataIn, &lvVariant, dtVariable) == NULL)
					return false;
			}
			break;
#endif // _Include_Variant

		default:
			*pErrOut = kInvalidInput;
			break;
	}

	if (lvVariant) {
		if (!FlattenData(&lvVariant, VariantDataType, pStrFlatDataOut, false))
			return false;
		PDAVariantFree(lvVariant);
	}
	return true;
}

Boolean NetworkVariableWrite(uInt32* pVarId,
							 NetworkVariableAttrs attrs,
							 Cluster errIn,
							 void* pDataIn,
							 DataType dtDataIn,
							 DataType dtVariable,
							 Cluster errOut)
{
	Boolean bPrevError =
#if defined(_Include_Cluster_Utils) || defined(_Include_Everything)
		CopyErrorInToOut(errIn, errOut);
#else
		false;
#endif // _Include_Cluster_Utils

	Err err = 0;
	int32 nErrCode = 0;
	Boolean bErrStatus = false;
	String strFlatData = NULL;

	DEBUG_LOG3(("NetworkVariableWrite begin - %d\n", os_time));
	
	if (bPrevError)
		goto netVarWriteOut;

	err = NetworkVariableStart();
	if (err) {
		goto netVarWriteOut;
	}

	if (*pVarId == 0) {
		// no variable id, need to connect to variable manager
		err = NetworkVariableConnect(attrs, kNetVarWrite, false, &bErrStatus, &nErrCode, pVarId);
		if (err || bErrStatus) {
			goto netVarWriteOut;
		}
	}

	// at this point, we have a valid variable id

#if NETVARWRITEASNATIVE
	if (!NetworkVariableFlattenData(pDataIn, dtDataIn, dtVariable, &strFlatData, &err)) {
		return false;
	}

	if (!err) {
		err = NetworkVariableSendWriteCommand(*pVarId, kNetVarWrite, attrs.pUrl, strFlatData, &bErrStatus, &nErrCode);
		MemPtrFree(strFlatData); // PDAStrFree(strFlatData);
		if (!err && (nErrCode == kErrVariableHasNoValue))
			// variable has no value, retry sending data as lvvariant
#endif // NETVARWRITEASNATIVE
		{
			if (!NetworkVariableFlattenDataAsLvVariant(pDataIn, dtDataIn, dtVariable, &strFlatData, &err)) {
				return false;
			}
			err = NetworkVariableSendWriteCommand(*pVarId, kNetVarWriteAsLvVariant, attrs.pUrl, strFlatData, &bErrStatus, &nErrCode);
			MemPtrFree(strFlatData); // PDAStrFree(strFlatData);
		}
#if NETVARWRITEASNATIVE
	}
#endif // NETVARWRITEASNATIVE

netVarWriteOut:
	if (!bPrevError) {
		if (err) {
			*pVarId = 0; // there is a socket error, so reset connection and start over
			NetworkVariableReset();
			bErrStatus = true;
		}
		else if (nErrCode) {
			err = nErrCode;
			if (err == kInvalidInput) {
				*pVarId = 0; // error 1 returned by the connection manager means we have a stale refnum
			}
		}

#if defined(_Include_Cluster_Utils) || defined(_Include_Everything)
		if (err) {
			SendErrorExt(errOut, bErrStatus, err, _LVT("Shared Variable Write"));
		}
#endif // _Include_Cluster_Utils
	}

	FreeInputHandle(pDataIn, dtDataIn);
	DEBUG_LOG3(("NetworkVariableWrite end - %d\n", os_time));
	return true;
}

#endif // SocketSupport
#endif // CStatic
#endif // defined(_Include_VariableSupport) || defined(_Include_Everything)


#if defined(_Include_VariableSingleProcessSupport) || defined(_Include_Everything)

#ifndef CStatic // Unimplemented for static

#include "ObjList.h"

typedef struct SINGLEPROCESSVAROBJ {
	TextPtr pUrl;
	DataType dt;
	void* pData;
	Timestamp128 ts128;
} SingleProcessVarObj, *SingleProcessVarObjPtr;
 
typedef struct SINGLEPROCESSVARLIST {
	ListObj* pList;
} SingleProcessVarList, *SingleProcessVarListPtr;

SingleProcessVarList gSingleProcessVariablesList = { NULL };

void SingleProcessVariableCleanup() {
	ListObj* pObj = gSingleProcessVariablesList.pList;
	while (pObj) {
		ListObj* pTemp = pObj;
		SingleProcessVarObjPtr pAttr = pObj->pAttribute;
		if (pAttr) {
			if (pAttr->pUrl) {
				MemPtrFree(pAttr->pUrl);
			}
			if (pAttr->pData) {
				FreeInputHandle(pAttr->pData, pAttr->dt);
				MemPtrFree(pAttr->pData);
			}
			MemPtrFree(pAttr);
		}
		pObj = pObj->pNext;
		MemPtrFree(pTemp);
	}
}

static Err SingleProcessVariableCreate(TextPtr pUrl, DataType dtVariable, uInt32* pIdOut) {
	ListObj* pObj;
	SingleProcessVarObjPtr pAttr = MemPtrNew(sizeof(SingleProcessVarObj));
	if (!pAttr) {
		return kMemErr;
	}
	pAttr->dt = dtVariable;
	// copy url
	pAttr->pUrl = MemPtrNew(StrLen(pUrl)+1);
	if (!pAttr->pUrl) {
		MemPtrFree(pAttr);
		return kMemErr;
	}
	StrCopy(pAttr->pUrl, pUrl);
	// initialize data pointer to NULL
	pAttr->pData = NULL;
	// insert into the list
	pObj = NewListObj((uInt32)pAttr, pAttr);
	if (!pObj) {
		MemPtrFree(pAttr->pUrl);
		MemPtrFree(pAttr);
		return kMemErr;
	}
	AddObjAtEnd(&gSingleProcessVariablesList.pList, pObj);
	*pIdOut = (uInt32)pAttr;
	return kNoErr;
}

// returns the variable id if single process variable is found
// returns 0 if single process variable is not found in the list
static uInt32 SingleProcessVariableFind(TextPtr pUrl) {
	ListObj* pObj = gSingleProcessVariablesList.pList;
	while (pObj) {
		SingleProcessVarObjPtr pAttr = pObj->pAttribute;
		if (!StrCompare(pAttr->pUrl, pUrl)) {
			return (uInt32)pAttr;
		}
		pObj = pObj->pNext;
	}
	return 0;
}

static Err SingleProcessVariableWriteData(uInt32 id, void* pDataIn, DataType dtDataIn) {
	SingleProcessVarObjPtr pAttr;

	if (!id)
		return kInvalidInput;

	pAttr = (SingleProcessVarObjPtr)id;
	if (!pAttr->pData) {
		pAttr->pData = MemPtrNew(DataSize(pAttr->dt));
		if (!pAttr->pData)
			return kMemErr;
		InitOutput(pAttr->pData, pAttr->dt);
		if (dtDataIn != pAttr->dt) {
			AllocateOutputIfNeeded(pAttr->pData, pAttr->dt);
		}
	}
	pAttr->ts128 = GetDtSecTs();
	CopyValue(pDataIn, dtDataIn, pAttr->pData, pAttr->dt);

	return kNoErr;
}

static Err SingleProcessVariableReadData(uInt32 id, void* pDataOut, DataType dtDataOut, Timestamp128* pTimestampOut) {
	SingleProcessVarObjPtr pAttr;

	if (!id)
		return kInvalidInput;

	pAttr = (SingleProcessVarObjPtr)id;
	CopyValue(pAttr->pData, pAttr->dt, pDataOut, dtDataOut);

	if (pTimestampOut) {
		*pTimestampOut = pAttr->ts128;
	}

	return kNoErr;
}

Boolean SingleProcessVariableRead(uInt32* pVarId,
							TextPtr pUrl,
							Cluster errIn,
							void* pDataOut,
							DataType dtDataOut,
							Timestamp128* pTimestampOut,
							Cluster errOut) {
	Boolean bPrevError =
#if defined(_Include_Cluster_Utils) || defined(_Include_Everything)
		CopyErrorInToOut(errIn, errOut);
#else
		false;
#endif // _Include_Cluster_Utils

	Err err = 0;
	Boolean bTimestamp = (pTimestampOut != NULL) ? true : false;

	// init outputs
	InitOutput(pDataOut, dtDataOut);
	InitOutput(pTimestampOut, Timestamp128DataType);

	if (bPrevError)
		goto singleProcReadOut;

	if (!*pVarId) {
		// find single process var if id is zero
		*pVarId = SingleProcessVariableFind(pUrl);
	}

	if (*pVarId) {
		// read the value if the var exists
		err = SingleProcessVariableReadData(*pVarId, pDataOut, dtDataOut, pTimestampOut);
	}

singleProcReadOut:

#if defined(_Include_Cluster_Utils) || defined(_Include_Everything)
	if (!bPrevError && err) {
		SendError(errOut, err, _LVT("Shared Variable Read"));
	}
#endif // _Include_Cluster_Utils

	AllocateOutputIfNeeded(pDataOut, dtDataOut);

	return true;
}

Boolean SingleProcessVariableWrite(uInt32* pVarId,
							 TextPtr pUrl,
							 Cluster errIn,
							 void* pDataIn,
							 DataType dtDataIn,
							 DataType dtVariable,
							 Cluster errOut) {
	Boolean bPrevError =
#if defined(_Include_Cluster_Utils) || defined(_Include_Everything)
		CopyErrorInToOut(errIn, errOut);
#else
		false;
#endif // _Include_Cluster_Utils

	Err err = 0;

	if (bPrevError)
		goto singleProcWriteOut;

	if (!*pVarId) {
		// find single process var if id is zero
		*pVarId = SingleProcessVariableFind(pUrl);
		if (!*pVarId) {
			// create single process var if not found
			err = SingleProcessVariableCreate(pUrl, dtVariable, pVarId);
			if (err) {
				goto singleProcWriteOut;
			}
		}
	}

	// at this point id is not zero, write the data
	err = SingleProcessVariableWriteData(*pVarId, pDataIn, dtDataIn);

singleProcWriteOut:

#if defined(_Include_Cluster_Utils) || defined(_Include_Everything)
	if (!bPrevError && err) {
		SendError(errOut, err, _LVT("Shared Variable Write"));
	}
#endif // _Include_Cluster_Utils

	FreeInputHandle(pDataIn, dtDataIn);
	return true;
}

#endif // CStatic

#endif // defined(_Include_VariableSingleProcessSupport) || defined(_Include_Everything)

#endif // defined(_Include_VariableSupport) || defined(_Include_VariableSingleProcessSupport) || defined(_Include_Everything)

#if 0 // Unit Test for Whitebox Testing
Boolean LVNetworkVarUnitTest() {
	Boolean bResult = true;
	Err err;
	uInt32 id, nData;
	BYTE* pBuf, * pTmp;
	HKEY hKey = NULL;
	NetworkVariableAttrs attrs = { 0 };
	String stringData = NULL;
	cmplx64 cmplx64Data;
	cmplx128 cmplx128Data;
	Timestamp128 tsData = { 0 };
	DataType dtArray1DStr = 0x30000 | ArrayDataType;
	DataType dtArray1DNum = 0xC0000 | ArrayDataType;
	DataType dtArray2DNum = 0xA0000 | ArrayDataType;
	Array arrayData = NULL;
	DataType dtErrIn = 0x0 | ClusterDataType;
	Cluster errIn = PDAClusterNewEmptyHandle(dtErrIn);
	Cluster errOut = NULL;
	DataType dtWaveform = 0x50000 | BigClusterDataType;
	Cluster waveformIn =  NULL;
	OSTextPtr fakeDllValue = _LVT("C:\\Documents and Settings\\mhadikus\\Desktop\\MockLogos\\nitaglvpda\\ReleaseErr\\");
	OSTextPtr dllValue = _LVT("C:\\Documents and Settings\\mhadikus\\Desktop\\MockLogos\\nitaglvpda\\Release\\");
	// create registry subkey for DLL location
	bResult = RegCreateKeyEx(HKEY_LOCAL_MACHINE, _LVT("Software\\National Instruments\\LabVIEW\\Shared Variables\\8.5.0"), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS;
	// test DLL not found
	if (bResult) bResult = (err = NetworkVariableStart());
	// create registry value for fake DLL location
	if (bResult) bResult = RegSetValueEx(hKey, _LVT("Dll"), 0, REG_SZ, fakeDllValue, StrLen(fakeDllValue)) == ERROR_SUCCESS;
	// test loading fake DLL
	if (bResult) bResult = (err = NetworkVariableStart());
	// test unloading fake DLL
	NetworkVariableCleanup();
	// test connect with no DLL
	if (bResult) NetworkVariableConnect(NULL, NULL, false, false, 0, 0, &err, false, NULL, NULL, NULL);
	if (bResult) bResult = err;
	// test finding alias - error cases
	if (bResult) bResult = FindResolveMachineAlias(_LVT("My Computer"), _LVT("My Computer\\myvar\\myvar1")) == NULL;
	g_pMachineAliasBuf = MemPtrNew(256);
	strcpy(g_pMachineAliasBuf, "My Computer");
	if (bResult) bResult = FindResolveMachineAlias(_LVT("My Computer"), _LVT("My Computer\\myvar\\myvar1")) == NULL;
	strcpy(g_pMachineAliasBuf, "My Computer\"");
	if (bResult) bResult = FindResolveMachineAlias(_LVT("My Computer"), _LVT("My Computer\\myvar\\myvar1")) == NULL;
	if (bResult) bResult = FindResolveMachineAlias(_LVT("My Computer 1"), _LVT("My Computer\\myvar\\myvar1")) == NULL;
	MemPtrFree(g_pMachineAliasBuf);
	g_pMachineAliasBuf = NULL;
	// create registry value for DLL location
	if (bResult) bResult = RegSetValueEx(hKey, _LVT("Dll"), 0, REG_SZ, dllValue, StrLen(dllValue)) == ERROR_SUCCESS;
	// test loading DLL
	if (bResult) bResult = !(err = NetworkVariableStart());
	// test reloading DLL
	if (bResult) bResult = !(err = NetworkVariableStart());
	// test loading aliases file
	if (bResult) LoadMachineAliases();
	// test finding alias
	if (bResult) bResult = FindResolveMachineAlias(_LVT(""), _LVT("")) == NULL;
	if (bResult) 
	{ 
		TextPtr pNewUrl = FindResolveMachineAlias(_LVT("My Computer"), _LVT("My Computer\\myvar\\myvar1"));
		if (pNewUrl) {
			MemPtrFree(pNewUrl);
			bResult = true;
		}
	}
	if (bResult) bResult = NetworkVariableConstructAliasUrl(_LVT(""), _LVT("")) == NULL;
	if (bResult)
	{
		TextPtr pNewUrl = NetworkVariableConstructAliasUrl(_LVT("My Computer"), _LVT("My Computer\\myvar\\myvar1"));
		if (pNewUrl) {
			MemPtrFree(pNewUrl);
			bResult = true;
		}
	}
	// test connect
	if (bResult) NetworkVariableConnect(_LVT("My Computer\\myvar\\myvar1"), _LVT("My Computer"), true, false, 0, 0, &err, false, NULL, NULL, NULL);
	if (bResult) bResult = !err;
	if (bResult) NetworkVariableConnect(_LVT("My Computer\\myvar\\myvar2"), _LVT("My Computer"), false, true, 1000, 1000, &err, true, NULL, NULL, NULL);
	if (bResult) bResult = !err;
	if (bResult) NetworkVariableConnect(_LVT("My Computer\\myvar\\myvar3"), _LVT("My Computer"), true, false, 0, 0, &err, false, NULL, NULL, NULL);
	if (bResult) bResult = !err;
	if (bResult) NetworkVariableConnect(_LVT("My Computer\\myvar\\myvar4"), _LVT("My Computer"), true, false, 0, 0, &err, false, NULL, NULL, NULL);
	if (bResult) bResult = !err;
	if (bResult) { id = NetworkVariableConnect(_LVT("My Computer\\myvar\\myvar5"), _LVT("My Computer"), true, false, 0, 0, &err, false, NULL, NULL, NULL); }
	if (bResult) bResult = !err;
	// test disconnect
	if (bResult) bResult = !NetworkVariableDisconnect(id);
	// test read
	id = 0;
	if (bResult) bResult = NetworkVariableRead(&id, attrs, NULL, &nData, uInt32DataType, NULL, NULL);
	id = 5;
	if (bResult) bResult = NetworkVariableRead(&id, attrs, NULL, &nData, uInt32DataType, NULL, NULL);
	id = 100;
	if (bResult) bResult = NetworkVariableRead(&id, attrs, NULL, &stringData, StringDataType, NULL, NULL);
	if (stringData) PDAStrFree(stringData);
	stringData = NULL;
	id = 101;
	if (bResult) bResult = NetworkVariableRead(&id, attrs, NULL, &tsData, Timestamp128DataType, NULL, NULL);
	nData = 1;
	PDAClusterSetElemByPos(errIn, BooleanDataType, 0, &nData, uInt32DataType);
	PDAClusterSetElemByPos(errIn, int32DataType, 1, &nData, uInt32DataType);
	if (bResult) bResult = NetworkVariableRead(&id, attrs, errIn, &tsData, Timestamp128DataType, NULL, NULL);
	if (bResult) bResult = NetworkVariableRead(&id, attrs, NULL, &tsData, -1, NULL, NULL);
	// test write
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, errIn, &tsData, Timestamp128DataType, Timestamp128DataType, NULL);
	id = 5;
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &nData, uInt32DataType, uInt32DataType, NULL);
	id = 0;
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &nData, uInt32DataType, uInt32DataType, NULL);
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &tsData, Timestamp128DataType, Timestamp128DataType, NULL);
	stringData = PDAStrNewFromStr(_LVT("string test"));
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &stringData, StringDataType, StringDataType, NULL);
	stringData = NULL;
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &cmplx64Data, cmplx64DataType, cmplx64DataType, NULL);
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &cmplx128Data, cmplx128DataType, cmplx128DataType, NULL);
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &nData, uInt32DataType, cmplx64DataType, NULL);
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &nData, uInt32DataType, cmplx128DataType, NULL);
	arrayData = PDAArrNew1D(1, StringDataType);
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &arrayData, dtArray1DStr, dtArray1DStr, NULL);
	arrayData = NULL;
	arrayData = PDAArrNew1D(5, int32DataType);
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &arrayData, dtArray1DNum, dtArray1DNum, NULL);
	arrayData = NULL;
	arrayData = PDAArrNew2D(5, 8, int32DataType);
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &arrayData, dtArray2DNum, dtArray2DNum, NULL);
	arrayData = NULL;
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &nData, uInt32DataType, dtArray1DNum, NULL);
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &errIn, dtErrIn, dtErrIn, NULL);
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, &nData, -1, -1, NULL);
	waveformIn =  PDAClusterNewEmptyHandle(dtWaveform);
	if (bResult) bResult = NetworkVariableWrite(&id, attrs, NULL, waveformIn, dtWaveform, dtWaveform, NULL);
	PDAClusterFreeHandle(waveformIn, dtWaveform);
	waveformIn = NULL;
	// test unloading DLL
	NetworkVariableCleanup();
	// delete registry subkey for DLL location
	if (hKey) RegCloseKey(hKey);
	if (hKey)
	{
		RegDeleteKey(HKEY_LOCAL_MACHINE, _LVT("Software\\National Instruments\\LabVIEW\\Shared Variables\\8.5.0"));
		RegDeleteKey(HKEY_LOCAL_MACHINE, _LVT("Software\\National Instruments\\LabVIEW\\Shared Variables"));
	}
	// test MDTFlavorFromDataType
	if (bResult) bResult = (MDTFlavorFromDataType(int16DataType) == mdtUniformInt16Waveform);
	if (bResult) bResult = (MDTFlavorFromDataType(doubleDataType) == mdtUniformFlt64Waveform);
	if (bResult) bResult = (MDTFlavorFromDataType(floatDataType) == mdtUniformFlt32Waveform);
	if (bResult) bResult = (MDTFlavorFromDataType(uCharDataType) == mdtUniformUInt8Waveform);
	if (bResult) bResult = (MDTFlavorFromDataType(uInt16DataType) == mdtUniformUInt16Waveform);
	if (bResult) bResult = (MDTFlavorFromDataType(uInt32DataType) == mdtUniformUInt32Waveform);
	if (bResult) bResult = (MDTFlavorFromDataType(CharDataType) == mdtUniformInt8Waveform);
	if (bResult) bResult = (MDTFlavorFromDataType(int32DataType) == mdtUniformInt32Waveform);
	if (bResult) bResult = (MDTFlavorFromDataType(cmplx64DataType) == mdtUniformCplx64Waveform);
	if (bResult) bResult = (MDTFlavorFromDataType(cmplx128DataType) == mdtUniformCplx128Waveform);
	if (bResult) bResult = (MDTFlavorFromDataType(StringDataType) == mdtNotAFlavor);
	// test FlexDataTypeToDataType
	if (bResult) bResult = FlexDataTypeToDataType(kFD_Bool) == BooleanDataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_I1) == CharDataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_UI1) == UCharDataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_I2) == int16DataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_UI2) == uInt16DataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_I4) == int32DataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_UI4) == uInt32DataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_R4) == floatDataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_R8) == doubleDataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_C4) == cmplx64DataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_C8) == cmplx128DataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_NITime) == Timestamp128DataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_AString) == StringDataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_Array) == ArrayDataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_Cluster) == ClusterDataType;
	if (bResult) bResult = FlexDataTypeToDataType(kFD_FlexData) == VariantDataType;
	if (bResult) bResult = FlexDataTypeToDataType(-1) == 0;
	// test FlexDataTypeFromDataType
	if (bResult) bResult = FlexDataTypeFromDataType(BooleanDataType) == kFD_Bool;
	if (bResult) bResult = FlexDataTypeFromDataType(CharDataType) == kFD_I1;
	if (bResult) bResult = FlexDataTypeFromDataType(UCharDataType) == kFD_UI1;
	if (bResult) bResult = FlexDataTypeFromDataType(int16DataType) == kFD_I2;
	if (bResult) bResult = FlexDataTypeFromDataType(uInt16DataType) == kFD_UI2;
	if (bResult) bResult = FlexDataTypeFromDataType(int32DataType) == kFD_I4;
	if (bResult) bResult = FlexDataTypeFromDataType(uInt32DataType) == kFD_UI4;
	if (bResult) bResult = FlexDataTypeFromDataType(floatDataType) == kFD_R4;
	if (bResult) bResult = FlexDataTypeFromDataType(doubleDataType) == kFD_R8;
	if (bResult) bResult = FlexDataTypeFromDataType(cmplx64DataType) == kFD_C4;
	if (bResult) bResult = FlexDataTypeFromDataType(cmplx128DataType) == kFD_C8;
	if (bResult) bResult = FlexDataTypeFromDataType(Timestamp128DataType) == kFD_NITime;
	if (bResult) bResult = FlexDataTypeFromDataType(StringDataType) == kFD_AString;
	if (bResult) bResult = FlexDataTypeFromDataType(ArrayDataType) == kFD_Array;
	if (bResult) bResult = FlexDataTypeFromDataType(ClusterDataType) == kFD_Cluster;
	if (bResult) bResult = FlexDataTypeFromDataType(VariantDataType) == kFD_FlexData;
	if (bResult) bResult = FlexDataTypeFromDataType(-1) == 0;
	if (bResult) bResult = FlexDataFromPtr(&nData, -1, NULL, NULL);
	if (bResult) bResult = FlexDataTDFromDataType(-1, NULL, NULL);
	// test flat-unflat
	if (bResult) bResult = FlexDataFlat(NULL, -1, NULL, NULL);
	if (bResult) {
		// string
		bResult = false;
		stringData = PDAStrNewFromStr(_LVT("123"));
		FlexDataFlat(stringData, StringDataType, NULL, &nData);
		if (nData) {
			pBuf = MemPtrNew(nData);
			if (!FlexDataFlat(stringData, StringDataType, pBuf, &nData)) {
				String stringOut = NULL;
				pTmp = pBuf;
				FlexDataUnflat(&pTmp, &stringOut, StringDataType);
				if (stringOut)
				{
					bResult = PDAStrLen(stringOut) == 3;
					stringOut = NULL;
				}
			}
			MemPtrFree(pBuf);
		}
		PDAStrFree(stringData);
		stringData = NULL;
	}
	if (bResult) {
		// cluster
		bResult = false;
		FlexDataFlat(errIn, dtErrIn, NULL, &nData);
		if (nData) {
			pBuf = MemPtrNew(nData);
			if (!FlexDataFlat(errIn, dtErrIn, pBuf, &nData)) {
				pTmp = pBuf;
				errOut = PDAClusterNewEmptyHandle(dtErrIn);
				bResult = !FlexDataUnflat(&pTmp, errOut, dtErrIn);
				PDAClusterFreeHandle(errOut, dtErrIn);
				errOut = NULL;
			}
			MemPtrFree(pBuf);
		}
	}
	if (bResult) {
		// cluster
		bResult = false;
		nData = 0;
		FlexDataFromPtr(errIn, dtErrIn, NULL, &nData);
		if (nData) {
			pBuf = MemPtrNew(nData);
			nData = 0;
			if (!FlexDataFromPtr(errIn, dtErrIn, pBuf, &nData)) {
				pTmp = pBuf;
				errOut = PDAClusterNewEmptyHandle(dtErrIn);
				bResult = !FlexDataToPtr(pBuf, nData, errOut, dtErrIn);
				PDAClusterFreeHandle(errOut, dtErrIn);
				errOut = NULL;
			}
			MemPtrFree(pBuf);
		}
	}
	if (bResult) {
		// waveform / big cluster
		bResult = false;
		waveformIn =  PDAClusterNewEmptyHandle(dtWaveform);
		FlexDataFlat(waveformIn, dtWaveform, NULL, &nData);
		if (nData) {
			pBuf = MemPtrNew(nData);
			if (!FlexDataFlat(waveformIn, dtWaveform, pBuf, &nData)) {
				Cluster waveformOut = NULL;
				pTmp = pBuf;
				bResult = !FlexDataUnflat(&pTmp, &waveformOut, dtWaveform);
				if (waveformOut) {
					PDAClusterFree(&waveformOut, dtWaveform);
					waveformOut = NULL;
				}
			}
			MemPtrFree(pBuf);
		}
		PDAClusterFreeHandle(waveformIn, dtWaveform);
		waveformIn = NULL;
	}
	if (bResult) {
		// 1D array of strings
		bResult = false;
		arrayData = PDAArrNew1D(3, StringDataType);
		FlexDataFlat(arrayData, dtArray1DStr, NULL, &nData);
		if (nData) {
			pBuf = MemPtrNew(nData);
			if (!FlexDataFlat(arrayData, dtArray1DStr, pBuf, &nData)) {
				Array arrayOut = NULL;
				pTmp = pBuf;
				FlexDataUnflat(&pTmp, &arrayOut, dtArray1DStr);
				if (arrayOut)
				{
					bResult = PDAArrNElems(arrayOut) == 3;
					PDAArrFree(arrayOut);
					arrayOut = NULL;
				}
			}
			MemPtrFree(pBuf);
		}
		PDAArrFree(arrayData);
		arrayData = NULL;
	}
	if (bResult) {
		// 1D array of numerics
		bResult = false;
		arrayData = PDAArrNew1D(5, int32DataType);
		FlexDataFlat(arrayData, dtArray1DNum, NULL, &nData);
		if (nData) {
			pBuf = MemPtrNew(nData);
			if (!FlexDataFlat(arrayData, dtArray1DNum, pBuf, &nData)) {
				Array arrayOut = NULL;
				pTmp = pBuf;
				FlexDataUnflat(&pTmp, &arrayOut, dtArray1DNum);
				if (arrayOut)
				{
					bResult = PDAArrNElems(arrayOut) == 5;
					PDAArrFree(arrayOut);
					arrayOut = NULL;
				}
			}
			MemPtrFree(pBuf);
		}
		PDAArrFree(arrayData);
		arrayData = NULL;
	}
	if (bResult) {
		// 2D array of numerics
		bResult = false;
		arrayData = PDAArrNew2D(5, 8, int32DataType);
		FlexDataFlat(arrayData, dtArray2DNum, NULL, &nData);
		if (nData) {
			pBuf = MemPtrNew(nData);
			if (!FlexDataFlat(arrayData, dtArray2DNum, pBuf, &nData)) {
				Array arrayOut = NULL;
				pTmp = pBuf;
				FlexDataUnflat(&pTmp, &arrayOut, dtArray2DNum);
				if (arrayOut)
				{
					bResult = PDAArrNElems(arrayOut) == 40;
					PDAArrFree(arrayOut);
					arrayOut = NULL;
				}
			}
			MemPtrFree(pBuf);
		}
		PDAArrFree(arrayData);
		arrayData = NULL;
	}
	if (bResult) {
		// 2D array of numerics
		bResult = false;
		arrayData = PDAArrNew2D(10, 10, int32DataType);
		nData = 0;
		FlexDataFromPtr(arrayData, dtArray2DNum, NULL, &nData);
		if (nData) {
			pBuf = MemPtrNew(nData);
			nData = 0;
			if (!FlexDataFromPtr(arrayData, dtArray2DNum, pBuf, &nData)) {
				Array arrayOut = NULL;
				pTmp = pBuf;
				FlexDataToPtr(pBuf, nData, &arrayOut, dtArray2DNum);
				if (arrayOut)
				{
					bResult = PDAArrNElems(arrayOut) == 100;
					PDAArrFree(arrayOut);
					arrayOut = NULL;
				}
			}
			MemPtrFree(pBuf);
		}
		PDAArrFree(arrayData);
		arrayData = NULL;
	}
	if (bResult) {
		// complex
		bResult = false;
		nData = 0;
		FlexDataFromPtr(&cmplx128Data, cmplx128DataType, NULL, &nData);
		if (nData) {
			pBuf = MemPtrNew(nData);
			nData = 0;
			if (!FlexDataFromPtr(&cmplx128Data, cmplx128DataType, pBuf, &nData)) {
				pTmp = pBuf;
				bResult = !FlexDataToPtr(pBuf, nData, &cmplx128Data, cmplx128DataType);
			}
			MemPtrFree(pBuf);
		}
	}
	if (bResult) {
		// Time Stamp
		bResult = false;
		nData = 0;
		tsData.secHi = 0;
		tsData.secLow = 10;
		tsData.fracHi = 20;
		tsData.fracLow = 30;
		FlexDataFromPtr(&tsData, Timestamp128DataType, NULL, &nData);
		if (nData) {
			pBuf = MemPtrNew(nData);
			nData = 0;
			if (!FlexDataFromPtr(&tsData, Timestamp128DataType, pBuf, &nData)) {
				pTmp = pBuf;
				tsData.secHi = 0;
				tsData.secLow = 0;
				tsData.fracHi = 0;
				tsData.fracLow = 0;
				bResult = (!FlexDataToPtr(pBuf, nData, &tsData, Timestamp128DataType)
							&& tsData.secHi == 0 &&	tsData.secLow == 10
							&& tsData.fracHi == 20 && tsData.fracLow == 30);
			}
			if (bResult) bResult = !FlexDataCompareTypes(-1, pBuf);
			if (bResult) bResult = !FlexDataCompareTypes(RefNumDataType, pBuf);
			MemPtrFree(pBuf);
		}
	}
	if (bResult) bResult = FlexDataToPtr(NULL, 0, NULL, -1);
	if (bResult) bResult = FlexDataUnflat(NULL, NULL, -1);
	PDAClusterFreeHandle(errIn, dtErrIn);
	errIn = NULL;
	SingleProcessVariableWriteData(0, NULL, 0);
	SingleProcessVariableReadData(0, NULL, 0, NULL);
	return bResult;
}
#endif
