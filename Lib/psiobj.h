/*
     My Psion. Psion link program (Psion Series 5 (3)).
     User interface layer - header file.

     (C) Andrei A. Porodko (porro@cbs-edu.chel.su).
*/
#ifndef _PSIOBJ
#define _PSIOBJ

#pragma pack(1)

/*
    Interface objects desctoptions. Total four layers.

     Psion
      |
      ----> Drive
             |
             ---->Folder
                    |
                    ---->File
*/
typedef struct _fItem {
        UCHAR  name [_MAX_FNAME+1];
        ULONG  date;
        ULONG  attr;
        ULONG  size;
        ULONG  type;
        }      fItem;

extern fItem _Export myPsion;
extern fItem * _Export fpsList;         // list of Psion's objects 
                                        // disks/folders/files
extern int _Export   fpsListTotal;      // total objects in list.
extern UCHAR _Export psWildCard [];
extern CHAR _Export pcLocale [32];
extern CHAR _Export psionLocale [32];

APIRET psCopy2ps ( UCHAR srcName[], UCHAR dstName[] );
APIRET psCopy2os ( UCHAR srcName[], UCHAR dstName[] );
APIRET psInit ( char *comName, ULONG comBaud );
void   psStop ( void );
void   psSetLocale ( void );
APIRET psFolderList ( UCHAR pattern[] );
APIRET psCDir ( UCHAR path[] );
APIRET psPwd ( UCHAR path[] );
int    psGetLibVersion ( void );
BOOL   psCheckLibVersion ( char vString[] );
ULONG  toPsiTime ( FDATE *d, FTIME *t );
FDATE  *toOsDate ( ULONG t, FDATE *pt );
FTIME  *toOsTime ( ULONG t, FTIME *pd );
CHAR   *toPsiName ( CHAR fname[] );
CHAR   *toOsName ( CHAR fname[] );

#endif
