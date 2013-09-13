#include <locale.h>
#include "psilib.h"
#include <io.h>
#include <sys\stat.h>


int main ( int argc, char *argv[] )
{
    APIRET rc = 0;
    UCHAR  *comport;
    UCHAR  *baud;
    struct  stat Fstat;

if ( argc < 3 )
    {
    printf ( "Two arguments : Source file (Psion), dest file.\n" );
    return ( EOF );
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
            printf ( "Copying %s from Psion to %s\n", argv[1], argv[2] );
            psCopy2os ( argv[1], argv[2] );
            rfsvDisconnect();
            }
        }
    psStop ();
    }
if ( rc || getRFSVerror() )
    printf ( "\nLink error \"%d\", Psion Error = %d\n", rc, getRFSVerror() );
return ( rc );
}
