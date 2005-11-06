/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/render.h"
#endif 

void raydium_callback_image(void);
void raydium_timecall_callback(void);


// color is a GLfloat[4] (RGBA)
void raydium_render_lightmap_color(GLfloat *color)
{
memcpy(raydium_render_lightmap_color_value,color,raydium_internal_size_vector_float_4);

// Err ... no :/ There's no current color for other texture units than 0 ...
/*
glActiveTextureARB(GL_TEXTURE1_ARB);
glColor4fv(raydium_render_lightmap_color_value);
glActiveTextureARB(GL_TEXTURE0_ARB);
*/
}

void raydium_render_lightmap_color_4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
GLfloat col[4];
col[0]=r;
col[1]=g;
col[2]=b;
col[3]=a;
raydium_render_lightmap_color(col);
}

// let's keep this define here, until full tests
#define RAYDIUM_RENDER_MAX_TEXUNITS 2
int raydium_rendering_prepare_texture_unit(GLenum tu,GLuint tex)
{
// cache state of each texunit
static GLuint texunit_state[RAYDIUM_RENDER_MAX_TEXUNITS];
static int first=1;
int tui;

if(first)
    {
    int i;
    for(i=0;i<RAYDIUM_RENDER_MAX_TEXUNITS;i++)
	texunit_state[i]=0;    

    first=0;
    }

tui=tu-GL_TEXTURE0_ARB;


if(tui>=RAYDIUM_RENDER_MAX_TEXUNITS)
    {
    raydium_log("render: texture unit %i is invalid (%i max, see RAYDIUM_RENDER_MAX_TEXUNITS",
    tui,RAYDIUM_RENDER_MAX_TEXUNITS);
    return 0;
    }

if(texunit_state[tui]==tex)
    return 0;

texunit_state[tui]=tex;

//printf("preparing texunut %i with %s\n",tui,raydium_texture_name[tex]);

// prepare "lightmaps" texture units
if(tui>0)
{
 glEnd(); // ugly, but we must close all shapes, if any
 glActiveTextureARB(tu);
 
#ifndef RENDER_ALLOW_LIGHTING_FOR_LIGHTMAPS
 if(raydium_texture_islightmap[tex])
 {
 glDisable(GL_LIGHTING); 
 }
 else if(raydium_light_enabled_tag)
	glEnable(GL_LIGHTING);
#endif 
 
 if(tex)
 {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,tex);

  if(raydium_texture_islightmap[tex])
  {
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glColor4fv(raydium_render_lightmap_color_value);
  }
  else
  {
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_EXT);
    glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 2);
  } 
 } 
 else 
 {
 glDisable(GL_TEXTURE_2D);
// glBindTexture(GL_TEXTURE_2D,0); 
 }
 glActiveTextureARB(GL_TEXTURE0_ARB);
}
else // "standard" textunit
{
  // default (according GL specs) DIFFUSE value.
  GLfloat one[]={0.8f, 0.8f, 0.8f, 1.f};
  GLfloat zero[]={0.0,0.0,0.0,0.0};
  GLfloat *rgb;

  glColor4f(1.f,1.f,1.f,1.f);

  if(raydium_texture_blended[tex]==RAYDIUM_TEXTURE_BLEND_BLENDED)
  {
  glEnable(GL_BLEND);
  glDepthMask(GL_FALSE);
  glDisable(GL_ALPHA_TEST);
//  glDisable(GL_FOG);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  }

  if(raydium_texture_blended[tex]==RAYDIUM_TEXTURE_BLEND_CUTOUT)
  {
  glEnable(GL_BLEND);
  glDepthMask(GL_TRUE);
  glAlphaFunc(GL_GREATER,0);
  glEnable (GL_ALPHA_TEST);
//  glDisable(GL_FOG);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  }

  if(raydium_texture_blended[tex]==RAYDIUM_TEXTURE_BLEND_NONE)
  {
  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
  glDisable(GL_ALPHA_TEST);
//  glEnable(GL_FOG);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  }


  if(raydium_texture_blended[tex]==RAYDIUM_TEXTURE_PHANTOM)
  {
  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_TEXTURE_2D);
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  }


  if(raydium_texture_rgb[tex][0]>=0 && raydium_texture_blended[tex]!=RAYDIUM_TEXTURE_PHANTOM)
  {
  if(raydium_render_rgb_force_tag)
    rgb=raydium_render_rgb_force;
  else
    rgb=raydium_texture_rgb[tex];
  
  glDisable(GL_TEXTURE_2D);
  glColor4f(rgb[0],rgb[1],rgb[2],1.f);
   if(raydium_light_enabled_tag)
   {
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rgb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rgb);
   }
  }
  else 
  {
  glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, one);
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, zero);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,tex);
  //printf("%s\n",raydium_texture_name[tex]);
  }
} // end "standard" texture
return 1;
}


void raydium_rendering_internal_prepare_texture_render(GLuint tex)
{
raydium_rendering_prepare_texture_unit(GL_TEXTURE0_ARB,tex);
}

void raydium_rendering_internal_restore_render_state(void)
{
//#define ONE 0.8 // default DIFFUSE value.
GLfloat one[]={0.8f, 0.8f, 0.8f, 1.f};

//return; // make no sens to restore state since next texture will reset it

glDisable(GL_BLEND);
glDepthMask(GL_TRUE);
glEnable(GL_TEXTURE_2D);
glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, one);
}

// 2D quick 'n ugly clipping test
char infov(GLfloat x, GLfloat y)
{
#ifdef RENDER_DEBUG_NO_CLIP
return 1;
#endif
if((x+raydium_camera_cursor_place[0])>(raydium_camera_x-raydium_projection_far) &&
   (x+raydium_camera_cursor_place[0])<(raydium_camera_x+raydium_projection_far) &&
   (y+raydium_camera_cursor_place[1])>(raydium_camera_y-raydium_projection_far) &&
   (y+raydium_camera_cursor_place[1])<(raydium_camera_y+raydium_projection_far) ) return 1; else return 0;
}

void raydium_rendering_from_to_simple(GLuint from, GLuint to)
{
GLuint i;
//printf("%i -> %i\n",from,to);
glBegin(GL_TRIANGLES);
  for(i=from;i<to;i+=3)
    {
    glVertex3f(raydium_vertex_x[i+0], raydium_vertex_y[i+0], raydium_vertex_z[i+0]);
    glVertex3f(raydium_vertex_x[i+1], raydium_vertex_y[i+1], raydium_vertex_z[i+1]);
    glVertex3f(raydium_vertex_x[i+2], raydium_vertex_y[i+2], raydium_vertex_z[i+2]);
    }
glEnd();
}

void raydium_rendering_from_to(GLuint from, GLuint to)
{
GLuint tex,i,j;
int multi_prepared=0;

if(raydium_shadow_rendering)
    {
    raydium_rendering_from_to_simple(from,to);
    return;
    }

for(tex=1;tex<raydium_texture_index;tex++)
{
  // prepare first texture unit
  raydium_rendering_prepare_texture_unit(GL_TEXTURE0_ARB,tex);
  // ... and reset next one
  raydium_rendering_prepare_texture_unit(GL_TEXTURE1_ARB,0);

  glBegin(GL_TRIANGLES);
  

  for(i=from,j=0;i<to;i+=3)
  if(raydium_vertex_texture[i]==tex)
  {
#ifdef DEBUG_RENDER_DISABLE_DISPLAYLISTS
   if(!infov(raydium_vertex_x[i  ],raydium_vertex_y[i  ]) &&
      !infov(raydium_vertex_x[i+1],raydium_vertex_y[i+1]) &&
      !infov(raydium_vertex_x[i+2],raydium_vertex_y[i+2]) )
        continue;
#endif

    
#ifdef RENDER_DEBUG_TAG
    if(raydium_vertex_tag[i  ] ||
       raydium_vertex_tag[i+1] ||
       raydium_vertex_tag[i+2] )
	glColor4f(1.f,0.f,1.f,1.f);
    else
	glColor4f(1.f,1.f,1.f,1.f);
#endif    

    if(raydium_vertex_texture_multi[i])
    {
	if(raydium_rendering_prepare_texture_unit(GL_TEXTURE1_ARB,raydium_vertex_texture_multi[i]))
	    {
	    //glEnd(); // done by "prepare_texture_multi"
	    glBegin(GL_TRIANGLES);
	    multi_prepared=1;
	    }

	// THIS CODE IS DUPLICATED FOR SPEED REASON (1)
	for(j=0;j<3;j++)
	{
	glNormal3f(raydium_vertex_normal_visu_x[i+j],raydium_vertex_normal_visu_y[i+j],raydium_vertex_normal_visu_z[i+j]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,raydium_vertex_texture_u[i+j],raydium_vertex_texture_v[i+j]);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB,raydium_vertex_texture_multi_u[i+j],raydium_vertex_texture_multi_v[i+j]);
	//printf("%f %f\n",raydium_vertex_texture_multi_u[i+j],raydium_vertex_texture_multi_v[i+j]);
	glVertex3f(raydium_vertex_x[i+j], raydium_vertex_y[i+j], raydium_vertex_z[i+j]);
	raydium_vertex_counter++;
	}
    }
    else
    {
	// cancel previous multitexturing settings
	if(multi_prepared)
	    {
	    raydium_rendering_prepare_texture_unit(GL_TEXTURE1_ARB,0);
	    multi_prepared=0;
	    glBegin(GL_TRIANGLES);
	    }

	// THIS CODE IS DUPLICATED FOR SPEED REASON (2)
	for(j=0;j<3;j++)
	{
	glNormal3f(raydium_vertex_normal_visu_x[i+j],raydium_vertex_normal_visu_y[i+j],raydium_vertex_normal_visu_z[i+j]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB,raydium_vertex_texture_u[i+j],raydium_vertex_texture_v[i+j]);
	glVertex3f(raydium_vertex_x[i+j], raydium_vertex_y[i+j], raydium_vertex_z[i+j]);
	raydium_vertex_counter++;
	}
    }
  }
  glEnd();
} // end for "all textures"

raydium_rendering_prepare_texture_unit(GL_TEXTURE1_ARB,0);
}


void raydium_rendering(void)
{
raydium_rendering_from_to(0,raydium_vertex_index);
}

void raydium_rendering_finish(void)
{
static int fps=0;
static clock_t last=0;
fps++;
if(!last) last=clock();

raydium_callback_image();
glFlush();
raydium_rendering_internal_restore_render_state();
//glutPostRedisplay();

#ifdef DEBUG_MOVIE
if(raydium_key[GLUT_KEY_F11])
{
char name[128];
static int frame;
sprintf(name,"movie/frame%04d.jpg",frame);
raydium_capture_frame_jpeg(name);
frame++;
}
#endif


#ifndef DEBUG_SHADOW_MAP_VIEW
glutSwapBuffers();
#endif

raydium_shadow_map_generate();
raydium_key_last=0;
raydium_mouse_click=0;
raydium_camera_pushed=0; 
glPopMatrix();
if(clock() > last + CLOCKS_PER_SEC)
    {
    last=clock();
    raydium_render_fps=fps;
    fps=0;
    }
}

void raydium_rendering_wireframe(void)
{
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void raydium_rendering_normal(void)
{
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void raydium_rendering_rgb_force(GLfloat r, GLfloat g, GLfloat b)
{
raydium_render_rgb_force_tag=1;
raydium_render_rgb_force[0]=r;
raydium_render_rgb_force[1]=g;
raydium_render_rgb_force[2]=b;
raydium_render_rgb_force[3]=1.0;
}

void raydium_rendering_rgb_normal(void)
{
raydium_render_rgb_force_tag=0;
}


void raydium_rendering_displaylists_enable(void)
{
raydium_render_displaylists_tag=1;
}

void raydium_rendering_displaylists_disable(void)
{
raydium_render_displaylists_tag=0;
}
