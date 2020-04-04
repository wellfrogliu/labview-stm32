/*----------------------------------------------------------------------------
 *      R T L  -  T C P N E T
 *----------------------------------------------------------------------------
 *      Name:    NET_CONFIG.H
 *      Purpose: Common TcpNet Definitions
 *      Rev.:    V3.20
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2008 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#ifndef __NET_CONFIG_H__
#define __NET_CONFIG_H__

#include "RTL.h"

/*----------------------------------------------------------------------------
 *      General Data Types
 *---------------------------------------------------------------------------*/

#ifndef NULL
 #ifdef __cplusplus              // EC++
   #define NULL   0
 #else
   #define NULL ((void *) 0)
 #endif
#endif

#ifndef __RTL_EXT__
 #define __RTL_EXT__
 //typedef char             S8;	 /*defined elsewhere as of MDK 3.5*/
 typedef unsigned char      U8;
 typedef short              S16;
 typedef unsigned short     U16;
 typedef int                S32;
 typedef unsigned int       U32;
 typedef long long          S64;
 typedef unsigned long long U64;
 typedef unsigned char      BIT;
 typedef unsigned int       BOOL;
 #ifndef __TRUE
  #define __TRUE        1
 #endif
 #ifndef __FALSE
  #define __FALSE       0
 #endif
#endif

#define SWAPB(w)        ((w>>8)|(w<<8)&0xFFFF)/* Swap-exchange Hi<->Lo Bytes */
#define HIB(w)          (U8)(w>>8)          /* Extract Hi byte of Word       */
#define LOB(w)          (U8)(w)             /* Extract Lo byte of Word       */

#define OS_HEADER_LEN   4         /* TcpNet 'os_frame' header size           */

/* Network Interfaces */
#define NETIF_ETH       0         /* Network interface: Ethernet             */
#define NETIF_PPP       1         /* Network interface: PPP                  */
#define NETIF_SLIP      2         /* Network interface: Slip                 */

/* Ethernet Defines */
#define ETH_ADRLEN      6         /* Ethernet Address Length in bytes        */
#define IP_ADRLEN       4         /* IP Address Length in bytes              */
                                  /* Frame Header length common for all      */
#define PHY_HEADER_LEN  (2*ETH_ADRLEN + 2) /* network interfaces.            */

#define ARP_HEADER_LEN  (2*(ETH_ADRLEN + IP_ADRLEN) + 8)
#define ETH_MTU         1514      /* Ethernet Frame Max Transfer Unit        */
#define HW_TYPE_ETH     0x0001    /* Hardware type: Ethernet                 */
#define ETH_PROT_IP     0x0800    /* Ethernet Protocol type: IP              */
#define ETH_PROT_ARP    0x0806    /* Ethernet Protocol type: ARP, RARP       */

/* PPP Defines */
#define PPP_ESC         0x7D      /* PPP ESC character                       */
#define PPP_FLAG        0x7E      /* PPP Start/Stop framing flag             */

#define PPP_MTU         1500      /* PPP Frame Max Transfer Unit             */
#define PPP_FRMOFS      (2*ETH_ADRLEN - 2)
#define PPP_HEADER_LEN  4         /* PPP Header Length                       */
#define PPP_PROT_LCP    0xC021    /* PPP Protocol type: LCP                  */
#define PPP_PROT_PAP    0xC023    /* PPP Protocol type: PAP                  */
#define PPP_PROT_CHAP   0xC223    /* PPP Protocol type: CHAP                 */
#define PPP_PROT_IPCP   0x8021    /* PPP Protocol type: IPCP                 */
#define PPP_PROT_IP     0x0021    /* PPP Protocol type: IP                   */

/* PPP-LCP Defines */
#define LCP_HEADER_LEN  4         /* LCP Header length                       */

#define LCP_CONF_RQ     1         /* LCP Configure-Request                   */
#define LCP_CONF_ACK    2         /* LCP Configure-Ack                       */
#define LCP_CONF_NAK    3         /* LCP Configure-Nak                       */
#define LCP_CONF_REJ    4         /* LCP Configure-Reject                    */
#define LCP_TERM_RQ     5         /* LCP Terminate-Request                   */
#define LCP_TERM_ACK    6         /* LCP Terminate-Ack                       */
#define LCP_CODE_REJ    7         /* LCP Code-Reject                         */
#define LCP_PROT_REJ    8         /* LCP Protocol-Reject                     */
#define LCP_ECHO_RQ     9         /* LCP Echo-Request                        */
#define LCP_ECHO_REP    10        /* LCP Echo-Reply                          */
#define LCP_DISC_RQ     11        /* LCP Discard-Request                     */

/* PPP-LCP Options */
#define LCP_OPT_MRU     1         /* LCP Opt: Maximum Receive Unit           */
#define LCP_OPT_ACCMAP  2         /* LCP Opt: Async Control Character Map    */
#define LCP_OPT_AUTHP   3         /* LCP Opt: Authentication Protocol        */
#define LCP_OPT_QUALP   4         /* LCP Opt: Quality Protocol               */
#define LCP_OPT_MAGIC   5         /* LCP Opt: Magic Number                   */
#define LCP_OPT_PROTC   7         /* LCP Opt: Protocol Field Compression     */
#define LCP_OPT_ADDRC   8         /* LCP Opt: Addr and Ctrl Field Comression */

/* PPP State Flags */
#define PPP_SYNC        0x01      /* Synchronized, HDLC open flag received   */
#define PPP_IS_ESC      0x02      /* Previous char received was ESC char     */
#define PPP_MALLOC      0x04      /* PPP memory buffer allocated             */
#define PPP_IS_FRAME    0x08      /* PPP valid frame received                */
#define PPP_TX_ESC      0x10      /* PPP transmit ESC character              */
#define PPP_PAP_AUTH    0x20      /* PPP PAP authent. start for client mode  */
#define PPP_LCP_CONF    0x40      /* PPP LCP config. start for client mode   */
#define PPP_IPCP_CONF   0x80      /* PPP IPCP config. start for client mode  */

/* PPP-PAP Defines */
#define PAP_HEADER_LEN  4         /* PAP Header length                       */

#define PAP_AUTH_RQ     1         /* PAP Authenticate request                */
#define PAP_AUTH_ACK    2         /* PAP Authenticate acknowledge            */
#define PAP_AUTH_NAK    3         /* PAP Authenticate Not acknowledge        */

/* PPP-IPCP Defines */
#define IPCP_HEADER_LEN 4         /* IPCP Header length                      */

#define IPCP_CONF_RQ    1         /* IPCP Configure-Request                  */
#define IPCP_CONF_ACK   2         /* IPCP Configure-Ack                      */
#define IPCP_CONF_NAK   3         /* IPCP Configure-Nak                      */
#define IPCP_CONF_REJ   4         /* IPCP Configure-Reject                   */
#define IPCP_TERM_RQ    5         /* IPCP Terminate-Request                  */
#define IPCP_TERM_ACK   6         /* IPCP Terminate-Ack                      */
#define IPCP_CODE_REJ   7         /* IPCP Code-Reject                        */

/* PPP-IPCP Options */
#define IPCP_OPT_IPADDRS 1        /* IPCP Opt: IP Addressess                 */
#define IPCP_OPT_IPCOMPR 2        /* IPCP Opt: IP Compression Protocol       */
#define IPCP_OPT_IPADDR  3        /* IPCP Opt: IP Address                    */

/* SLIP Defines */
#define SLIP_END        0xC0      /* SLIP end of packet                      */
#define SLIP_ESC        0xDB      /* SLIP escape characters (bit stuffing)   */
#define SLIP_ESC_END    0xDC      /* SLIP end data byte                      */
#define SLIP_ESC_ESC    0xDD      /* SLIP esc data byte                      */

#define SLIP_MTU        1500      /* SLIP Frame Max Transfer Unit            */
#define SLIP_FRMOFS     PHY_HEADER_LEN  /* SLIP frame offset                 */

/* SLIP State Flags */
#define SLIP_SYNC       0x01      /* Synchronized, Start of Packet received  */
#define SLIP_IS_ESC     0x02      /* Previous char received was ESC char     */
#define SLIP_MALLOC     0x04      /* PPP memory buffer allocated             */
#define SLIP_IS_FRAME   0x08      /* PPP valid frame received                */
#define SLIP_TX_ESC     0x10      /* PPP transmit ESC character              */

/* ARP, RARP Opcode for 'Operation Field' value */
#define ARP_RQ          0x0001    /* ARP Request opcode                      */
#define ARP_REP         0x0002    /* ARP Reply 'opcode                       */
#define RARP_RQ         0x0003    /* RARP Request opcode                     */
#define RARP_REP        0x0004    /* RARP Reply opcode                       */

/* ARP Info States  */
#define ARP_STATE_FREE     0      /* Unused entry (initial value)            */
#define ARP_STATE_RESERVED 1      /* Reserved entry (allocated)              */
#define ARP_STATE_PENDING  2      /* Entry is used but incomplete            */
#define ARP_STATE_RESOLVED 3      /* Resolved entry                          */
#define ARP_STATE_REFRESH  4      /* Entry is being refreshed                */

/* ARP Info Types   */
#define ARP_FIXED_IP    0         /* Fixed IP adrs is refreshed after tout   */
#define ARP_TEMP_IP     1         /* Temp adrs is removed after timeout      */

/* IP Defines */
#define IP_HEADER_LEN   20        /* IP Header Length in bytes               */
#define IP_OPT_LEN      40        /* Max IP Header option field length       */
#define IP_MAX_HLEN     (IP_HEADER_LEN + IP_OPT_LEN)
#define IP_DATA_OFS     (PHY_HEADER_LEN + IP_HEADER_LEN)

#define IP_DEF_VERHLEN  0x45      /* Default IP Version and Header Length    */
#define IP_NO_FRAGMENT  0x4000    /* Don't Fragment Flag                     */
#define IP_FRAGOFFMASK  0x1FFF    /* Fragment offset mask                    */
#define IP_MOREFRAGS    0x2000    /* More fragments Flag                     */

/* Protocol over IP */
#define IP_ICMP         0x01      /* ICMP over IP                            */
#define IP_IGMP         0x02      /* IGMP over IP                            */
#define IP_UDP          0x11      /* UDP over IP                             */
#define IP_TCP          0x06      /* TCP over IP                             */

/* ICMP Defines */
#define ICMP_HEADER_LEN 4         /* ICMP Header Length in bytes             */

/* ICMP message types */
#define ICMP_ECHO_REP   0         /* ICMP Echo Reply                         */
#define ICMP_ECHO_RQ    8         /* ICMP Echo Request                       */

/* IGMP Defines */
#define IGMP_HEADER_LEN 8         /* IGMP Header Length in bytes             */

/* IGMP Info States */
#define IGMP_STATE_NON   0        /* Non-Member - free Entry                 */
#define IGMP_STATE_DELAY 1        /* Delaying Member wait for Querry/Report  */
#define IGMP_STATE_IDLE  2        /* Idle Member of a group                  */

/* IGMP message types */
#define IGMP_MEM_QUERRY 0x11      /* IGMP Host Membership Querry (IGMPv1)    */
#define IGMP_MEM_REPORT 0x12      /* IGMP Host Membership Report (IGMPv1)    */

/* UDP Defines */
#define UDP_HEADER_LEN  8         /* UDP Header Length in bytes              */
#define UDP_DATA_OFS    (PHY_HEADER_LEN + IP_HEADER_LEN + UDP_HEADER_LEN)
#define UDP_MTU         (ETH_MTU - PHY_HEADER_LEN - IP_HEADER_LEN)

/* UDP Options */
#define UDP_OPT_SEND_CS 0x01      /* Calculate Checksum for UDP send frames  */
#define UDP_OPT_CHK_CS  0x02      /* Verify Checksum for received UDP frames */

/* UDP States */
#define UDP_STATE_FREE     0      /* Entry is free and unused                */
#define UDP_STATE_CLOSED   1      /* Entry allocated, socket still closed    */
#define UDP_STATE_OPENED   2      /* Socket opened for communication         */

/* TCP Defines */
#define TCP_HEADER_LEN  20        /* TCP Header Length in bytes              */
#define TCP_OPT_LEN     40        /* Max TCP Header option field length      */
#define TCP_MAX_HLEN    (TCP_HEADER_LEN + TCP_OPT_LEN)
#define TCP_DATA_OFS    (PHY_HEADER_LEN + IP_HEADER_LEN + TCP_HEADER_LEN)
#define TCP_TOS_NORMAL  0         /* TCP Type of Service                     */
#define TCP_MTU         1460      /* TCP default max. transfer unit          */
#define TCP_DEF_MSS     1460      /* TCP default max. segment size option    */
#define TCP_DEF_WINSIZE 5840      /* TCP default window size                 */

/* TCP Flags */
#define TCP_FLAG_URG    0x20      /* Urgent Pointer Field significant        */
#define TCP_FLAG_ACK    0x10      /* Acknowledgment field significant        */
#define TCP_FLAG_PSH    0x08      /* Push Function                           */
#define TCP_FLAG_RST    0x04      /* Reset the connection                    */
#define TCP_FLAG_SYN    0x02      /* Synchronize sequence numbers            */
#define TCP_FLAG_FIN    0x01      /* Finished, No more data from sender      */
#define TCP_FLAG_MASK   0x3F      /* Mask of all TCP Flags                   */

/* TCP Internal Flags */
#define TCP_IFLAG_CLOSING  0x01   /* Socket Close Pending                    */
#define TCP_IFLAG_SENDOPT  0x02   /* Send Max. Segment Size option           */
#define TCP_IFLAG_SPLIT    0x04   /* Split Frame Sent                        */

/* TCP States */
#define TCP_STATE_FREE     0      /* Entry is free and unused                */
#define TCP_STATE_CLOSED   1      /* Entry allocated, socket still closed    */
#define TCP_STATE_LISTEN   2      /* Socket waiting for incoming connection  */
#define TCP_STATE_SYN_REC  3      /* SYN frame received                      */
#define TCP_STATE_SYN_SENT 4      /* SYN packet sent to establish a connect. */
#define TCP_STATE_FINW1    5      /* Tcp_close started FIN packet was sent   */
#define TCP_STATE_FINW2    6      /* Our FIN ack-ed, waiting for remote FIN  */
#define TCP_STATE_CLOSING  7      /* Received FIN independently of our FIN   */
#define TCP_STATE_LAST_ACK 8      /* Waiting for last ACK for our FIN        */
#define TCP_STATE_TWAIT    9      /* Timed waiting for 2MSL                  */
#define TCP_STATE_CONNECT  10     /* TCP Connection established              */

/* TCP Socket Types */
#define TCP_TYPE_NONE      0x00   /* Socket type not assigned                */
#define TCP_TYPE_SERVER    0x01   /* Socket Type Server (open for listening) */
#define TCP_TYPE_CLIENT    0x02   /* Socket Type Client (initiate connect)   */
#define TCP_TYPE_DELAY_ACK 0x04   /* Socket Type Delayed Acknowledge         */
#define TCP_TYPE_CLIENT_SERVER (TCP_TYPE_SERVER | TCP_TYPE_CLIENT)
#define TCP_TYPE_MASK      0x07   /* Mask for checking the Tcp Type          */

/* TCP Callback Events */
#define TCP_EVT_CONREQ     0      /* Connect request received event          */
#define TCP_EVT_CONNECT    1      /* Connection established event            */
#define TCP_EVT_CLOSE      2      /* Connection was properly closed          */
#define TCP_EVT_ABORT      3      /* Connection is for some reason aborted   */
#define TCP_EVT_ACK        4      /* Previously send data acknowledged       */
#define TCP_EVT_DATA       5      /* Data received event                     */

/* HTTP Defines */
#define HTTP_SERVER_PORT   80     /* HTTP Server standard port               */
#define HTTP_CGI_MAXLINE   120    /* HTTP CGI Script max. size of one line   */

/* HTTP States */
#define HTTP_STATE_IDLE     0     /* Session is free and unused              */
#define HTTP_STATE_ERROR    1     /* Error, session not initialized          */
#define HTTP_STATE_RESERVED 2     /* Reserved, session not activated         */
#define HTTP_STATE_ACTIVE   3     /* Active, waiting for GET or POST         */
#define HTTP_STATE_FILE     4     /* Sending file                            */
#define HTTP_STATE_CGI      5     /* Processiong CGI script file             */
#define HTTP_STATE_TEXT     6     /* Sending CGI generated data              */
#define HTTP_STATE_UNAUTH   7     /* Unauthorized access to Web Server       */

/* HTTP File Types */
#define HTTP_TYPE_HTML      1     /* File type: text/html                    */
#define HTTP_TYPE_GIF       2     /* File type: gif                          */
#define HTTP_TYPE_JPEG      3     /* File type: jpeg                         */
#define HTTP_TYPE_BMP       4     /* File type: bmp                          */
#define HTTP_TYPE_PNG       5     /* File type: png                          */
#define HTTP_TYPE_JAVA      6     /* File type: java script                  */
#define HTTP_TYPE_CGI       7     /* File type: cgi (html with tags)         */
#define HTTP_TYPE_CSS       8     /* File type: css (cascading stytle sheet) */

/* HTTP Internal Flags */
#define HTTP_FLAG_CGI      0x01   /* Processing CGI Script file              */
#define HTTP_FLAG_HEADER   0x02   /* HTTP Header has been sent               */
#define HTTP_FLAG_NOFILE   0x04   /* Requested file not found.               */
#define HTTP_FLAG_CGIREP   0x08   /* Repeat again CGI Script line            */
#define HTTP_FLAG_POSTWAIT 0x10   /* Waiting for POST data for Netscape      */
#define HTTP_FLAG_NEXT     0x20   /* Next CGI line already fetched           */
#define HTTP_FLAG_DFILE    0x40   /* External Data File opened               */
#define HTTP_FLAG_SFILE    0x80   /* External Script File opened             */

/* Telnet Defines */
#define TNET_SERVER_PORT   23     /* Telnet Server standard port             */
#define TNET_CMD_SE        240    /* End of Subnegotiation parameters        */
#define TNET_CMD_NOP       241    /* No Operation                            */
#define TNET_CMD_DMARK     242    /* Data Mark                               */
#define TNET_CMD_BREAK     243    /* NVT Character Break                     */
#define TNET_CMD_IP        244    /* Interrupt Process                       */
#define TNET_CMD_AO        245    /* Abort Output                            */
#define TNET_CMD_AYT       246    /* Are You There                           */
#define TNET_CMD_EC        247    /* Erase Character                         */
#define TNET_CMD_EL        248    /* Erase Line                              */
#define TNET_CMD_GA        249    /* Go Ahead                                */
#define TNET_CMD_SB        250    /* Subnegotiation Begin                    */
#define TNET_CMD_WILL      251    /* Telnet Command WILL                     */
#define TNET_CMD_WONT      252    /* Telnet Command WON'T                    */
#define TNET_CMD_DO        253    /* Telnet Command DO                       */
#define TNET_CMD_DONT      254    /* Telnet Command DON'T                    */
#define TNET_CMD_IAC       255    /* Telnet Command IAC                      */
#define TNET_OPT_ECHO      1      /* Telnet Option Echo                      */
#define TNET_OPT_SUPGOAH   3      /* Telnet Option Supress Go Ahead          */
#define TNET_OPT_TERMTYPE  24     /* Telnet Option Terminal Type             */
#define TNET_OPT_WINSIZE   31     /* Terminal Option window size             */

/* Telnet ASCII Key Codes */
#define ASCII_ESC          27     /* ESC ascii code                          */
#define ASCII_CTRLC        3      /* ^C ascii code                           */
#define ASCII_BS           8      /* Back Space                              */
#define ASCII_DEL          127    /* Delete Key                              */
#define ASCII_CR           13     /* Carriage Return, Enter key              */
#define ASCII_LF           10     /* Line Feed, New Line                     */

/* Telnet States */
#define TNET_STATE_IDLE      0    /* Session is free and unused              */
#define TNET_STATE_ERROR     1    /* Session Error, not initialized          */
#define TNET_STATE_RESERVED  2    /* Session reserved, not activated         */
#define TNET_STATE_ACTIVE    3    /* Session active                          */
#define TNET_STATE_NEGOTIATE 4    /* Connected, negotiate parameters         */
#define TNET_STATE_CONNECT   5    /* Connected, Send a header reply          */
#define TNET_STATE_AUTHUSER  6    /* User Authorization state                */
#define TNET_STATE_SENDING   7    /* Sending data in repeated loop           */

/* Telnet Flags */
#define TNET_FLAG_FIRST   0x01    /* First Telnet Header                     */
#define TNET_FLAG_SEND    0x02    /* Send Data, wait for acknowledge         */
#define TNET_FLAG_CLOSE   0x04    /* Telnet Session Close request            */
#define TNET_FLAG_PASSW   0x08    /* Password is not echoed back             */
#define TNET_FLAG_USEROK  0x10    /* Username valid                          */
#define TNET_FLAG_DELAY   0x20    /* Delay function is active                */

/* TFTP Defines */
#define TFTP_SERVER_PORT  69      /* TFTP Server standard port               */

/* TFTP States */
#define TFTP_STATE_IDLE   0       /* Session is idle                         */
#define TFTP_STATE_ACTIVE 1       /* Session is active                       */

/* TFTP Opcodes */
#define TFTP_OPCODE_RRQ   1       /* Packet is Read Request                  */
#define TFTP_OPCODE_WRQ   2       /* Packet is Write Request                 */
#define TFTP_OPCODE_DATA  3       /* Data Packet                             */
#define TFTP_OPCODE_ACK   4       /* ACK Packet                              */
#define TFTP_OPCODE_ERR   5       /* Error Packet                            */

/* TFTP Error codes */
#define TFTP_NOT_DEFINED       0  /* Not Definet Error                       */
#define TFTP_FILE_NOT_FOUND    1  /* File Not Found Error                    */
#define TFTP_ACCESS_VIOLATION  2  /* Access Violation Error                  */
#define TFTP_DISK_FULL         3  /* Disk Full Error                         */
#define TFTP_ILLEGAL_OPERATION 4  /* Not supported Opcode                    */
#define TFTP_FILE_EXIST        6  /* File Already Exists Error               */

/* TFTP Flags */
#define TFTP_FLAG_LASTB   0x01    /* Last block sent                         */

/* SMTP States */
#define SMTP_STATE_IDLE      0    /* SMTP Client is idle                     */
#define SMTP_STATE_ERROR     1    /* Error, not initialized                  */
#define SMTP_STATE_OPEN_REQ  2    /* User has requested sending email        */
#define SMTP_STATE_OPEN_SENT 3    /* TCP connection request sent             */
#define SMTP_STATE_OPENED    4    /* SMTP TCP Socket opened                  */
#define SMTP_STATE_SREADY    5    /* SMTP server Ready (has indicated 220)   */
#define SMTP_STATE_HELO_SENT 6    /* HELO packet sent to Servert by us       */
#define SMTP_STATE_HELO_ACK  7    /* Server has acked HELO by 250            */
#define SMTP_STATE_FROM_SENT 8    /* MAIL FROM sent to Server                */
#define SMTP_STATE_FROM_ACK  9    /* Server has acked MAIL FROM by 250       */
#define SMTP_STATE_TO_SENT   10   /* RCPT TO sent to Server                  */
#define SMTP_STATE_TO_ACK    11   /* Server has acked RCPT TO by 250         */
#define SMTP_STATE_DREQ_SENT 12   /* DATA sent to Server                     */
#define SMTP_STATE_DREQ_ACK  13   /* Server has acked DATA by 354            */
#define SMTP_STATE_SENDING   14   /* We are sending email data               */
#define SMTP_STATE_END_SENT  15   /* End sequence sent to Server             */
#define SMTP_STATE_END_ACK   16   /* Server has acked CRLF.CRLF sequence     */
#define SMTP_STATE_QUIT_SENT 17   /* QUIT sent to Server                     */
#define SMTP_STATE_QUIT_ACK  18   /* Server has acked quit by 221            */
#define SMTP_STATE_ERR_REC   19   /* Server responed with error              */

/* SMTP Callback Events */
#define SMTP_EVT_SUCCESS   0      /* Email successfully sent                 */
#define SMTP_EVT_TIMEOUT   1      /* Timeout sending email                   */
#define SMTP_EVT_ERROR     2      /* Error when sending email                */

/* SMTP server reply codes */
#define SMTP_CMD_SREADY    220    /* Server is ready                         */
#define SMTP_CMD_OK        250    /* Requested mail action ok, ready         */
#define SMTP_CMD_DATAOK    354    /* Start mail input; end with <CRLF>.<CRLF>*/
#define SMTP_CMD_QUITOK    221    /* Service closing transmission Channel    */

/* NBNS Defines */
#define NBNS_SERVER_PORT   137    /* NBNS Server UDP port                    */
#define NBNS_CLIENT_PORT   137    /* NBNS Client UDP port                    */
#define NBNS_FLAGS_NQUERRY 0x0110 /* Name Querry Flags                       */
#define NBNS_FLAGS_NQURESP 0x8500 /* Name Querry Response No Errors          */

/* BOOTP Defines */
#define BOOTP_SERVER_PORT  67     /* BOOTP Server standard port              */
#define BOOTP_CLIENT_PORT  68     /* BOOTP Client standard port              */
#define BOOTP_REQ          1      /* BOOTP Request Opcode                    */
#define BOOTP_REPLY        2      /* BOOTP Reply Opcode                      */
#define BOOTP_FLG_UNICAST  0x0000 /* Flags - unicast message                 */
#define BOOTP_FLG_BROADC   0x8000 /* Flags - broadcast message               */
#define BOOTP_TRANS_ID     0xE20317E2 /* Transaction ID                      */
#define BOOTP_MAGIC_COOKIE 0x63825363 /* Magic Cookie                        */

/* BOOTP Options */
#define BOOTP_OPT_NETMASK  1      /* Subnet mask option BOOTP client         */
#define BOOTP_OPT_DEFGW    3      /* Default gateway option BOOTP client     */
#define BOOTP_OPT_END      0xFF   /* End of options marker                   */

/* BOOTP States */
#define BOOTP_STATE_DISABLED   0  /* BOOTP Client is disabled                */
#define BOOTP_STATE_ENABLED    1  /* BOOTP Client is enabled                 */
#define BOOTP_STATE_SEND_REQ   2  /* Send Request is required                */
#define BOOTP_STATE_REPLY_WAIT 4  /* BOOTP Client is waiting for reply       */
#define BOOTP_STATE_REPLY_GET  5  /* BOOTP Client has received reply         */

/* DHCP Defines */
#define DHCP_SERVER_PORT   67     /* DHCP Server standard port               */
#define DHCP_CLIENT_PORT   68     /* DHCP Client standard port               */

/* DHCP States */
#define DHCP_STATE_DISABLED    0  /* DHCP Client is disabled.                */
#define DHCP_STATE_INIT_REBOOT 1  /* DHCP Reboot, request existing IP        */
#define DHCP_STATE_REBOOTING   2  /* DHCP Client is rebooting                */
#define DHCP_STATE_INIT        3  /* Initialize DHCP Client, request new IP  */
#define DHCP_STATE_SELECTING   4  /* Select DHCP Server, get offer           */
#define DHCP_STATE_REQUESTING  5  /* Request sent, waiting for reply         */
#define DHCP_STATE_BOUND       6  /* IP address resolved and valid           */
#define DHCP_STATE_RENEWING    7  /* IP address will expire, renewing        */
#define DHCP_STATE_REBINDING   8  /* Renew failed, find HTTP Server, rebind  */

/* DHCP messages */
#define DHCP_DISCOVER      1
#define DHCP_OFFER         2
#define DHCP_REQUEST       3
#define DHCP_DECLINE       4
#define DHCP_ACK           5
#define DHCP_NAK           6
#define DHCP_RELEASE       7
#define DHCP_INFORM        8

/* DHCP options */
#define DHCP_OPT_PAD            0
#define DHCP_OPT_END            255
#define DHCP_OPT_SUBNET_MASK    1
#define DHCP_OPT_TIME_OFFSET    2
#define DHCP_OPT_ROUTER         3
#define DHCP_OPT_TIME_SERVER    4
#define DHCP_OPT_NAME_SERVER    5
#define DHCP_OPT_DNS_SERVER     6
#define DHCP_OPT_HOST_NAME      12
#define DHCP_OPT_POP3_SERVER    70
#define DHCP_OPT_REQUESTED_IP   50
#define DHCP_OPT_LEASE_TIME     51
#define DHCP_OPT_OVERLOAD       52
#define DHCP_OPT_MSG_TYPE       53
#define DHCP_OPT_SERV_IDENT     54
#define DHCP_OPT_PARAM_REQUEST  55
#define DHCP_OPT_T1_VALUE       58
#define DHCP_OPT_T2_VALUE       59
#define DHCP_OPT_CLIENT_IDENT   61

/* DNS defines */
#define DNS_SERVER_PORT   53      /* DNS Server UDP standard port            */
#define DNS_FLAGS_STQUER  0x0100  /* Name Querry Flags                       */

/* DNS States */
#define DNS_STATE_READY   0       /* DNS Ready, waiting for user request     */
#define DNS_STATE_BUSY    1       /* DNS Busy with resolving host IP         */
#define DNS_STATE_RESEND  2       /* Request timed out, resending            */

/* DNS user event codes */
#define DNS_EVT_SUCCESS   0       /* Host name successfully resolved         */
#define DNS_EVT_NONAME    1       /* DNS Error, no such name                 */
#define DNS_EVT_TIMEOUT   2       /* Timeout resolving host                  */
#define DNS_EVT_ERROR     3       /* Erroneous response packet               */

/* DNS 'get_host_by_name()' result codes */
#define DNS_RES_OK        0       /* Function finished OK                    */
#define DNS_ERROR_BUSY    1       /* DNS Client busy, can't process request  */
#define DNS_ERROR_LABEL   2       /* Host name Label too long                */
#define DNS_ERROR_NAME    3       /* Host name loo long                      */
#define DNS_ERROR_NOSRV   4       /* Prim. DNS server not specified (0.0.0.0)*/
#define DNS_ERROR_UDPSEND 5       /* UDP Send frame error                    */

/* Debug Module Definitions */
#define MEM               0       /* Dynamic Memory Module ID                */
#define ETH               1       /* Ethernet Module ID                      */
#define PPP               2       /* PPP Module ID                           */
#define SLIP              3       /* SLIP Module ID                          */
#define ARP               4       /* ARP Module ID                           */
#define IP                5       /* IP Module ID                            */
#define ICMP              6       /* ICMP Module ID                          */
#define IGMP              7       /* IGMP Module ID                          */
#define UDP               8       /* UDP Module ID                           */
#define TCP               9       /* TCP Module ID                           */
#define NBNS             10       /* NBNS Module ID                          */
#define DHCP             11       /* DHCP Module ID                          */
#define DNS              12       /* DNS Module ID                           */
#define APP              13       /* Application Modules ID                  */

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus               // EC++
extern "C"  {
#endif

typedef struct mem {              /* << Memory Pool management struct >>     */
   struct mem  *next;             /* Next Memory Block in the list           */
   U32    len;                    /* Length of following block               */
   U8     content[1];             /* Memory Block Data content               */
} MEMP;


typedef struct os_frame {         /* << TcpNet system frame buffer struct >> */
   U16 length;                    /* Total Length of data in frame           */
   U16 index;                     /* Buffer Position Index                   */
   U8  data[1];                   /* Buffer data (protocol headers + data)   */
} OS_FRAME;


typedef struct eth_header {       /* << Ethernet Header format >>            */
   U8  DestHwAdr[ETH_ADRLEN];     /* Ethernet destination hardware address   */
   U8  SrcHwAdr[ETH_ADRLEN];      /* Ethernet source hardware address        */
   U16 Prot;                      /* Protocol field of the Ethernet header.  */
   U8  Data[1];                   /* Frame data  46-1500 bytes               */
} ETH_HEADER;

#define ETH_FRAME(frame)        ((ETH_HEADER *)&frame->data[0])

typedef struct ppp_header {       /* << PPP Header format >>                 */
   U16 Fcs;                       /* PPP frame control checksum - appended   */
   U8  Unused[8];                 /* Unused array.                           */
   U8  Adr;                       /* PPP Address Field - usually 0xFF        */
   U8  Ctrl;                      /* PPP Control Field - usually 0x03        */
   U16 Prot;                      /* Protocol field of the PPP header        */
   U8  Data[1];                   /* Frame data  46-1500 bytes               */
} PPP_HEADER;

#define PPP_FRAME(frame)        ((PPP_HEADER *)&frame->data[0])

typedef struct lcp_header {       /* << LCP Header format >>                 */
   U8  Code;                      /* LCP Code field                          */
   U8  Id;                        /* Code identification                     */
   U16 Len;                       /* Code length                             */
} LCP_HEADER;

#define LCP_FRAME(frame)        ((LCP_HEADER *)&frame->data[PPP_FRMOFS+PPP_HEADER_LEN])

/* PPP-PAP Header is the same format as LCP */
#define PAP_HEADER        LCP_HEADER
#define PAP_FRAME         LCP_FRAME

/* PPP-IPCP Header is the same format as LCP */
#define IPCP_HEADER       LCP_HEADER
#define IPCP_FRAME        LCP_FRAME

/* PPP-CP Header - common header for PPP Control Protocols */
#define PPP_CP_FRAME      LCP_FRAME
#define PPP_CP_CODE_REJ   LCP_CODE_REJ
#define PPP_CP_PROT_REJ   LCP_PROT_REJ
#define PPP_CP_HEADER_LEN LCP_HEADER_LEN

typedef struct arp_header {       /* << ARP Header format >>                 */
   U16 HwAdrSpace;                /* Hardware Address Space                  */
   U16 ProtAdrSpace;              /* Protocol Address Space                  */
   U8  HwLen;                     /* MAC address len                         */
   U8  ProtLen;                   /* IP address length                       */
   U16 OpCode;                    /* ARP/RARP Opcode                         */
   U8  SrcHwAdr[ETH_ADRLEN];      /* Source Hardware Address                 */
   U8  SrcIpAdr[IP_ADRLEN];       /* Source IP Address                       */
   U8  DestHwAdr[ETH_ADRLEN];     /* Destination Hardware Address            */
   U8  DestIpAdr[IP_ADRLEN];      /* Destination IP Address                  */
} ARP_HEADER;

#define ARP_FRAME(frame)        ((ARP_HEADER *)&ETH_FRAME(frame)->Data[0])


typedef struct ip_header {        /* << IP Header format >>                  */
   U8  VerHLen;                   /* Version & Header Length field           */
   U8  Tos;                       /* Type Of Service field                   */
   U16 Len;                       /* Total Length of IP packet               */
   U16 Id;                        /* IP Identification number                */
   U16 FragOfs;                   /* IP Flags & Fragment offsett             */
   U8  Ttl;                       /* Time to live                            */
   U8  Prot;                      /* High level Protocol over IP             */
   U16 Chksum;                    /* IP Header Checksum                      */
   U8  SrcIpAdr[IP_ADRLEN];       /* Source IP address                       */
   U8  DestIpAdr[IP_ADRLEN];      /* Destination IP address                  */
} IP_HEADER;

#define IP_FRAME(frame)         ((IP_HEADER *)&ETH_FRAME(frame)->Data[0])


typedef struct icmp_header {      /* << ICMP Header format >>                */
   U8  Type;                      /* Type of the message                     */
   U8  Code;                      /* Message Code byte                       */
   U16 Chksum;                    /* Checksum of ICMP header                 */
   U8  Data[1];                   /* ICMP frame start of Data field          */
} ICMP_HEADER;


typedef struct igmp_header {      /* << IGMP Header format >>                */
   U8  VerType;                   /* Version & Type of the message           */
   U8  Unused;                    /* Unused field                            */
   U16 Chksum;                    /* Checksum of IGMP message                */
   U8  GrpIpAdr[IP_ADRLEN];       /* Group IP address                        */
} IGMP_HEADER;

#define IGMP_FRAME(frame)       ((IGMP_HEADER *)&frame->data[IP_DATA_OFS])

typedef struct udp_header {       /* << UDP Header format >>                 */
   U16 SrcPort;                   /* Source port                             */
   U16 DestPort;                  /* Destination port                        */
   U16 Len;                       /* Length of UDP frame                     */
   U16 Chksum;                    /* UDP frame checksum                      */
   U8  Data[1];                   /* UDP frame start of Data field           */
} UDP_HEADER;


typedef struct tcp_header {       /* << TCP Header format >>                 */
   U16 SrcPort;                   /* Source port                             */
   U16 DestPort;                  /* Destination port                        */
   U32 SeqNr;                     /* Frame Sequence Number                   */
   U32 AckNr;                     /* Frame Acknowledgment Number             */
   U8  HLen;                      /* Header Lenght of frame                  */
   U8  Flags;                     /* TCP Frame Flags                         */
   U16 Window;                    /* Size of Window                          */
   U16 Chksum;                    /* TCP frame checksum                      */
   U16 UrgentPtr;                 /* Urgent pointer                          */
} TCP_HEADER;


typedef struct nbns_header {      /* << NBNS Header Format >>                */
   U16 TransId;                   /* NBNS Transaction Id                     */
   U16 Flags;                     /* NBNS Frame Flags                        */
   U16 QdCount;                   /* Num.of entries in Question section      */
   U16 AnCount;                   /* Answer Name Count                       */
   U16 NsCount;                   /* Name Service Resource Record Count      */
   U16 ArCount;                   /* Name Service Additional Record Count    */
   U8  Data[1];                   /* Message Data Field starts here          */
} NBNS_HEADER;

#define NBNS_FRAME(buf)         ((NBNS_HEADER *)buf)
#define DNS_FRAME(buf)          ((NBNS_HEADER *)buf)


typedef struct arp_info {         /* << ARP Cache Entry info >>              */
   U8  State;                     /* ARP Cache entry current state           */
   U8  Type;                      /* Cache Entry type                        */
   U8  Retries;                   /* Number of Retries left                  */
   U8  Tout;                      /* Cache Entry Timeout                     */
   U8  HwAdr[ETH_ADRLEN];         /* Ethernet Hardware Address               */
   U8  IpAdr[IP_ADRLEN];          /* Ethernet IP Address                     */
} ARP_INFO;


typedef struct igmp_info {        /* << IGMP Group info >>                   */
   U8  State;                     /* Group membership current state          */
   U8  Tout;                      /* Timeout Timer for sending reports       */
   U8  GrpIpAdr[IP_ADRLEN];       /* Group IP address                        */
} IGMP_INFO;


typedef struct udp_info {         /* << UDP Socket info >>                   */
   U8  State;                     /* UDP Socket entry current state          */
   U8  McastTtl;                  /* MultiCast Time To Live                  */
   U16 LocPort;                   /* Local UDP port of Socket                */
   U8  Tos;                       /* UDP Type of Service                     */
   U8  Opt;                       /* UDP Socket Options                      */
                                  /* Application Event CallBack function     */
   U16 (*cb_func)(U8 socket, U8 *rem_ip, U16 port, U8 *buf, U16 len);
} UDP_INFO;


typedef struct tcp_info {         /* << TCP Socket info >>                   */
   U8  State;                     /* TCP Socket entry current state          */
   U8  Type;                      /* TCP Socket type                         */
   U8  Flags;                     /* State machine flags                     */
   U8  Tos;                       /* Type of service allocated               */
   U8  RemIpAdr[IP_ADRLEN];       /* Remote IP address                       */
   U16 RemPort;                   /* Remote TCP port                         */
   U16 LocPort;                   /* Local TCP port                          */
   U16 MaxSegSize;                /* Max. Segment Size Option                */
   U32 SendSeq;                   /* Current Send Sequence Number not acked  */
   U32 SendSeqNext;               /* Next Send Sequence Number               */
   U32 RecSeqNext;                /* Next Receive Sequence Number            */
   U16 Tout;                      /* Socket idle timeout (in seconds)        */
   U16 AliveTimer;                /* Keep Alive timer value                  */
   U16 RetryTimer;                /* Retransmission timer value              */
   U8  TxFlags;                   /* TCP Transmit Flags                      */
   U8  Retries;                   /* Number of retries left before aborting  */
   OS_FRAME *ReTransFrm;          /* Retransmission frame                    */
                                  /* Application Event-CallBack function     */
   U16 (*cb_func)(U8 socket, U8 event, U8 *p1, U16 p2);
} TCP_INFO;


typedef struct http_info {        /* << HTTP Session info >>                 */
   U8  State;                     /* HTTP Session current state              */
   U8  Socket;                    /* binded TCP Socket                       */
   U16 Flags;                     /* State machine Flags                     */
   U8  FType;                     /* File Type: html, jpeg, gif,...          */
   U8  PostSt;                    /* POST method Processing Status           */
   U32 CGIvar;                    /* CGI session private variable (cgi_func) */
   U32 DLen;                      /* Data length (number of bytes to send)   */
   U32 Count;                     /* Total number of bytes sent              */
   U16 BCnt;                      /* Number of bytes sent in last packet     */
   U8  Lang[6];                   /* Selected Web Language: en-us, de, fr .. */
   U8 *Script;                    /* Script Buffer pointer                   */
   void *sFile;                   /* Script Interpreter File pointer         */
   void *dFile;                   /* Data File pointer                       */
} HTTP_INFO;


typedef struct http_file {        /* << HTTP File info >>                    */
   U8  Name[30];                  /* File Name                               */
   U8  *Start;                    /* File Start Address in ROM               */
   U32 Size;                      /* File Size in bytes                      */
} HTTP_FILE;


typedef struct tnet_info {        /* << Telnet Session info >>               */
   U8  State;                     /* Telnet Session current state            */
   U8  Socket;                    /* binded TCP Socket                       */
   U8  Flags;                     /* State machine Flags                     */
   U8  BCnt;                      /* Received Data byte count                */
   U16 Tout;                      /* Timeout delay counter                   */
   U16 XCnt;                      /* Repeat index and Timeout counter        */
   U8  Widx;                      /* Fifo buffer Write index                 */
   U8  Ridx;                      /* Fifo buffer Read index                  */
   U8  Fifo[16];                  /* Fifo buffer for pressed key codes       */
   U8  LBuf[80];                  /* Data Line Buffer                        */
} TNET_INFO;


typedef struct tftp_info {        /* << TFTP Session info >>                 */
   U8  State;                     /* TFTP Session current state              */
   U8  Retries;                   /* Number of retries                       */
   U8  Flags;                     /* State machine Flags                     */
   U16 Timer;                     /* Timeout Timer value                     */
   U8  RemIpAdr[IP_ADRLEN];       /* Remote IP address                       */
   U16 RemPort;                   /* Remote TCP port                         */
   U16 BlockNr;                   /* Block Number                            */
   void *File;                    /* File Handle pointer                     */
   U32 FPos;                      /* File Position indicator                 */
} TFTP_INFO;


typedef struct dns_info {         /* << DNS Cache Entry info >>              */
   U32 HostId;                    /* Host Id (CRC32 value of host name)      */
   U32 Ttl;                       /* Cache Entry Time To Live                */
   U8  IpAdr[IP_ADRLEN];          /* Ethernet IP Address                     */
} DNS_INFO;


typedef struct localm {           /* << Local Machine info >>                */
   U8 IpAdr[IP_ADRLEN];           /* Local IP Address                        */
   U8 DefGW[IP_ADRLEN];           /* Default GateWay                         */
   U8 NetMask[IP_ADRLEN];         /* Net Mask                                */
   U8 PriDNS[IP_ADRLEN];          /* Primary DNS Server                      */
   U8 SecDNS[IP_ADRLEN];          /* Secondary DNS Server                    */
} LOCALM;


typedef struct remotem {          /* << Remote Machine Info >>               */
   U8 IpAdr[IP_ADRLEN];           /* Remote IP address                       */
   U8 HwAdr[ETH_ADRLEN];          /* Remote Hardware Address                 */
} REMOTEM;


typedef enum {                    /* << Fatal System Error Codes >>          */
  ERR_MEM_ALLOC,
  ERR_MEM_FREE,
  ERR_MEM_CORRUPT,
  ERR_UDP_ALLOC,
  ERR_TCP_ALLOC,
  ERR_TCP_STATE
} ERROR_CODE;


/*----------------------------------------------------------------------------
 *      RTL User configuration part BEGIN
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//
// <h>System Definitions
// =====================
// <i> Global TCPnet System definitions
//   <s.15>Local Host Name
//   <i> This is the name under which embedded host can be
//   <i> accessed on a local area network.
//   <i> Default: "labview"
#ifndef LHOST_NAME
#define LHOST_NAME     "labview"
#endif

//   <o>Memory Pool size <1500-32000:4><#/4>
//   <i> This is the size of a memory pool in bytes. Buffers for
//   <i> TCPnet packets are allocated from this memory pool.
//   <i> Default: 8000 bytes
#ifndef MEM_SIZE
#define MEM_SIZE       875
#endif

//   <o>Tick Timer interval <10=> 10 ms <20=> 20 ms <25=> 25 ms
//                          <40=> 40 ms <50=> 50 ms <100=> 100 ms <200=> 200 ms
//   <i> System Tick Timer interval for software timers
//   <i> Default: 100 ms
#ifndef TICK_INTERVAL
#define TICK_INTERVAL  100
#endif

// </h>
// <e>Ethernet Network Interface
// =============================
// <i> Enable or disable Ethernet Network Interface
#ifndef ETH_ENABLE
#define ETH_ENABLE     1
#endif

//   <h>MAC Address
//   ==============
//   <i> Local Ethernet MAC Address
//   <i> Value FF:FF:FF:FF:FF:FF is not allowed.
//   <i> It is an ethernet Broadcast MAC address.
//     <o>Address byte 1 <0x00-0xff:2>
//     <i> LSB is an ethernet Multicast bit.
//     <i> Must be 0 for local MAC address.
//     <i> Default: 0x00
#ifndef _MAC1
#define USE_HW_MAC
#define _MAC1          0x1E
#endif

//     <o>Address byte 2 <0x00-0xff>
//     <i> Default: 0x30
#ifndef _MAC2
#define _MAC2          0x30
#endif

//     <o>Address byte 3 <0x00-0xff>
//     <i> Default: 0x6C
#ifndef _MAC3
#define _MAC3          0x6C
#endif

//     <o>Address byte 4 <0x00-0xff>
//     <i> Default: 0x00
#ifndef _MAC4
#define _MAC4          0xA2
#endif

//     <o>Address byte 5 <0x00-0xff>
//     <i> Default: 0x00
#ifndef _MAC5
#define _MAC5          0x45
#endif

//     <o>Address byte 6 <0x00-0xff>
//     <i> Default: 0x01
#ifndef _MAC6
#define _MAC6          0x5E
#endif

//   </h>
//   <h>IP Address
//   =============
//   <i> Local Static IP Address
//   <i> Value 255.255.255.255 is not allowed.
//   <i> It is a Broadcast IP address.
//     <o>Address byte 1 <0-255>
//     <i> Default: 192
#ifndef _IP1
#define _IP1           192
#endif

//     <o>Address byte 2 <0-255>
//     <i> Default: 168
#ifndef _IP2
#define _IP2           168
#endif

//     <o>Address byte 3 <0-255>
//     <i> Default: 0
#ifndef _IP3
#define _IP3           0
#endif

//     <o>Address byte 4 <0-255>
//     <i> Default: 100
#ifndef _IP4
#define _IP4           100
#endif

//   </h>
//   <h>Subnet mask
//   ==============
//   <i> Local Subnet mask
//     <o>Mask byte 1 <0-255>
//     <i> Default: 255
#ifndef _MSK1
#define _MSK1          255
#endif

//     <o>Mask byte 2 <0-255>
//     <i> Default: 255
#ifndef _MSK2
#define _MSK2          255
#endif

//     <o>Mask byte 3 <0-255>
//     <i> Default: 255
#ifndef _MSK3
#define _MSK3          255
#endif

//     <o>Mask byte 4 <0-255>
//     <i> Default: 0
#ifndef _MSK4
#define _MSK4          0
#endif

//   </h>
//   <h>Default Gateway
//   ==================
//   <i> Default Gateway IP Address
//     <o>Address byte 1 <0-255>
//     <i> Default: 192
#ifndef _GW1
#define _GW1           192
#endif

//     <o>Address byte 2 <0-255>
//     <i> Default: 168
#ifndef _GW2
#define _GW2           168
#endif

//     <o>Address byte 3 <0-255>
//     <i> Default: 0
#ifndef _GW3
#define _GW3           0
#endif

//     <o>Address byte 4 <0-255>
//     <i> Default: 254
#ifndef _GW4
#define _GW4           254
#endif

//   </h>
//   <h>Primary DNS Server
//   =====================
//   <i> Primary DNS Server IP Address
//     <o>Address byte 1 <0-255>
//     <i> Default: 194
#ifndef _pDNS1
#define _pDNS1         194
#endif

//     <o>Address byte 2 <0-255>
//     <i> Default: 25
#ifndef _pDNS2
#define _pDNS2         25
#endif

//     <o>Address byte 3 <0-255>
//     <i> Default: 2
#ifndef _pDNS3
#define _pDNS3         2
#endif

//     <o>Address byte 4 <0-255>
//     <i> Default: 129
#ifndef _pDNS4
#define _pDNS4         129
#endif

//   </h>
//   <h>Secondary DNS Server
//   =======================
//   <i> Secondary DNS Server IP Address
//     <o>Address byte 1 <0-255>
//     <i> Default: 194
#ifndef _sDNS1
#define _sDNS1         194
#endif

//     <o>Address byte 2 <0-255>
//     <i> Default: 25
#ifndef _sDNS2
#define _sDNS2         25
#endif

//     <o>Address byte 3 <0-255>
//     <i> Default: 2
#ifndef _sDNS3
#define _sDNS3         2
#endif

//     <o>Address byte 4 <0-255>
//     <i> Default: 130
#ifndef _sDNS4
#define _sDNS4         130
#endif

//   </h>
//   <h>ARP Definitions
//   ==================
//   <i> Address Resolution Protocol Definitions
//     <o>Cache Table size <5-100>
//     <i> Number of cached hardware/IP addresses
//     <i> Default: 10
#ifndef ARP_TABSIZE
#define ARP_TABSIZE    10
#endif

//     <o>Cache Timeout in seconds <5-255>
//     <i> A timeout for a cached hardware/IP addresses
//     <i> Default: 150
#ifndef ARP_TIMEOUT
#define ARP_TIMEOUT    150
#endif

//     <o>Number of Retries <0-20>
//     <i> Number of Retries to resolve an IP address
//     <i> before ARP module gives up
//     <i> Default: 4
#ifndef ARP_MAXRETRY
#define ARP_MAXRETRY   4
#endif

//     <o>Resend Timeout in seconds <1-10>
//     <i> A timeout to resend the ARP Request
//     <i> Default: 2
#ifndef ARP_RESEND
#define ARP_RESEND     2
#endif

//   </h>
//   <e>IGMP Group Management
//   ========================
//   <i> Enable or disable Internet Group Management Protocol
#ifndef IGMP_ENABLE
#define IGMP_ENABLE    0
#endif

//     <o>Membership Table size <2-50>
//     <i> Number of Groups this host can join
//     <i> Default: 5
#ifndef IGMP_TABSIZE
#define IGMP_TABSIZE   5
#endif

//   </e>
//   <q>NetBIOS Name Service
//   =======================
//   <i> When this option is enabled, the embedded host can be
//   <i> accessed by his name on the local LAN using NBNS protocol.
//   <i> You need to modify also the number of UDP Sockets,
//   <i> because NBNS protocol uses one UDP socket to run.
#ifndef NBNS_ENABLE
#define NBNS_ENABLE    1
#endif

//   <q>Dynamic Host Configuration
//   =============================
//   <i> When this option is enabled, local IP address, Net Mask
//   <i> and Default Gateway are obtained automatically from
//   <i> the DHCP Server on local LAN.
//   <i> You need to modify also the number of UDP Sockets,
//   <i> because DHCP protocol uses one UDP socket to run.
#ifndef DHCP_ENABLE
#define DHCP_ENABLE    1
#endif

// </e>

// <e>PPP Network Interface
// ========================
// <i> Enable or disable PPP Network Interface
#define PPP_ENABLE     0

//   <h>IP Address
//   =============
//   <i> Local Static IP Address
//     <o>Address byte 1 <0-255>
//     <i> Default: 192
#define _IP1P          192

//     <o>Address byte 2 <0-255>
//     <i> Default: 168
#define _IP2P          168

//     <o>Address byte 3 <0-255>
//     <i> Default: 125
#define _IP3P          125

//     <o>Address byte 4 <0-255>
//     <i> Default: 1
#define _IP4P          1

//   </h>
//   <h>Subnet mask
//   ==============
//   <i> Local Subnet mask
//     <o>Mask byte 1 <0-255>
//     <i> Default: 255
#define _MSK1P         255

//     <o>Mask byte 2 <0-255>
//     <i> Default: 255
#define _MSK2P         255

//     <o>Mask byte 3 <0-255>
//     <i> Default: 255
#define _MSK3P         255

//     <o>Mask byte 4 <0-255>
//     <i> Default: 0
#define _MSK4P         0

//   </h>
//   <h>Primary DNS Server
//   =====================
//   <i> Primary DNS Server IP Address
//     <o>Address byte 1 <0-255>
//     <i> Default: 194
#define _pDNS1P        194

//     <o>Address byte 2 <0-255>
//     <i> Default: 25
#define _pDNS2P        25

//     <o>Address byte 3 <0-255>
//     <i> Default: 2
#define _pDNS3P        2

//     <o>Address byte 4 <0-255>
//     <i> Default: 129
#define _pDNS4P        129

//   </h>
//   <h>Secondary DNS Server
//   =======================
//   <i> Secondary DNS Server IP Address
//     <o>Address byte 1 <0-255>
//     <i> Default: 194
#define _sDNS1P        194

//     <o>Address byte 2 <0-255>
//     <i> Default: 25
#define _sDNS2P        25

//     <o>Address byte 3 <0-255>
//     <i> Default: 2
#define _sDNS3P        2

//     <o>Address byte 4 <0-255>
//     <i> Default: 130
#define _sDNS4P        130

//   </h>
//   <q>Obtain Client IP address automatically
//   =========================================
//   <i> This option only applies when PPP Dial-up is used to dial
//   <i> to remote PPP Server. If checked, network connection
//   <i> dynamically obtains an IP address from remote PPP Server.
#define PPP_GETIP      1

//   <q>Use Default Gateway on remote Network
//   ========================================
//   <i> This option only applies when both Ethernet and PPP Dial-up
//   <i> are used. If checked, data that cannot be sent to local LAN
//   <i> is forwarded to Dial-up network instead.
#define PPP_DEFGW      1

//   <h>Async Control Character Map
//   ==============================
//   <i> A map of control characters 0..31 which are transmitted 
//   <i> escaped as a 2 byte sequence.
//     <o>Map char 31..24 mask <0x00-0xff>
//     <i> Maps control characters from 31...24
//     <i> Default: 0x00
#define _ACCM1         0x00

//     <o>Map char 23..16 mask <0x00-0xff>
//     <i> Maps control characters from 23...16
//     <i> For XON/XOFF set this value to: 0x0A
//     <i> Default: 0x00
#define _ACCM2         0x0A

//     <o>Map char 15..8 mask <0x00-0xff>
//     <i> Maps control characters from 15...8
//     <i> Default: 0x00
#define _ACCM3         0x00

//     <o>Map char 7..0 mask <0x00-0xff>
//     <i> Maps control characters from 7...0
//     <i> Default: 0x00
#define _ACCM4         0x00

//   </h>
//   <h>Retransmissions and Timeouts
//   ===============================
//   <i> Several protocol settings.
//     <o>LCP Number of Retries <0-20>
//     <i> How many times Link Control Protocol will try to retransmit
//     <i> data before giving up. Increase this value for links with
//     <i> low baud rates or high latency.
//     <i> Default: 2
#define LCP_MAXRETRY   2

//     <o>LCP Retry Timeout in seconds <1-10>
//     <i> If no response received within this time frame,
//     <i> LCP module will try to resend data again.
//     <i> Default: 2
#define LCP_RETRYTOUT  2

//     <o>PAP Number of Retries <0-20>
//     <i> How many times Password Authentication Protocol will try to 
//     <i> retransmit data before giving up. Increase this value for links
//     <i> with low baud rates or high latency.
//     <i> Default: 3
#define PAP_MAXRETRY   3

//     <o>PAP Retry Timeout in seconds <1-10>
//     <i> If no response received within this time frame,
//     <i> PAP module will try to resend data again
//     <i> Default: 3
#define PAP_RETRYTOUT  3

//     <o>IPCP Number of Retries <0-20>
//     <i> How many times Internet Protocol Control Protocol will try
//     <i> to retransmit data before giving up. Increase this value for
//     <i> links with low baud rates or high latency.
//     <i> Default: 3
#define IPCP_MAXRETRY  3

//     <o>IPCP Retry Timeout in seconds <1-10>
//     <i> If no response received within this time frame,
//     <i> IPCP module will try to resend data again
//     <i> Default: 2
#define IPCP_RETRYTOUT 2

//   </h>
// </e>
// <e>SLIP Network Interface
// ========================
// <i> Enable or disable SLIP Network Interface
#define SLIP_ENABLE    0

//   <h>IP Address
//   =============
//   <i> Local Static IP Address
//     <o>Address byte 1 <0-255>
//     <i> Default: 192
#define _IP1S          192

//     <o>Address byte 2 <0-255>
//     <i> Default: 168
#define _IP2S          168

//     <o>Address byte 3 <0-255>
//     <i> Default: 225
#define _IP3S          225

//     <o>Address byte 4 <0-255>
//     <i> Default: 1
#define _IP4S          1

//   </h>
//   <h>Subnet mask
//   ==============
//   <i> Local Subnet mask
//     <o>Mask byte 1 <0-255>
//     <i> Default: 255
#define _MSK1S         255

//     <o>Mask byte 2 <0-255>
//     <i> Default: 255
#define _MSK2S         255

//     <o>Mask byte 3 <0-255>
//     <i> Default: 255
#define _MSK3S         255

//     <o>Mask byte 4 <0-255>
//     <i> Default: 0
#define _MSK4S         0

//   </h>
//   <h>Primary DNS Server
//   =====================
//   <i> Primary DNS Server IP Address
//     <o>Address byte 1 <0-255>
//     <i> Default: 194
#define _pDNS1S        194

//     <o>Address byte 2 <0-255>
//     <i> Default: 25
#define _pDNS2S        25

//     <o>Address byte 3 <0-255>
//     <i> Default: 2
#define _pDNS3S        2

//     <o>Address byte 4 <0-255>
//     <i> Default: 129
#define _pDNS4S        129

//   </h>
//   <h>Secondary DNS Server
//   =======================
//   <i> Secondary DNS Server IP Address
//     <o>Address byte 1 <0-255>
//     <i> Default: 194
#define _sDNS1S        194

//     <o>Address byte 2 <0-255>
//     <i> Default: 25
#define _sDNS2S        25

//     <o>Address byte 3 <0-255>
//     <i> Default: 2
#define _sDNS3S        2

//     <o>Address byte 4 <0-255>
//     <i> Default: 130
#define _sDNS4S        130

//   </h>
//   <q>Use Default Gateway on remote Network
//   ========================================
//   <i> This option only applies when both Ethernet and SLIP Dial-up
//   <i> are used. If checked, data that cannot be sent to local LAN
//   <i> is forwarded to Dial-up network instead.
#define SLIP_DEFGW     1

// </e>
// <e>UDP Sockets
// ==============
// <i> Enable or disable UDP Sockets
#ifndef UDP_ENABLE
#define UDP_ENABLE     1
#endif

//   <o>Number of UDP Sockets <1-20>
//   <i> Number of available UDP sockets
//   <i> Default: 5
#ifndef UDP_NUMSOCKS
#define UDP_NUMSOCKS   5
#endif

//   <o>Highest port for autoselect <1-32767>
//   <i> Highest port number for UDP port when the port is not provided
//   <i> by the user but is automatically selected by the system.
//   <i> Default: 1023
#ifndef UDP_LASTPORT
#define UDP_LASTPORT   1023
#endif

// </e>
// <e>TCP Sockets
// ==============
// <i> Enable or disable TCP Sockets
#ifndef TCP_ENABLE
#define TCP_ENABLE     1
#endif

//   <o>Number of TCP Sockets <1-20>
//   <i> Number of available TCP sockets
//   <i> Default: 5
#ifndef TCP_NUMSOCKS
#define TCP_NUMSOCKS   5
#endif

//   <o>Highest port for autoselect <1-32767>
//   <i> Highest port number for TCP port when the port is not provided
//   <i> by the user but is automatically selected by the system.
//   <i> Default: 1023
#ifndef TCP_LASTPORT
#define TCP_LASTPORT   1023
#endif

//   <o>TCP Number of Retries <0-20>
//   <i> How many times TCP module will try to retransmit data
//   <i> before giving up. Increase this value for high-latency
//   <i> and low_throughput networks.
//   <i> Default: 5
#ifndef TCP_MAXRETRY
#define TCP_MAXRETRY   5
#endif

//   <o>TCP Retry Timeout in seconds <1-10>
//   <i> If data frame not acknowledged within this time frame,
//   <i> TCP module will try to resend data again
//   <i> Default: 4
#ifndef TCP_RETRYTOUT
#define TCP_RETRYTOUT  4
#endif

//   <o>TCP Default Connect Timeout in seconds <1-600>
//   <i> Default TCP Socket Keep Alive timeout. When it expires
//   <i> with no TCP data frame send, TCP Connection is closed.
//   <i> Default: 120
#ifndef TCP_DEFTOUT
#define TCP_DEFTOUT    120
#endif

/* TCP fixed timeouts */
#ifndef TCP_INIT_RETRY_TOUT
#define TCP_INIT_RETRY_TOUT 1     /* TCP initial Retransmit period in sec.   */
#endif

#ifndef TCP_SYN_RETRY_TOUT
#define TCP_SYN_RETRY_TOUT  2     /* TCP SYN frame retransmit period in sec. */
#endif

#ifndef TCP_CONRETRY
#define TCP_CONRETRY        7     /* Number of retries to establish a conn.  */
#endif
// </e>
// <e>HTTP Server
// ==============
// <i> Enable or disable HTTP Server
#define HTTP_ENABLE    0

//   <o>Number of HTTP Sessions <1-10>
//   <i> Number of simultaneously active HTTP Sessions.
//   <i> Modify also the number of TCP Sockets because
//   <i> each HTTP session uses it's own TCP socket
//   <i> Default: 3
#define HTTP_NUMSESS   3

//   <e>Enable User Authentication
//     <i> When enabled, the user will have to authenticate
//     <i> himself by username and password before accessing
//     <i> any page on this Embedded WEB server.
#define HTTP_ENAUTH    1

//     <s.20>Authentication Realm string
//     <i> Default: "Embedded WEB Server"
#define HTTP_AUTHREALM "Embedded WEB Server"

//     <s.15>Authentication Username
//     <i> Default: "admin"
#define HTTP_AUTHUSER  "admin"

//     <s.15>Authentication Password
//     <i> Default: ""
#define HTTP_AUTHPASSW ""

//   </e>
// </e>
// <e>Telnet Server
// ================
// <i> Enable or disable Telnet Server
#define TNET_ENABLE    0

//   <o>Number of Telnet Connections <1-10>
//   <i> Number of simultaneously active Telnet Connections.
//   <i> Modify also the number of TCP Sockets because
//   <i> each Telnet connection uses it's own TCP socket
//   <i> Default: 1
#define TNET_NUMSESS   2

//   <e>Enable User Authentication
//   <i> When enabled, the user will have to authenticate
//   <i> himself by username and password before access
//   <i> to the system is allowed.
#define TNET_ENAUTH    1

//     <s.15>Authentication Username
//     <i> Default: "admin"
#define TNET_AUTHUSER  "admin"

//     <s.15>Authentication Password
//     <i> Default: ""
#define TNET_AUTHPASSW ""

//   </e>
// </e>
// <e>TFTP Server
// ==============
// <i> Enable or disable TFTP Server
#define TFTP_ENABLE    0

//   <o>Number of TFTP Sessions <1-10>
//   <i> Number of simultaneously active TFTP Sessions
//   <i> All TFTP Sessions use the same UDP socket listening
//   <i> on defalut TFTP Server port 69.
//   <i> Default: 1
#define TFTP_NUMSESS   1

//   <o>TFTP Inactive Session Timeout in seconds <5-120>
//   <i> When timeout expires TFTP Session is closed. This timeout
//   <i> is used when the UDP connection is broken because of error.
//   <i> Default: 15
#define TFTP_DEFTOUT   15

//   <o>TFTP Number of Retries <1-10>
//   <i> How many times TFTP Server will try to retransmit data
//   <i> before giving up.
//   <i> Default: 4
#define TFTP_MAXRETRY  4

// </e>
// <e>DNS Client
// =============
// <i> Enable or disable DNS Client
#ifndef DNS_ENABLE
#define DNS_ENABLE     1
#endif

//     <o>Cache Table size <5-100>
//     <i> Number of cached DNS host names/IP addresses
//     <i> Default: 20
#ifndef DNS_TABSIZE
#define DNS_TABSIZE    20
#endif

// </e>
// <e>SMTP Client
// ==============
// <i> Enable or disable SMTP Client
#define SMTP_ENABLE     0

//     <o>Response Timeout in seconds <5-120>
//     <i> This is a time for SMTP Client to wait for a response from
//     <i> SMTP Server. If timeout expires, Client aborts operation.
//     <i> Default: 20
#define SMTP_DEFTOUT   20

// </e>
//------------- <<< end of configuration section >>> -----------------------


/*----------------------------------------------------------------------------
 *      Functions
 *---------------------------------------------------------------------------*/

/* Net_Config.c */
extern void init_system (void);
extern void run_system (void);
__weak void process_hl_igmp (OS_FRAME *frame);
__weak void process_hl_udp (OS_FRAME *frame);
__weak void process_hl_tcp (OS_FRAME *frame);
extern BOOL dispatch_frame (OS_FRAME *frame, U8 netif);
extern BOOL eth_chk_adr (OS_FRAME *frame);
extern U8  *eth_get_adr (U8 *ipadr);
extern void sys_error (ERROR_CODE code);

/* at_Mem.c */
extern OS_FRAME *alloc_mem (U32 byte_size);
extern void free_mem (OS_FRAME *mem_ptr);

/* at_Ethernet.c */
extern void init_eth_link (void);
extern void run_eth_link (void);
extern void put_in_queue (OS_FRAME *frame);
extern BOOL eth_send_frame (OS_FRAME *frame);

/* at_Ppp.c */
extern void init_ppp_link (void);
extern void run_ppp_link (void);
extern BOOL ppp_send_frame (OS_FRAME *frame, U16 prot);

/* at_Slip.c */
extern void init_slip_link (void);
extern void run_slip_link (void);
extern BOOL slip_send_frame (OS_FRAME *frame);

/* at_Lib.c */
extern int  mem_copy (void *dp, void *sp, int len);
extern void mem_rcopy (void *dp, void *sp, int len);
extern BOOL mem_comp (void *sp1, void *sp2, int len);
extern void mem_set (void *dp, U8 val, int len);
extern BOOL mem_test (void *sp, U8 val, int len);
extern BOOL str_scomp (U8 *sp, U8 const *cp);
extern int  str_copy (U8 *dp, U8 *sp);
extern void str_up_case (U8 *dp, U8 *sp);
extern U16  SwapB (U16 w16);
extern U16  get_u16 (U8 *p16);
extern U32  get_u32 (U8 *p32);
extern void set_u32 (U8 *p32, U32 val);

/* at_Igmp.c */
extern void init_igmp (void);
extern void run_igmp_host (void);
extern void process_igmp (OS_FRAME *frame);
extern BOOL igmp_is_member (U8 *ipadr);

/* at_Udp.c */
extern void init_udp (void);
extern void process_udp (OS_FRAME *frame);

/* at_Tcp.c */
extern void init_tcp (void);
extern void tcp_poll_sockets (void);
extern void process_tcp (OS_FRAME *frame_r);

/* at_Http.c */
extern void init_http (void);
extern void run_http_server (void);
extern void *http_fopen (U8 *name);
extern void http_fclose (void *file);
extern U16  http_fread (void *file, U8 *buf, U16 len);
extern BOOL http_fgets (void *file, U8 *buf, U16 size);
extern void cgi_process_var (U8 *qstr);
extern void cgi_process_data (U8 *dat, U16 len);
extern U16  cgi_func (U8 *env, U8 *buf, U16 buflen, U32 *pcgi);
extern U8  *http_get_var (U8 *env, void *ansi, U16 maxlen);
extern U8  *http_get_lang (void);
extern void http_get_info (REMOTEM *info);
extern U8   http_get_session (void);

/* at_Telnet.c */
extern void init_tnet (void);
extern void run_tnet_server (void);
extern U16  tnet_cbfunc (U8 code, U8 *buf, U16 buflen);
extern U16  tnet_process_cmd (U8 *cmd, U8 *buf, U16 buflen, U16 xcnt);
extern BOOL tnet_ccmp (U8 *buf, U8 *cmd);
extern void tnet_set_delay (U16 cnt);
extern void tnet_get_info (REMOTEM *info);
extern U8   tnet_get_session (void);
extern BOOL tnet_msg_poll (U8 session);

/* at_Tftp.c */
extern void init_tftp (void);
extern void run_tftp_server (void);
extern void *tftp_fopen (U8 *fname, U8 *mode);
extern void tftp_fclose (void *file);
extern U16  tftp_fread (void *file, U32 fpos, U8 *buf);
extern U16  tftp_fwrite (void *file, U8 *buf, U16 len);

/* at_Bootp.c */
extern void init_bootp (void);
extern void bootp_client (void);

/* at_Dhcp.c */
extern void init_dhcp (void);
extern void run_dhcp_client (void);

/* at_Nbns.c */
extern void init_nbns (void);

/* at_Dns.c */
extern void init_dns (void);
extern void run_dns_client (void);
extern U8   get_host_by_name (U8 *hostn, void (*cbfunc)(U8, U8 *));

/* at_Smtp.c */
extern void init_smtp (void);
extern void run_smtp_client (void);
extern U16  smtp_cbfunc (U8 code, U8 *buf, U16 buflen, U16 xcnt);

/* Ethernet Device Driver */
extern void init_ethernet (void);
extern void send_frame (OS_FRAME *frame);
__weak void poll_ethernet (void);
__weak void int_enable_eth (void);
__weak void int_disable_eth (void);

/* Serial Device Driver */
extern void init_serial (void);
extern int  com_getchar (void);
extern BOOL com_putchar (U8 c);
extern BOOL com_tx_active (void);

/* Modem Device Driver */
extern void init_modem (void);
extern void modem_dial (U8 *dialnum);
extern void modem_hangup (void);
extern void modem_listen (void);
extern BOOL modem_online (void);
extern BOOL modem_process (U8 ch);
extern void modem_run (void);

#ifdef __cplusplus               // EC++
}
#endif

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

#endif

