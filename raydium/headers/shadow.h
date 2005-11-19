#ifndef _SHADOW__H
#define _SHADOW__H
#include "../shadow.h"

void raydium_shadow_init(void);
void raydium_shadow_enable(void);
void raydium_shadow_disable(void);
signed char raydium_shadow_isenabled(void);
void raydium_shadow_light_main(GLuint l);
void raydium_shadow_ground_change(int object);
void raydium_shadow_map_generate(void);
void raydium_shadow_map_render(void);
void raydium_shadow_object_draw(GLuint o);


#endif