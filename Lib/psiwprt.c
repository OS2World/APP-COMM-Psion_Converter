#include <conio.h>
#include <locale.h>
#include "psilib.h"
#include "pswprt.h"

static char PsiLibVer [] = PSLIB_VERSION;
#define P_FADIR    ((RFSVversion==PV_SERIES_3)?PV3_FADIR:PV5_DIRECTORY)

#define VoidStub int(* _Optlink)(const void*,const void*)

int main ( int argc, char *argv[] )
{
    APIRET rc = 0;
    UCHAR  Name [_MAX_PS_PATH+1];
    register UCHAR *pname;
    register int c,i;
    UCHAR  *comport;
    UCHAR  *baud;
    FILE   *Fp;
    ULONG  pswThreadID;


printf ( "PsiLib version 0x%02x\n", psGetLibVersion() );

printf ( "PsiLib version is %s\n", psCheckLibVersion(PsiLibVer)?"correct":"wrong" );

if ( (comport=getenv("PSICOM")) != NULL )
    strcpy ( psComName, comport );
else
    strcpy ( psComName, "COM2" );

if ( (baud=getenv("PSIBAUD")) != NULL )
    psComBaud = atol(baud);
else
    psComBaud = 115200;

printf ( "Serial port %s, baud %ld\n", psComName, psComBaud );

rc = psInit ( psComName, psComBaud );

printf ( "Psion locale %s, PC locale %s\n", psionLocale, pcLocale );

if ( rc == PERROR_LOCALE )
    {
    printf ( "\07Warning !!!. No national language translation.\n" );
    rc = 0;
    }
if ( rc == 0 )
    {
    if ( argc > 1 )
        {
        printf ( "Print already prepared job\n" );
        pswRunPrint (); 
        rc = pswPrintJob ( 1, argv[1], psHpsPrn );
        wprtDisconnect();
        }
    else
        {
        if ( !(rc=runPlpLink()) )
            {
            if ( !(rc=rfsvConnect()) )
                {
                if ( !(rc=rpcsConnect()) )
                    {
                    if ( !(rc=wprtConnect()) )
                        {
                        if ( (rc=pswInitPrinter()) == 0 )
                            {
                            printf ( "Capture new job (ESC to interrupt process)\n" );
                            if ( (rc=pswRunPrint()) == 0 )
                                 {
                                     char CmdLine [80];

                                 cputs ( "Directory for ?" );
                                 for ( i = 0; ; )
                                     {
                                     if ( kbhit() != 0 )
                                         {
                                         c = getch();
                                         if ( c == 27 )
                                             break;
                                         if ( c == 13 )
                                             {
                                             CmdLine [i] = '\0';
                                             rc = psFolderList ( CmdLine );
                                             if ( fpsListTotal && fpsList [0].name[1] == ':' )
                                                 {
                                                 printf ( "\nVolumes list of %s\n\n", CmdLine );             
                                                 for ( i = 0; i < fpsListTotal; i++ )
                                                     printf ( "%s %9ld\n", fpsList [i].name,fpsList [i].size );
                                                 }
                                             else
                                                 {
                                                 for ( i = 0; i < fpsListTotal; i++ )
                                                     {
                                                     if ( (fpsList [i].attr&P_FADIR) )
                                                         strupr ( fpsList [i].name );
                                                     else
                                                         strlwr ( fpsList [i].name );
                                                     }
                                                 qsort(fpsList,fpsListTotal,sizeof(fItem),(VoidStub)strcmp);
                                                 printf ( "\nDirectory list of %s\n\n", CmdLine );             
                                                 for ( i = 0; i < fpsListTotal; i++ )
                                                     {
                                                     printf ( "%-30s %9ld %s\n", toOsName(fpsList [i].name),fpsList [i].size,
                                                     psAttr2str(fpsList[i].attr,Name) );
                                                     }
                                                 }
                                             i = 0;                                             
                                             cputs ( "Directory for ?" );
                                             }
                                         else
                                             {
                                             CmdLine [i++] = c;
                                             putch ( c );
                                             }
                                         }
                                     DosSleep ( 10 );
                                     }
                                 }
                            }
                        wprtDisconnect();
                        }
                   rpcsDisconnect();
                   }
                rfsvDisconnect();
                }
            }
        }
    psStop ();
    }
if ( rc || getcPsionError(wprtCurrentChannel) )
    printf ( "\nLink error \"%d\", Psion Error = %d\n", rc, getcPsionError(wprtCurrentChannel) );
return ( rc );
}
