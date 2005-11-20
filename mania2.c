char *version="0.20";
char *title="CQFD Corp. Mania2";

// since we play with our own camera and not Raydium's one:
#define RENDER_DEBUG_NO_CLIP
#define NO_ODE_SUPPORT
#include "raydium/index.c"
#include "mania.h"


#define POP_MODE_ELEM	1
#define POP_MODE_BOX	2


void grid_generate_obj(void);


GLfloat modl_zoom=10;
GLint px=0,py=0;
GLfloat pz=0;
GLint curobj=0;
GLint curbox=0;
GLint curangle=0;
int pop_mode=POP_MODE_ELEM;
char autotag=0;


int n_boxpresets=0;	
Box boxpreset[MAX_ELEMS];

void grid_init_elem(int e)
{
grid[e].state=0;
grid[e].flags=0;
}

void box_init_elem(int e)
{
box[e].state=0;
box[e].type=0;
}


void grid_init_all(void)
{
int i;

for(i=0;i<MAX_ELEMS;i++)
    grid_init_elem(i);

raydium_log("Grid ok");
}

void box_init_all(void)
{
int i;

for(i=0;i<MAX_ELEMS;i++)
    box_init_elem(i);

raydium_log("Boxes ok");
}


void grid_save(char *filename)
{
FILE *fp;
int i;

fp=raydium_file_fopen(filename,"wt");
if(!fp)
    {
    raydium_log("cannot open '%s' for writing",filename);
    return;
    }

fprintf(fp,"d %s\n",tdata);

for(i=0;i<MAX_ELEMS;i++)
  if(grid[i].state)
    fprintf(fp,"g %f %f %f %i %i %s\n",grid[i].x,
				     grid[i].y,
				     grid[i].z,
				     grid[i].rot,
				     grid[i].flags,
				     raydium_object_name[grid[i].obj]);

for(i=0;i<MAX_ELEMS;i++)
  if(box[i].state)
    fprintf(fp,"b %f %f %f %f %f %f %i\n",box[i].x,
				          box[i].y,
				          box[i].z,
				          box[i].tx,
				          box[i].ty,
				          box[i].tz,
				          box[i].type);


fclose(fp);
}

int grid_load(char *filename)
{
FILE *fp;
int i,j;
int c;
char name[RAYDIUM_MAX_NAME_LEN];

fp=raydium_file_fopen(filename,"rt");
if(!fp)
    {
    raydium_log("cannot open '%s' for reading",filename);
    return 0;
    }

grid_init_all();
box_init_all();

i=j=0;

while(1)
{
c=fgetc(fp);
fgetc(fp);

if(c==EOF) break;

if(c=='g')
{
    fscanf(fp,"%f %f %f %i %i %s\n",&grid[i].x,
				      &grid[i].y,
				      &grid[i].z,
				      &grid[i].rot,
				      &grid[i].flags,
				      name);
    grid[i].state=1;
    grid[i].obj=raydium_object_find_load(name);
    i++;
}

if(c=='b')
{
    fscanf(fp,"%f %f %f %f %f %f %i\n",&box[j].x,
				       &box[j].y,
				       &box[j].z,
				       &box[j].tx,
				       &box[j].ty,
				       &box[j].tz,
				       &box[j].type);
    box[j].state=1;
    j++;
}

if(c=='d')
{
    fgets(tdata,4000,fp);
    if(tdata[strlen(tdata)-1]=='\n')
	tdata[strlen(tdata)-1]=0;
}

    if(i==MAX_ELEMS || j==MAX_ELEMS)
	{
	raydium_log("MAX_ELEMS reached, loading not completed");
	break;
	}
}
raydium_log("%s: %i grid elements loaded, %i box(es)",filename,i,j);
fclose(fp);
grid_generate_obj();
return 1;
}

float xcos(int i)
{
return( (float)cos(i*PI/180) );
}

float xsin(int i)
{
return( (float)sin(i*PI/180) );
}


void rotatez(GLfloat angle, GLint obj)
{
int i;
GLfloat x,y;

for(i=raydium_object_start[obj];i<raydium_object_end[obj];i++)
 {
 x=raydium_vertex_x[i]-0.5;
 y=raydium_vertex_y[i]-0.5;

 raydium_vertex_x[i]=x*raydium_trigo_cos(angle) - y*raydium_trigo_sin(angle);
 raydium_vertex_y[i]=x*raydium_trigo_sin(angle) + y*raydium_trigo_cos(angle);
 raydium_vertex_x[i]+=0.5;
 raydium_vertex_y[i]+=0.5;
 }
}


int raydium_object_new_empty(char *name)
{
raydium_object_start[raydium_object_index]=raydium_vertex_index;
raydium_object_end[raydium_object_index]=raydium_vertex_index;
strcpy(raydium_object_name[raydium_object_index],name);
return(raydium_object_index++);
}

void grid_generate_obj(void)
{
int obj;
int i,j;

obj=raydium_object_find("mania.tri");
raydium_vertex_index=raydium_object_start[obj];

for(i=0;i<MAX_ELEMS;i++)
    if(grid[i].state)
    {
    rotatez(grid[i].rot,grid[i].obj);
	for(j=raydium_object_start[grid[i].obj];j<raydium_object_end[grid[i].obj];j++)
	{
	raydium_vertex_x[raydium_vertex_index]=raydium_vertex_x[j]+grid[i].x;
	raydium_vertex_y[raydium_vertex_index]=raydium_vertex_y[j]+grid[i].y;
	raydium_vertex_z[raydium_vertex_index]=raydium_vertex_z[j]+grid[i].z;
	raydium_vertex_texture_u[raydium_vertex_index]=raydium_vertex_texture_u[j];
	raydium_vertex_texture_v[raydium_vertex_index]=raydium_vertex_texture_v[j];
	raydium_vertex_texture[raydium_vertex_index]=raydium_vertex_texture[j];
	raydium_vertex_texture_multi[raydium_vertex_index]=raydium_vertex_texture_multi[j];
	raydium_vertex_texture_multi_u[raydium_vertex_index]=raydium_vertex_texture_multi_u[j];
	raydium_vertex_texture_multi_v[raydium_vertex_index]=raydium_vertex_texture_multi_v[j];
	raydium_vertex_index++;
	}
    rotatez(-grid[i].rot,grid[i].obj);
    }
raydium_object_end[obj]=raydium_vertex_index;
}


void dump_object_to(char *filename, GLint obj)
{
FILE *fp;
//GLuint tex;
GLuint i;
char text[256];

fp=fopen(filename,"wt");
if(!fp) { printf("cannot write to file \"%s\", fopen() failed\n",filename); return; }
fprintf(fp,"1\n");

 for(i=raydium_object_start[obj];i<raydium_object_end[obj];i++)
 {
  if(raydium_vertex_texture_multi[i])
  sprintf(text,"%s;%s",raydium_texture_name[raydium_vertex_texture[i]],raydium_texture_name[raydium_vertex_texture_multi[i]]);
  else
  strcpy(text,raydium_texture_name[raydium_vertex_texture[i]]);

  if(raydium_vertex_texture[i]) 
  fprintf(fp,"%f %f %f %f %f %f %f %f %s\n",
  raydium_vertex_x[i],raydium_vertex_y[i],raydium_vertex_z[i],
  raydium_vertex_normal_visu_x[i], raydium_vertex_normal_visu_y[i], raydium_vertex_normal_visu_z[i],
  raydium_vertex_texture_u[i],raydium_vertex_texture_v[i],
  text);
 }

fclose(fp);
printf("tri file generated.\n");
}


void dump_data_to(char *filename)
{
FILE *fp;
char d[4096];
int i,start,cpt;

fp=fopen(filename,"wt");
if(!fp) { printf("cannot write to file \"%s\", fopen() failed\n",filename); return; }

fprintf(fp,"// generated track data (mania2)\n\n");

strcpy(d,tdata);
cpt=0;
start=0;
for(i=0;i<=strlen(tdata);i++)
    {
    if(d[i]==';' || d[i]==0)
	{
	d[i]=0;
	//printf("%i %s\n",cpt,d+start);
	if(cpt==0)
	    fprintf(fp,"name=\"%s\";\n",d+start);
	if(cpt==1)
	    fprintf(fp,"author=\"%s\";\n",d+start);
	if(cpt==2)
	    fprintf(fp,"gold_time=%s;\n",d+start);
	if(cpt==3)
	    fprintf(fp,"author_time=%s;\n",d+start);

	// finalize
	cpt++;
	start=i+1;
	}
    }

fprintf(fp,"\n// EOF\n");
fclose(fp);
printf("data file generated.\n");
}

void draw_axes(void)
{
GLfloat size=modl_zoom/3;
GLfloat font=modl_zoom/30;

glBegin(GL_LINES);

glVertex3f(0,0,0);
glVertex3f(size,0,0);
glVertex3f(size+font,-font,0);
glVertex3f(size+font+font,font,0);
glVertex3f(size+font,font,0);
glVertex3f(size+font+font,-font,0);

glVertex3f(0,0,0);
glVertex3f(0,size,0);
glVertex3f(0,size+font+(font/2),0);
glVertex3f(0,size+font,0);
glVertex3f(0,size+font+(font/2),0);
glVertex3f(-font/2,size+font+font+(font/2),0);
glVertex3f(0,size+font+(font/2),0);
glVertex3f(font/2,size+font+font+(font/2),0);

glVertex3f(0,0,0);
glVertex3f(0,0,size);
glVertex3f(0,font,size+font-(font/2));
glVertex3f(0,font,size+font+(font/2));
glVertex3f(0,-font,size+font-(font/2));
glVertex3f(0,-font,size+font+(font/2));
glVertex3f(0,font,size+font-(font/2));
glVertex3f(0,-font,size+font+(font/2));

glEnd();
}


void draw_grid(void)
{
GLfloat x;
GLfloat y;
GLfloat z;

glBegin(GL_POINTS);
for(x=-modl_zoom;x<=modl_zoom;x++)
for(y=-modl_zoom;y<=modl_zoom;y++)
for(z=-modl_zoom;z<=modl_zoom;z++)
 {
 glColor3f(0.5,0.5,0.5);
 glVertex3f(x+px,y+py,z);
 }

glColor3f(0.7,0.7,0.7);
glEnd();
}

void draw_box(float x,float y,float z,float ox, float oy, float oz, float tx,float ty,float tz,int type)
{
float lx,ly,lz;
char dir='*';


lx=tx*0.5;
ly=ty*0.5;
lz=tz*0.5;
x+=ox;
y+=oy;
z+=oz;


 switch(type)
  {
  case TYPE_CHECKPOINT:
	    glColor3f(0,0,1);
	    break;

  case TYPE_START_E:
	    dir='e';
	    glColor3f(0,1,0);
	    break;
  case TYPE_START_W:
	    dir='w';
	    glColor3f(0,1,0);
	    break;
  case TYPE_START_N:
	    dir='n';
	    glColor3f(0,1,0);
	    break;
  case TYPE_START_S:
	    dir='s';
	    glColor3f(0,1,0);
	    break;

  case TYPE_END:
	    glColor3f(1,0,0);
	    break;
  case TYPE_LOOP:
	    glColor3f(1,1,1);
	    break;

  case TYPE_TURBO_E:
	    dir='e';
	    glColor3f(1,1,0);
	    break;
  case TYPE_TURBO_W:
	    dir='w';
	    glColor3f(1,1,0);
	    break;
  case TYPE_TURBO_N:
	    dir='n';
	    glColor3f(1,1,0);
	    break;
  case TYPE_TURBO_S:
	    dir='s';
	    glColor3f(1,1,0);
	    break;

  } // end switch

//glColor3f(1,1,1);
glBegin(GL_LINES);
glVertex3f (x,y,z);
switch(dir)
  {
  case 'e':
    glVertex3f (x+1,y,z);
    break;
  case 'w':
    glVertex3f (x-1,y,z);
    break;
  case 'n':
    glVertex3f (x,y+1,z);
    break;
  case 's':
    glVertex3f (x,y-1,z);
    break;
  }
glEnd();


glBegin(GL_TRIANGLE_STRIP);
glNormal3f (-1,0,0);
glVertex3f (-lx+x,-ly+y,-lz+z);
glVertex3f (-lx+x,-ly+y,lz+z);
glVertex3f (-lx+x,ly+y,-lz+z);
glVertex3f (-lx+x,ly+y,lz+z);
glNormal3f (0,1,0);
glVertex3f (lx+x,ly+y,-lz+z);
glVertex3f (lx+x,ly+y,lz+z);
glNormal3f (1,0,0);
glVertex3f (lx+x,-ly+y,-lz+z);
glVertex3f (lx+x,-ly+y,lz+z);
glNormal3f (0,-1,0);
glVertex3f (-lx+x,-ly+y,-lz+z);
glVertex3f (-lx+x,-ly+y,lz+z);
glEnd();

// top face
glBegin (GL_TRIANGLE_FAN);
glNormal3f (0,0,1);
glVertex3f (-lx+x,-ly+y,lz+z);
glVertex3f (lx+x,-ly+y,lz+z);
glVertex3f (lx+x,ly+y,lz+z);
glVertex3f (-lx+x,ly+y,lz+z);
glEnd();

// bottom face
glBegin (GL_TRIANGLE_FAN);
glNormal3f (0,0,-1);
glVertex3f (-lx+x,-ly+y,-lz+z);
glVertex3f (-lx+x,ly+y,-lz+z);
glVertex3f (lx+x,ly+y,-lz+z);
glVertex3f (lx+x,-ly+y,-lz+z);
glEnd();

}

void draw_selection(void)
{

if(pop_mode==POP_MODE_BOX)
 {
 draw_box(px,py,pz,
 boxpreset[curbox].x,
 boxpreset[curbox].y,
 boxpreset[curbox].z,
 boxpreset[curbox].tx,
 boxpreset[curbox].ty,
 boxpreset[curbox].tz,
 boxpreset[curbox].type);
 }

glBegin(GL_LINES);
glColor3f(0,1,0);
glVertex3f(px,py,pz);
glVertex3f(px,py+1,pz);
glVertex3f(px,py+1,pz);
glVertex3f(px+1,py+1,pz);
glVertex3f(px+1,py+1,pz);
glVertex3f(px+1,py,pz);
glVertex3f(px+1,py,pz);
glVertex3f(px,py,pz);
glEnd();

if(pop_mode==POP_MODE_ELEM)
 {
 glPushMatrix();
 glTranslatef(px,py,pz);
 rotatez(curangle,curobj);
 raydium_object_draw(curobj);
 rotatez(-curangle,curobj);
 glPopMatrix();
 }

}

void add(void)
{
GLint i;
raydium_log("pop !");

for(i=0;i<MAX_ELEMS;i++)
    if(!grid[i].state)
	{
	grid[i].state=1;
	grid[i].x=px;
	grid[i].y=py;
	grid[i].z=pz;
	grid[i].flags=0;
	grid[i].rot=curangle;
	grid[i].obj=curobj;	
	grid_generate_obj();
	return;
	}

raydium_log("MAX_ELEMS reached");
}

void add_box(void)
{
GLint i;
float x,y,z;
float tx,ty,tz;
int type;

x=px+boxpreset[curbox].x;
y=py+boxpreset[curbox].y;
z=pz+boxpreset[curbox].z;
tx=boxpreset[curbox].tx;
ty=boxpreset[curbox].ty;
tz=boxpreset[curbox].tz;
type=boxpreset[curbox].type;

raydium_log("pop the box !");

for(i=0;i<MAX_ELEMS;i++)
    if(!box[i].state)
	{
	box[i].state=1;
	box[i].x=x;
	box[i].y=y;
	box[i].z=z;
	box[i].tx=tx;
	box[i].ty=ty;
	box[i].tz=tz;
	box[i].type=type;
	return;
	}
raydium_log("MAX_ELEMS reached for boxes");
}

void del(void)
{
GLint i;

if(pop_mode==POP_MODE_ELEM)
for(i=0;i<MAX_ELEMS;i++)
    if( grid[i].state && 
        grid[i].x==px &&
        grid[i].y==py &&
        grid[i].z==pz )
	    grid[i].state=0;

if(pop_mode==POP_MODE_BOX)
for(i=0;i<MAX_ELEMS;i++)
    if( box[i].state   && 
        box[i].x>=px   &&
        box[i].x<=px+1 &&
        box[i].y>=py   &&
        box[i].y<=py+1 &&
        box[i].z>=pz   &&
        box[i].z<=pz+1 )
	    box[i].state=0;
	    
grid_generate_obj();
}

void dump_boxes_to(char *filename, float fact, float mx, float my, float mz)
{
FILE *fp;
int i;

fp=fopen(filename,"wt");
if(!fp)
    {
    raydium_log("cannot open '%s' for writing",filename);
    return;
    }


for(i=0;i<MAX_ELEMS;i++)
  if(box[i].state)
    fprintf(fp,"%f %f %f %f %f %f %i\n",  box[i].x*fact+mx,
				          box[i].y*fact+my,
				          box[i].z*fact+mz,
				          box[i].tx*fact,
				          box[i].ty*fact,
				          box[i].tz*fact,
				          box[i].type);
fclose(fp);
}

void export_all(void)
{
#define FACT 6
#define MOVE_X -5
#define MOVE_Y -3
#define MOVE_Z -1
int i;
int obj;

obj=raydium_object_find("mania.tri");

// resize and move
for(i=raydium_object_start[obj];i<raydium_object_end[obj];i++)
 {
 raydium_vertex_x[i]*=FACT;
 raydium_vertex_x[i]+=MOVE_X;

 raydium_vertex_y[i]*=FACT;
 raydium_vertex_y[i]+=MOVE_Y;

 raydium_vertex_z[i]*=FACT;
 raydium_vertex_z[i]+=MOVE_Z;
 }

raydium_normal_regenerate_all();
dump_object_to("mania.tri",obj);
dump_boxes_to("mania.box",FACT,MOVE_X,MOVE_Y,MOVE_Z);
dump_data_to("mania.dat");

grid_generate_obj();
if(!autotag)
    grid_save("dyn_track.mni");
}

void mouse_n_keys_event(void)
{
GLfloat rotx,roty;

//printf("%i\n",raydium_key_last);
if(raydium_key_last==1027) exit(0);
if(raydium_key_last==1045)  modl_zoom--;
if(raydium_key_last==1043)  modl_zoom++;
if(raydium_key_last==1100)  del();
if(raydium_key_last==1009)  curangle+=90;
if(raydium_key_last==1032 && pop_mode==POP_MODE_ELEM)  add();
if(raydium_key_last==1032 && pop_mode==POP_MODE_BOX)  add_box();
if(raydium_key_last==2) export_all();

if(raydium_key_last==101) py++;
if(raydium_key_last==103) py--;
if(raydium_key_last==100) px--;
if(raydium_key_last==102) px++;

if(raydium_key_last==105 && pop_mode==POP_MODE_ELEM) curobj--;
if(raydium_key_last==104 && pop_mode==POP_MODE_ELEM) curobj++;
if(raydium_key_last==105 && pop_mode==POP_MODE_BOX) curbox--;
if(raydium_key_last==104 && pop_mode==POP_MODE_BOX) curbox++;
if(raydium_key_last==106) pz+=0.25;
if(raydium_key_last==107) pz-=0.25;

if(raydium_key_last==1013 && pop_mode==POP_MODE_ELEM) pop_mode=POP_MODE_BOX;
else
if(raydium_key_last==1013 && pop_mode==POP_MODE_BOX) pop_mode=POP_MODE_ELEM;


if(curobj<0) curobj=0;
if(curbox<0) curbox=0;
if((curobj+1)>=raydium_object_index) curobj=raydium_object_index-2;
if(curbox>=n_boxpresets) curbox=n_boxpresets-1;

curangle%=360;
if(modl_zoom<1) modl_zoom=1;

// Camera
 if(raydium_mouse_button[0]) 
 {
  rotx=((float)raydium_mouse_x-((float)raydium_window_tx/(float)2)) * ((float)360/(float)raydium_window_tx);
  roty=((float)raydium_mouse_y-((float)raydium_window_ty/(float)2)) * ((float)360/(float)raydium_window_ty);
  glRotatef(roty,1,0,0);
  glRotatef(rotx,0,1,0);
 }
glTranslatef(-px,-py,-pz);

}


void update_vars(void)
{
raydium_projection=RAYDIUM_PROJECTION_ORTHO;
raydium_projection_near=-modl_zoom;
raydium_projection_far=modl_zoom;
raydium_projection_left=-modl_zoom;
raydium_projection_right=modl_zoom;
raydium_projection_bottom=-modl_zoom;
raydium_projection_top=modl_zoom;
raydium_window_resize_callback(raydium_window_tx,raydium_window_ty);
}



void display(void)
{
int i;
raydium_clear_frame();

//glLoadIdentity();
update_vars();

mouse_n_keys_event();

glDisable(GL_TEXTURE_2D);
draw_grid();
draw_axes();

for(i=0;i<MAX_ELEMS;i++)
  if(box[i].state)
    {
    // draw boxes here
    draw_box(0,0,0,
             box[i].x,box[i].y,box[i].z,
             box[i].tx,box[i].ty,box[i].tz,
	     box[i].type);
    }

draw_selection();
glColor3f(1.0,1.0,1.0);
glEnable(GL_TEXTURE_2D);

//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
raydium_object_draw_name("mania.tri");

raydium_rendering_finish();
raydium_light_disable();
}

void load_all_in(char *name)
{
char trifile[255]; // eeeerk :)
FILE *fp;
fp=raydium_file_fopen(name,"rt");
if(!fp)
    {
    printf("Need a %s config file, with all available tri files..\n",name);
    exit(1);
    }

while(fgets(trifile,255,fp)!=NULL)
    {
//    if(!strlen(trifile)) continue;
//    trifile[strlen(trifile)-1]=0;
    raydium_parser_trim(trifile);
    printf("-%s-\n",trifile);
    if(!strlen(trifile)) continue;
    if(trifile[0]=='#') continue;
    if(trifile[0]=='*')
	{
	boxpreset[n_boxpresets].state=1;
	sscanf(trifile+2,"%f %f %f %f %f %f %i",
				       &boxpreset[n_boxpresets].x,
				       &boxpreset[n_boxpresets].y,
				       &boxpreset[n_boxpresets].z,
				       &boxpreset[n_boxpresets].tx,
				       &boxpreset[n_boxpresets].ty,
				       &boxpreset[n_boxpresets].tz,
				       &boxpreset[n_boxpresets].type);
	n_boxpresets++;
	}
    else raydium_object_load(trifile);
    }
fclose(fp);
}

char file_exists(char *name)
{
FILE *fp;
fp=fopen(name,"r");
if(fp) { fclose(fp); return 1; }
return 0;
}


int main(int argc, char **argv)
{
char autogen[256];
char window[256];
sprintf(window,"%s - %s",title,version);

sprintf(tdata,"no name;unknown;0;0");

raydium_init_args(argc,argv);

if(raydium_init_cli_option("auto",autogen))
    {
    raydium_window_create(0,0,RAYDIUM_RENDERING_NONE,"");    
    autotag=1;
    }
else
    {
    raydium_window_create(700,700,RAYDIUM_RENDERING_WINDOW,window);
    }

raydium_background_color_change(0,0,0,1);
raydium_rendering_displaylists_disable();

// place here all available objets:
load_all_in("mania.cfg");
raydium_object_new_empty("mania.tri");
grid_init_all();
box_init_all();

// stop here (must be LAST object)
if(!autotag)
    {
    if(file_exists("dyn_track.mni"))
	grid_load("dyn_track.mni");
    }
else
    {
    if(!grid_load(autogen))
	return 1;
    export_all();
    return 0;
    }

raydium_mouse_show();
raydium_texture_filter=RAYDIUM_TEXTURE_FILTER_TRILINEAR;
//raydium_texture_current=1;

raydium_callback(&display);

return(0);
}
