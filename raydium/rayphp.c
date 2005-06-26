/*
    Raydium - CQFD Corp.
    http://raydium.cqfd-corp.org
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
