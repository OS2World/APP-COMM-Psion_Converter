/*
     My Psion. Psion link program (Psion Series 5 (3)).
     RFSV layer - header file.

     (C) Andrei A. Porodko (porro@cbs-edu.chel.su).

     Based on my reading source-code by Rudolf Koenig
 (rfkoenig@immd4.informatik.uni-erlangen.de), Philip Proudman
 (former email, current address unknown proudman@btinternet.com)
 and Matt Gumbley (matt@gumbley.demon.co.uk)
*/
#ifndef _PSFILE
#define _PSFILE

#include <direct.h>

#pragma pack(1)

/*
    Simple constants
*/
#define _MAX_PS_FNAME   255
#define _MAX_PS_VNAME   31
#define _MAX_PS_DEVNAME 3
#define _DEF5_PATH      "C:\\"
#define _DEF3_PATH      "M:\\"
#define _DEF_PATH       (RFSVversion==PV_SERIES_3?_DEF3_PATH:_DEF5_PATH)
#define _MAX_PS_PATH    (_MAX_PS_DEVNAME+_MAX_PS_FNAME+1)
#define _MAX_DRIVE_LETTERS 26
#define _MAX_CMDBUF     1024
#define _MAX_IOBUF      max(_MAX_V3IOBUF,_MAX_V5IOBUF)


/*
    File seek modes.
*/
#define P_FABS                  1 // Position absolute from the start of file
#define P_FCUR                  2 // Position relative from the end of file
#define P_FEND                  3 // Position relative to the current pos
#define P_FRSENSE               4 // Sense the record position
#define P_FRSET                 5 // Set the record position
#define P_FREWIND               6 // Rewind a text file

/*
    defines for changing directories
*/
#define P_CD_ROOT               0 // goto root directory
#define P_CD_PARENT             1 // goto parent directory
#define P_CD_SUBDIR             2 // goto subdirectory

/*
    File system type. Flat, tree
*/
#define P_FSYSTYPE_FLAT         0
#define P_FSYSTYPE_TREE         1

/*
    Device status flags
*/
#define P_FMEDIA_UNKNOWN        0x0000
#define P_FMEDIA_FLOPPY         0x0001
#define P_FMEDIA_HARDDISK       0x0002
#define P_FMEDIA_FLASH          0x0003
#define P_FMEDIA_RAM            0x0004
#define P_FMEDIA_ROM            0x0005
#define P_FMEDIA_WRITEPROTECTED 0x0006
#define P_FMEDIA_COMPRESSIBLE   0x8000
#define P_FMEDIA_DYNAMIC_SIZE   0x4000
#define P_FMEDIA_INTERNAL       0x2000
#define P_FMEDIA_DUAL_DENSITY   0x1000
#define P_FMEDIA_FORMATTABLE    0x0800

/*
    Data structutes
*/

typedef struct _PSDEVINFO {
    USHORT   mediatype;
    USHORT   removable;
    ULONG    size;
    ULONG    free;
    UCHAR    volname[_MAX_PS_VNAME+1];
    USHORT   batterystate;
    UCHAR    dummy [16];
    UCHAR    name [_MAX_PS_DEVNAME+1];
    }    PSDEVINFO;

/*
    Independent data structutes.
*/
typedef struct _PSFHANDLE {
    ULONG    handle;
    ULONG volatile status;
    UCHAR    name [_MAX_PS_FNAME+1];
    Buffer   obuf;
    Buffer   ibuf;
    }    PSFHANDLE;

typedef struct _PS_SESSION {
    UCHAR  currentdir [_MAX_PS_PATH];
    USHORT volatile serialNumber;// not used in version 3 of Psion-protocol
    int volatile PsionError;     // error state for channel
    }    PS_SESSION;

typedef struct _PSFILEINFO {
    ULONG    attr;
    ULONG    size;
    ULONG    date;
    ULONG    type;
    UCHAR    name [_MAX_PS_FNAME+1];
    }    PSFILEINFO;

void      psSelectProtocol ( USHORT ver );

typedef struct _PSAPI {
    PSFHANDLE *(*Fopen) ( UCHAR Filename[], ULONG attr, APIRET *rc );
    PSFHANDLE *(*CreateFile) ( UCHAR Filename[], ULONG attrib, APIRET *rc );
    PSFHANDLE *(*ReplaceFile) ( UCHAR Filename[], ULONG attrib, APIRET *rc );
    PSFHANDLE *(*FopenDir) ( UCHAR Pattern[], ULONG attrib, APIRET *rc );
    PSFHANDLE *(*FopenDev) ( UCHAR Pattern[], APIRET *rc );
    APIRET    (*Fclose) ( PSFHANDLE *fd );
    APIRET    (*Fread) ( UCHAR *data, int *psize, PSFHANDLE *fd );
    APIRET    (*Fwrite) ( UCHAR *data, int *psize, PSFHANDLE *fd );
    APIRET    (*DirRead) ( PSFHANDLE *fd );
    APIRET    (*DirEntry) ( PSFHANDLE *fd, PSFILEINFO *fi );
    APIRET    (*DeviceRead) ( PSFHANDLE *fd, PSDEVINFO *fi );
    APIRET    (*Fflush) ( PSFHANDLE *fd );
    APIRET    (*GetFileInfo) ( UCHAR *Filename, PSFILEINFO *fi );
    APIRET    (*Delete) ( UCHAR *Filename );
    APIRET    (*MakeDir) ( UCHAR *dir );
    APIRET    (*GetDevInfo) ( UCHAR *Device, PSDEVINFO *fi );
    APIRET    (*Fseek) ( PSFHANDLE *fd, ULONG *poffset, ULONG mode );
    APIRET    (*SetFileDate) ( UCHAR *Filename, ULONG date );
    APIRET    (*SetFileStatus) ( UCHAR *Filename, ULONG s, ULONG us );
    APIRET    (*Rename) ( UCHAR *Oldname, UCHAR *NewName );
    APIRET    (*FsetEOF) ( PSFHANDLE *fd, ULONG offset );
    UCHAR    *(*Attr2str) ( ULONG fattr, UCHAR buf[] );
    APIRET    (*RemoveDir) ( UCHAR *dir );
    ULONG     (*GetFileType) ( UCHAR *Filename );
    }   PSAPI;

#define psFopen         (*cProt.Fopen)
#define psCreateFile    (*cProt.CreateFile)
#define psReplaceFile   (*cProt.ReplaceFile)
#define psFopenDir      (*cProt.FopenDir)
#define psFopenDev      (*cProt.FopenDev)
#define psFclose        (*cProt.Fclose)
#define psFread         (*cProt.Fread)
#define psFwrite        (*cProt.Fwrite)
#define psDirRead       (*cProt.DirRead)
#define psDirEntry      (*cProt.DirEntry)
#define psDeviceRead    (*cProt.DeviceRead)
#define psFflush        (*cProt.Fflush)
#define psGetFileInfo   (*cProt.GetFileInfo)
#define psDelete        (*cProt.Delete)
#define psMakeDir       (*cProt.MakeDir)
#define psRemoveDir     (*cProt.RemoveDir)
#define psGetDevInfo    (*cProt.GetDevInfo)
#define psFseek         (*cProt.Fseek)
#define psSetFileDate   (*cProt.SetFileDate)
#define psSetFileStatus (*cProt.SetFileStatus)
#define psRename        (*cProt.Rename)
#define psFsetEOF       (*cProt.FsetEOF)
#define psAttr2str      (*cProt.Attr2str)
#define psGetFileType   (*cProt.GetFileType)

APIRET  psReconnect ( void );

#include "ps3file.h"
#include "ps5file.h"

#define E_FILE_EOF   ((RFSVversion==PV_SERIES_3)?EV3_FILE_EOF:EV5_EOF)
#define E_FILE_NXIST ((RFSVversion==PV_SERIES_3)?EV3_FILE_NXIST:EV5_NOT_FOUND)
#define E_FILE_ALLOC ((RFSVversion==PV_SERIES_3)?EV3_FILE_ALLOC:EV5_NO_MEMORY)
#define P_FADIR      ((RFSVversion==PV_SERIES_3)?PV3_FADIR:PV5_DIRECTORY)
#define P_FAVOLUME   ((RFSVversion==PV_SERIES_3)?PV3_FAVOLUME:PV5_VOLUME)
#define P_FAPSION    (0x80000000)
#define P_ATTRIB     ((RFSVversion==PV_SERIES_3)?PV3_FADIR|PV3_FAHIDDEN|PV3_FASYSTEM:\
                                               PV5_HIDDEN|PV5_SYSTEM|PV5_DIRECTORY|0x10000000)


extern PSAPI volatile _Export cProt;   // current protocol API set
extern PSAPI volatile _Export v3Prot;
extern PSAPI volatile _Export v5Prot;
extern PS_SESSION _Export psSessions [_MAX_CHANNELS];

#define getRFSVerror()         (psSessions[rfsvCurrentChannel].PsionError)
#define resetRFSVerror()       (psSessions[rfsvCurrentChannel].PsionError=0)

#define getcPsionError(c)      (psSessions[(c)].PsionError)
#define resetcPsionError(c)    (psSessions[(c)].PsionError=0)
#define resetcSerialCounter(c) (psSessions[(c)].serialNumber=0)

#endif
