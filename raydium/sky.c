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
raydium_rendering_internal_prepare_texture_render(raydium_texture_current_main);
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
raydium_rendering_internal_prepare_texture_render(raydium_texture_current_main);
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
raydium_rendering_internal_prepare_texture_render(raydium_texture_current_main);
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
raydium_rendering_internal_prepare_texture_render(raydium_texture_current_main);
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
raydium_rendering_internal_prepare_texture_render(raydium_texture_current_main);
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
raydium_rendering_internal_prepare_texture_render(raydium_texture_current_main);
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

void raydium_sky_sphere_render(GLfloat x, GLfloat y, GLfloat z,int detail)
{	
	int i, j;
	GLfloat currentradious,z1,z2,ang1;
	//static cause this will be used to store the vertices of the sphere
	static GLfloat p[RAYDIUM_SKY_SPHERE_MAX_DETAIL][RAYDIUM_SKY_SPHERE_MAX_DETAIL][3];

	//keeping safe the current matrix
	glPushMatrix();	
	//glPushAttrib(GL_COLOR_BUFFER_BIT);
	//increasing angles to simulate orbit
	raydium_sky_sphere_angle_orbit_u += 10 * raydium_frame_time;
	raydium_sky_sphere_angle_orbit_v += 1 * raydium_frame_time;
	//turning off "specials" for render
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDisable(GL_TEXTURE_2D);
	glColor4fv(raydium_background_color);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
	//MAKING THE SPHERE
	
	//check if the points are already calculated.
	//there is no reason to enter constantly.
	if(raydium_sky_sphere_generated!=1)
	{
		//getting the points of the sphere, but no drawing
		for(i=0;i<=detail;i++)
		{
			//getting the radious for each section of the sphere
			currentradious		=	raydium_trigo_sin(180*((float)i/(float)detail));
			//getting the heights for each section of the sphere
			z1					=	raydium_trigo_cos(180*((float)i/(float)detail));		
			for(j=0;j<=detail;j++)
			{
				ang1		=	360*((float)j/(float)detail);
				p[i][j][0]	=	currentradious * raydium_trigo_cos(ang1);
				p[i][j][1]	=	currentradious * raydium_trigo_sin(ang1);
				p[i][j][2]	=	z1;
			}
		}
		raydium_sky_sphere_generated	=	1;
	}

	//locating the modelview in the pos of the camera
	glTranslatef(x,y,z);
	//rotating according the orbit angles(ugly)
	glRotatef(raydium_sky_sphere_angle_orbit_v,0,0,1);
	glRotatef(raydium_sky_sphere_angle_orbit_u,1,0,0);
	//now drawing with the obtained values
	for(i=0;i<detail;i++)
	{
		for(j=0;j<detail;j++)
		{
			//calculating colors
			z1	=	1-(float)i/detail;
			z2	=	1-(float)(i+1)/detail;
			
			glBegin(GL_TRIANGLES);
			
			glColor4f(z1,z1,z1,1);
			glVertex3f(p[i][j][0],p[i][j][1],p[i][j][2]);
			
			glColor4f(z1,z1,z1,1);
			glVertex3f(p[i][j+1][0],p[i][j+1][1],p[i][j+1][2]);
			
			glColor4f(z2,z2,z2,1);
			glVertex3f(p[i+1][j][0],p[i+1][j][1],p[i+1][j][2]);
			
			glColor4f(z2,z2,z2,1);
			glVertex3f(p[i+1][j][0],p[i+1][j][1],p[i+1][j][2]);
			
			glColor4f(z2,z2,z2,1);
			glVertex3f(p[i+1][j+1][0],p[i+1][j+1][1],p[i+1][j+1][2]);
			
			glColor4f(z1,z1,z1,z1);
			glVertex3f(p[i][j+1][0],p[i][j+1][1],p[i][j+1][2]);
			
			glEnd();
		}
	}

	//restoring previus states
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	if(raydium_fog_enabled_tag) glEnable(GL_FOG);
	if(raydium_light_enabled_tag) glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE);
	//glPopAttrib();
	glPopMatrix();
}

void raydium_sky_atmosphere_enable(void)
{
	raydium_sky_atmosphere_enable_tag=1;
	//this will force to recalculate the sphere once
	raydium_sky_sphere_generated=0;
	raydium_log("atmosphere created");
}

void raydium_sky_atmosphere_disable(void)
{
	raydium_sky_atmosphere_enable_tag=0;
}

void raydium_sky_atmosphere_render( GLfloat x, GLfloat y, GLfloat z,int detail)
{
	if(raydium_sky_atmosphere_enable_tag)
	{	
		raydium_sky_sphere_render(x,y,z,detail);
	}
}

signed char raydium_sky_atmosphere_check(void)
{
	return raydium_sky_atmosphere_enable_tag;
}
