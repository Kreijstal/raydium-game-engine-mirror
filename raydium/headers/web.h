#ifndef _WEB_H
#define _WEB_H

#include "../web.h"

/*=
HTTP Web Tools
4200
**/

void raydium_web_answer(char *message, int fd);
void raydium_web_request(int fd);
void raydium_web_start(char *title);
void raydium_web_callback(void);
void raydium_web_init(void);
void raydium_web_extension_add(char *ext, char *mime);
signed char raydium_web_client_get(char *filename);


#endif
