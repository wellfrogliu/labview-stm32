/******************************************************************************/
/* RL-ARM_TCPWrapper.c: LabVIEW wrapper for RL-ARM TCPNet                     */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2007 Keil Software. All rights reserved.                     */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#ifndef _TCPWRAPPER_RTX_H_
#define _TCPWRAPPER_RTX_H_

int  RTX_tcp_init (void);
U8   RTX_tcp_open_socket (U8 type, U8 tos, U16 tout, U16 bufSize);
BOOL RTX_tcp_close_socket (U8 socket);
BOOL RTX_tcp_listen (U8 socket, U16 locport);
BOOL RTX_tcp_connect (U8 socket, U8 *remip, U16 remport, U16 locport);
BOOL RTX_tcp_abort (U8 socket);
U8   RTX_tcp_get_state (U8 socket);
BOOL RTX_tcp_send (U8 socket, U8 *buf, U16 *dlen, int timeout);
BOOL RTX_tcp_recv (U8 socket, U8 *buf, U16 *dlen);
U16  RTX_tcp_rx_bytes (U8 socket);
U16  RTX_tcp_rx_crlf (U8 socket, U16 maxLen);

U8   RTX_udp_open_socket (U8 tos, U8 opt, U16 bufSize);
BOOL RTX_udp_close_socket (U8 socket);
BOOL RTX_udp_listen (U8 socket, U16 locport);
BOOL RTX_udp_send (U8 socket, U8* remip, U16 remport, U8 *buf, U16 *dlen);
BOOL RTX_udp_recv (U8 socket, U8 *buf, U16 *dlen, U8 *remip, U16 *remport);
U16  RTX_udp_rx_bytes (U8 socket);

BOOL RTX_inet_get_addr (U8 socket, U8 *pLocIP, U16 *pLocPort, U8 *pRemIP, U16 *pRemPort);
BOOL RTX_inet_get_ipaddr (U8 *ipAddr);
BOOL RTX_inet_get_hostname (U8 *hostname, U32 nameLen);
void RTX_inet_ntoa(const U8 *pIpAddr, char *pIpStr);
void RTX_inet_aton(const char *pIpStr, U8 *pIpAddr);
BOOL RTX_get_host_by_name (const char *pIpStr, U8 *pIpAddr);


#endif /* #ifndef _TCPWRAPPER_RTX_H_ */

