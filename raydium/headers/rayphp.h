//!NOBINDINGS
#ifndef _RAYPHP_H
#define _RAYPHP_H

/*=
RayPHP (internals)
3600
**/

// Introduction
/**
Raydium also use RayPHP (Raydium/PHP interface) for its own needs.
For PHP part of these functions, see "rayphp/" directory.
So far, RayPHP is dedicated to R3S (Raydium Server Side Scripts) access.
All this is mostly usefull for internal uses, since Raydium provides ##fopen##
wrappers, thru ##raydium_file_fopen##.
**/

extern int raydium_rayphp_repository_file_get (char *path);
/**
Will contact R3S servers for downloading ##path## file.
**/

extern int raydium_rayphp_repository_file_put (char *path, int depends);
/**
Will contact R3S servers for uploading ##path## file. Set ##depends## to
true (1) if you also want to upload dependencies, false (0) otherwise.
**/

extern int raydium_rayphp_repository_file_list(char *filter);
/**
Will contact R3S servers to get file list, using ##filter## (shell-like
syntax). Default ##filter## is ##*##.
**/

#endif
