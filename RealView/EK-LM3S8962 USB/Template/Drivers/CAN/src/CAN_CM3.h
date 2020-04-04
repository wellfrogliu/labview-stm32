/*----------------------------------------------------------------------------
 *      R T L  -  C A N   D r i v e r
 *----------------------------------------------------------------------------
 *      Name:    RTX_CAN.h
 *      Purpose: Header for CAN Generic Layer Driver
 *      Rev.:    V3.20
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2008 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>                      /* RTX kernel functions & defines      */

#define CAN_CLOCK         8000000
#define USE_CAN_CTRL1           1
#define USE_CAN_CTRL2           0
#define USE_CAN_CTRL3           0
#define CAN_No_SendObjects     20
#define CAN_No_ReceiveObjects  20
#if   (USE_CAN_CTRL3)
  #define CAN_CTRL_MAX_NUM      3
#elif (USE_CAN_CTRL2)
  #define CAN_CTRL_MAX_NUM      2
#elif (USE_CAN_CTRL1)
  #define CAN_CTRL_MAX_NUM      1
#else
  #error "No CAN Controller defined"
#endif

/* Error values that functions can return                                    */
typedef enum   
{ CAN_OK = 0,                       /* No error                              */
  CAN_NOT_IMPLEMENTED_ERROR,        /* Function has not been implemented     */
  CAN_MEM_POOL_INIT_ERROR,          /* Memory pool initialization error      */
  CAN_BAUDRATE_ERROR,               /* Baudrate was not set                  */
  CAN_TX_BUSY_ERROR,                /* Transmitting hardware busy            */
  CAN_OBJECTS_FULL_ERROR,           /* No more rx or tx objects available    */
  CAN_ALLOC_MEM_ERROR,              /* Unable to allocate memory from pool   */
  CAN_DEALLOC_MEM_ERROR,            /* Unable to deallocate memory           */
  CAN_TIMEOUT_ERROR,                /* Timeout expired                       */
  CAN_UNEXIST_CTRL_ERROR,           /* Controller does not exist             */
  CAN_UNEXIST_CH_ERROR,             /* Channel does not exist                */
}  CAN_ERROR;


/* Definition of memory pool type, for CAN messages                          */
#define CAN_msgpool_declare(name,cnt)     U32 name[cnt*(sizeof(CAN_msg)/4) + 3]

/* Definition of mailbox array, for used controllers                         */
#define mbx_arr_declare(name,arr_num,cnt) U32 name[arr_num][4 + cnt]

/* CAN message object structure                                              */
typedef struct  {
  U32 id;                 /* 29 bit identifier                               */
  U8  data[8];            /* Data field                                      */
  U8  len;                /* Length of data field in bytes                   */
  U8  ch;                 /* Object channel                                  */
  U8  format;             /* 0 - STANDARD, 1- EXTENDED IDENTIFIER            */
  U8  type;               /* 0 - DATA FRAME, 1 - REMOTE FRAME                */
}  CAN_msg;

/* Symbolic names for formats of CAN message                                 */
typedef enum {STANDARD_FORMAT = 0, EXTENDED_FORMAT} CAN_FORMAT;

/* Symbolic names for type of CAN message                                    */
typedef enum {DATA_FRAME = 0, REMOTE_FRAME}         CAN_FRAME;


/* Functions defined in module CAN_Hw.c                                      */
CAN_ERROR CAN_hw_loopback   (U32 ctrl);
CAN_ERROR CAN_hw_setup      (U32 ctrl);
CAN_ERROR CAN_hw_init       (U32 ctrl, U32 baudrate);
CAN_ERROR CAN_hw_start      (U32 ctrl);
CAN_ERROR CAN_hw_tx_empty   (U32 ctrl);
CAN_ERROR CAN_hw_wr         (U32 ctrl, CAN_msg *msg);
CAN_ERROR CAN_hw_set        (U32 ctrl, CAN_msg *msg);
CAN_ERROR CAN_hw_rx_object  (U32 ctrl, U32 ch, U32 id, CAN_FORMAT filter_type);
CAN_ERROR CAN_hw_tx_object  (U32 ctrl, U32 ch, CAN_FORMAT filter_type);

/* Functions defined in module CAN.c                                         */
CAN_ERROR CAN_init      (U32 ctrl, U32 baudrate);
CAN_ERROR CAN_start     (U32 ctrl);
CAN_ERROR CAN_send      (U32 ctrl, CAN_msg *msg, U16 timeout);
CAN_ERROR CAN_request   (U32 ctrl, CAN_msg *msg, U16 timeout);
CAN_ERROR CAN_set       (U32 ctrl, CAN_msg *msg, U16 timeout);
CAN_ERROR CAN_receive   (U32 ctrl, CAN_msg *msg, U16 timeout);
CAN_ERROR CAN_rx_object (U32 ctrl, U32 ch, U32 id, CAN_FORMAT filter_type);
CAN_ERROR CAN_tx_object (U32 ctrl, U32 ch, CAN_FORMAT filter_type);



/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

