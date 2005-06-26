/*
    Raydium - CQFD Corp.
    http://raydium.cqfd-corp.org
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/sky.h"
#endif

void raydium_sky_box_cache(void)
{
raydium_texture_current_set_name("BOXfront.tga");
raydium_texture_current_set_name("BOXback.tga");
raydium_texture_current_set_name("BOXleft.tga");
raydium_texture_current_set_name("BOXright.tga");
raydium_texture_current_set_name("BOXbottom.tga");
raydium_texture_current_set_name("BOXtop.tga");
}


void raydium_sky_box_render(GLfloat x, GLfloat y, GLfloat z)
{
//GLfloat one[]={1.,1.,1.,1.};
#define sizeb ((raydium_projection_far-raydium_projection_near)/2.f)
/*
GLfloat (x+size)=(x+5);
GLfloat sizey=(x+5);
GLfloat (z+size)=(x+5);
GLfloat minv=(x-5);
*/

if(raydium_fog_enabled_tag && !raydium_sky_force) return;

glDisable(GL_LIGHTING);
glDisable(GL_FOG);
//glColor4fv(raydium_background_color);
//glColor4fv(one);

raydium_texture_current_set_name("BOXfront.tga");
raydium_rendering_internal_prepare_texture_render(raydium_texture_current);
//glBindTexture(GL_TEXTURE_2D,raydium_texture_current_set_name("BOXfront.tga"));
glColor4fv(raydium_background_color);
glDepthMask(GL_FALSE);
glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f((x+sizeb),(y-sizeb),(z-sizeb));
    glTexCoord2f(0.0f, 1.0f); glVertex3f((x+sizeb),(y+sizeb),(z-sizeb));
    glTexCoord2f(0.0f, 0.0f); glVertex3f((x+sizeb),(y+sizeb),(z+sizeb)); 
    glTexCoord2f(1.0f, 0.0f); glVertex3f((x+sizeb),(y-sizeb),(z+sizeb));
glEnd();

				
raydium_texture_current_set_name("BOXback.tga");
raydium_rendering_internal_prepare_texture_render(raydium_texture_current);
//glBindTexture(GL_TEXTURE_2D,raydium_texture_current_set_name("BOXback.tga"));
glColor4fv(raydium_background_color);
glDepthMask(GL_FALSE);
glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f((x-sizeb),(y-sizeb),(z-sizeb));
    glTexCoord2f(1.0f, 1.0f); glVertex3f((x-sizeb),(y+sizeb),(z-sizeb));
    glTexCoord2f(1.0f, 0.0f); glVertex3f((x-sizeb),(y+sizeb),(z+sizeb)); 
    glTexCoord2f(0.0f, 0.0f); glVertex3f((x-sizeb),(y-sizeb),(z+sizeb));
glEnd();


raydium_texture_current_set_name("BOXright.tga");
raydium_rendering_internal_prepare_texture_render(raydium_texture_current);
//glBindTexture(GL_TEXTURE_2D,raydium_texture_current_set_name("BOXright.tga"));
glColor4fv(raydium_background_color);
glDepthMask(GL_FALSE);
glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f((x-sizeb),(y+sizeb),(z-sizeb));
    glTexCoord2f(1.0f, 1.0f); glVertex3f((x+sizeb),(y+sizeb),(z-sizeb));
    glTexCoord2f(1.0f, 0.0f); glVertex3f((x+sizeb),(y+sizeb),(z+sizeb)); 
    glTexCoord2f(0.0f, 0.0f); glVertex3f((x-sizeb),(y+sizeb),(z+sizeb));
glEnd();

raydium_texture_current_set_name("BOXleft.tga");
raydium_rendering_internal_prepare_texture_render(raydium_texture_current);
//glBindTexture(GL_TEXTURE_2D,raydium_texture_current_set_name("BOXleft.tga"));
glColor4fv(raydium_background_color);
glDepthMask(GL_FALSE);
glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f((x-sizeb),(y-sizeb),(z-sizeb));
    glTexCoord2f(0.0f, 1.0f); glVertex3f((x+sizeb),(y-sizeb),(z-sizeb));
    glTexCoord2f(0.0f, 0.0f); glVertex3f((x+sizeb),(y-sizeb),(z+sizeb)); 
    glTexCoord2f(1.0f, 0.0f); glVertex3f((x-sizeb),(y-sizeb),(z+sizeb));
glEnd();

raydium_texture_current_set_name("BOXtop.tga");
raydium_rendering_internal_prepare_texture_render(raydium_texture_current);
//glBindTexture(GL_TEXTURE_2D,raydium_texture_current_set_name("BOXtop.tga"));
glColor4fv(raydium_background_color);
glDepthMask(GL_FALSE);
glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f((x-sizeb),(y-sizeb),(z+sizeb));
    glTexCoord2f(1.0f, 0.0f); glVertex3f((x+sizeb),(y-sizeb),(z+sizeb));
    glTexCoord2f(1.0f, 1.0f); glVertex3f((x+sizeb),(y+sizeb),(z+sizeb)); 
    glTexCoord2f(0.0f, 1.0f); glVertex3f((x-sizeb),(y+sizeb),(z+sizeb));
glEnd();


raydium_texture_current_set_name("BOXbottom.tga");
raydium_rendering_internal_prepare_texture_render(raydium_texture_current);
//glBindTexture(GL_TEXTURE_2D,raydium_texture_current_set_name("BOXbottom.tga"));
glColor4fv(raydium_background_color);
glDepthMask(GL_FALSE);
glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f((x-sizeb),(y-sizeb),(z-sizeb));
    glTexCoord2f(1.0f, 1.0f); glVertex3f((x+sizeb),(y-sizeb),(z-sizeb));
    glTexCoord2f(1.0f, 0.0f); glVertex3f((x+sizeb),(y+sizeb),(z-sizeb)); 
    glTexCoord2f(0.0f, 0.0f); glVertex3f((x-sizeb),(y+sizeb),(z-sizeb));
glEnd();


if(raydium_light_enabled_tag) glEnable(GL_LIGHTING);
if(raydium_fog_enabled_tag) glEnable(GL_FOG);
glDepthMask(GL_TRUE);

}
