/******************************************************************************/
/* TCPWrapper_rtx.c: LabVIEW wrapper for RL-ARM TCPNet                        */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2007-2008 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include "LVConfig.h"

#if DEBUG_LOG > 0
extern U16 os_time;
#endif

#if SocketSupport

#include "RLARM_Net_Config.h"
#include "RLARM_TCPWrapper.h"
#include "CCGWinsock.h"

#define RTA_ARP_TIMEOUT     500                   // ARP timeout in ms
#define RTA_ARP_RETRIES     6                     // Number of ARP retries before giving up


typedef struct _tag_LV_CBuf {
  U32 rxBufSize;
  U32 rxBufCnt;
  U32 rdIdx;
  U32 wrIdx;
  char *rxBuf;
} LV_CBuf;

typedef struct _tag_sockData {    
  U8 sockNo;
  U8 locIP[IP_ADRLEN];
  U16 locPort;
  U8 remIP[IP_ADRLEN];
  U16 remPort;
  U32 rxBufSize;
  U32 rxBufCnt;
  U32 rdIdx;
  U32 wrIdx;
  char *rxBuf;
} sockData;


extern U8 const tcp_NumSocks;
extern U8 const udp_NumSocks;
extern LOCALM const localm[3];                // Contains the local IP address
extern U8        lhost_name[16];              // IP hostname
extern struct udp_info udp_socket[];
extern void DoSysTaskDelay(uInt32 ms);

static U64 tcp_timer_stack[TCP_TIMER_STACK_BYTES/8]; 
static U64 tcp_stack[TCP_TASK_STACK_BYTES/8];

static OS_MUT stackMutx;                      // Mutex for the TCP IP stack
static sockData *tcpSockets = NULL;
static sockData *udpSockets = NULL;

static U16 tcp_callback (U8 soc, U8 event, U8 *ptr, U16 par);
static U16 udp_callback (U8 socket, U8 *remip, U16 remport, U8 *buf, U16 len);
static void dns_cbfunc (U8 event, U8 *ip);
static BOOL is_str_ip (const char *sp);
static BOOL check_arp (U8* remip);

static BOOL initXXPBuf (sockData *pSockBuf, U16 bufSize);
static void uninitXXPBuf(sockData *pSockBuf);
static void writeXXPBuf (sockData *pSockBuf, char *pData, U16 *len);
static void readXXPBuf (sockData *pSockBuf, char *pData, U16 *len);
static void peekXXPBuf (sockData *pSockBuf, char *pData, U16 *len);
static void peekXXPBuf4CRLF (sockData *pSockBuf, U16 *len);
   
static sockData * getTCPSock(U8 sock);
static sockData * getUDPSock(U8 sock);
static void releaseSock (sockData *pSockBuf);

__task void tcp_timer_task (void);
__task void tcp_task   (void);

static U32 tcp_timer_tid;
static U32 tcp_tid;



/*===================================================================*/
// Init functions
/*===================================================================*/

int RTX_tcp_init (void) {

  os_mut_init(stackMutx);
  
  // Allocate the sockets structure on the heap
  if (tcp_NumSocks) {
    tcpSockets = MemPtrNew (tcp_NumSocks * sizeof(sockData));
    if (!tcpSockets) {
      // Out of memory
      return (-1);
    }
    else {
      memset (tcpSockets, 0, tcp_NumSocks * sizeof(sockData));
    }
  }

  // Allocate the sockets structure on the heap
  if (udp_NumSocks) {		   
    udpSockets = MemPtrNew (udp_NumSocks * sizeof(sockData));
    if (!udpSockets) {
      // Out of memory
      return (-1);
    }
    else {
      memset (udpSockets, 0, udp_NumSocks * sizeof(sockData));
    }
  }

  init_TcpNet ();
  tcp_timer_tid = os_tsk_create_user (tcp_timer_task, 30, &tcp_timer_stack, sizeof(tcp_timer_stack));
  tcp_tid = os_tsk_create_user (tcp_task, DEFAULT_PRIORITY, &tcp_stack, sizeof(tcp_stack));

  return 0;
}

int RTX_tcp_uninit (void) {
  os_tsk_delete (tcp_timer_tid);
  os_tsk_delete (tcp_tid);
  if (tcpSockets)
    MemPtrFree(tcpSockets);
  if (udpSockets)
    MemPtrFree(udpSockets);
  return (0);
}

/*===================================================================*/
// TCP functions
/*===================================================================*/


U8 RTX_tcp_open_socket (U8 type, U8 tos, U16 tout, U16 bufSize) {
  sockData *pSockBuf;  
  int i;
  U8 sock;

  os_mut_wait(stackMutx, (-1));                           // Get stack mutex...
  sock = tcp_get_socket (type | TCP_TYPE_DELAY_ACK, tos, tout, tcp_callback);
  if (sock) {
    pSockBuf = &tcpSockets[sock - 1];         
    if (initXXPBuf(pSockBuf, bufSize)) {                  // Initialise the buffer      
      pSockBuf->sockNo = sock;
      // Set the local IP address
      memcpy(pSockBuf->locIP, localm[NETIF_ETH].IpAdr, IP_ADRLEN);
    }
    else {
      tcp_release_socket(sock);
      sock = 0;
    }        
  }
  os_mut_release(stackMutx);                              // Release stack mutex... 

  return (sock);
}


BOOL RTX_tcp_close_socket (U8 socket) {
  BOOL retVal = __FALSE;
  sockData *pSockBuf;
  U8 sockState;
  
  pSockBuf = getTCPSock(socket);
  if (pSockBuf) {
    
    sockState = tcp_get_state (socket);               // Get current state of the socket    
    if (sockState != TCP_STATE_LISTEN) {              // Never close a listening socket
      retVal = tcp_close(socket);                     // Close the socket
      // Wait for socket to be closed	
      while ((sockState != TCP_STATE_CLOSED) && (sockState != TCP_STATE_LISTEN)) {
        os_mut_release(stackMutx);                    // Release stack mutex...
        SysTaskDelay(1);                              // Switch tasks (to stack thread)
        os_mut_wait(stackMutx, (-1));                 // Get stack mutex...
        sockState = tcp_get_state (socket);           // Get current state of the socket
      }
    }

    if (sockState != TCP_STATE_LISTEN) {              // If the socket is a listen socket, leave open for now     
      tcp_release_socket (socket);                    // Release the socket
  
      uninitXXPBuf(pSockBuf);                         // Release the RX buffer if allocated
      memset(pSockBuf->remIP, 0, IP_ADRLEN);          // Clear the remote connection details      
      pSockBuf->remPort = 0;          
      memset(pSockBuf->locIP, 0, IP_ADRLEN);          // Clear the local connection details
      pSockBuf->locPort = 0;                        
      pSockBuf->sockNo = 0;
    }
    
    releaseSock(pSockBuf);
  }

  return (retVal);
}

BOOL RTX_tcp_listen (U8 socket, U16 locport) {
  BOOL retVal = __FALSE;
  sockData *pSockBuf;

  pSockBuf = getTCPSock(socket);
  if (pSockBuf) {
    retVal = tcp_listen (socket, locport);
    if (retVal) pSockBuf->locPort = locport;
    releaseSock(pSockBuf);
  }

  return (retVal);
}

BOOL RTX_tcp_connect (U8 socket, U8 *remip, U16 remport, U16 locport) {
  BOOL retVal = __FALSE;
  sockData *pSockBuf;

  pSockBuf = getTCPSock(socket);
  if (pSockBuf) {
    retVal = tcp_connect (socket, remip, remport, locport);
    if (retVal) pSockBuf->locPort = locport;
    releaseSock(pSockBuf);
  }

  return (retVal);
}

BOOL RTX_tcp_abort (U8 socket) {
  sockData *pSockBuf;  

  pSockBuf = getTCPSock(socket);
  if (pSockBuf) {
    // TODO
    releaseSock(pSockBuf);
  }

  return __FALSE;
}

U8 RTX_tcp_get_state (U8 socket) {
  U8 retVal = TCP_STATE_FREE;
  sockData *pSockBuf;
  
  pSockBuf = getTCPSock(socket);
  if (pSockBuf) {
    retVal = tcp_get_state (socket);
    releaseSock(pSockBuf);
  }
  return (retVal);
}

BOOL RTX_tcp_send (U8 socket, U8 *buf, U16 *dlen, int timeout) {
  BOOL retVal = __FALSE;
  U8 *sendbuf;
  uInt32 maxlen, lenToSend, lenRemaining;
  U16 lenWritten = 0;
  sockData *pSockBuf;  
  U8 sockState;

  DEBUG_LOG3(("RTX_tcp_send begin - %d\n", os_time));
  
  pSockBuf = getTCPSock(socket);
  if (pSockBuf) { 
    // Get the max segment size   
    maxlen = tcp_max_dsize (socket);
    
    // Send the data, in multiple segments if necessary
    lenRemaining = *dlen;
    retVal = __TRUE;
    while ((lenRemaining > 0) && (retVal == __TRUE)) { 
      // Wait for the socket to be ready
      while (!tcp_check_send(socket)) {
        if (timeout != (-1)) {        
          if (timeout == 0) {        
            // Send timeout occurred
            retVal = __FALSE;         
            goto err_tcp;
          }
          timeout--;
        }
		sockState = tcp_get_state(socket);
		if(sockState == TCP_STATE_FREE || sockState == TCP_STATE_CLOSED){
            retVal = __FALSE;         
            goto err_tcp;
		}
        os_mut_release(stackMutx);                  // Release buffer mutex...
        SysTaskDelay(1);                            // Wait for OK to send
        os_mut_wait(stackMutx, (-1));               // Get buffer mutex...
      }
      lenToSend = lenRemaining > maxlen ? maxlen : lenRemaining;
      sendbuf = tcp_get_buf (lenToSend);
      if (sendbuf) {
        memcpy (sendbuf, buf, lenToSend);
        retVal = tcp_send (socket, sendbuf, lenToSend);
        buf += lenToSend;
        lenRemaining -= lenToSend;        
      }
      else {
        retVal = __FALSE;         
        goto err_tcp;  
      }
    } 
          
err_tcp:
    lenWritten = *dlen - lenRemaining;
    releaseSock(pSockBuf);
  }
  *dlen = lenWritten;                                 // Set the length sent
  DEBUG_LOG3(("RTX_tcp_send end - %d\n", os_time));

  return (retVal);
}
   
BOOL RTX_tcp_recv (U8 socket, U8 *buf, U16 *dlen) {
  BOOL retVal = __FALSE;
  U16 lenRead = 0;
  sockData *pSockBuf;

  pSockBuf = getTCPSock(socket);
  if (pSockBuf) {
    lenRead = *dlen;
    readXXPBuf (pSockBuf, buf, &lenRead);
    retVal = __TRUE;
    releaseSock(pSockBuf);
  }
  *dlen = lenRead;                                    // Set the length read

  return (retVal);
}

BOOL RTX_tcp_recv_timeout (U8 socket, U8 *buf, U16 *dlen, int timeout, Err *err){
    U16 len = *dlen;
	U16 rxlen = 0;
	U8 sockState;

	DEBUG_LOG3(("RTX_tcp_recv_timeout begin - %d\n", os_time));

	if(err) *err = 0;
	while(rxlen < *dlen && timeout){
	    sockState = tcp_get_state(socket);
		if(sockState == TCP_STATE_FREE || sockState == TCP_STATE_CLOSED){
			if(err) *err = WSAENOTCONN;
			return(__FALSE);
		}
		if(RTX_tcp_recv(socket, buf+rxlen, &len) != __TRUE){
			*dlen = rxlen;						// set the length recevied
			if(err) *err = WSAEOUTOFMEM;
			return(__FALSE);
		}
		rxlen += len;
		len = *dlen - rxlen;
		if(timeout > 0)							// protect against -1 timeout
		   timeout--;
		if(len)                                 // Still more to recv, delay first
			SysTaskDelay(1);
	}

	DEBUG_LOG3(("RTX_tcp_recv_timeout end - %d\n", os_time));
	if(*dlen == rxlen)
		return(__TRUE);
	else{
	    if(err) *err = WSAETIMEDOUT;
		*dlen = rxlen;							// set the length received
		return(__FALSE);
	}
}

U16 RTX_tcp_rx_bytes (U8 socket) {
  U16 rxBytes = 0;
  sockData *pSockBuf;

  pSockBuf = getTCPSock(socket);
  if (pSockBuf) {
    rxBytes = pSockBuf->rxBufCnt;                     // Get the rx buffer count
    releaseSock(pSockBuf);
  }

  return (rxBytes);
}

U16 RTX_tcp_rx_crlf (U8 socket, U16 maxLen) {
  U16 crlfEndOffset = 0;
  sockData *pSockBuf;

  pSockBuf = getTCPSock(socket);
  if (pSockBuf) {
    crlfEndOffset = maxLen;  
    peekXXPBuf4CRLF (pSockBuf, &crlfEndOffset);       // Find the CRLF

    releaseSock(pSockBuf);
  }

  return (crlfEndOffset);
}
   
static U16 tcp_callback (U8 soc, U8 event, U8 *ptr, U16 par) {
  /* This function is called on debug lsock event */
  U16 retVal = 0;
  U32 i;
  sockData *pSockBuf = &tcpSockets[soc - 1];
  U16 lenWrite = par;
  volatile BOOL crap;

  switch (event) {
    case TCP_EVT_CONREQ:
      /* Remote host is trying to connect to our TCP socket. */
      /* 'ptr' points to Remote IP, 'par' holds the remote port. */
      /* Store the remote port and remote address to the socket struct */
      memcpy(pSockBuf->remIP, ptr, IP_ADRLEN);
      pSockBuf->remPort = par;
            
      /* Return 1 to accept connection, or 0 to reject connection */
      retVal = (1);
    break;

    case TCP_EVT_CONNECT:
      /* Socket is connected to remote peer. */      
    break;

    case TCP_EVT_ABORT:
      /* Connection was aborted */
    case TCP_EVT_CLOSE:
      /* Connection has been closed */ 
      soc = soc;       
    break;

    case TCP_EVT_ACK:
      /* Our sent data has been acknowledged by remote peer */      
    break;

    case TCP_EVT_DATA:
      /* TCP data frame has been received, 'ptr' points to data */
      /* Data length is 'par' bytes */      

      /* We can afford to block here for a little while, if the input buffer is full.
         The stack has it's own internal buffer that will fill with other incoming
         packets. However, no packets will be transmitted, so we should not block
         too long before discarding the packet. 100ms in 10ms intervals is a good choice */
      i = 0;
      while ((pSockBuf->rxBufSize - pSockBuf->rxBufCnt) < par) {
        // Not enough buffer, so wait until there is (this allows stack to drop packets gracefully)!
        os_mut_release(stackMutx);                    // Release buffer mutex... 
        os_tsk_pass();                                // Give up time slice to equally low priority tasks
        os_mut_wait(stackMutx, (-1));                 // Get buffer mutex...
      }

      writeXXPBuf (pSockBuf, ptr, &lenWrite); 
           
    break;
  }

  return (retVal);
}





/*===================================================================*/
// UDP functions
/*===================================================================*/

U8 RTX_udp_open_socket (U8 tos, U8 opt, U16 bufSize) {
  sockData *pSockBuf;  
  int i;
  U8 sock;

  os_mut_wait(stackMutx, (-1));                           // Get stack mutex...
  sock = udp_get_socket (tos, opt, udp_callback);
  if (sock) {
    pSockBuf = &udpSockets[sock - 1];      

    if (initXXPBuf (pSockBuf, bufSize)) {         
      pSockBuf->sockNo = sock;
      // Set the local IP address
      memcpy(pSockBuf->locIP, localm[NETIF_ETH].IpAdr, IP_ADRLEN);
    }
    else {
      tcp_release_socket(sock);
      sock = 0;
    }    
  }
  os_mut_release(stackMutx);                              // Release stack mutex...

  return (sock);
}


BOOL RTX_udp_close_socket (U8 socket) {
  BOOL retVal = __FALSE;
  sockData *pSockBuf;

  pSockBuf = getUDPSock(socket);
  if (pSockBuf) {
    udp_close(socket);                              // Close the socket
    retVal = udp_release_socket(socket);            // Release the socket resources

    uninitXXPBuf(pSockBuf);                         // Release the internal buffer      
    memset(pSockBuf->remIP, 0, IP_ADRLEN);          // Clear the remote connection details      
    pSockBuf->remPort = 0;          
    memset(pSockBuf->locIP, 0, IP_ADRLEN);          // Clear the local connection details
    pSockBuf->locPort = 0;          
    pSockBuf->sockNo = 0;                           // Clear the socket number

    releaseSock(pSockBuf);
  }

  return (retVal);
}

BOOL RTX_udp_listen (U8 socket, U16 locport) {
  BOOL retVal = __FALSE;
  sockData *pSockBuf;

  pSockBuf = getUDPSock(socket);
  if (pSockBuf) {
    retVal = udp_open (socket, locport);
    if (retVal) pSockBuf->locPort = locport;    
    releaseSock(pSockBuf);
  }

  return (retVal);
}

BOOL RTX_udp_send (U8 socket, U8* remip, U16 remport, U8 *buf, U16 *dlen) {
  BOOL retVal = __FALSE;
  U8 *sendbuf;
  U16 lenToSend = *dlen;
  U16 lenWritten = 0;
  sockData *pSockBuf;

  pSockBuf = getUDPSock(socket);
  if (pSockBuf) {
    retVal = check_arp(remip);
    if (retVal == __TRUE) {
      sendbuf = udp_get_buf (lenToSend);
      if (sendbuf) {
        memcpy (sendbuf, buf, lenToSend);
        retVal = udp_send (socket, remip, remport, sendbuf, lenToSend);      
        lenWritten = lenToSend;
      }
      else {
        retVal = __FALSE;
      }
    }
    releaseSock(pSockBuf);
  }
  *dlen = lenWritten;                                 // Set the length sent

  return (retVal);
}


BOOL RTX_udp_recv (U8 socket, U8 *buf, U16 *dlen, U8 *remip, U16 *remport) {
  BOOL retVal = __FALSE;
  U16 dataLen, lenRead, lenDiscard, i, u16Temp;
  char cNull;
  sockData *pSockBuf;

  pSockBuf = getUDPSock(socket);
  if (pSockBuf) {
    // Read the packet from the packet buffer
    // FORMAT: | IPADR | PORT | LEN | DATA |
  
    // Read the IP address (if this returns a zero length, no packet is available)
    lenRead = IP_ADRLEN;
    readXXPBuf (pSockBuf, (char *)remip, &lenRead);

    if (lenRead) {      
      lenRead = sizeof(U16);
      readXXPBuf (pSockBuf, (char *)remport, &lenRead);       // Read the port      
      lenRead = sizeof(U16);  
      readXXPBuf (pSockBuf, (char *)&dataLen, &lenRead);      // Read the data length      
      if (*dlen < dataLen) {
        lenRead = *dlen;
        lenDiscard = dataLen - lenRead; 
      }
      else {
        lenRead = dataLen;
        lenDiscard = 0;
      }      
      readXXPBuf (pSockBuf, (char *)buf, &lenRead);           // Read the data

      // Discard data from the packet that would not fit in the read buffer
      // TODO: Optimize this!
      if (lenDiscard) {
        for (i=0; i<lenDiscard; i++) {
          readXXPBuf (pSockBuf, &cNull, &u16Temp);
        }
      }
    }

    retVal = __TRUE;
    releaseSock(pSockBuf);
  }
  *dlen = lenRead;                                            // Set the length read

  return (retVal);
}

U16 RTX_udp_rx_bytes (U8 socket) {
  U16 rxBytes = 0;
  sockData *pSockBuf;
  U16 lenUDPDesc = IP_ADRLEN + sizeof(U16) + sizeof(U16);
  U16 lenPeeked;
  U8 *pDataLen;
  U8 peekBuf[IP_ADRLEN + sizeof(U16) + sizeof(U16)];

  pSockBuf = getUDPSock(socket);
  if (pSockBuf) {
    if (pSockBuf->rxBufCnt) {
      lenPeeked = lenUDPDesc; 
      peekXXPBuf(pSockBuf, peekBuf, &lenPeeked);
      if (lenPeeked == lenUDPDesc) {
        pDataLen = peekBuf + IP_ADRLEN + sizeof(U16);
        memcpy(&rxBytes, pDataLen, sizeof(U16));
      }
    }
    releaseSock(pSockBuf);
  }

  return (rxBytes);
}

static U16 udp_callback (U8 socket, U8 *remip, U16 remport, U8 *buf, U16 len) {
  /* This function is called when UDP data is received */
  sockData *pSockBuf = &udpSockets[socket - 1];
  U16 lenWrite = IP_ADRLEN + sizeof(U16) + sizeof(U16) + len;

  /* We can afford to block here for a little while, if the input buffer is full.
     The stack has it's own internal buffer that will fill with other incoming
     packets. However, no packets will be transmitted, so we should not block
     too long before discarding the packet. 100ms in 10ms intervals is a good choice */
  while ((pSockBuf->rxBufSize - pSockBuf->rxBufCnt) < lenWrite) {
    // Not enough buffer, so wait until there is (this allows stack to drop packets gracefully)!
    os_mut_release(stackMutx);                    // Release buffer mutex... 
    os_tsk_pass();                                // Give up time slice to equally low priority tasks
    os_mut_wait(stackMutx, (-1));                 // Get buffer mutex...
  }

  // Write the packet to the packet buffer
  // FORMAT: | IPADR | PORT | LEN | DATA |
  lenWrite = IP_ADRLEN;
  writeXXPBuf (pSockBuf, remip, &lenWrite);
  lenWrite = sizeof(remport);
  writeXXPBuf (pSockBuf, (char *)&remport, &lenWrite);
  lenWrite = sizeof(len);
  writeXXPBuf (pSockBuf, (char *)&len, &lenWrite);
  lenWrite = len;
  writeXXPBuf (pSockBuf, buf, &lenWrite);

  return (0);
}


/*===================================================================*/
// IP Helper functions
/*===================================================================*/


BOOL RTX_inet_get_addr (U8 socket, U8 *pLocIP, U16 *pLocPort, U8 *pRemIP, U16 *pRemPort) {
  BOOL retVal = __FALSE;
  sockData *pSockBuf;
    
  pSockBuf = getTCPSock(socket);
  if (pSockBuf) {
    if (pLocIP) memcpy(pLocIP, pSockBuf->locIP, IP_ADRLEN);
    if (pLocPort) *pLocPort = pSockBuf->locPort;
    if (pRemIP) memcpy(pRemIP, pSockBuf->remIP, IP_ADRLEN);
    if (pRemPort) *pRemPort = pSockBuf->remPort;
      
    retVal = __TRUE;
    releaseSock(pSockBuf);
  }

  return (retVal);
}

BOOL RTX_inet_get_ipaddr (U8 *ipAddr) {
  memcpy(ipAddr, localm[NETIF_ETH].IpAdr, IP_ADRLEN); // Set the local IP address
  return __TRUE;
}

BOOL RTX_inet_get_hostname (U8 *hostname, U32 nameLen) {
  BOOL retVal = __FALSE;
  if (nameLen >= 16) {
    memcpy(hostname, lhost_name, 16);       // Copy the hostname
    retVal = __TRUE;
  }
  return retVal;
}


void RTX_inet_ntoa(const U8 *pIpAddr, char *pIpStr) { 
  sprintf(pIpStr, "%d.%d.%d.%d", pIpAddr[0], pIpAddr[1], pIpAddr[2], pIpAddr[3]);
}

static U8 *dns_pIpAddr;
static OS_SEM dns_sem;
static OS_MUT dns_mtx;
static U8 dns_res;
static int dnsInit = 0;

BOOL RTX_get_host_by_name (const char *pIpStr, U8 *pIpAddr) {
  BOOL retVal = __FALSE;
  int ipTmp[4];

  if (!dnsInit) {
    os_sem_init (dns_sem, 0);
    os_mut_init (dns_mtx);
    dnsInit = 1;
  }
  
  if (is_str_ip (pIpStr)) {    /* Check if IP address provided in string format. */
    sscanf ((const S8 *)pIpStr, "%d.%d.%d.%d",&ipTmp[0],&ipTmp[1],&ipTmp[2],&ipTmp[3]);
    pIpAddr[0] = ipTmp[0];
    pIpAddr[1] = ipTmp[1];
    pIpAddr[2] = ipTmp[2];
    pIpAddr[3] = ipTmp[3];
    retVal = __TRUE;
  }
  else {                      /* Otherwise attempt a host name request */  
    os_mut_wait (dns_mtx, (-1));                  // allow only one request at a time
    
    dns_pIpAddr = pIpAddr; 
 
    os_mut_wait(stackMutx, (-1));                 // Get stack mutex...
    get_host_by_name ((U8 *)pIpStr, dns_cbfunc);  // Do the get host by name
    os_mut_release(stackMutx);                    // Release stack mutex... 
	
    os_sem_wait (dns_sem, (-1));                  // wait for dns callback to complete
    if (dns_res == DNS_RES_OK) retVal = __TRUE;
    os_mut_release (dns_mtx);
  }

  return retVal;
}

static void dns_cbfunc (U8 event, U8 *ip) {
  switch (event) {
    case DNS_RES_OK:
      memcpy (dns_pIpAddr, ip, IP_ADRLEN);
      break;
    default:
      memset (dns_pIpAddr, 0, IP_ADRLEN);
  }
  dns_res = event;
  os_sem_send(dns_sem);
}

static BOOL is_str_ip (const char *sp) {
   /* Check if provided host name is a string IP. */
   int i,ndp,ch;
   BOOL fnum;

   ndp  = 0;
   fnum = __TRUE;
   for (i = 0; (ch = *sp) != 0; sp++, i++) {
      if (i > 15) {
         return (__FALSE);
      }
      if (!fnum && ch == '.') {
         ndp++;
         fnum = __TRUE;
         continue;
      }
      if (ch >= '0' && ch <= '9') {
         fnum = __FALSE;
         continue;
      }
      return (__FALSE);
   }
   if (i < 7 || ndp > 3) {
      return (__FALSE);
   }
   return (__TRUE);
}

static BOOL check_arp (U8* remip) {
  int iRetries = 0;

  while ((arp_cache_ip (remip, ARP_FIXED_IP) == __FALSE) && (iRetries < RTA_ARP_RETRIES)) {
    os_mut_release(stackMutx);                    // Release stack mutex...            
    os_dly_wait(RTA_ARP_TIMEOUT);                 // Wait for the ARP to complete
    os_mut_wait(stackMutx, (-1));                 // Get stack mutex...
    iRetries++;
  }

  return (arp_cache_ip (remip, ARP_FIXED_IP));
}


/*===================================================================*/
// sockData manipulation functions
/*===================================================================*/

static sockData * getTCPSock(U8 sock) {
  sockData *pSockBuf = NULL;
  U8 sockNo;
  
  if ((sock > 0) && (sock <= tcp_NumSocks)) {
    pSockBuf = &tcpSockets[sock - 1];

    os_mut_wait(stackMutx, (-1));               // Get stack mutex...
    sockNo = pSockBuf->sockNo;
    if (sockNo <= 0) {
      os_mut_release(stackMutx);                // Release stack mutex...
      pSockBuf = NULL;                          // The socket number is valid, but the socket is closed
    }
  }
  return pSockBuf;
}

static sockData * getUDPSock(U8 sock) {
  sockData *pSockBuf = NULL;
  U8 sockNo;
  
  if ((sock > 0) && (sock <= udp_NumSocks)) {
    pSockBuf = &udpSockets[sock - 1];

    os_mut_wait(stackMutx, (-1));               // Get stack mutex...
    sockNo = pSockBuf->sockNo;
    if (sockNo <= 0) {
      os_mut_release(stackMutx);                // Release stack mutex...
      pSockBuf = NULL;                          // The socket number is valid, but the socket is closed
    }
  }
  return pSockBuf;
}

static void releaseSock (sockData *pSockBuf) {
  os_mut_release(stackMutx);                    // Release stack mutex...
}

U16 getUDPLocalPort (U8 socket) {
  U16 retVal = 0;
  UDP_INFO *udp_soc = NULL;
 
  if (socket != 0 && socket <= udp_NumSocks) {
    udp_soc = &udp_socket[socket-1];
    if (udp_soc->State == UDP_STATE_OPENED) {
      retVal = udp_soc->LocPort;
    }
  }
 
  return (retVal);
}

/*===================================================================*/
// Circular Buffer Management functions
/*===================================================================*/
  
static BOOL initXXPBuf (sockData *pSockBuf, U16 bufSize) {
  BOOL retVal = __FALSE;

  pSockBuf->rdIdx = pSockBuf->wrIdx = 0;
  pSockBuf->rxBufSize = 0;
    
  // Allocate the RX buffer            
  pSockBuf->rxBuf = MemPtrNew(bufSize);
  if (pSockBuf->rxBuf) {      
    pSockBuf->rxBufSize = bufSize;
    retVal = __TRUE;
  }

  return retVal;
}

static void uninitXXPBuf(sockData *pSockBuf) {
  if (pSockBuf->rxBuf) {                            // Release the RX buffer if allocated
    MemPtrFree(pSockBuf->rxBuf);
    pSockBuf->rxBuf = NULL;
  }
}

static void writeXXPBuf (sockData *pSockBuf, char *pData, U16 *len) {
  U32 toEndCnt, wrCnt, wLen, i;
  U16 lenWritten = 0;

  lenWritten = *len;
  wLen = *len;

  // Write the received data to the circular buffer
  pSockBuf->rxBufCnt += wLen;                               // Update the rx data count
  toEndCnt = pSockBuf->rxBufSize - pSockBuf->wrIdx;         // Calculate the count to buffer end
  wrCnt = wLen > toEndCnt ? toEndCnt : wLen;                // See if amount to write is greater than to buffer end
  
  memcpy (pSockBuf->rxBuf + pSockBuf->wrIdx, pData, wrCnt); // Write data up to buffer end
  wLen -= wrCnt;                                            // Update remaining count (non-zero if buffer wrapping)
  pSockBuf->wrIdx += wrCnt;                                 // Update the write index
  if (pSockBuf->wrIdx == pSockBuf->rxBufSize) {
    pSockBuf->wrIdx = 0;                                    // Wrap the write index if necessary
  }      

  if (wLen > 0) {                                           // If wLen > 0, the buffer needs wrapping
    // We need to wrap the buffer
    pData += wrCnt;                                         // Update the in data pointer
    memcpy (pSockBuf->rxBuf, pData, wLen);                  // Write the wrapped data
    pSockBuf->wrIdx = wLen;                                 // Update the write index
  }

  *len = lenWritten;
}


#define FLAG_PEEKING     0x01
#define FLAG_FIND_CRLF   0x02

static void peekreadXXPBuf (sockData *pSockBuf, char *pData, U16 *len, U32 flags) {
  U16 lenToRead, lenRead = 0;
  U32 toEndCnt, rdCnt, tempRdIdx, tempBufCnt;
  U16 i;
  BOOL bFoundCR = __FALSE;

  // Copy the real read index and buffer counts to a temporary variables for peeking
  tempRdIdx = pSockBuf->rdIdx;
  tempBufCnt = pSockBuf->rxBufCnt;

  // Calculate the length of data we can read
  lenToRead = ((pSockBuf->rxBufCnt) > (*len)) ? *len : tempBufCnt;

  // Read the data from the RX buffer
  lenRead = lenToRead;                                    // Set return length
  tempBufCnt -= lenToRead;                                // Update the rx data count
  toEndCnt = pSockBuf->rxBufSize - tempRdIdx;             // Calculate the count to buffer end
  rdCnt = lenToRead > toEndCnt ? toEndCnt : lenToRead;    // See if amount to read is greater than to buffer end
  
  if (!(flags & FLAG_FIND_CRLF)) {
    memcpy (pData, pSockBuf->rxBuf + tempRdIdx, rdCnt);   // Write data up to buffer end
  }
  else {
    for (i=0; i<rdCnt; i++) {
      if (!bFoundCR) {                                    
        if (*(pSockBuf->rxBuf + i) == '\015') {           // Check for CR
          bFoundCR = __TRUE;
        }
      }
      else {
        if (*(pSockBuf->rxBuf + i) == '\012') {           // Check for LF
          *len = i;
          return;                                         // Found CRLF
        }
        bFoundCR = __FALSE;
      }      
    }
  }
  
  lenToRead -= rdCnt;                                     // Update remaining count (non-zero if buffer wrapping)
  tempRdIdx += rdCnt;                                     // Update the read index
                                                          
  if (tempRdIdx == pSockBuf->rxBufSize) {
    tempRdIdx = 0;                                        // Wrap the read index if necessary
  }                                                         
  if (lenToRead > 0) {                                    // If lenToRead > 0, the buffer needs wrapping
    // We need to wrap the buffer
    pData += rdCnt;                                       // Update the out data pointer
    if (!(flags & FLAG_FIND_CRLF)) {
      memcpy (pData, pSockBuf->rxBuf, lenToRead);         // Read the wrapped data
    }
    else {
      for (i=0; i<lenToRead; i++) {
        if (!bFoundCR) {
          if (*(pSockBuf->rxBuf + i) == '\015') {         // Check for CR
            bFoundCR = __TRUE;
          }
        }
        else {
          if (*(pSockBuf->rxBuf + i) == '\012') {         // Check for LF
            *len = rdCnt + i;
            return;                                       // Found CRLF
          }
          bFoundCR = __FALSE;
        }      
      }
    }
    tempRdIdx = lenToRead;                                // Update the read index
  }

  if (!(flags & FLAG_PEEKING)) {
    pSockBuf->rdIdx = tempRdIdx;                          // If not peeking, update the real read index
    pSockBuf->rxBufCnt = tempBufCnt;                       // and the real buffer count
  }
  else {
    if (flags & FLAG_FIND_CRLF) lenRead = 0;              // Otherwise if looking for CRLF, then we haven't found it!
  }

  *len = lenRead; 
}

static void readXXPBuf (sockData *pSockBuf, char *pData, U16 *len) {
  peekreadXXPBuf (pSockBuf, pData, len, 0);
}

static void peekXXPBuf (sockData *pSockBuf, char *pData, U16 *len) {
  peekreadXXPBuf (pSockBuf, pData, len, FLAG_PEEKING);
}

static void peekXXPBuf4CRLF (sockData *pSockBuf, U16 *len) {
  peekreadXXPBuf (pSockBuf, NULL, len, FLAG_PEEKING | FLAG_FIND_CRLF);
}



/*===================================================================*/
// RTX TCP Main and Timer Tasks
/*===================================================================*/

__task void tcp_timer_task (void) {
   /* System tick timer task */
   os_itv_set (100);
   while (1) {
      timer_tick ();
      os_itv_wait ();
   }
}

__task void tcp_task (void) {
   /* Main Thread of the TcpNet. This task should have */
   /* the lowest priority because it is always READY. */
   while (1) {
      os_mut_wait(stackMutx, (-1));                           // Get stack mutex...
      main_TcpNet();
      os_mut_release(stackMutx);                    // Release stack mutex...
      os_tsk_pass();
   }
}


#endif // if SocketSupport








