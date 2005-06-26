#ifndef _FOG_H
#define _FOG_H
/*=
Fog
500
**/

// Introduction
/**
Fog is usefull for two major reasons:

1. Realism: Just try, and you'll understand: 
amazing depth impression, no ?

2. Speed: For a correct fog effect (i'm talking 
about estetic aspect), you must bring near_clipping to a closer value,
reducing the overall number of triangles displayed at the same time. 
**/

extern void raydium_fog_enable (void);
/**
Obvious
**/

extern void raydium_fog_disable (void);
/**
Obvious
**/

extern void raydium_fog_color_update (void);
/**
If you have modified ##raydium_background_color## array, you must 
call this function, applying the specified color to hardware.
See also: ##raydium_background_color_change##
**/

extern void raydium_fog_mode (void);
/**
Do not use. Instable prototype.
**/

#endif
