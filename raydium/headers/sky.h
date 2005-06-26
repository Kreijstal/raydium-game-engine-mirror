#ifndef _SKY_H
#define _SKY_H

/*=
Sky and environement boxes
1900
**/

// Introduction
/**
Skyboxes are mostly automated.

For now, Raydium will use ##BOXfront.tga##, ##BOXback.tga##, ##BOXleft.tga##,
##BOXright.tga##, ##BOXbottom.tga## and ##BOXtop.tga## and will draw a 
skybox only if fog is disabled (this is not for technical reasons, 
but only for realism, just think about it ;)... but you can force 
skybox with fog using ##raydium_sky_force## if you really want).
**/


extern void raydium_sky_box_cache (void);
/**
As skybox texture are sometimes large files, you can pre-load skybox
with this function. If you don't do it, Raydium will load textures 
during the first frame of your application.
**/

extern void raydium_sky_box_render (GLfloat x, GLfloat y, GLfloat z);
/**
Internal use.
**/

#endif
