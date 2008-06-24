/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/
#ifndef _SPRITES_H
#define _SPRITES_H
/*=
Sprites (viewer axis aligned 2D billboards)
2900
**/

// Introduction
/**
Raydium can use it's own sprite system.
Each sprite needs an .sprite file. This file is as this:
collision={0.8,0.7,0.8}

size=1

coords={0.0,0.25,0.0,0.25}
group={1,1}
texture="sprite1-test.tga"

The collision variables defines the size of an ODE box.
Size is the size of the displayed sprite. It won't affect the ODE object
, just the graphical stuff.

Then you have to define each frame of the sprite.
It's important to have in mind that the sprite can have "groups".
Those groups are used to team up some related sprites.
For example we can have 3 sprites of a forward movement, those could be
in one specific group.
In that way you can change from onu group to another with a raydium
function easily.
Even more. The frames of an sprite group will be animated automatically
and when the animation comes to the end of the group then you can chosee
what will be the next acction:
You can stop animation, you can restart the group animation or you can
jump to a new group.
You can indicate a "jump" to another group with
something like:
group={7,11} That would jump to the group 11
group={5,-1} -1 means STOP THE ANIMATION
group={3,-2} -2 means LOOP IN THE SAME GROUP


**/

//returns the first sprite id available
__rayapi int raydium_sprite_check_available(void);
/**
Internal use. 
**/

//function to draw directly the sprite. Internal. Don't use
__rayapi void raydium_sprite_billboard(float x, float y, float z,float ux, float uy, float uz, float rx, float ry, float rz, int textureid, float s0, float s1, float t0, float t1,float size);
/**
Internal use. 
**/
//Load an sprite (filename.sprite) preloading its textures.
//TODO:it should check if the sprite file is already loaded and if it's true
//TODO:clean file loading, ugly.
//then it should ignore the file load and get the texture id.

__rayapi int raydium_sprite_load(char *fichero,int etiqueta);

/**
This function allows you to load an sprite. It will returns the id(int)
of the new sprite.
**/

//get the name (char*) of an sprite from its related ODE object id
__rayapi char *raydium_sprite_get_name_from_object(int obj);
/**
You can get the name of an sprite with this function.
**/

//get the ODE object id from a certain sprite according its spriteid
__rayapi int raydium_sprite_object_get(int spriteid);
/**
Function to get the ODE object linked to the sprite.
**/

__rayapi void sprite_render_frame(float x, float y, float z, int spriteid,int frame,float scalex,float scaley);
/**
Internal use. 
**/

__rayapi void raydium_sprite_move(int sprite,float x, float y, float z);
/**
With this function you can move (in univsrese coordinates) an sprite
giving the 3 coordinates.
**/

__rayapi void raydium_sprite_move_relative(int sprite, float deltax, float deltay, float deltaz);
/**
Same than previous but it uses relative coordinates. Usefull for ingame
displacements.
**/

__rayapi void raydium_sprite_display(int id);
/**
This function will display the given sprite. Usually should be called in
the display callback.
**/

//function to change the animation group of one sprite
__rayapi void raydium_sprite_group_change(int sprite,int group);
/**
Function to change the animation group of a certain sprite.
**/

__rayapi void raydium_sprite_free(int sprite);
/**
Internal use. DONT'T USE.experimental
**/

__rayapi int raydium_sprite_copy(int other);
/**
Internal use. DONT'T USE.experimental
**/

__rayapi float *raydium_sprite_get_pos(int number);
/**
Returns a 3float array with the position (universe coordinates) of a
frame
**/

//function to change the type of one sprite
__rayapi int raydium_sprite_set_type(int id,int value);
/**
Internal use. 
**/

//function to change the name of one sprite
__rayapi int raydium_sprite_set_name(int id,char *cadena);
/**
With this function you can change the name of an sprite
**/

//function to return the id of one sprite according its name.
//NOTE: the default name of one sprite is the filename of its sprite.
__rayapi int raydium_sprite_find(char *name);
/**
This function will return the sprite id(int) of an sprite according its
name
**/
#endif

