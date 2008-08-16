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

//IMPORTANT: LITERALS AND STRINGS MUST BE BETWEEN DOUBLE QUOTES

//window width (default:  800)
width=800
//window height (default:  600)
height=600
//window style: window or fullscreen (default: window )
windowtype="window"
//title of the window,application (default:  Raydium application 0.1)
title="Raydium Application"
//texture filter: none, bilinear, trilinear, aniso (default:  trilinear)
filter="aniso"
//view angle, ie fov (default:  60)
fov=60
//distance to the nearets object(plane) draw   (default:  0.001)
near=0.001
//distance to the further   object(plane) draw (default:  2500)
far=2500
//fog: on/enable or off/disable (default:  off)
fog="off"
//lighting: on/enable or off/disable (default: on )
lighting="on"
//light 0 parameters (default:  0,50,150,200,1000000,1,0.9,0.7)
light0=0,50,150,200,1000000,1,0.9,0.7
//background color (default:  1,0.9,0.7,1)
background=1,0.9,0.7,1
//Fake HDR effect(default:off)
hdr="off"
//data can be foldered into "data" folders and its subfolders:
//"foldered" or anything else. (default:"foldered")
paths="foldered"
//Sky type: "box" or "dynamic" or "none"
sky="box"
##
It returns 1 if the load process ends correctly, or 0 is something crash in the
middle.

**/

#endif
