/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/log.h"
#endif 

void raydium_console_line_add(char *format, ...);

// need to be secured
void raydium_log(char *format, ...)
{
char str[RAYDIUM_MAX_NAME_LEN];
va_list argptr;


va_start(argptr,format);
vsprintf(str,format,argptr);
va_end(argptr);

printf("Raydium: %s\n",str);
if(raydium_log_file) fprintf(raydium_log_file,"%s\n",str);
raydium_console_line_add(str);
}
