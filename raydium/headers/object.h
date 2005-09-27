#ifndef _OBJECT_H
#define _OBJECT_H
/*=
Objects
2300
**/

// Introduction
/**
With the following functions, you can easily draw and manage
mesh objects (.tri file).
**/

extern GLint raydium_object_find (char *name);
/**
Lookups an object by his ##name##. This function will return -1 if the
object's not found, and will not try to load the .tri file.
**/

extern signed char raydium_object_isvalid(int obj);
/**
Internal use, but you can call this function if you want to verify if an
object id is valid (in bounds).
**/

extern GLint raydium_object_find_load (char *name);
/**
Same as above (##raydium_object_load##), but will try to load object
**/

extern void raydium_object_reset (GLuint o);
/**
Internal use. Do not call.
**/

extern int raydium_object_load (char *filename);
/**
Load ##filename## as a .tri file, and returns corresponding id, or
-1 in case of error.
**/

extern void raydium_object_draw (GLuint o);
/**
Draws ##o## (index) object, using current matrixes.
**/

extern void raydium_object_draw_name (char *name);
/**
Same as above, but you only have to provide object's ##name## (".tri file"). 
If this object was not already loaded, this function will do it for you.
**/

extern void raydium_object_deform (GLuint obj, GLfloat ampl);
/**
Early devel state. Useless as is.
**/

extern void raydium_object_deform_name (char *name, GLfloat ampl);
/**
Early devel state. Useless as is.
**/

extern GLfloat raydium_object_find_dist_max (GLuint obj);
/**
This function will return will return the distance form (0,0,0) 
to the farest point of ##obj## object.
**/

extern void raydium_object_find_axes_max (GLuint obj, GLfloat * tx, GLfloat * ty, GLfloat * tz);
/**
This function returns the (maximum) size of the bounding box 
of ##obj## (relative to (0,0,0)).
**/


extern GLint raydium_object_anim_find(int object, char *name);
extern void raydium_object_anim_generate_internal(int object);
extern void raydium_object_anim_frame(int object, GLfloat frame);
extern void raydium_object_anim_frame_name(char *object, GLfloat frame);
extern void raydium_object_anim(int object, int anim);
extern void raydium_object_anim_name(char *object, char *anim);


#endif
