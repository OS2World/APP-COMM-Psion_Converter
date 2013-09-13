/*
    My Psion. Psion link program (Psion Series 5 (3)).
    User's interface layer.

    main.
*/
#include <psilib.h>


int   main ( int argc, char *argv[] )
{
        APIRET rc;

if ( argc < 3 )
    exit ( 0 );

strcpy ( psComName, "com2" );
psComBaud = 115200;

rc = psInit ( psComName, psComBaud );
if ( rc == PERROR_LOCALE )
    {
    printf ( "\07Warning !!!. No national language translation.\n" );
    }
if ( !(rc=runPlpLink()) )
    {
    if ( !(rc=rfsvConnect()) )
        {              // never don't forget to convert to Psion's codepage
        rc = psCopy2os(toPsiName(argv[1]), argv[2]);
        rfsvDisconnect();
        }
    DosPostEventSem ( psLinkSem );
    psStopSerialPort ( psComPort );
    }
return ( rc );
}
