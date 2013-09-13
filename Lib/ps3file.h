/*
     My Psion. Psion link program (Psion Series 5 (3)).
     RFSV layer - header file.

     (C) Andrei A. Porodko (porro@cbs-edu.chel.su).

     Based on my reading source-code by Rudolf Koenig
 (rfkoenig@immd4.informatik.uni-erlangen.de), Philip Proudman
 (former email, current address unknown proudman@btinternet.com)
 and Matt Gumbley (matt@gumbley.demon.co.uk)

        File-server protocol V3.
    Each message consists in :
    COMMAND (word)  - command
    SIZE    (word)  - data size
    DATA    (bytes) - data for command.
*/
#ifndef _PSV3FILE
#define _PSV3FILE

#pragma pack(1)

#define _MAX_V3IOBUF      (_DEF_PACKETSIZE*3)
/*
    File protocol commands
*/
#define PSV3_FOPEN        0       // File Open
#define PSV3_FCLOSE       2       // File Close
#define PSV3_FREAD        4       // File Read
#define PSV3_FDIRREAD     6       // Read Directory entries
#define PSV3_FDEVICEREAD  8       // Device Information
#define PSV3_FWRITE       10      // File Write
#define PSV3_FSEEK        12      // File Seek
#define PSV3_FFLUSH       14      // Flush
#define PSV3_FSETEOF      16
#define PSV3_RENAME       18
#define PSV3_DELETE       20
#define PSV3_FINFO        22
#define PSV3_SFSTAT       24
#define PSV3_PARSE        26
#define PSV3_MKDIR        28
#define PSV3_OPENUNIQUE   30
#define PSV3_STATUSDEVICE 32
#define PSV3_PATHTEST     34
#define PSV3_STATUSSYSTEM 36
#define PSV3_CHANGEDIR    38
#define PSV3_SFDATE       40
#define PSV3_RESPONSE     42

/*
    File open modes
*/
#define PV3_FOPEN         0x0000  // Open file
#define PV3_FCREATE       0x0001  // Create file
#define PV3_FREPLACE      0x0002  // Replace file
#define PV3_FAPPEND       0x0003  // Append records
#define PV3_FUNIQUE       0x0004  // Unique file open
#define PV3_FSTREAM       0x0000  // Stream access to a binary file
#define PV3_FSTREAM_TEXT  0x0010  // Stream access to a text file
#define PV3_FTEXT         0x0020  // Record access to a text file
#define PV3_FDIR          0x0030  // Record access to a directory file
#define PV3_FFORMAT       0x0040  // Format a device
#define PV3_FDEVICE       0x0050  // Record access to device name list
#define PV3_FNODE         0x0060  // Record access to node name list
#define PV3_FUPDATE       0x0100  // Read and write access
#define PV3_FRANDOM       0x0200  // Random access
#define PV3_FSHARE        0x0400  // File can be shared


/*
    File attributes flags (field attr, struct PSFILEINFO).
*/
#define PV3_FAWRITE               0x01  // can the file be written to?
#define PV3_FAHIDDEN              0x02  // set if file is hidden
#define PV3_FASYSTEM              0x04  // set if file is a system file
#define PV3_FAVOLUME              0x08  // set if the name is a volume name
#define PV3_FADIR                 0x10  // set if file is a directory file
#define PV3_FAMOD                 0x20  // has the file been modified ?
#define PV3_FAREAD                0x100 // can the file be read ?
#define PV3_FAEXEC                0x200 // is the file executable ?
#define PV3_FASTREAM              0x400 // is the file a byte stream file ?
#define PV3_FATEXT                0x800 // is it a text file ?


typedef struct _PSV3PARSE {
    UCHAR    vlen;              // volume length ("REM::") in name
    UCHAR    dlen;              // disk length in name
    UCHAR    plen;              // path length in name
    UCHAR    flen;              // filename length
    USHORT   reserved;
    UCHAR    name [_MAX_PS_PATH];
    }    PSV3PARSE;

typedef struct _PSV3SYSINFO {
    USHORT   rfsv_ver;
    USHORT   fsys_type;
    USHORT   false;
    UCHAR    reserved [26];
    }    PSV3SYSINFO;


APIRET    psV3Command ( Buffer *outbuf, Buffer *inpbuf, int channel );
PSFHANDLE *psV3Fopen ( UCHAR Filename[], ULONG attrib, APIRET *rc );
PSFHANDLE *psV3FopenFile ( UCHAR Filename[], ULONG attrib, APIRET *rc );
PSFHANDLE *psV3CreateFile ( UCHAR Filename[], ULONG attrib, APIRET *rc );
PSFHANDLE *psV3ReplaceFile ( UCHAR Filename[], ULONG attrib, APIRET *prc );
PSFHANDLE *psV3FopenDir ( UCHAR Pattern[], ULONG attrib, APIRET *prc );
PSFHANDLE *psV3FopenDev ( UCHAR Pattern[], APIRET *prc );
APIRET    psV3Fclose ( PSFHANDLE *fd );
APIRET    psV3Fread ( UCHAR *data, int *psize, PSFHANDLE *fd );
APIRET    psV3Fwrite ( UCHAR *data, int *psize, PSFHANDLE *fd );
APIRET    psV3DirRead ( PSFHANDLE *fd );
APIRET    psV3DirEntry ( PSFHANDLE *fd, PSFILEINFO *fi );
APIRET    psV3DeviceRead ( PSFHANDLE *fd, PSDEVINFO *fi );
APIRET    psV3Fflush ( PSFHANDLE *fd );
APIRET    psV3GetFileInfo ( UCHAR *Filename, PSFILEINFO *fi );
APIRET    psV3ChangeDir ( UCHAR *base, UCHAR *dir, ULONG mode );
APIRET    psV3Delete ( UCHAR *Filename );
APIRET    psV3MakeDir ( UCHAR *dir );
APIRET    psV3GetDevInfo ( UCHAR *Device, PSDEVINFO *fi );
APIRET    psV3Fseek ( PSFHANDLE *fd, ULONG *poffset, ULONG mode );
APIRET    psV3SetFileDate ( UCHAR *Filename, ULONG date );
APIRET    psV3GetSystemInfo ( PSV3SYSINFO *si );
APIRET    psV3SetFileStatus ( UCHAR *Filename, ULONG status, ULONG ustatus );
APIRET    psV3Rename ( UCHAR *Oldname, UCHAR *NewName );
APIRET    psV3FsetEOF ( PSFHANDLE *fd, ULONG offset );
APIRET    psV3PathTest ( UCHAR *dir );
APIRET    psV3Parse ( UCHAR *Path, UCHAR *WildCard, UCHAR *Volume, PSV3PARSE *p );
UCHAR    *psV3Attr2str ( ULONG fattr, UCHAR buf[] );
ULONG    psV3GetFileType ( UCHAR *Filename );

ULONG    psV3getFileType ( UCHAR Ext[] );

/*
    Error codes for Psion-  file operations.
*/
#define EV3_FILE_EXIST            (-32)  /* File already exists */
#define EV3_FILE_NXIST            (-33)  /* File does not exist */
#define EV3_FILE_WRITE            (-34)  /* Write to file failed */
#define EV3_FILE_READ             (-35)  /* Read from file failed */
#define EV3_FILE_EOF              (-36)  /* Read past end of file */
#define EV3_FILE_FULL             (-37)  /* Disk/serial read buffer full */
#define EV3_FILE_NAME             (-38)  /* Invalid file name */
#define EV3_FILE_ACCESS           (-39)  /* Access denied */
#define EV3_FILE_LOCKED           (-40)  /* Record or file locked */
#define EV3_FILE_DEVICE           (-41)  /* Device does not exist */
#define EV3_FILE_DIR              (-42)  /* Directory does not exist */
#define EV3_FILE_RECORD           (-43)  /* Record too large */
#define EV3_FILE_RDONLY           (-44)  /* File has read only access */
#define EV3_FILE_INV              (-45)  /* Invalid I/O operation */
#define EV3_FILE_PENDING          (-46)  /* I/O pending (not yet completed) */
#define EV3_FILE_VOLUME           (-47)  /* Invalid volume */
#define EV3_FILE_CANCEL           (-48)  /* Async operation was cancelled */
#define EV3_FILE_ALLOC            (-10)  /* No memory for control block */
#define EV3_FILE_DISC             (-50)  /* link is disconnected */
#define EV3_FILE_CONNECT          (-51)  /* link already connected */
#define EV3_FILE_RETRAN           (-52)  /* Retransmission threshold exceeded */
#define EV3_FILE_LINE             (-53)  /* Physical link failure */
#define EV3_FILE_INACT            (-54)  /* Inactivity timer expired */
#define EV3_FILE_PARITY           (-55)  /* Serial parity error */
#define EV3_FILE_FRAME            (-56)  /* Serial framing error */
#define EV3_FILE_OVERRUN          (-57)  /* Serial overrun error */
#define EV3_MDM_CONFAIL           (-58)  /* modem can't connect to remote modem */
#define EV3_MDM_BUSY              (-59)  /* remote number busy */
#define EV3_MDM_NOANS             (-60)  /* there was no answer */
#define EV3_MDM_BLACKLIST         (-61)  /* number blacklisted by the modem*/
#define EV3_FILE_NOTREADY         (-62)  /* Drive not ready error */
#define EV3_FILE_UNKNOWN          (-63)  /* Unknown media in drive */
#define EV3_FILE_DIRFULL          (-64)  /* Root dir full error */
#define EV3_FILE_PROTECT          (-65)  /* Device write protect */
#define EV3_FILE_CORRUPT          (-66)  /* Media is corrupt */
#define EV3_FILE_ABORT            (-67)  /* User aborted operation error */
#define EV3_FILE_ERASE            (-68)  /* Failed to Erase Flash pack */
#define EV3_FILE_INVALID          (-69)  /* Invalid file for DBF system */

#endif
