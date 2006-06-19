/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/fog.h"

#endif 

void raydium_fog_enable(void)
{
raydium_fog_enabled_tag=1;
raydium_fog_apply();
}

void raydium_fog_disable(void)
{
raydium_fog_enabled_tag=0;
raydium_fog_apply();
}

void raydium_fog_color_update(void)
{
glFogfv(GL_FOG_COLOR,raydium_background_color);
}

void raydium_fog_mode_set(GLuint mode)
{
raydium_fog_mode=mode;
}

int raydium_fog_mode_get(void)
{
return raydium_fog_mode;
}

void raydium_fog_density_set(GLfloat density)
{
raydium_fog_density=density;
}

float raydium_fog_density_get(void)
{
return raydium_fog_density;
}

void raydium_fog_near_set(GLfloat near)
{
raydium_fog_near=near;
}

float raydium_fog_near_get(void)
{
return raydium_fog_near;
}
void raydium_fog_far_set(GLfloat far)
{
raydium_fog_far=far;
}

float raydium_fog_far_get(void)
{
return raydium_fog_far;
}
void raydium_fog_apply(void)
{
	char info[255];
	if(raydium_fog_enabled_tag)
	{
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE,raydium_fog_mode);
		glFogfv(GL_FOG_COLOR,raydium_background_color);
		glFogf(GL_FOG_DENSITY, raydium_fog_density);
		glHint(GL_FOG_HINT, GL_FASTEST);
		if(raydium_fog_far==0)
		{	
			raydium_fog_far=raydium_projection_far;
			raydium_fog_near= raydium_projection_far/4;
		}		
		glFogf(GL_FOG_START,raydium_fog_near);
		glFogf(GL_FOG_END,raydium_fog_far);		
		glFogi(GL_FOG_MODE,raydium_fog_mode);
		switch(raydium_fog_mode)
		{
			case GL_LINEAR:
			strcpy(info,"linear");
			break;
			case GL_EXP:
			strcpy(info,"exp");
			break;
			case GL_EXP2:
			strcpy(info,"exp2");
			break;
			default:
			strcpy(info,"error");
			break;
		}	
		raydium_log("Fog activated. Mode: %s; Density: %f; near: %f, far: %f.",info, raydium_fog_density,raydium_fog_near,raydium_fog_far );	
	}
	else
	{
		glDisable(GL_FOG);
		raydium_log("Fog deactivated.");
	}
}

void raydium_fog_wait(void)
{
	glDisable(GL_FOG);
	raydium_log("Fog stopped.");
}
