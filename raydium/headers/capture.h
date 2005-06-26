#ifndef _CAPTURE_H
#define _CAPTURE_H
/*=
Capture (2D)
700
**/

// Quickview
/**
Captures are made in TGA format (without RLE compression) and saved into
the current directory.
This function may fail (garbage in resulting capture) if frame size if 
not "standard", mostly after a window resize.
**/

extern void raydium_capture_frame(char *filename);
/**
Capture current frame to ##filename##.
**/

extern void raydium_capture_frame_auto(void);
/**
Same as above, but to an auto-generated filename (raycap*).
**/

#endif
