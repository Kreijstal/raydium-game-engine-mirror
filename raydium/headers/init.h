#ifndef _INIT_H
#define _INIT_H
/*=
Initialization
2400
**/

// Introduction
/**
This file is mainly designed for internal uses, but there's anyway 
some interesting functions. 
**/

extern void raydium_init_lights (void);
/**
Internal use. Must be moved to light.c.
**/

extern void raydium_init_objects (void);
/**
Internal use. Must be moved to object.c.
**/

extern void raydium_init_key (void);
/**
Internal use. Must be moved to key.c.
**/

extern void raydium_init_reset (void);
/**
This function is supposed to reset the whole Raydium engine: 
textures, vertices, lights, objects, ...
Never tested yet, and probaly fails for many reasons when called more than
one time.
**/

extern void raydium_init_engine (void);
/**
Internal use. **Never** call this function by yourself, it may cause
huge memory leaks.
**/

#endif
