/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/signal.h"
#endif

void raydium_signal_handler(int sig)
{
raydium_log("SIGINT disabled.",sig); // sig not used (compilo warning)
}

void raydium_signal_install_trap(void)
{

if(signal(SIGINT,raydium_signal_handler)==SIG_ERR)
    raydium_log("Signal Handler: FAILED !");
else
    raydium_log("Signal Handler: OK");
}
