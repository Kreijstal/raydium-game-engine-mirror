/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/rayphp.h"
#endif 

int raydium_rayphp_repository_file_get(char *path)
{
int status;
int force=0;
char file[RAYDIUM_MAX_NAME_LEN];

if(strlen(path)==0)
    {
    raydium_log("RayPHP: error: empty filename");
    return 0;
    }

strcpy(file,path);

if(raydium_init_cli_option("repository-force",NULL)) force=1;

raydium_register_variable(file,RAYDIUM_REGISTER_STR,"filename");
raydium_register_variable(&status,RAYDIUM_REGISTER_INT,"status");
raydium_register_variable(&force,RAYDIUM_REGISTER_INT,"force");

raydium_php_exec("rayphp/getfile.php");

raydium_register_variable_unregister_last();
raydium_register_variable_unregister_last();
raydium_register_variable_unregister_last();

if(status<0) status=0;

return status;
}


// put "path" file (and dependencies, if asked)
int raydium_rayphp_repository_file_put(char *path, int depends)
{
char file[RAYDIUM_MAX_NAME_LEN];
int status;

strcpy(file,path);
raydium_register_variable(file,RAYDIUM_REGISTER_STR,"filename");
raydium_register_variable(&status,RAYDIUM_REGISTER_INT,"status");
raydium_register_variable(&depends,RAYDIUM_REGISTER_INT,"depends");

raydium_php_exec("rayphp/putfile.php");

raydium_register_variable_unregister_last();
raydium_register_variable_unregister_last();
raydium_register_variable_unregister_last();

if(status<0) status=0;

return status;
}

int raydium_rayphp_repository_file_list(char *filter)
{
char *res;
int maxsize=1*1024*1024;
int status=0;
char ifilter[RAYDIUM_MAX_NAME_LEN];
int count=0;


res=malloc(maxsize);

if(!res)
    {
    raydium_log("rayphp: repository listing: malloc failed");
    return 0;
    }

strcpy(ifilter,filter);
raydium_register_variable(ifilter,RAYDIUM_REGISTER_STR,"filter");
raydium_register_variable(res,RAYDIUM_REGISTER_STR,"list");
raydium_register_variable(&status,RAYDIUM_REGISTER_INT,"status");
raydium_register_variable(&maxsize,RAYDIUM_REGISTER_INT,"size");

raydium_php_exec("rayphp/listrepos.php");

raydium_register_variable_unregister_last();
raydium_register_variable_unregister_last();
raydium_register_variable_unregister_last();
raydium_register_variable_unregister_last();

if(status)
    {
    int i,len;
    int last=0;
    
    len=strlen(res);
    
    for(i=0;i<len;i++)
	{
	if(res[i]=='\n')
	    {
	    res[i]=0;
	    if(strlen(res+last))
		{
		raydium_log("%s",res+last);
		count++;
		}
	    last=i+1;
	    }
	}
    }
raydium_log("%i file(s)",count);
free(res);
return status;
}

