#ifndef _CLEAR_H
#define _CLEAR_H
/*=
Frame clearing
801
**/

extern void raydium_clear_frame (void);
/**
You need to call this function every frame to clear all hardware buffers.
**/

extern void raydium_clear_color_update (void);
/**
Will apply background color modification. Probably useless for you.
**/
#endif
