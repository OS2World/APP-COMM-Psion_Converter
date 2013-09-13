/*
     Psion I/O library header file.
     (C) Andrei A. Porodko (porro@cbs-edu.chel.su).

     Based on my reading source-code by Rudolf Koenig
 (rfkoenig@immd4.informatik.uni-erlangen.de), Philip Proudman
 (former email, current address unknown proudman@btinternet.com)
 and Matt Gumbley (matt@gumbley.demon.co.uk)
*/
#define INCL_BASE
#define INCL_DOS
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#define INCL_DOSSESMGR
#define INCL_SHLERRORS
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plpncp.h"
#include "psfile.h"
#include "psrpcs.h"
#include "pswprt.h"
#include "psiobj.h"

#define PSLIB_VERSION "Psion link library v1.51 (c) 1999-2002 Andrei A. Porodko"
