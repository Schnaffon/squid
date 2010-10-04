/*
 * Author:   Jens-S. V?ckler <voeckler@rvs.uni-hannover.de>
 * Ripped from Purge tool implementation.
 */
#include "config.h"
#include "psignal.h"

#if _SQUID_AIX_
extern const char* const sys_siglist[];
#define _sys_nsig 64
#define _sys_siglist sys_siglist
#endif

/// purpose: print message, colon, space, signal name and LF.
/// paramtr: sig (IN): signal number
///          msg (IN): message to prepend
void
psignal( int sig, const char* msg )
{
    if ( msg && *msg ) fprintf( stderr, "%s: ", msg );
    if ( sig > 0 && sig < _sys_nsig )
        fprintf( stderr, "%s\n", _sys_siglist[sig] );
    else
        fputs( "(unknown)\n", stderr );
}