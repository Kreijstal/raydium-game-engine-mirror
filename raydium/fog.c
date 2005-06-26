/*
    Raydium - CQFD Corp.
    http://raydium.cqfd-corp.org
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/fog.h"

#endif 

void raydium_fog_enable(void)
{
glEnable(GL_FOG);
raydium_fog_enabled_tag=1;
}

void raydium_fog_disable(void)
{
glDisable(GL_FOG);
raydium_fog_enabled_tag=0;
}

void raydium_fog_color_update(void)
{
glFogfv(GL_FOG_COLOR,raydium_background_color);
}

// NOT A FINAL PROTOTYPE ! DO NOT USE BY YOURSELF ! (test only)
// Note : i've removed args until final signature (if any)
void raydium_fog_mode(void)
{
glFogi(GL_FOG_MODE,GL_LINEAR);
glFogf(GL_FOG_START,raydium_projection_far/4); // hum...
glFogf(GL_FOG_END,raydium_projection_far);
raydium_fog_color_update();
}
