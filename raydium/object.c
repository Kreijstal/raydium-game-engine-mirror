/*
    Raydium - CQFD Corp.
    http://raydium.cqfd-corp.org
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/object.h"
#endif 

GLint raydium_object_find(char *name)
{
GLint i;
for(i=0;i<raydium_object_index;i++)
if(!strcmp(raydium_object_name[i],name)) return i;

return -1;
}


void raydium_object_reset(GLuint o)
{
raydium_object_name[o][0]=0;
raydium_object_start[o]=0;
raydium_object_end[o]=0;
}


int raydium_object_load(char *filename)
{
if(raydium_object_find(filename)>=0)
    {
    raydium_log("ERROR: object: %s already loaded",filename);
    return -1;
    }
raydium_object_start[raydium_object_index]=raydium_vertex_index;
read_vertex_from(filename);
raydium_object_end[raydium_object_index]=raydium_vertex_index;
strcpy(raydium_object_name[raydium_object_index],filename);
return(raydium_object_index++);
}

GLint raydium_object_find_load(char *name)
{
int ret;

ret=raydium_object_find(name);
if(ret<0) return raydium_object_load(name);
return ret;
}


void raydium_object_draw(GLuint o)
{
#ifndef DEBUG_RENDER_DISABLE_DISPLAYLISTS
static GLuint dl[RAYDIUM_MAX_OBJECTS];
static char dl_state[RAYDIUM_MAX_OBJECTS];
static int first=0;
int i;

if(first)
    for(i=0;i<RAYDIUM_MAX_OBJECTS;i++)
	dl_state[i]=-1;


if(raydium_render_displaylists_tag)
{
 if(!dl_state[o])
    {
    // build DL
    dl_state[o]=1;
    dl[o]=glGenLists(1);
    raydium_log("Object: creating display list for object %s",raydium_object_name[o]);
    glNewList(dl[o],GL_COMPILE);
    raydium_rendering_from_to(raydium_object_start[o],raydium_object_end[o]);
    glEndList();
    }
  glCallList(dl[o]);
}
else raydium_rendering_from_to(raydium_object_start[o],raydium_object_end[o]);

#else
// No display lists, draw 
raydium_rendering_from_to(raydium_object_start[o],raydium_object_end[o]);
#endif
}

void raydium_object_draw_name(char *name)
{
GLint i;

i=raydium_object_find(name);

if(i>=0) raydium_object_draw(i); 
else 
raydium_object_draw(raydium_object_load(name));
}


void raydium_object_deform(GLuint obj,GLfloat ampl)
{
GLuint i;


for(i=raydium_object_start[obj];i<raydium_object_end[obj];i++)
    {
    raydium_vertex_x[i]+=raydium_random_neg_pos_1()*ampl;
    raydium_vertex_y[i]+=raydium_random_neg_pos_1()*ampl;
    raydium_vertex_z[i]+=raydium_random_neg_pos_1()*ampl;
    }

/*
GLfloat ox,oy,oz;
GLfloat nx,ny,nz;
GLint j;

for(i=raydium_object_start[obj];i<raydium_object_end[obj];i++)
    {
    ox=raydium_vertex_x[i];
    oy=raydium_vertex_y[i];
    oz=raydium_vertex_z[i];

    nx=ox+raydium_random_neg_pos_1()*ampl;
    ny=oy+raydium_random_neg_pos_1()*ampl;
    nz=oz+raydium_random_neg_pos_1()*ampl;

    for(j=i;j<raydium_object_end[obj];j++)
	if(raydium_vertex_x[j]==ox && 
	   raydium_vertex_y[j]==oy && 
	   raydium_vertex_z[j]==oz)
	{
	raydium_vertex_x[j]=nx;
	raydium_vertex_y[j]=ny;
	raydium_vertex_z[j]=nz;
	}
    }*/
}


void raydium_object_deform_name(char *name,GLfloat ampl)
{
raydium_object_deform(raydium_object_find(name),ampl);
}


GLfloat raydium_object_find_dist_max(GLuint obj)
{
GLfloat max=0,val;
int i;
for(i=raydium_object_start[obj];i<raydium_object_end[obj];i++)
    {
    val=sqrt((raydium_vertex_x[i]*raydium_vertex_x[i])+
	     (raydium_vertex_y[i]*raydium_vertex_y[i])+
	    (raydium_vertex_z[i]*raydium_vertex_z[i]) );
    if(val>max) max=val;
    }
return max;
}

void raydium_object_find_axes_max(GLuint obj, GLfloat *tx, GLfloat *ty, GLfloat *tz)
{
int i;
*tx=*ty=*tz=0;

for(i=raydium_object_start[obj];i<raydium_object_end[obj];i++)
    {
    if(raydium_trigo_abs(raydium_vertex_x[i])>*tx) *tx=raydium_trigo_abs(raydium_vertex_x[i]);
    if(raydium_trigo_abs(raydium_vertex_y[i])>*ty) *ty=raydium_trigo_abs(raydium_vertex_y[i]);
    if(raydium_trigo_abs(raydium_vertex_z[i])>*tz) *tz=raydium_trigo_abs(raydium_vertex_z[i]);
    }
*tx*=2;
*ty*=2;
*tz*=2;
}
