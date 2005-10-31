#ifndef _INIT_H
#define _INIT_H
/*=
Initialization
2400
**/

// Introduction
/**
This file is mainly designed for internal uses, but there's anyway 
some interesting functions. 
**/

extern int raydium_init_cli_option (char *option, char *value);
/**
This function will search command line ##option##.
If this option is found, the functions stores any argument to ##value## and
returns 1.
The function will return 0 if ##option## is not found.

Example (search for: ##--ground##)
%%(c)
char model[RAYDIUM_MAX_NAME_LEN];
if(raydium_init_cli_option("ground",model))
    {
    setground(model);
    }
%%
**/

extern int raydium_init_cli_option_default (char *option, char *value, char *default_value);
/**
Same as above, but allows you to provide a default value (##default##) if
the ##option## is not found on command line.
**/

extern void raydium_init_lights (void);
/**
Internal use. Must be moved to light.c.
**/

extern void raydium_init_objects (void);
/**
Internal use. Must be moved to object.c.
**/

extern void raydium_init_key (void);
/**
Internal use. Must be moved to key.c.
**/

extern void raydium_init_reset (void);
/**
This function is supposed to reset the whole Raydium engine: 
textures, vertices, lights, objects, ...
Never tested yet, and probaly fails for many reasons when called more than
one time.
**/

extern void raydium_init_engine (void);
/**
Internal use. **Never** call this function by yourself, it may cause
huge memory leaks.
**/

extern void raydium_init_args (int argc, char **argv);
/**
You must use this function, wich send application arguments to Raydium 
and external libs (GLUT, OpenAL, ...).
This must be done **before** any other call to Raydium.
Example:
%%(c)
int main(int argc, char **argv)
{
raydium_init_args(argc,argv);
[...]
%%
**/

#endif
