#ifndef _FILE_H
#define _FILE_H
/*=
Files
2100
**/

// Warning
/**
It's important to use only functions with ##raydium_file_*## prefix.
All other functions may change or disappear. Upper level functions are
available (see ##object.c##).
**/

// Introduction
/**
##file.c## use .tri mesh files (text), available in 3 versions:

1. version 1: providing normals and uv texture mapping informations.
2. version 0: providing normals.
3. version -1: only providing vertices. 
	 
Version 1 example file:
%%
1
5.1 15.75 -3.82 0.0000 0.0000 -1.0000 0.5158 0.5489 rgb(0.5,0.5,0.5)
6.3 11.75 -3.82 0.0000 0.0000 -1.0000 0.5196 0.5365 rgb(0.5,0.5,0.5)
5.0 11.75 -3.82 0.0000 0.0000 -1.0000 0.5158 0.5365 rgb(0.5,0.5,0.5)
...
%%
	      
You can find the file version on first line, and then data.
Next lines: vertex position (x,y,z), normal (x,y,z), texture mapping (u,v)
and texture (string). 
**/


#define DONT_SAVE_DUMMY_TEXTURE
extern void raydium_file_dirname(char *dest,char *from);
/**
Reliable and portable version of libc's ##dirname## function.
This function extracts directory from ##from## filename, and writes it
to ##dest##.
No memory allocation will be done by the function.
**/

extern void raydium_file_log_fopen_display(void);
/**
Display (console) all filenames that were opened before the call.
##--files## command line option will call this function at the application's
exit, closed or not.
**/

extern FILE *raydium_file_fopen(char *file, char *mode);
/**
Raydium wrapper to libc's ##fopen## function.
This function will:
- Update some stats
- Try to download the file from repositories if no local version is found, or
will try to update the file if asked (##--repository-refresh## or 
##repository-force##). See R3S on Raydium's Wiki.
**/

#ifdef PHP_SUPPORT
extern int raydium_rayphp_repository_file_get(char *file);
#else
#define raydium_php_repository_file_get fopen
#endif
extern void dump_vertex_to (char *filename);
/**
This function save all scene to filename (.tri file) in version 1.
Vertice may be sorted.
Please, try to do not use this function.
**/

extern void dump_vertex_to_alpha (char *filename);
/**
Now useless and deprecated.
**/

extern int raydium_file_set_textures (char *name);
/**
Internal use.
This function analyze texture filename, and search for extended multitexturing
informations (u,v and another texture).
**/

extern void read_vertex_from (char *filename);
/**
Loads filename. Again, avoid use of this function.
**/

#endif
