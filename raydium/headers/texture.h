#ifndef _TEXTURE_H
#define _TEXTURE_H
/*=
Textures
1200
**/

// Introduction
/**
For now, Raydium only handles TGA uncompressed texture.
As explainded in the first part of this guide, Raydium provides three 
texture filters (none, bilinear, trilinear using MipMaps ).

Texture sizes must be a power of two, 8 (alpha mask), 24 (RGB) or 32 (RGBA) bits.

Raydium now supports materials with a simple "rgb(r,g,b)" string 
as texture name, where r, g and b are 0 <= x <= 1 (floats).
With 3 negative values, you will generate a "phantom texture". Phantom textures
are only drawn into the z-buffer (and not color buffer).
Texture clamping and multitexturing are supported by Raydium, but not
documented here for now. If you're interested, have a look at source code, or
take a look at the Wiki.

Tips: "BOX_", ";", "|".
**/


extern char raydium_texture_size_is_correct (GLuint size);
/**
Returns true if ##size## is a correct texture size, depending of
hardware capacities and "power of 2" constraint.
**/

extern GLuint raydium_texture_load_internal(char *filename, char *as, char faked, int live_id_fake);
/**
Internal use.
**/

extern GLuint raydium_texture_load (char *filename);
/**
Loads "filename" texture into hardware memory. Function results 
texture index, but in most cases, you can identify later a texture 
by his name, without providing his index, so you can probably ignore 
this value.

0 is returned if texture loading have failed.
**/

extern GLuint raydium_texture_load_erase (char *filename, GLuint to_replace);
/**
Same as above, but ##to_replace## texture (index) is erased with ##filename##.
**/

extern char raydium_texture_current_set (GLuint current);
/**
Switch active texture to "current" index. Mostly used for runtime object 
creation:
"set current texture, add vertices, set another texture, 
add vertices, ... and save all to an objet" 
(see below for vertices management).
**/

extern char raydium_texture_current_set_name (char *name);
/**
Same as above, but using texture name. This function will load ##name## 
if not alread done.
**/

extern GLuint raydium_texture_find_by_name (char *name);
/**
Returns index for texture "name", and load it if not already done.
**/

extern void raydium_texture_filter_change (GLuint filter);
/**

This function will change all filters at anytime.
Please note that this function will reload every texture and can be very slow.

%%(c)
// will switch all textures to bilinear filter.
raydium_texture_filter_change(RAYDIUM_TEXTURE_FILTER_BILINEAR)%%
**/

#endif
