/*
     My Psion. Psion link program (Psion Series 5 (3)).
     RFSV layer - header file.

     (C) Andrei A. Porodko (porro@cbs-edu.chel.su).

     Based on my reading source-code by Rudolf Koenig
 (rfkoenig@immd4.informatik.uni-erlangen.de), Philip Proudman
 (former email, current address unknown proudman@btinternet.com)
 and Matt Gumbley (matt@gumbley.demon.co.uk)

        File-server protocol V5.
    Each message consists in :
    COMMAND (word)  - command
    SERNUM  (word)  - packet's serial number
    DATA    (bytes) - data for command. 
*/
#ifndef _PSV5FILE
#define _PSV5FILE

#pragma pack(1)

#define _MAX_V5IOBUF      2048
/*
    File protocol commands
*/
#define PSV5_FCLOSE       0x01    // File Close
#define PSV5_OPENDIR      0x10    // Open directory list
#define PSV5_RESPONSE     0x11
#define PSV5_FDIRREAD     0x12    // Read Directory entries
#define PSV5_GETDRIVELIST 0x13    // Open devices list
#define PSV5_FDEVINFO     0x14    // Device Information
#define PSV5_SETVOLNAME   0x15
#define PSV5_FOPEN        0x16
#define PSV5_TEMPFILE     0x17
#define PSV5_FREAD        0x18
#define PSV5_FWRITE       0x19
#define PSV5_FSEEK        0x1a
#define PSV5_DELETE       0x1b
#define PSV5_REMOTE_ENTRY 0x1c
#define PSV5_FFLUSH       0x1d
#define PSV5_SETEOF       0x1e
#define PSV5_RENAME       0x1f
#define PSV5_MKDIR        0x20
#define PSV5_RMDIR        0x21
#define PSV5_SETATTR      0x22
#define PSV5_GETATTR      0x23
#define PSV5_SETMODIFIED  0x24
#define PSV5_ISMODIFIED   0x25
#define PSV5_SETPATH      0x26
#define PSV5_GETPATH      0x27
#define PSV5_RWFILE       0x28
#define PSV5_CREATE_FILE  0x29
#define PSV5_REPLACE_FILE 0x2a
#define PSV5_PATH_TEST    0x2b
#define PSV5_LOCK         0x2d
#define PSV5_UNLOCK       0x2e
#define PSV5_OPEN_DIR_UID 0x2f
#define PSV5_GETDRV_NAME  0x30
#define PSV5_SETDRV_NAME  0x31
#define PSV5_REPLACE      0x32

/*
    File open modes
*/
#define PV5_FEXCLUSIVE    0x0000
#define PV5_FSHARE_READ   0x0001
#define PV5_FSHARE_ANY    0x0002
#define PV5_FBINARY       0x0000
#define PV5_FTEXT         0x0020
#define PV5_FRANDOM       0x0200

/*
    File attributes flags (field attr, struct PSFILEINFO).
*/
#define PV5_RONLY                 0x0001 // if read-only file
#define	PV5_HIDDEN                0x0002 // hidden file
#define PV5_SYSTEM                0x0004 // system file
#define	PV5_DIRECTORY             0x0010 // if directory entry 
#define PV5_ARCHIVE               0x0020 // file archived
#define PV5_VOLUME                0x0040 // if volume name
#define	PV5_NORMAL                0x0080 // file normal
#define PV5_TEMPORARY             0x0100 // temporary file
#define	PV5_COMPRESSED            0x0800 // file compressed


APIRET    psV5Command ( Buffer *outbuf, Buffer *inpbuf, int channel );
PSFHANDLE *psV5FopenDir ( UCHAR Pattern[], ULONG attrib, APIRET *prc );
UCHAR     *psV5Attr2str ( ULONG fattr, UCHAR buf[] );
APIRET    psV5Fclose ( PSFHANDLE *fd );
APIRET    psV5Fflush ( PSFHANDLE *fd );
APIRET    psV5DirRead ( PSFHANDLE *fd );
APIRET    psV5DirEntry ( PSFHANDLE *fd, PSFILEINFO *fi );
PSFHANDLE *psV5FopenDev ( UCHAR Pattern[], APIRET *prc );
APIRET    psV5DeviceRead ( PSFHANDLE *fd, PSDEVINFO *fi );
APIRET    psV5GetDevInfo ( UCHAR *Device, PSDEVINFO *fi );
PSFHANDLE *psV5FopenFile ( UCHAR Filename[], ULONG attrib, APIRET *rc );
PSFHANDLE *psV5CreateFile ( UCHAR Filename[], ULONG attrib, APIRET *rc );
PSFHANDLE *psV5ReplaceFile ( UCHAR Filename[], ULONG attrib, APIRET *prc );
APIRET    psV5Fread ( UCHAR *data, int *psize, PSFHANDLE *fd );
APIRET    psV5Fwrite ( UCHAR *data, int *psize, PSFHANDLE *fd );
APIRET    psV5GetFileInfo ( UCHAR *Filename, PSFILEINFO *fi );
APIRET    psV5MakeDir ( UCHAR *dir );
APIRET    psV5RemoveDir ( UCHAR *dir );
APIRET    psV5Delete ( UCHAR *Filename );
APIRET    psV5Fseek ( PSFHANDLE *fd, ULONG *poffset, ULONG mode );
APIRET    psV5Rename ( UCHAR *Oldname, UCHAR *NewName );
APIRET    psV5FsetEOF ( PSFHANDLE *fd, ULONG offset );
APIRET    psV5SetFileStatus ( UCHAR *Filename, ULONG status, ULONG ustatus );
ULONG     psV5micro2time ( ULONG microHigh, ULONG microLow );
void      psV5time2micro ( ULONG time, ULONG *microHigh, ULONG *microLow );
APIRET    psV5SetFileDate ( UCHAR *Filename, ULONG date );
ULONG     psV5GetFileType ( UCHAR *Filename );

ULONG     psV5getFileType ( ULONG UID[3] );

#define storeSerialNum(buf,c) {storeWord((buf),psSessions[(c)].serialNumber);\
                               if(psSessions[(c)].serialNumber < 0xffff)\
                                 ++psSessions [(c)].serialNumber;\
                               else\
                                   psSessions [(c)].serialNumber=0;}

QWORD _Optlink _ulplus ( QWORD f, QWORD s );
QWORD _Optlink _ulminus  ( QWORD f, QWORD s );
QWORD _Optlink _ulshll ( QWORD v, ULONG arg );   // logical left shift
QWORD _Optlink _ulshrl ( QWORD v, ULONG arg );   // logical right shift
QWORD _Optlink _ulmul ( QWORD f, QWORD s );
QWORD _Optlink _uldiv ( QWORD dev, QWORD divisor );

/*
    Error codes for Psion-  file operations.
*/
#define EV5_NONE                        (0)
#define	EV5_NOT_FOUND                   (-1)
#define EV5_GENERAL                     (-2)
#define EV5_CANCEL                      (-3)
#define EV5_NO_MEMORY                   (-4)
#define EV5_NOT_SUPPORTED               (-5)
#define EV5_ARGUMENT                    (-6)
#define EV5_TOTAL_LOSS_OF_PRECISION     (-7)
#define EV5_BAD_HANDLE                  (-8)
#define EV5_OVERFLOW                    (-9)
#define EV5_UNDERFLOW                   (-10)
#define	EV5_ALREADY_EXISTS              (-11)
#define EV5_PATH_NOT_FOUND              (-12)
#define EV5_DIED                        (-13)
#define EV5_IN_USE                      (-14)
#define EV5_SERVER_TERMINATED           (-15)
#define EV5_SERVER_BUSY                 (-16)
#define EV5_COMPLETION                  (-17)
#define EV5_NOT_READY                   (-18)
#define EV5_UNKNOWN                     (-19)
#define EV5_CORRUPT                     (-20)
#define EV5_ACCESS_DENIED               (-21)
#define EV5_LOCKED                      (-22)
#define EV5_WRITE                       (-23)
#define EV5_DISMOUNTED                  (-24)
#define EV5_EOF                         (-25)
#define EV5_DISK_FULL                   (-26)
#define EV5_BAD_DRIVER                  (-27)
#define EV5_BAD_NAME                    (-28)
#define EV5_COMMS_LINE_FAIL             (-29)
#define EV5_COMMS_FRAME                 (-30)
#define EV5_COMMS_OVERRUN               (-31)
#define EV5_COMMS_PARITY                (-32)
#define EV5_TIMEOUT                     (-33)
#define EV5_COULD_NOT_CONNECT           (-34)
#define EV5_COULD_NOT_DISCONNECT        (-35)
#define EV5_DISCONNECTED                (-36)
#define EV5_BAD_LIBRARY_ENTRY_POINT     (-37)
#define EV5_BAD_DESCRIPTOR              (-38)
#define EV5_ABORT                       (-39)
#define EV5_TOO_BIG                     (-40)
#define EV5_DIVIDE_BY_ZERO              (-41)
#define EV5_BAD_POWER                   (-42)
#define EV5_DIR_FULL                    (-43)

#endif
