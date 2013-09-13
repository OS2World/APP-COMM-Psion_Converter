#include <locale.h>
#include "psilib.h"
#include <time.h>

#define P_FADIR    ((RFSVversion==PV_SERIES_3)?PV3_FADIR:PV5_DIRECTORY)

#define VoidStub int(* _Optlink)(const void*,const void*)


int main ( int argc, char *argv[] )
{
    APIRET rc = 0;
    UCHAR  Name [_MAX_PS_PATH+1];
    register UCHAR *pname;
    register int i;    
    UCHAR  *comport;
    UCHAR  *baud;
    ULONG  autobaud;
    

if ( argc < 2 )
    pname = "MyPsion/2";
else
    {
    if ( strchr(argv[1],'*') == NULL )
        sprintf ( pname=Name, "%s\\%s", argv[1], psWildCard );
    else
        pname = argv[1];
    }
if ( (comport=getenv("PSICOM")) != NULL )
    strcpy ( psComName, comport );
if ( (baud=getenv("PSIBAUD")) != NULL )
    psComBaud = atol(baud);

rc = psInit ( psComName, psComBaud );

if ( rc == PERROR_LOCALE )
    {
    printf ( "\07Warning !!!. No national language translation.\n" );
    rc = 0;
    }
if ( rc == 0 )
    {
    printf ( "Serial port %s, baud %ld\n", psComName, psComBaud );
    if ( !(rc=runPlpLink()) )
        {
        if ( !(rc=rfsvConnect()) )
            {
            rc = psFolderList ( pname );
            if ( fpsListTotal && fpsList [0].name[1] == ':' )
                {
                printf ( "\nVolumes list of %s\n\n", pname );             
                for ( i = 0; i < fpsListTotal; i++ )
                    {
                    printf ( "%s %9ld\n", fpsList [i].name,fpsList [i].size );
                    }
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
                printf ( "\nDirectory list of %s\n\n", pname );             
                for ( i = 0; i < fpsListTotal; i++ )
                    {
                    printf ( "%-30s %9ld %s\n", toOsName(fpsList [i].name),fpsList [i].size,
                                       psAttr2str(fpsList[i].attr,Name) );
                    }
                }
            rfsvDisconnect();
            }
        }
    psStop ();
    }
if ( rc || getRFSVerror() )
    printf ( "\nLink error \"%d\", Psion Error = %d\n", rc, getRFSVerror() );
return ( rc );
}
