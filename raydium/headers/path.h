#ifndef _PATH__H
#define _PATH__H
#include "../path.h"

/*=
File path
2110
**/

// Introduction
/**
When ##raydium_file_fopen()## is called, by Raydium or by an application,
the Path API is used to search the file.
When the file is open for writing, Raydium will check if the current working
directory is writable. If this is not the case, Raydium will try to write the
file in the user home directory (~/.appname/data/), and will create it if
needed.
For reading, Raydium will also use the current directory first, and then will
search in a list of directories of your choice. The user home directory is
registered by default in this list.

If the '/' character is present in the requested filename, the path system
is disabled and regular fopen() behavior is used.
**/

// Example
/**
%%(c)
raydium_path_ext("./media/textures/","tga");
raydium_path_ext("./media/fonts/","tga");
raydium_path_ext("./media/shaders/","vert");
raydium_path_ext("./media/shaders/","frag");
raydium_path_ext("./media/meshes/","tri");
raydium_path_ext("./media/themes/","gui");
raydium_path_ext("./media/particles/","prt");
raydium_path_ext("./media/cars/","car");
raydium_path_ext("./media/cams/","cam");
raydium_path_add("./media/"); // "unsorted" files
%%
**/

__rayapi int raydium_path_find_free(void);
/**
Internal (search a free path slot).
**/

__rayapi signed char raydium_path_ext(char *dir, char *ext);
/**
Register ##dir## directory for files with ##ext## extension.
Return 0 when it fails.
**/

__rayapi signed char raydium_path_add(char *dir);
/**
Register ##dir## directory.
Return 0 when it fails.
**/

__rayapi signed char raydium_path_write(char *dir);
/**
Change the writing directory to ##dir## directory. You should probably also
register this new directory, using ##raydium_path_add()##.
**/

__rayapi signed char raydium_path_string_from(char *str);
/**
Reset all registrations (only current directory stays) and replace it with the
provided formated string. Here it is an example for such string:

%%/home/xfennec/.myapp/data:./media/textures/*.tga:./media/fonts/*.tga:
./media/shaders/*.vert:./media/shaders/*.frag:./media/meshes/*.tri:
./media/themes/*.gui:./media/particles/*.prt:./media/cars/*.car:
./media/cams/*.cam:./media%%
(do not include line feeds)

This string is based an the example at the top of this chapter.
**/

__rayapi int raydium_path_string_to(char *out);
/**
Dumps all registrations to ##out## string.
**/

__rayapi void raydium_path_resolv(char *in, char *out, char mode);
/**
Internal (find the best absolute path for the requested file).
**/

__rayapi void raydium_path_dump(void);
/**
Dumps read and write paths to console.
**/

__rayapi void raydium_path_reset(void);
/**
Reset all registrations.
You should probably better use ##raydium_path_string_from()##.
**/

__rayapi void raydium_path_init(void);
/**
Internal.
**/

#endif
