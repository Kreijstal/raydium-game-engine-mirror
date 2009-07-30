/*
    Raydium - CQFD Corp.
    http://raydium.org/
    Released under both BSD license and Lesser GPL library license.
    See "license.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/log.h"
#endif

void raydium_console_line_add(char *format, ...);

void raydium_log(char *format, ...)
{
char str[RAYDIUM_MAX_NAME_LEN];
va_list argptr;
int retlen;
int i,offset;


va_start(argptr,format);
retlen = vsnprintf(str,RAYDIUM_MAX_NAME_LEN - 1, format,argptr);
va_end(argptr);

if(retlen < 0) retlen = 0;
str[retlen] = '\0';

printf("Raydium: %s\n",str);
if(raydium_log_file) fprintf(raydium_log_file,"%s\n",str);

raydium_parser_trim_right(str);
retlen=strlen(str);
offset=0;
for(i=0;i<retlen+1;i++)
	{
	if(str[i]=='\n' || str[i]==0)
		{
		str[i]=0;
		raydium_console_line_add("%s", str+offset);
		offset=i+1;
		}
	}
}
