#include <locale.h>
#include <time.h>
#include "psilib.h"
#include "psrpcs.h"


int main ( int argc, char *argv[] )
{
    APIRET rc = 0;
    USHORT ver;
    static ProcessList psvPS[64];
    USHORT psvPSsize;
    static char Owner [128];
    register int i;
    static MachineInfo mInfo;
    time_t ctm;


if ( (rc=rpcConnect(NULL,0,NULL)) == 0 )
    {
    printf ( "RPCS server started\n" );
    if ( !(rc=psvGetVersion(&ver)) )
        {
        printf ( "RPCS version %d.%d\n", ver&0xFF, (ver>>8)&0xFF );
        if ( !(rc=psvGetMachineInfo(&mInfo)) )
              {
              printf ( "mInfo.Time.TimeLo  0x%08x\n"
                       "mInfo.Time.TimeHi  0x%08x\n",
                        mInfo.Time.TimeLo, mInfo.Time.TimeHi );
              time ( &ctm );
              psV5time2micro ( ctm, 
                               &mInfo.Time.TimeHi,
                               &mInfo.Time.TimeLo );
              rc = psvSetTime ( &mInfo.Time );
              printf ( "Time %s\n", rc?"isn't set":"is set" );
              }
         }
    rpcDisconnect();
    }
if ( rc || getRPCSerror() )
    printf ( "\nLink error \"%d\", Psion Error = %d\n", rc, getRPCSerror() );
return ( rc );
}

/*
               psvPSsize = sizeof(psvPS)/sizeof(psvPS[0]);
               rc = psvProcessList ( 'C', psvPS, &psvPSsize );
               printf ( "Total %d processes running on Psion\n", psvPSsize );
               for ( i = 0; !rc && i < psvPSsize; i++ )
                   printf ( "%-30s - %-30s\n", psvPS[i].Prog, psvPS[i].Argv );
               if ( !(rc=psvStopProgram(psvPS[0].Prog)) )
                   printf ( "Process %s stopped\n",psvPS[0].Prog );
               else
                   printf ( "Unable to stop process %s\n",psvPS[0].Prog );               

               if ( !(rc=psvRunProgram("c:\\Documents\\Word","")) )
                   printf ( "Program succesfully started.\n" );
                else
                   printf ( "Program not started. Reason code %ld\n", rc );

               if ( !(rc=psvGetMachineType(&ver)) )
                   printf ( "Machine type 0x%0lx\n", ver );

               if ( !(rc=psvGetMachineInfo(&mInfo)) )
                   {
                   printf ( "MachineType   0x%0lx\n"
                            "ROMversion    %u\n"
                            "ROMbuild      %u\n"
                            "MachineName   %s\n"
                            "Display       %dx%d\n"
                            "RAMsize       %lu\n"
                            "RAMmaxFree    %lu\n"
                            "RAMfree       %lu\n"
                            "RAMdiskSize   %lu\n"
                            "EPROM         %lu\n",
                            mInfo.MachineType,
                            mInfo.ROMversion,
                            mInfo.ROMbuild,
                            mInfo.MachineName,
                            mInfo.DisplayWidth, mInfo.DisplayHeight,
                            mInfo.RAMsize,
                            mInfo.ROMsize,
                            mInfo.RAMmaxFree,
                            mInfo.RAMfree,
                            mInfo.RAMdiskSize,
                            mInfo.EPROM );
                   }

               if ( !(rc=psvProcessList('C',psvPS,&psvPSsize)) )
                   {
                   printf ( "Total %d processes running on Psion\n", psvPSsize );
                   rc = psvIsProgRunning ( "sdsd" );
                   printf ( "Result of checking is %ld\n", rc );
                   }

               psvPSsize = sizeof(psvPS)/sizeof(psvPS[0]);
               rc = psvProcessList ( 'C', psvPS, &psvPSsize );
               printf ( "Total %d processes running on Psion\n", psvPSsize );
               for ( i = 0; !rc && i < psvPSsize; i++ )
                   printf ( "%-30s - %-30s\n", psvPS[i].Prog, psvPS[i].Argv );

               rc = psvGetOwner ( Owner );
               printf ( "Owner %s\n", Owner );
*/