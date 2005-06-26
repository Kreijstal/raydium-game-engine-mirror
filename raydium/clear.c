/*
    Raydium - CQFD Corp.
    http://raydium.cqfd-corp.org
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/clear.h"
#endif 

void raydium_clear_frame(void)
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
raydium_frame_first_camera_pass=1;
raydium_vertex_counter=0;
}

void raydium_clear_color_update(void)
{
glClearColor(raydium_background_color[0],
	     raydium_background_color[1],
	     raydium_background_color[2],
	     raydium_background_color[3]);
}
