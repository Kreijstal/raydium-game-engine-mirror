/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/cli.h"

#endif 

int raydium_init_cli_option(char *option, char *value)
{
int i;
int found;
char full_option[RAYDIUM_MAX_NAME_LEN];
char head[3];

strcpy(full_option  ,"--");
strcpy(full_option+2,option);

found=0;

for(i=1;i<raydium_init_argc;i++)
    if(!strcasecmp(full_option,raydium_init_argv[i]))
	found=i;

if(!found) return 0; 	// option not found

if((found+1) >= raydium_init_argc)
    {
    if(value) strcpy(value,"");
    return 1;	// found, no value (last option of cli)
    }

strncpy(head,raydium_init_argv[found+1],3);
head[2]=0;

if(!strcasecmp(head,"--"))
    {
    if(value) strcpy(value,"");
    return 1;	// found, no value (value starts with -- : is a option)
    }

if(value) strcpy(value,raydium_init_argv[found+1]);
return 1;
}


int raydium_init_cli_option_default(char *option, char *value, char *default_value)
{
if(!raydium_init_cli_option(option,value))
    strcpy(value,default_value);
return 1;
}


void raydium_init_args(int argc, char **argv)
{
int i;
char logfile[RAYDIUM_MAX_NAME_LEN];

raydium_init_argc=argc;
raydium_init_argv=malloc(argc*sizeof(char *));

for(i=0;i<argc;i++)
    {
      raydium_init_argv[i]=malloc(strlen(argv[i])+1);
      strcpy(raydium_init_argv[i],argv[i]);
    }
raydium_log("Using Raydium %s",raydium_version);

if(raydium_init_cli_option("logfile",logfile))
    {
    raydium_log_file=fopen(logfile,"wt");
    if(!raydium_log_file) raydium_log("init: Warning: cannot open logfile (%s) with rw mode",logfile);
    }
else raydium_log_file=NULL;

raydium_log("command line args: OK");

#ifndef RAYDIUM_NETWORK_ONLY
raydium_file_dirname(raydium_init_wd,raydium_init_argv[0]);
if(!chdir(raydium_init_wd))
    raydium_log("chdir to '%s': OK",raydium_init_wd);
else
    perror("chdir");    
#endif
}
