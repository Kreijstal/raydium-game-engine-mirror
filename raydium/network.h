/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

// WARNING: this file is for "RAYDIUM_NETWORK_ONLY" applications only,
// providing a small subset of needed Raydium API.

#ifndef NETWORK_H
#define NETWORK_H
#ifdef RAYDIUM_NETWORK_ONLY

int raydium_rayphp_repository_file_get(char *path);

FILE *raydium_file_fopen(char *file, char *mode)
{
FILE *fp;

fp=fopen(file,mode);
if(fp)
    return fp;

raydium_rayphp_repository_file_get(file);

fp=fopen(file,mode);
if(fp)
    return fp;

return NULL;
}

int raydium_init_cli_option(char *option, char *value)
{
return 0;
}


#endif
#endif
