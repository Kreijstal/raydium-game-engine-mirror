#ifndef _TRIGO_H
#define _TRIGO_H

/*=
Maths
200
*/

// Little introduction to trigo.c
/**
This section is mostly designed for internal uses, but provides some
usefull maths functions.
**/

extern GLfloat raydium_trigo_cos (GLfloat i);
/**
Obvious (degrees)
**/

extern GLfloat raydium_trigo_sin (GLfloat i);
/**
Obvious (degrees)
**/

extern GLfloat raydium_trigo_cos_inv (GLfloat i);
/**
Obvious (degrees)
**/

extern GLfloat raydium_trigo_sin_inv (GLfloat i);
/**
Obvious (degrees)
**/

#define raydium_trigo_abs(a) ( (a) < (0) ? (-a) : (a) )
/**
Obvious
**/

#define raydium_trigo_min(a,b) ( (a) < (b) ? (a) : (b) )
/**
Obvious
**/

#define raydium_trigo_max(a,b) ( (a) > (b) ? (a) : (b) )
/**
Obvious
**/

#define raydium_trigo_isfloat(a) ( (!isnan(a) && !isinf(a)) ? 1 : 0)
/**
Test two cases : "Not a Number" and "Infinite"
**/

extern void raydium_trigo_rotate (GLfloat * p, GLfloat rx, GLfloat ry, GLfloat rz, GLfloat * res);
/**
Rotate p (GLfloat * 3) by (rx,ry,rx) angles (degrees).
Result is stored in res (GLfloat * 3)
**/

extern void raydium_trigo_pos_to_matrix (GLfloat * pos, GLfloat * m);
/**
Generates a ODE style matrix (16 Glfloat) from pos (GLfloat * 3)
**/

extern void raydium_trigo_pos_get_modelview (GLfloat * res);
/**
Stores the current OpenGL MODELVIEW matrix in res (16 GLfloat)
**/

#endif
