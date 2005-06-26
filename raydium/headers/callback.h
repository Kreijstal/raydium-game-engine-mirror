#ifndef _CALLBACK_H
#define _CALLBACK_H

/*=
Callbacks
1500
**/

// Introduction
/**
This file contains many initializations, a few internal callbacks, but 
will provides a very important function for end-user, wich will 
gives user display function to Raydium: see below
**/


extern void raydium_callback_image (void);
/**
Internal use.
**/

extern void raydium_callback_set (void);
/**
Internal use.
**/

extern void raydium_callback (void (*loop));
/**
This function will loop over the provided display function, indefinitely.
"loop" must be: 
%%(c) void loop(void) %%
**/

#endif
