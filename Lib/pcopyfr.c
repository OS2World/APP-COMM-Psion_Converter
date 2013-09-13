#include <locale.h>
#include "psilib.h"
#include <io.h>
#include <sys\stat.h>



int main ( int argc, char *argv[] )
{
    APIRET rc = 0;
    UCHAR  *comport;
    UCHAR  *baud;
    UCHAR  *psicp;
    struct  stat Fstat;
    long   offset;
    PSFHANDLE *psFp;

if ( argc < 3 )
    {
    printf ( "Two arguments : Source file, dest file (Psion).\n" );
    return ( EOF );
    }

if ( (comport=getenv("PSICOM")) != NULL )
    strcpy ( psComName, comport );
if ( (baud=getenv("PSIBAUD")) != NULL )
    psComBaud = atol(baud);
if ( (psicp=getenv("PSICP")) != NULL )
    strcpy ( psionLocale, psicp );

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
            printf ( "Connected. Serial port %s, baud %ld\n", psComName, psComBaud );            
            printf ( "Copying %s to Psion: %s\n", argv[1], argv[2] );
            psCopy2ps ( argv[1], argv[2] );
            rfsvDisconnect();
            }
        }
    psStop ();
    }
if ( rc || getRFSVerror() )
    printf ( "\nLink error \"%d\", Psion Error = %d\n", rc, getRFSVerror() );
return ( rc );
}
