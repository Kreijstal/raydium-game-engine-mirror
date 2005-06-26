#ifndef _LAND_H
#define _LAND_H

/*=
Land
1800
**/

// Introduction
/**
Historically, this file was quite complex, since Raydium was using
his own physic. Now, this file is almost empty, since ODE integration
now provides new landscape functions.
**/

extern GLfloat raydium_land_internal_landtmp (GLfloat x, GLfloat y, GLfloat phase, GLfloat ampl, GLfloat periode);
extern void raydium_land_draw_water (GLfloat phase, GLfloat ampl, GLfloat periode, int sub, GLfloat pas, char *texture);
extern GLfloat raydium_land_surface (GLfloat x, GLfloat y, GLfloat * nx, GLfloat * ny, GLfloat * nz);
#endif
