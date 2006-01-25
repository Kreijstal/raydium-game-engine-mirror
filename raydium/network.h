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
void raydium_console_init(void);
void raydium_php_init(void);
signed char raydium_network_init(void);
void raydium_network_internal_dump(void);
signed char raydium_network_server_create(void);

void raydium_network_only_quit(int sig)
{
raydium_network_internal_dump();
exit(0);
}

void raydium_network_only_init(void)
{
setbuf(stdout,NULL);
signal(SIGINT,raydium_network_only_quit);
raydium_console_init();
#ifdef PHP_SUPPORT
raydium_php_init();
#endif
raydium_network_init();
raydium_network_server_create();
}


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

int raydium_init_cli_option_default(char *option, char *value,char *def)
{
strcpy(value,def);
return 1;
}


void raydium_osd_color_ega(char hexa)
{
//
}

void raydium_osd_start(void)
{
//
}

void raydium_osd_stop(void)
{
//
}

void raydium_osd_printf(GLfloat x, GLfloat y, GLfloat size, GLfloat spacer,char *texture, char *format, ...)
{
//
}

GLuint raydium_texture_find_by_name(char *name)
{
return 0;
}

signed char raydium_texture_current_set(GLuint current)
{
return 0;
}

signed char raydium_texture_current_set_name(char *name)
{
return 0;
}

void raydium_rendering_internal_prepare_texture_render(GLuint tex)
{
//
}


#endif
#endif
