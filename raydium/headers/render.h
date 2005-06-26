#ifndef _RENDER_H
#define _RENDER_H
/*=
Rendering
1300
**/

// Introduction
/*=
render.c contains Raydium rendering core, so only "public" and 
interesting function will be documented.

It' obvious for me that many parts of this code have to be 
rewritten (tips: slow, buggy, old, ... :) 
**/


extern int  raydium_rendering_prepare_texture_unit (GLenum tu, GLuint tex);
/**
This function will "prepare" hardawre texture unit ##tu## to render ##tex## texture.
There almost no reason to call this function by yourself.
**/

extern void raydium_rendering_internal_prepare_texture_render (GLuint tex);
/**
Same as above, but for texture unit #0 only.
**/

extern void raydium_rendering_internal_restore_render_state (void);
/**
Internal. Deprecated.
**/

// DO NOT DOCUMENT THIS ... THING !
extern char infov (GLfloat x, GLfloat y);

extern void raydium_rendering_from_to (GLuint from, GLuint to);
/**
Renders vertices from ##from## to ##to##.
Using object management functions is a better idea.
**/

extern void raydium_rendering (void);
/**
Renders all vertices (probably useless, now).
**/

extern void raydium_rendering_finish (void);
/**
You must call this function at the end of each frame. This will flush all
commands to hardware, fire a lot off callbacks, and prepare next frame.
**/

extern void raydium_rendering_wireframe (void);
/**
Switch to wireframe rendering.
**/

extern void raydium_rendering_normal (void);
/**
Switch back to standard rendering.
**/

extern void raydium_rendering_rgb_force (GLfloat r, GLfloat g, GLfloat b);
/**
Force all RGB colored vertices to take ##(r,g,b)## color. One example of this
use is for making "team colored" cars : Do not apply textures to some faces
while modelling, and force to team color each time you render a car.
**/

extern void raydium_rendering_rgb_normal (void);
/**
Disable "rgb force" state. See above.
**/

extern void raydium_rendering_displaylists_disable(void);
/**
Disable display lists usage.
Some old video cards and broken drivers may get better performances WITHOUT
display lists (on large objects, mainly).
**/

extern void raydium_rendering_displaylists_enable(void);
/**
Enable display lists usage. default state.
**/

#endif
