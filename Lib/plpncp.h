/*
     My Psion. Psion link program (Psion Series 5 (3)).
     PLP layer - header file.

     Based on my reading source-code by Rudolf Koenig
 (rfkoenig@immd4.informatik.uni-erlangen.de), Philip Proudman
 (former email, current address unknown proudman@btinternet.com)
 and Matt Gumbley (matt@gumbley.demon.co.uk)
*/
#ifndef _PLPNCP
#define _PLPNCP

#define INCL_BASE
#define INCL_DOS
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#define INCL_DOSSESMGR
#define INCL_SHLERRORS
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)

#define _DATA_PACKET       0x30
#define _ACK_PACKET        0x00
#define _NEWLINK_PACKET    0x20
#define _DISCONNECT_PACKET 0x10

#define _DEF_CONN_TIMEOUT           15000       // 15 sec. inactivity timeout
#define _DEF_RETRYTIMEOUT           5000        // 5 sec. for one retry
#define _DEF_POOL_TIMEOUT           5
#define _DEF_PACKETSIZE             280
#define _MAX_PACKETOVERHEAD         3
#define _MAX_RESEND                 5
#define _MAX_CHANNELS               8
#define _MAX_QUEUE                  32
#define _MAX_WAITTIME               200
#define _THREAD_STACK_SIZE          16384

#define NCP_PROCESSNAME_SIZE 20

#define NCON_MSG_DATA_XOFF          1
#define NCON_MSG_DATA_XON           2
#define NCON_MSG_CONNECT_TO_SERVER  3
#define NCON_MSG_CONNECT_RESPONSE   4
#define NCON_MSG_CHANNEL_CLOSED     5
#define NCON_MSG_NCP_INFO           6
#define NCON_MSG_CHANNEL_DISCONNECT 7
#define NCON_MSG_NCP_END            8
#define NCP_LAST_MESSAGE            1
#define NCP_NOT_LAST_MESSAGE        2
#define NCP_E_FILE_NEXIST           (-33)
#define PV_SERIES_5                 6
#define PV_SERIES_3                 3

extern char  _Export PLP_header [3];
extern char  _Export PLP_tail   [2];
extern char  _Export NcpConnectName [];
extern char  _Export RfsvConnectName [];
extern int   _Export RFSVversion;
extern UCHAR _Export psComName [8];
extern HEV _Export psLinkErrorSem;
extern HEV _Export psLinkSem;
extern HEV _Export psRunLinkSem;
extern HFILE _Export psComPort;
extern ULONG _Export psComBaud;

extern ULONG psComBauds [10];
extern ULONG psLinkThreadID;
extern ULONG psComTimeouts [10];

extern ULONG volatile _Export psTimeout;
extern ULONG volatile _Export psConnectTimeout;
extern ULONG volatile psLinkError;
extern BOOL  psComIsOpened;

#define nextInRing(h)   (((h)+1)>=_MAX_QUEUE?0:(h)+1)
#define prevInRing(h)   (((h)-1)<0?_MAX_QUEUE-1:(h)-1)

typedef struct BAUD {
            ULONG curBitRate;
            UCHAR curFraction;
            ULONG minBitRate;
            UCHAR minFraction;
            ULONG maxBitRate;
            UCHAR maxFraction;
            } BAUD;

typedef struct _Buffer {
            UCHAR  *data;
            int    len;
            int    size;
            int    n;
            } Buffer;

typedef struct _Packet {
            Buffer buf;
            UCHAR  type [2];
            }   Packet;

typedef struct _Queue {
            Buffer buf [_MAX_QUEUE];
            int    head;
            int    tail;
            int    counter;
            HMTX   lock;
            }  Queue;


typedef struct _NcpChannel {
            BOOL  volatile empty;
            Buffer buf;
            HEV   connected;
            HEV   disconnected;
            Queue iqueue;
            Queue oqueue;
            UCHAR name [NCP_PROCESSNAME_SIZE];
            } NcpChannel;

extern NcpChannel _Export psChannelsList [_MAX_CHANNELS];
extern int        volatile _Export psRemoteChannelsList[_MAX_CHANNELS];
extern int        volatile _Export rfsvCurrentChannel;
extern int        volatile _Export psLinkChannel;

APIRET psInitSerialPort ( char dev[], ULONG speed, HFILE *Com );
void   psStopSerialPort ( HFILE Com );
APIRET psSetBaudRate ( char dev[], ULONG *pbaud );
APIRET runPlpLink ( void );

/*
    Buffer manipulation routines.
*/
APIRET newBuffer ( Buffer *buf, int size );
APIRET initBuffer ( Buffer *buf );
void   clearBuffer ( Buffer *buf );
void   destroyBuffer ( Buffer *buf );
APIRET storeByte ( Buffer *buf, USHORT byte );
APIRET storeData ( Buffer *buf, UCHAR *data, int len );
APIRET storeWord ( Buffer *buf, USHORT word );
APIRET storeDword ( Buffer *buf, ULONG dword );
APIRET getByte ( Buffer *buf, USHORT *pbyte );
APIRET getWord ( Buffer *buf, USHORT *pword );
APIRET getString ( Buffer *buf, UCHAR *str );
APIRET getData ( Buffer *buf, UCHAR *data, int *len );
APIRET getDword ( Buffer *buf, ULONG *dword );
#define getDataRest(b)   ((b)->len-(b)->n)
void   skipNbytes ( Buffer *buf, int n );

/*
    PLP layer
*/
BOOL   getPacket ( HFILE Com, Packet *pkt, APIRET *prc );
void   resetPacket ( void );
void   add2CRC ( USHORT c, USHORT *crc );
APIRET putPacket ( HFILE Com, Packet *buf );
APIRET sendAck ( HFILE Com, SHORT seqID );
APIRET sendDisconnect ( HFILE Com, SHORT seqID );
APIRET sendNewLink ( HFILE Com, SHORT seqID );
APIRET sendPacket ( HFILE Com, Packet *toSend, SHORT seqID );
void  _System plpLink ( ULONG Com );
USHORT getBufCRC ( UCHAR *data, int len );
USHORT getPktCRC ( Packet *pkt );
SHORT  plpHaveData ( void );
APIRET plpGetData ( Packet *pkt );

/*
    NCP layer
*/
APIRET ncpReset ( void );
APIRET ncpMessage ( UCHAR *data, int len );
APIRET ncpAddDataToChannel (NcpChannel *ch,UCHAR *data,int len);
APIRET ncpControlChannel ( int channel,USHORT type,UCHAR *Cmd,int Len);
APIRET ncpDecodeControlMessage ( UCHAR *Buf, int Len );
int    ncpFirstUnusedChannel ( UCHAR ConName[] );
APIRET ncpDisconnect ( int channel );
APIRET ncpConnect ( UCHAR ConName[], int *pch );
void   ncpClear ( void );

APIRET initQueue ( Queue *Q );
void   destroyQueue ( Queue *Q );
APIRET pushBuf ( Queue *Q, Buffer *buf );
Buffer *popBuf ( Queue *Q );
int    getQueueSize ( Queue *Q );

/*
    RFSV layer - file service
*/
APIRET rfsvReceive  ( Buffer *buf, int channel );
APIRET rfsvSend ( Buffer *buf, int channel );
APIRET rfsvConnect ( void );
APIRET rfsvDisconnect ( void );


#define PERROR_BADCRC           1
#define PERROR_IO_CANTREAD      2
#define PERROR_NOMEMORY         3
#define PERROR_TIMEOUT          4
#define PERROR_BIGPACKET        5
#define PERROR_BREAK_CONNECTION 6
#define PERROR_BAD_PLPPACKET    7
#define PERROR_QUEUE_OVERFLOW   8
#define PERROR_NOEMPTY_CHANNEL  9
#define PERROR_UNKNOWN_CHANNEL  10
#define PERROR_UNKNOWN_STATUS   11
#define PERROR_UNKNOWN_NCPMSG   12
#define PERROR_BAD_NCPPACKET    13
#define PERROR_NULL_NCPPACKET   14
#define PERROR_CANT_STARTLINK   15
#define PERROR_NOSEMAPHORE      16
#define PERROR_BADQUEUE         17
#define PERROR_EOB              18
#define PERROR_BAD_RFSVRESPONSE 19
#define PERROR_PSION_ERROR      20
#define PERROR_RECONNECTED      21
#define PERROR_CANTAUTOBAUD     22
#define PERROR_FILENOTFOUND     23
#define PERROR_IO_CANTWRITE     24
#define PERROR_IO_MAXRESEND     25
#define PERROR_LOCALE           26
#define PERROR_CANTWRITEFILE    27

#define _DATA_TITLE 0
#define _SEND_TITLE 1
#define _RECV_TITLE 2
#define _END_TITLE  3

#ifdef DEBUG
void putDebug ( char Module[], int Line, char Format[],...);
void dumpDebug ( char buf[], int size );
void dumpByte ( short byte );
void putDebugByte ( short byte );
#define DBG   __FILE__,__LINE__
#endif

#endif
