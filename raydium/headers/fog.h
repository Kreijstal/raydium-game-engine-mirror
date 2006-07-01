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

There are 3 types of fog. They are: 

    * Linear:

		      Far-z
		fog= --------- 
		     Far-Near

    * Exp:

		       (-density*z)
		fog= e^

    * Exp2:

		       (-density*z)^2
		fog= e^

Above ##z## is the distance to the calculated point from the camera.
As you can see, linear mode doesn't use ##Density##; and Exp & Exp2 modes don't
use near and far values. Remember that.
**/

__rayapi void raydium_fog_enable (void);
/**
Obvious
**/

__rayapi void raydium_fog_disable (void);
/**
Obvious
**/

__rayapi void raydium_fog_color_update (void);
/**
If you have modified ##raydium_background_color## array, you must 
call this function, applying the specified color to hardware.
See also: ##raydium_background_color_change##
**/

__rayapi void raydium_fog_mode_set (GLuint mode);
/**
The fog mode can be change with this function. There are 3 different ways to apply
the fog:
1. RAYDIUM_FOG_MODE_LINEAR - Used by default the fog is directly aplied according
the distance. Not real world fog, but used to avoid drawing too distant objects.
The most used in games focussed to lower-spec machines.
##IMPORTANT##: EXP mode ignores the ##density## value, only uses ##near## and ##far##.
2. RAYDIUM_FOG_MODE_EXP    - The fog grows exponentially with the distance. Usual
mist in the real world. 
##IMPORTANT##: EXP mode ignores the ##near## and ##far## values, only uses the ##density##.
3. RAYDIUM_FOG_MODE_EXP2   - The fog grows twice exponentially with the distance.
Used when the observer is inside a cloud/mist.
##IMPORTANT##: EXP2 mode ignores the ##near## and ##far## values, only uses the ##density##.
**/

__rayapi int raydium_fog_mode_get(void);
/**
Returns an integer with the current mode of the fog. See table below:
GL_LINEAR	0x2601	9729
GL_EXP		0x0800	2048
GL_EXP2		0x0801	2049
**/

__rayapi void raydium_fog_density_set(GLfloat density);
/**
Sets the density of the fog.
Useless if you are using LINEAR mode.
**/
__rayapi float raydium_fog_density_get(void);
/**
Returns the density of the fog.
**/
__rayapi void raydium_fog_near_set(GLfloat near);
/**
Sets the near point to apply the fog.
Useless if you are using EXP o EXP2 modes.
**/
__rayapi float raydium_fog_near_get(void);
/**
Returns de near point of the fog.
**/
__rayapi void raydium_fog_far_set(GLfloat far);
/**
Sets the far point of the fog.
Useless if you are using EXP o EXP2 modes.
**/
__rayapi float raydium_fog_far_get(void);
/**
Return the far point of the fog.
**/

__rayapi void raydium_fog_apply(void);
/**
Used to apply changes in your setup of fog.
Also is used to continue a previously stopped fog. See: raydium_fog_wait(void) below.
**/

__rayapi void raydium_fog_wait(void);
/**
With this funciont you can deactivate TEMPORALY the fog, but the internal state
of the fog in raydium won't change, so when you use raydium_fog_apply, the fog will continue
like it was before being stoped.
It's very usefull for certain rendering effects that need to stop the fog temporaly.
**/

#endif
