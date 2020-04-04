
#include "LVSysIncludes.h"
#include "LVFuncsUsed.h"
#include "LVCCG.h"
#include "CCGDataSupport.h"
#include "RTX_CAN.h"

#define arrIn (*(PDAArrPtr *)&vhData)
#define ERRCODE_NOT_INIT        1
#define ERRCODE_NULL_POINTER    2

#if defined(_Include_RLARM_CAN_Functions) || defined(_Include_Everything)

void
RLARM_CAN_Open(unsigned int uController, unsigned int *puControllerOut,
               unsigned int uBaudRate, unsigned int *puBaudRateOut,
               long lErrIn, long *plErrOut)
{
    if(!lErrIn)
    {
        lErrIn = CAN_init(uController, uBaudRate);
    }

    if(puControllerOut)
    {
        *puControllerOut = uController;
    }

    if(puBaudRateOut)
    {
        *puBaudRateOut = uBaudRate;
    }

    if(plErrOut)
    {
		if(lErrIn)
        	*plErrOut = lErrIn + 21430;  //offset for LV error code database
		else
			*plErrOut = 0;
    }
}

void
RLARM_CAN_Start(unsigned int uController, unsigned int *puControllerOut,
                long lErrIn, long *plErrOut)
{
    if(!lErrIn)
    {
        lErrIn = CAN_start(uController);
    }

    if(puControllerOut)
    {
        *puControllerOut = uController;
    }

    if(plErrOut)
    {
        if(lErrIn)
        	*plErrOut = lErrIn + 21430;  //offset for LV error code database
		else
			*plErrOut = 0;
    }
}

void
RLARM_CAN_Set_Receive_ID(unsigned int uController,
                         unsigned int *puControllerOut,
                         unsigned int uID, unsigned int uChannel,
                         unsigned int uFormat,
                         long lErrIn, long *plErrOut)
{
    if(!lErrIn)
    {
        lErrIn = CAN_rx_object(uController, uChannel, uID, uFormat);
    }

    if(puControllerOut)
    {
        *puControllerOut = uController;
    }

    if(plErrOut)
    {
        if(lErrIn)
        	*plErrOut = lErrIn + 21430;  //offset for LV error code database
		else
			*plErrOut = 0;
    }
}

eRunStatus
RLARM_CAN_Read(ArgList *argsIn, ArgList *argsOut, uInt32* nStartMs, Boolean* pbFirstRun, Boolean bRunToFinish)
{ 
    CAN_msg msg;
    unsigned int *puArrBuf = NULL;
    unsigned int *puMsgBuf = NULL;
	unsigned int uController = 0;
	unsigned int *puControllerOut = NULL;
	unsigned int uTimeout = -1;
	unsigned int *puID = NULL;
	unsigned int *puLength = NULL;
	VoidHand *vhData = NULL;
	unsigned int *puChannel = NULL;
	unsigned int *puFormat = NULL;
	unsigned int *puType = NULL;
	long lErrIn = 0;
	long *plErrOut = NULL;
	uInt32 uCallTime = 0;
	uInt32 uCurrentTime = 0;
	uInt32 uIncrementalTime = gnRunInterval;
	eRunStatus runStat = eFinished;


	if (argsIn->args[0].pValue) {
		uController = LVPtrToLong(argsIn->args[0].pValue,argsIn->args[0].nType);
	}
	if (argsIn->args[1].pValue) {
		uTimeout = LVPtrToLong(argsIn->args[1].pValue,argsIn->args[1].nType);
	}
	if (argsIn->args[2].pValue) {
		lErrIn = LVPtrToLong(argsIn->args[2].pValue,argsIn->args[2].nType);
	}
	puLength = argsOut->args[0].pValue;
	vhData = argsOut->args[1].pValue; 
	puControllerOut = argsOut->args[2].pValue;
	puID = argsOut->args[3].pValue;
	puChannel = argsOut->args[4].pValue;
	puFormat = argsOut->args[5].pValue;
	puType = argsOut->args[6].pValue;
	plErrOut = argsOut->args[7].pValue;
	
    if(!lErrIn)
    {
        if(!vhData)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NULL_POINTER;
            }
            return;
        }

		if(*pbFirstRun) {
			*nStartMs = LVGetTicks();
		 	*pbFirstRun = FALSE;
		}
		if(uIncrementalTime > uTimeout) {
		 	uIncrementalTime = uTimeout;
		}

		if(!bRunToFinish) {
			uCallTime = LVGetTicks();
			uCurrentTime = uCallTime;
			if (uTimeout >= 0) {
				do {
        			lErrIn = CAN_receive(uController, &msg, uIncrementalTime);
					uCurrentTime += uIncrementalTime;
				} while ((uCurrentTime - *nStartMs) < uTimeout && (uCurrentTime - uCallTime) < gnRunInterval && lErrIn );
			}
			else {
			 	do {
        			lErrIn = CAN_receive(uController, &msg, uIncrementalTime);
					uCurrentTime += uIncrementalTime;
				} while ((uCurrentTime - uCallTime) < gnRunInterval && lErrIn );
			}
		}
		else {
			lErrIn = CAN_receive(uController, &msg, uTimeout);
		}

		if (lErrIn && uTimeout < 0) {
			runStat = eNotFinished;
		}
		else if (lErrIn && uCurrentTime < (uInt32)(*nStartMs + uTimeout)) {
			runStat = eNotFinished;
		}

        if(!lErrIn)
        {
            if(puID)
            {
                *puID = msg.id;
            }
            if(puLength)
            {
                *puLength = msg.len;
            }
            if(puChannel)
            {
                *puChannel = msg.ch;
            }
            if(puFormat)
            {
                *puFormat = msg.format;
            }
            if(puType)
            {
                *puType = msg.type;
            }

            if(vhData)
            {
			#if defined(_Include_Array_Utils)
				*vhData = PDAArrNew1D(8, uCharDataType);
		        if(!*vhData)
		        {
		            if(plErrOut)
		            {
		                *plErrOut = ERRCODE_NULL_POINTER;
		            }
		            return;
		        }

                puArrBuf = (unsigned int *)(NthElemFast(((PDAArrPtr)*vhData), 0, sizeof(unsigned char)));
                puMsgBuf = (unsigned int *)&msg.data[0];
                puArrBuf[0] = puMsgBuf[0];
                puArrBuf[1] = puMsgBuf[1];
			#endif
            }
        }
    }

	if(puControllerOut)
    {
        *puControllerOut = uController;
    }

    if(plErrOut)
    {
        if(lErrIn)
        	*plErrOut = lErrIn + 21430;  //offset for LV error code database
		else
			*plErrOut = 0;
    }

	return runStat;
	
}

eRunStatus
RLARM_CAN_Write(ArgList *argsIn, ArgList *argsOut, uInt32* nStartMs, Boolean* pbFirstRun, Boolean bRunToFinish)
{
    CAN_msg msg;
    unsigned int *puArrBuf = NULL;
    unsigned int *puMsgBuf = NULL;
	unsigned int uController = NULL;
	unsigned int *puControllerOut = NULL;
	unsigned int uTimeout = -1;
    unsigned int uID = 0; 
	unsigned int uLength = 0;
    unsigned int uChannel = 0;
	unsigned int uFormat = 0;
	unsigned int uType = 0; 
	VoidHand *vhData = NULL;
    long lErrIn = 0;
	long *plErrOut = 0;
	uInt32 uCallTime = 0;
	uInt32 uCurrentTime = 0;
	uInt32 uIncrementalTime = gnRunInterval;
	eRunStatus runStat = eFinished;

	if (argsIn->args[0].pValue) {
		uController = LVPtrToLong(argsIn->args[0].pValue,argsIn->args[0].nType);
	}
	if (argsIn->args[1].pValue) {
		uID = LVPtrToLong(argsIn->args[1].pValue,argsIn->args[1].nType);
	}
	if (argsIn->args[2].pValue) {
		uChannel = LVPtrToLong(argsIn->args[2].pValue,argsIn->args[2].nType);
	}
	if (argsIn->args[3].pValue) {
		uFormat = LVPtrToLong(argsIn->args[3].pValue,argsIn->args[3].nType);
	}
	if (argsIn->args[4].pValue) {
		uType = LVPtrToLong(argsIn->args[4].pValue,argsIn->args[4].nType);
	}
	if (argsIn->args[5].pValue) {
		lErrIn = LVPtrToLong(argsIn->args[5].pValue,argsIn->args[5].nType);
	}
	if (argsIn->args[6].pValue) {
		uTimeout = LVPtrToLong(argsIn->args[6].pValue,argsIn->args[6].nType);
	}
	vhData = argsIn->args[7].pValue; 
	if (argsIn->args[8].pValue) {
		uLength = LVPtrToLong(argsIn->args[8].pValue,argsIn->args[8].nType);
	}
	puControllerOut = argsOut->args[0].pValue;
	plErrOut = argsOut->args[1].pValue;

    if(!lErrIn)
    {
        if(!vhData)
        {
            if(plErrOut)
            {
                *plErrOut = ERRCODE_NULL_POINTER;
            }
            return;
        }


        msg.id = uID;
        msg.len = uLength;
        msg.ch = uChannel;
        msg.format = uFormat;
        msg.type = uType;

        puArrBuf = (unsigned int *)(NthElemFast(((PDAArrPtr)*vhData), 0, sizeof(unsigned char)));
        puMsgBuf = (unsigned int *)&msg.data[0];
        puMsgBuf[0] = puArrBuf[0];
        puMsgBuf[1] = puArrBuf[1];

		if(*pbFirstRun) {
			*nStartMs = LVGetTicks();
		 	*pbFirstRun = FALSE;
		}
		if(uIncrementalTime > uTimeout) {
		 	uIncrementalTime = uTimeout;
		}

		if(!bRunToFinish) {
			uCallTime = LVGetTicks();
			uCurrentTime = uCallTime;
			if (uTimeout >= 0) {
				do {
        			lErrIn = CAN_send(uController, &msg, uIncrementalTime);
					uCurrentTime += uIncrementalTime;
				} while ((uCurrentTime - *nStartMs) < uTimeout && (uCurrentTime - uCallTime) < gnRunInterval && lErrIn );
			}
			else {
			 	do {
        			lErrIn = CAN_send(uController, &msg, uIncrementalTime);
					uCurrentTime += uIncrementalTime;
				} while ((uCurrentTime - uCallTime) < gnRunInterval && lErrIn );
			}
		}
		else {
			lErrIn = CAN_send(uController, &msg, uTimeout);
		}

		if (lErrIn && uTimeout < 0) {
			runStat = eNotFinished;
		}
		else if (lErrIn && uCurrentTime < (uInt32)(*nStartMs + uTimeout)) {
			runStat = eNotFinished;
		}   


    }
	if(puControllerOut)
    {
        *puControllerOut = uController;
    }
    if(plErrOut)
    {
        if(lErrIn)
        	*plErrOut = lErrIn + 21430;  //offset for LV error code database
		else
			*plErrOut = 0;
    }
    if(vhData)
    {
	#if defined(_Include_Array_Utils)
        PDAArrFree( *vhData );
	#endif
    }
	return runStat;
	
}

#endif
