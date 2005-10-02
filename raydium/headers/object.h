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
Lookups an object by its ##name##. This function will return -1 if the
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

// Animations
/**
Raydium now supports mesh animation, thru MD2 (Quake 2) files. Raydium file
format was extended to version 2. If you want to create an animated mesh
for Raydium from a MD2 file, you may use Blender with "import-md2-0.14.py" 
script ( by Bob Holcomb, http://67.22.114.230:8082/programming/blender/index.html ) 
and export it back to a tri file using provided "triEXP-MD2-*.py" script.
All other tasks (loading, transformations, ...) are done the same way as
regular static mesh.

For Raydium, an animation is a set of "anims", and each "anim" is a set 
of "frames". Each "anim" gets its own name (see header of a version 2 file
for more informations), and since an animated object may be use for many 
players, Raydium provides an "instances" based system: setting things like
anim and frame for an object is done only for one instance of this object.
Instances are always available, no need to create or declare them.
That's all you need to use animation simple API.
**/

extern GLint raydium_object_anim_find(int object, char *name);
/**
Lookups an animation by its ##name##. This function will return -1 if the
animation's not found. Mostly for internal use.
**/

extern void raydium_object_anim_generate_internal(int object, int instance);
/**
Internal. Transformed mesh generation.
**/

extern void raydium_object_anim_frame(int object, int instance, GLfloat frame);
/**
Sets current ##frame## for one ##instance## of ##object##. ##frame## is
automatically bounded.
**/

extern void raydium_object_anim_frame_name(char *object, int instance, GLfloat frame);
/**
Same as above, but using ##object##'s name.
**/

extern void raydium_object_anim(int object, int instance, int anim);
/**
Sets current ##anim## for one ##instance## of ##object##.
**/

extern void raydium_object_anim_name(char *object, int instance, char *anim);
/**
Same as above, but using ##object##'s name and ##anim##'s name.
**/

extern void raydium_object_anim_instance(int object, int instance);
/**
With this function, you must set what instance will be drawn when
##raydium_object_draw()## will be called with ##object## argument.

Default is set to instance 0.
**/

extern void raydium_object_anim_instance_name(char *object, int instance);
/**
Same as above, but using ##object##'s name.
**/

#endif
