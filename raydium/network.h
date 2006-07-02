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

#include "php_wrappers.c"

void raydium_init_args(int argc, char **argv);
int raydium_rayphp_repository_file_get(char *path);
signed char raydium_rayphp_http_test(void);
void raydium_console_init(void);
void raydium_php_init(void);
signed char raydium_network_init(void);
void raydium_network_internal_dump(void);
signed char raydium_network_server_create(void);
void raydium_random_randomize(void);
signed char raydium_parser_db_get(char *key, char *value, char *def);
void raydium_register_function(void *addr,char *name);

void raydium_network_only_quit(int sig)
{
raydium_network_internal_dump();
exit(0);
}

#ifdef PHP_SUPPORT
// do the minimal reg_api job (this should not be done like this ! :/)
// part 1
PHP_i_sss(raydium_parser_db_get);
#endif

void raydium_network_only_init(int argc, char **argv)
{
setbuf(stdout,NULL);
signal(SIGINT,raydium_network_only_quit);
raydium_init_args(argc,argv);
raydium_random_randomize();
raydium_console_init();
#ifdef PHP_SUPPORT
raydium_php_init();
// do the minimal reg_api job (this should not be done like this ! :/)
// part 2
raydium_register_function(C2PHP(raydium_parser_db_get),"raydium_parser_db_get");
#endif
raydium_network_init();
raydium_network_server_create();
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

char *raydium_version(void)
{
     return raydium_version_string;
}


#endif
#endif
