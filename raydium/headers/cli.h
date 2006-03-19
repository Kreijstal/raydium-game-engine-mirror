#ifndef _CLI_H
#define _CLI_H
/*=
Command Line Interface
2401
**/

// Introduction
/**
Here, you'll find a few functions to deal with command line
interface of Raydium.
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
