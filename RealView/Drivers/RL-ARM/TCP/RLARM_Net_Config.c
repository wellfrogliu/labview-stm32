/*----------------------------------------------------------------------------
 *      R T L  -  T C P N E T
 *----------------------------------------------------------------------------
 *      Name:    NET_CONFIG.C
 *      Purpose: Configuration of RTL TcpNet by user.
 *      Rev.:    V3.20
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2008 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include "LVSysIncludes.h"
#include "LVCCG.h"
#include "LVConfig.h"

#if SocketSupport

#include "RLARM_Net_Config.h"


/*----------------------------------------------------------------------------
 *      RTL User configuration part END
 *---------------------------------------------------------------------------*/

#define TICK_RATE       1000 / TICK_INTERVAL

#if (SLIP_ENABLE | PPP_ENABLE | ETH_ENABLE) == 0
   #warning all network interfaces disabled in configuration
#endif

#if (SLIP_ENABLE & PPP_ENABLE) == 1
   #error both SLIP and PPP interfaces enabled
#endif

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/

/* Own and Broadcast Ethernet address */
#if ETH_ENABLE == 1
  #if !defined(USE_HW_MAC) || !defined(USE_MANUF_MAC) 
    U8       own_hw_adr[ETH_ADRLEN] = { _MAC1, _MAC2, _MAC3, _MAC4, _MAC5, _MAC6 };
  #endif
  U8 const MAC_Broadcast[ETH_ADRLEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
#endif
/* Broadcast IP address */
U8 const IP_Broadcast[IP_ADRLEN]  = { 255, 255, 255, 255 };
/* Local Machine info */
LOCALM const nlocalm[3] = {
  /* Ethernet: Local IP address */
#if ETH_ENABLE == 1
  { { _IP1, _IP2, _IP3, _IP4 },
    /* Default Gateway IP address */
    { _GW1, _GW2, _GW3, _GW4 },
    /* Subnet IP Mask */
    { _MSK1, _MSK2, _MSK3, _MSK4 },
    /* Primary DNS Server IP */
    { _pDNS1, _pDNS2, _pDNS3, _pDNS4 },
    /* Secondary DNS Server IP */
    { _sDNS1, _sDNS2, _sDNS3, _sDNS4 }
  },
#else
   { { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 } },
#endif
  /* PPP: Local IP address */
#if PPP_ENABLE == 1
  { { _IP1P, _IP2P, _IP3P, _IP4P },
    /* Default Gateway IP address */
    { 0, 0, 0, 0 },
    /* Subnet IP Mask */
    { _MSK1P, _MSK2P, _MSK3P, _MSK4P },
    /* Primary DNS Server IP */
    { _pDNS1P, _pDNS2P, _pDNS3P, _pDNS4P },
    /* Secondary DNS Server IP */
    { _sDNS1P, _sDNS2P, _sDNS3P, _sDNS4P }
  },
#else
   { { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 } },
#endif
  /* SLIP: Local IP address */
#if SLIP_ENABLE == 1
  { { _IP1S, _IP2S, _IP3S, _IP4S },
    /* Default Gateway IP address */
    { 0, 0, 0, 0 },
    /* Subnet IP Mask */
    { _MSK1S, _MSK2S, _MSK3S, _MSK4S },
    /* Primary DNS Server IP */
    { _pDNS1S, _pDNS2S, _pDNS3S, _pDNS4S },
    /* Secondary DNS Server IP */
    { _sDNS1S, _sDNS2S, _sDNS3S, _sDNS4S }
  }
#else
   { { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 } },
#endif
};
U8 const netif_cfg = (PPP_GETIP<<5 | SLIP_DEFGW<<4 | PPP_DEFGW<<3 |
                      SLIP_ENABLE<<2 | PPP_ENABLE<<1 | ETH_ENABLE);

/* Memory pool for Eternet Frames. */
U32 Mem_Pool[MEM_SIZE];
/* ARP cache table */
#if ETH_ENABLE == 1
  struct arp_info arp_table[ARP_TABSIZE];
#endif
#if (IGMP_ENABLE & ETH_ENABLE) == 1
  struct igmp_info igmp_table[IGMP_TABSIZE];
#endif
/* UDP socket Control block table */
#if UDP_ENABLE == 1
  struct udp_info udp_socket[UDP_NUMSOCKS];
#endif
/* TCP socket Control block table */
#if TCP_ENABLE == 1
  struct tcp_info tcp_socket[TCP_NUMSOCKS+1];
#endif
/* HTTP Server Session Control block table */
#if HTTP_ENABLE == 1
  struct http_info http_session[HTTP_NUMSESS];
#endif
/* Telnet Server Session Control block table */
#if TNET_ENABLE == 1
  struct tnet_info tnet_session[TNET_NUMSESS];
#endif
/* TFTP Server Session Control block table */
#if TFTP_ENABLE == 1
  struct tftp_info tftp_session[TFTP_NUMSESS];
#endif
/* DNS cache table */
#if DNS_ENABLE == 1
  struct dns_info dns_table[DNS_TABSIZE];
#endif

/* Exported defines to other modules */
U32 const MPool_Size     = sizeof (Mem_Pool);
U16 const tick_rate      = TICK_RATE;
U8        lhost_name[16] = LHOST_NAME;
#if ETH_ENABLE == 1
  U16 const arp_TabSize  = ARP_TABSIZE;
  U16 const arp_TimeOut  = ARP_TIMEOUT;
  U8  const arp_MaxRetry = ARP_MAXRETRY;
  U8  const arp_Resend   = ARP_RESEND;
#endif
#if PPP_ENABLE == 1
  U32 const ppp_ACCMap    = _ACCM1<<24 | _ACCM2<<16 | _ACCM3<<8 | _ACCM4;
  U16 const ppp_SerTout   = 5 * TICK_RATE;
  U16 const lcp_RetryTout = LCP_RETRYTOUT * TICK_RATE;
  U16 const pap_RetryTout = PAP_RETRYTOUT * TICK_RATE;
  U16 const ipcp_RetryTout= IPCP_RETRYTOUT * TICK_RATE;
  U8  const lcp_MaxRetry  = LCP_MAXRETRY;
  U8  const pap_MaxRetry  = PAP_MAXRETRY;
  U8  const ipcp_MaxRetry = IPCP_MAXRETRY;
#endif
#if (IGMP_ENABLE & ETH_ENABLE) == 1
  U16 const igmp_TabSize  = IGMP_TABSIZE;
  U8  const igmp_T200ms   = TICK_RATE / 5;
  U8  const IP_AllHosts[IP_ADRLEN] = {224, 0, 0, 1 };
#endif
#if SLIP_ENABLE == 1
  U16 const slip_SerTout  = 5 * TICK_RATE;
#endif
#if UDP_ENABLE == 1
  U8  const udp_NumSocks  = UDP_NUMSOCKS;
  U16 const udp_LastPort  = UDP_LASTPORT;
#endif
#if TCP_ENABLE == 1
  U8  const tcp_NumSocks  = TCP_NUMSOCKS;
  U8  const tcp_MaxRetry  = TCP_MAXRETRY;
  U16 const tcp_LastPort  = TCP_LASTPORT;
  U16 const tcp_RetryTout = TCP_RETRYTOUT * TICK_RATE;
  U16 const tcp_2MSSTout  = (200 + TICK_INTERVAL - 1) / TICK_INTERVAL;
  U16 const tcp_SynRetryTout = TCP_SYN_RETRY_TOUT * TICK_RATE;
  U16 const tcp_InitRetryTout= TCP_INIT_RETRY_TOUT * TICK_RATE;
  U16 const tcp_DefTout   = TCP_DEFTOUT;
  U8  const tcp_ConRetry  = TCP_CONRETRY;
#endif
#if HTTP_ENABLE == 1
  U8  const http_NumSess  = HTTP_NUMSESS;
  U8  const http_EnAuth   = HTTP_ENAUTH;
  U8  const http_auth_dev[]     = HTTP_AUTHREALM;
  U8  const http_auth_user[]    = HTTP_AUTHUSER;
  U8        http_auth_passw[20] = HTTP_AUTHPASSW;
#endif
#if TNET_ENABLE == 1
  U8  const tnet_NumSess  = TNET_NUMSESS;
  U8  const tnet_EnAuth   = TNET_ENAUTH;
  U8  const tnet_auth_user[]    = TNET_AUTHUSER;
  U8        tnet_auth_passw[20] = TNET_AUTHPASSW;
#endif
#if TFTP_ENABLE == 1
  U8  const tftp_NumSess  = TFTP_NUMSESS;
  U8  const tftp_MaxRetry = TFTP_MAXRETRY;
  U16 const tftp_DefTout  = TFTP_DEFTOUT;
#endif
#if DNS_ENABLE == 1
  U16 const dns_TabSize   = DNS_TABSIZE;
#endif
#if SMTP_ENABLE == 1
  U16 const smtp_DefTout  = SMTP_DEFTOUT;
#endif

/*----------------------------------------------------------------------------
 *      Fatal Error Handler
 *---------------------------------------------------------------------------*/

void sys_error (ERROR_CODE code) {
   /* This function is called when a fatal error is encountered. The normal */
   /* program execution is not possible anymore. Add your crytical error   .*/
   /* handler code here.                                                    */

   switch (code) {
      case ERR_MEM_ALLOC:
         /* Out of memory. */
         break;

      case ERR_MEM_FREE:
         /* Trying to release non existing memory block. */
         break;

      case ERR_MEM_CORRUPT:
         /* Memory Link pointer is Corrupted. */
         /* More data written than the size of allocated mem block. */
         break;

      case ERR_UDP_ALLOC:
         /* Out of UDP Sockets. */
         break;

      case ERR_TCP_ALLOC:
         /* Out of TCP Sockets. */
         break;

      case ERR_TCP_STATE:
         /* TCP State machine in undefined state. */
         break;
   }

   /* End-less loop */
   while (1);
}


/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/


/*--------------------------- init_system -----------------------------------*/

void init_system (void) {
   /* Initialize configured interfaces and applications. */

#if ETH_ENABLE == 1
   init_eth_link ();
#endif
#if PPP_ENABLE == 1
   init_ppp_link ();
#endif
#if SLIP_ENABLE == 1
   init_slip_link ();
#endif

#if (IGMP_ENABLE & ETH_ENABLE) == 1
   init_igmp ();
#endif
#if UDP_ENABLE == 1
   init_udp ();
#endif
#if TCP_ENABLE == 1
   init_tcp ();
#endif
#if HTTP_ENABLE == 1
   init_http ();
#endif
#if TNET_ENABLE == 1
   init_tnet ();
#endif
#if TFTP_ENABLE == 1
   init_tftp ();
#endif
#if (NBNS_ENABLE & ETH_ENABLE) == 1
   init_nbns ();
#endif
#if (DHCP_ENABLE & ETH_ENABLE) == 1
   init_dhcp ();
#endif
#if DNS_ENABLE == 1
   init_dns ();
#endif
#if SMTP_ENABLE == 1
   init_smtp ();
#endif
}


/*--------------------------- run_system ------------------------------------*/

void run_system (void) {
   /* Run configured interfaces and applications. */

#if ETH_ENABLE == 1
   run_eth_link ();
#endif
#if PPP_ENABLE == 1
   run_ppp_link ();
#endif
#if SLIP_ENABLE == 1
   run_slip_link ();
#endif

#if (IGMP_ENABLE & ETH_ENABLE) == 1
   run_igmp_host ();
#endif
#if TCP_ENABLE == 1
   tcp_poll_sockets ();
#endif
#if HTTP_ENABLE == 1
   run_http_server ();
#endif
#if TNET_ENABLE == 1
   run_tnet_server ();
#endif
#if TFTP_ENABLE == 1
   run_tftp_server ();
#endif
#if (DHCP_ENABLE & ETH_ENABLE) == 1
   run_dhcp_client ();
#endif
#if DNS_ENABLE == 1
   run_dns_client ();
#endif
#if SMTP_ENABLE == 1
   run_smtp_client ();
#endif
}


/*--------------------------- process_hl_igmp -------------------------------*/

#if (IGMP_ENABLE & ETH_ENABLE) == 1
void process_hl_igmp (OS_FRAME *frame) {
   /* Process IGMP protocol frame */
   process_igmp (frame);
}
#endif


/*--------------------------- process_hl_udp --------------------------------*/

#if UDP_ENABLE == 1
void process_hl_udp (OS_FRAME *frame) {
   /* Process udp protocol frame */
   process_udp (frame);
}
#endif


/*--------------------------- process_hl_tcp --------------------------------*/

#if TCP_ENABLE == 1
void process_hl_tcp (OS_FRAME *frame) {
   /* Process tcp protocol frame */
   process_tcp (frame);
}
#endif


/*--------------------------- dispatch_frame --------------------------------*/

BOOL dispatch_frame (OS_FRAME *frame, U8 netif) {
   /* Dispatch the outgoing frame to selected network interface. */

switch (netif) {
#if ETH_ENABLE == 1
      case NETIF_ETH:
         return (eth_send_frame (frame));
#endif
#if PPP_ENABLE == 1
      case NETIF_PPP:
         return (ppp_send_frame (frame, PPP_PROT_IP));
#endif
#if SLIP_ENABLE == 1
      case NETIF_SLIP:
         return (slip_send_frame (frame));
#endif
   }
   return (__FALSE);
}


/*--------------------------- eth_chk_adr -----------------------------------*/

#if ETH_ENABLE == 0
BOOL eth_chk_adr (OS_FRAME *frame) {
   /* Provide an empty function when the 'Ethernet Interface' is disabled. */
   frame = frame;
   return (__FALSE);
}
#endif


/*--------------------------- eth_get_adr -----------------------------------*/

#if ETH_ENABLE == 0
U8 *eth_get_adr (U8 *ipadr) {
   /* Provide an empty function when the 'Ethernet Interface' is disabled. */
   ipadr = ipadr;
   return (NULL);
}
#endif


/*--------------------------- igmp_is_member --------------------------------*/

#if (IGMP_ENABLE & ETH_ENABLE) == 0
BOOL igmp_is_member (U8 *ipadr) {
   /* Provide an empty function when IP Multicasting is not enabled. */
   ipadr = ipadr;
   return (__FALSE);
}
#endif


#endif // SocketSupport

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
