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
Here is a list of supported options:

Graphics:
##window## Define window rendering.
##fullscreen## Define Fullscreen rendering.

##xinerama-fullscreen## Select Xinerama FullScreen render windows.
##xinerama-screen## Select Screen for render windows. 

##filter## Define texture filter type (##none##,##bilinear##,##trilinear##,##aniso##).
##max-aniso## Define Anisotropic filter level to max value according to hardware capacity.
##compress## Enable texture compression.

Joystick:
##joydev## Define Jostick number used by raydium.
##evdev## Define Force FeedBack peripherial (linux only).

Video / WebCam:
##video-device## Define Video device (WebCam ...) used by live Api.
##video-size## Define default video Size 640x480, 320x240 ...

Network:
##name## Set player name for network application.
##ode-rate## Set physics sample rate for networked application.

Console:
##consoletexture## Console texture background.
##consolefont## Console font.

Path and files:
##home## Setting raydium global home directory.
##path## Define list of media path.
##write-path## Define destination directory for media file.
##files## List all opened files.
##logfile## Define logfile name.
##history## Console History file name.

Initialisation Scripts:
##autoexec## Php script executed just after engine initialization.
##autoexec2## php script executed when raydium_callback is called.

Repository:
##repository-disable## Disable getting media files form repository.
##repository-refresh## Refresh files form repository even if local file exist.
##repository-force## Force Repository use.
##rayphp## Path of rayphp php scripts.

Misc:
##regs## Dump registers.
**/

__rayapi int raydium_init_cli_option(char *option, char *value);
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

__rayapi int raydium_init_cli_option_default(char *option, char *value, char *default_value);
/**
Same as above, but allows you to provide a default value (##default##) if
the ##option## is not found on command line.
**/

__rayapi void raydium_init_internal_homedir_find(char *);
/**
Internal use.
**/

#ifndef RAYDLL
__rayapi void raydium_init_args(int argc, char **argv);
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

#ifndef RAYDLL
__rayapi void raydium_init_args_name(int argc, char **argv, char *app_name);
/**
Same as above, but with application short name. This string is used to
build things like runtime configuration directory name (~/.raydium/ by default).
Use this wrapper if you don't want to share your configuration with Raydium.
**/
#endif


/*
Ok ... all this is a very ugly part: under win32, a DLL use a different
atexit() queue than the application. We can't use the DLL_PROCESS_DETACH
DLL reason, since it seems that OpenAL DLL is unloaded *before* raydium.dll !

So the idea here is to provide a wrapper to the application for these two
init functions so they use their own atexit() queue, in a transparent way.

See atexit.h header for wrappers.
*/
#ifdef RAYDLL
__rayapi void raydium_init_args_hack(int argc, char **argv);
__rayapi void raydium_init_args_name_hack(int argc, char **argv, char *app_name);
#endif


#endif
