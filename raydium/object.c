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

signed char raydium_object_isvalid(int obj)
{
if(obj>=0 && obj<raydium_object_index)
    return 1;
return 0;
}


void raydium_object_reset(GLuint o)
{
raydium_object_name[o][0]=0;
raydium_object_start[o]=0;
raydium_object_end[o]=0;
raydium_object_anims[o]=0;
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
#endif

if(!raydium_object_isvalid(o))
    {
    raydium_log("object: draw: ERROR: id or name is invalid");
    return;
    }


// if animated : draw "only" first (generated) frame
if(raydium_object_anims[o]>0)
    {
    raydium_rendering_from_to(raydium_object_start[o],raydium_object_start[o]+raydium_object_anim_len[o]);
    return;
    }
// ...


#ifndef DEBUG_RENDER_DISABLE_DISPLAYLISTS
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

if(!raydium_object_isvalid(obj))
    {
    raydium_log("object: deform: ERROR: id or name is invalid");
    return;
    }

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

if(!raydium_object_isvalid(obj))
    {
    raydium_log("object: find_dist_max: ERROR: id or name is invalid");
    return -1;
    }

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

if(!raydium_object_isvalid(obj))
    {
    raydium_log("object: find_axes_max: ERROR: id or name is invalid");
    return;
    }

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

// Animation support starts here

GLint raydium_object_anim_find(int object, char *name)
{
int i;

if(!raydium_object_isvalid(object))
    {
    raydium_log("object: anim_find: ERROR: id or name is invalid");
    return -1;
    }

for(i=0;i<raydium_object_anims[object];i++)
    if(!strcmp(raydium_object_anim_names[object][i],name))
	return i;

return -1;
}

#define _pondavg(a,b,f) ( (a)+(((b)-(a))*(f)) )
//#define _pondavg(a,b,f) ( (a) )

void raydium_object_anim_generate_internal(int object)
{
int i;
GLfloat factor;
int anim_frames;
int frame_a,frame_b;

if(!raydium_object_isvalid(object))
    {
    raydium_log("object: generate_internal: ERROR: id or name is invalid");
    return;
    }

// here starts the big job ! :)

// How many frames for the current anim ?
anim_frames=
    raydium_object_anim_end[object][raydium_object_anim_current[object]] - 
    raydium_object_anim_start[object][raydium_object_anim_current[object]];


// slow ... :( (any good idea to make a modulo on a float ?)

while(raydium_object_anim_frame_current[object]>(anim_frames+1))
    raydium_object_anim_frame_current[object]-=(anim_frames+1);


//printf("frame (int): %i\n",(int)raydium_object_anim_frame_current[object]);
//printf("%f %i\n",raydium_object_anim_frame_current[object],anim_frames);

factor=raydium_object_anim_frame_current[object]-(int)raydium_object_anim_frame_current[object];

frame_a=raydium_object_start[object]+
       (raydium_object_anim_start[object][raydium_object_anim_current[object]] *
        raydium_object_anim_len[object]) +
       (((int)raydium_object_anim_frame_current[object]) *
        raydium_object_anim_len[object]) +
       raydium_object_anim_len[object];

// must verify this test !!!
if( ((int)raydium_object_anim_frame_current[object]) >= anim_frames)
    {
    frame_b=raydium_object_start[object] +
           (raydium_object_anim_start[object][raydium_object_anim_current[object]] *
            raydium_object_anim_len[object]) +
    	    raydium_object_anim_len[object];
    }
else    
    frame_b=frame_a+raydium_object_anim_len[object];

//printf("refresh from %i (a) and %i (b), factor = %.2f (%i af)\n",frame_a,frame_b,factor,anim_frames);

for(i=0;i<raydium_object_anim_len[object];i++)
    {
    raydium_vertex_x[raydium_object_start[object]+i]=_pondavg(raydium_vertex_x[frame_a+i],raydium_vertex_x[frame_b+i],factor);
    raydium_vertex_y[raydium_object_start[object]+i]=_pondavg(raydium_vertex_y[frame_a+i],raydium_vertex_y[frame_b+i],factor);
    raydium_vertex_z[raydium_object_start[object]+i]=_pondavg(raydium_vertex_z[frame_a+i],raydium_vertex_z[frame_b+i],factor);

    raydium_vertex_normal_visu_x[raydium_object_start[object]+i]=_pondavg(raydium_vertex_normal_visu_x[frame_a+i],raydium_vertex_normal_visu_x[frame_b+i],factor);
    raydium_vertex_normal_visu_y[raydium_object_start[object]+i]=_pondavg(raydium_vertex_normal_visu_y[frame_a+i],raydium_vertex_normal_visu_y[frame_b+i],factor);
    raydium_vertex_normal_visu_z[raydium_object_start[object]+i]=_pondavg(raydium_vertex_normal_visu_z[frame_a+i],raydium_vertex_normal_visu_z[frame_b+i],factor);

    raydium_vertex_texture_u[raydium_object_start[object]+i]=_pondavg(raydium_vertex_texture_u[frame_a+i],raydium_vertex_texture_u[frame_b+i],factor);
    raydium_vertex_texture_v[raydium_object_start[object]+i]=_pondavg(raydium_vertex_texture_v[frame_a+i],raydium_vertex_texture_v[frame_b+i],factor);

    raydium_vertex_texture[raydium_object_start[object]+i]=raydium_vertex_texture[frame_a+i];
    }
}

void raydium_object_anim_frame(int object, GLfloat frame)
{
if(!raydium_object_isvalid(object))
    {
    raydium_log("object: anim_frame: ERROR: id or name is invalid");
    return;
    }

raydium_object_anim_frame_current[object]=frame;
raydium_object_anim_generate_internal(object);
}

void raydium_object_anim_frame_name(char *object, GLfloat frame)
{
raydium_object_anim_frame(raydium_object_find(object),frame);
}


void raydium_object_anim(int object, int anim)
{
if(!raydium_object_isvalid(object))
    {
    raydium_log("object: anim: ERROR: id or name is invalid");
    return;
    }

if(anim<0 || anim>=raydium_object_anims[object])
    {
    raydium_log("object: anim: ERROR: id or name is invalid for animation");
    return;    
    }

raydium_object_anim_current[object]=anim;
}

void raydium_object_anim_name(char *object, char *anim)
{
int o;
o=raydium_object_find(object);
raydium_object_anim(o,raydium_object_anim_find(o,anim));
}

