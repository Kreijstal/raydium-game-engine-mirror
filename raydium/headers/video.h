#ifndef _VIDEO_H
#define _VIDEO_H

#include "../video.h"

/*=
Video playback
4100
**/

// Introduction
/**
Raydium supports simple video playback, thru a special video codec (JPGS),
useful for menus enhancements, "speaking" thumbnails, ...
This codec only supports video, use sound API if needed.
You will find an small utility, ##mk_jpgs## in Raydium source tree, didacted to
movie creation.
**/

// How to create a movie ?
/**
First, compile ##mk_jpgs##: example: ##gcc mk_jpgs -o mk_jpgs## or any other
standard build command.
Then, generate JPEG pictures (using a temporary directory, if possible):
##mplayer movie.avi -vo jpeg:quality=50 -vf scale=256:256##, where you may
change quality factor and output size. Use "hardware friendly" sizes (64,
128,256,...) !
You can now build JPGS file:
##./mk_jpgs 25 256 256 video.jpgs## (fps, size x, size y, output file)
**/

void raydium_video_init(void);
/**
Internal use.
**/

signed char raydium_video_isvalid(int i);
/**
Internal use, but you can call this function if you want to verify if a
video id is valid (in bounds and open).
**/

int raydium_video_find_free(void);
/**
Internal use.
Finds a free video slot.
**/

int raydium_video_find(char *name);
/**
Resolvs video ##name##, returning suitable texture id (native Raydium texture
id).
**/

void raydium_video_jpeg_decompress(FILE *fp,unsigned char *to);
/**
Internal.
**/

int raydium_video_open(char *filename, char *as);
/**
This function will open and prepare video ##filename##, and will attach
this video to a "live texture" (see Live API chapter, if needed).
**/

void raydium_video_callback_video(int id);
/**
Internal use.
**/

void raydium_video_callback(void);
/**
Internal use. Frame callback.
**/

void raydium_video_delete(int id);
/**
Will delete video ##id##. Warning: this function will not delete
associated Live texture, so you may open a new video with the same
texture name, but video size must be the same a the previous one.
**/

void raydium_video_delete_name(char *name);
/**
Same as above, using video name.
**/

#endif
