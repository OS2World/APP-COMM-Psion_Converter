/*
    My Psion. Psion link program (Psion Series 5, 7, Notebook).
    WPRT layer.
*/
#ifndef _PSIWPRT
#define _PSIWPRT

#define PSW_INIT_PRINTER     0x00
#define PSW_GET_DATA         0xf0
#define PSW_CANCEL_JOB       0xf1
#define PSW_STOP_SERVER      0xff

#define WPRT_EOP             0xff
#define WPRT_PROTOCOL_VER    0x0002
#define _MAX_PRINT_BUF       2048
#define WPRT_PAGE_PREGAP     650

#define getWPRTerror()      (psSessions[wprtCurrentChannel].PsionError)
#define resetWPRTerror()    (psSessions[wprtCurrentChannel].PsionError=0)

extern int   volatile _Export wprtCurrentChannel;
extern char  _Export WprtConnectName [];
extern HAB   psHab;
extern HPS   psHpsPrn;
extern BOOL  psHpsInitialized;
extern ULONG psPrintThreadID;

#pragma pack(1)

typedef struct _ItemCmd {
        char  ident;
        APIRET (*handler) ( struct _ItemCmd *pCmd, FILE *Fp, HPS hpsPrn );
        int   size;
        }  ItemCmd;

extern ItemCmd ItemCmdList [];
extern int     totalItemCmd;
        
APIRET  wprtConnect ( void );
APIRET  wprtDisconnect ( void );
APIRET  pswInitPrinter ( void );
APIRET  pswGetData ( UCHAR *data, int *psize );
APIRET  pswCancelJob ( void );
void _System pswPrint ( ULONG Prn );
APIRET  pswCommand ( Buffer *outbuf, Buffer *inpbuf, 
                            int channel, BOOL getStatus );
APIRET  pswPrintJob ( int nJob, char TempJob[], HPS hpsPrn );
APIRET  pswRunPrint ( void );
void    pswInitItemTable ( void );

APIRET  pswhDefHandler ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhPageStart ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhPageEnd ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhRegion ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhFont ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhUnderline ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhStrikeout ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhNewline ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhCarreturn ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhForegroundColor ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhBackgroundColor ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhLine ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhFillRect ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhText ( ItemCmd *pcmd, FILE *Fp, HFILE Prn );
APIRET  pswhBitmap ( ItemCmd *pcmd, FILE *Fp, HPS hpsPrn );
APIRET  pswhPoligon ( ItemCmd *pcmd, FILE *Fp, HPS hpsPrn );
APIRET  pswhLabel ( ItemCmd *pcmd, FILE *Fp, HPS hpsPrn );
APIRET  pswhRemoveClip ( ItemCmd *pcmd, FILE *Fp, HPS hpsPrn );
APIRET  pswhEllipse ( ItemCmd *pcmd, FILE *Fp, HPS hpsPrn );
APIRET  pswhThickness ( ItemCmd *pcmd, FILE *Fp, HPS hpsPrn );

#define pswMm2Twips(v)    ((double)(v)*1440.0/25.4)
#define psw2Y(y)          ((long)pswMm2Twips(hcInfoDef.yTopClip)-(y))
#define LCID_NORMAL       1
#define FATTR_SEL_SUPERSCRIPT 0x1000
#define FATTR_SEL_SUBSCRIPT   0x2000
#define PSWH_TEXT_RIGHT   0x02000000
#define PSWH_TEXT_CENTER  0x01000000
#define TWIPS_PER_POINT   20

#endif
