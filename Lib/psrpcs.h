/*
    My Psion. Psion link program (Psion Series 5 (3)).
    RPCS layer.
*/
#ifndef _PSIRPC
#define _PSIRPC

/*
    Remote procedure call commands.
*/
#define PSV_GET_VERSION      0x00
#define PSV_EXEC_PROGRAM     0x01
#define PSV_LIST_PROGRAMS    0x02
#define PSV_STOP_PROGRAM     0x03
#define PSV_IS_PROG_RINNING  0x04
#define PSV_START_FORMAT     0x05
#define PSV_KEEPON_FORMAT    0x06
#define PSV_QUERY_DRIVE      0x07
#define PSV_GET_OWNER        0x08
#define PSV_GET_MACHINE_TYPE 0x09
#define PSV_GET_CMD_LINE     0x0a
#define PSV_STOP_FILE        0x0b
#define PSV_GET_MACHINE_INFO 0x64
#define PSV_SET_TIME         0x6b

#define RPCS_VERSION         0x0801
#define _MAX_PROGRAM_NAME    128

#define getRPCSerror()      (psSessions[rpcsCurrentChannel].PsionError)
#define resetRPCSerror()    (psSessions[rpcsCurrentChannel].PsionError=0)


typedef struct _ProcessList {
    UCHAR Prog [_MAX_PS_FNAME];
    UCHAR Argv [_MAX_PS_FNAME];
    }  ProcessList;

typedef struct _psTime {
    ULONG  TimeLo;
    ULONG  TimeHi;
    ULONG  Country;
    ULONG  UTCoffset;
    ULONG  DST;
    ULONG  DSTzone;
    }  psTime;

typedef struct _MachineInfo {
    ULONG  MachineType;
    USHORT ROMversion;
    USHORT ROMbuild;
    UCHAR  reserved1[8];
    UCHAR  MachineName [16];
    ULONG  DisplayWidth;
    ULONG  DisplayHeight;
    ULONG  MachineUIDLo;
    ULONG  MachineUIDHi;
    psTime Time;                // look above about the psTime structure
    ULONG  MainBattInsLo;
    ULONG  MainBattInsHi;
    ULONG  MainBattStatus;
    ULONG  MainBattUsedLo;
    ULONG  MainBattUsedHi;
    ULONG  MainBattCurrent;
    ULONG  MainBattUsed;
    ULONG  MainBattVoltage;
    ULONG  MainBattMaxVoltage;
    ULONG  BackBattStatus;
    ULONG  BackBattVoltage;
    ULONG  BackBattMaxVoltage;
    ULONG  ExternalPower;
    ULONG  BackBattUsedLo;
    ULONG  BackBattUsedHi;
    ULONG  Unknown;
    ULONG  RAMsize;
    ULONG  ROMsize;
    ULONG  RAMmaxFree;
    ULONG  RAMfree;
    ULONG  RAMdiskSize;
    ULONG  RegistrySize;
    ULONG  EPROM;
    ULONG  Language;
    UCHAR  reserved2[88];
    }  MachineInfo;

extern int   volatile _Export rpcsCurrentChannel;
extern char  _Export RpcsConnectName [];

APIRET  rpcsConnect ( void );
APIRET  rpcsDisconnect ( void );
APIRET  linkRegisterServer ( int channel, UCHAR *Server );
APIRET  psvCommand ( Buffer *outbuf, Buffer *inpbuf, int channel, BOOL getStat );

APIRET  psvRunProgram ( UCHAR Prog[], UCHAR Arg[] );
APIRET  psvGetVersion ( USHORT *ver );
APIRET  psvProcessList ( USHORT Drive, ProcessList psvPS[], int *psvPSsize );
APIRET  psvStopProgram ( UCHAR Prog[] );
APIRET  psvIsProgRunning ( UCHAR Prog[] );
APIRET  psvGetOwner ( UCHAR Owner[] );
APIRET  psvGetMachineType ( USHORT *ptype );
APIRET  psvGetMachineInfo ( MachineInfo *pInfo );
APIRET  psvSetTime ( psTime *pTime );

#endif
