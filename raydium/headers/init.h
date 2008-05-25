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

__rayapi char *raydium_version(void);
/**
Return Raydium Engine version as a static string. Format is "x.yyy".
You can also find defines for this, named ##RAYDIUM_MAJOR## (x)
and ##RAYDIUM_MINOR## (yyy).
**/

__rayapi  void raydium_init_lights (void);
/**
Internal use. Must be moved to light.c.
**/

__rayapi  void raydium_init_objects (void);
/**
Internal use. Must be moved to object.c.
**/

__rayapi  void raydium_init_key (void);
/**
Internal use. Must be moved to key.c.
**/

__rayapi  void raydium_init_reset (void);
/**
This function is supposed to reset the whole Raydium engine: 
textures, vertices, lights, objects, ...
Never tested yet, and probaly fails for many reasons when called more than
one time.
**/

__rayapi  void raydium_init_engine (void);
/**
Internal use. **Never** call this function by yourself, it may cause
huge memory leaks.
**/

__rayapi int raydium_init_load(char *filename);
/**
This function is used to load a configuration file,##filename## ,and then it 
will do automatically all the process of initialization of the aplication.
The config file must have the pattern "variable=value", one variable per line. 
It allow comments.
An example(templante in fact) config file could be this one:
##

#IMPORTANT: LITERALS AND STRINGS MUST BE BETWEEN DOUBLE QUOTES

width=800 										#window width (default:  800)
height=600 										#window height (default:  600)
windowtype="window" 							#window style: window or fullscreen (default: window )
title="Application title"                       #title of the window,application (default:  Raydium application 0.1)
filter="aniso" 								    #texture filter: none, bilinear, trilinear, aniso (default:  trilinear)
fov=60 											#view angle, ie fov (default:  60)
near=0.001 										#distance to the nearets object(plane) draw	(default:  0.001)
far=2500 										#distance to the further   object(plane) draw (default:  2500)
fog="off" 										#fog: on/enable or off/disable (default:  off)
lighting="on" 									#lighting: on/enable or off/disable (default: on )
light0=0,50,150,200,1000000,1,0.9,0.7 			#light 0 parameters (default:  0,50,150,200,1000000,1,0.9,0.7)
background=1,0.9,0.7,1 							#background color (default:  1,0.9,0.7,1)
sky="normal" 									#sky method: none, normal, sphere (default:  normal) (NOT USED YET)
##
It returns 1 if the load process ends correctly, or 0 is something crash in the 
middle.

**/

#endif
