/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/file.h"
#endif 

// WARNING: most functions of this file are not part of Raydium yet !
// So, be carefull with functions without "raydium_file" prefix.


#define DONT_SAVE_DUMMY_TEXTURE


// far better than glibc's 'dirname' (and portable)
void raydium_file_dirname(char *dest,char *from)
{
char *c;
int n;

c=strrchr(from,'/'); // Unix
if(!c)
      c=strrchr(from,'\\'); // win32

if(!c)
    {
    strcpy(dest,"./");
    return;
    }
n=c-from;
memcpy(dest,from,n+1);
dest[n+1]=0;
}


void raydium_file_log_fopen_display(void)
{
int i;

raydium_log("List of all opended files:");

for(i=0;i<raydium_file_log_fopen_index;i++)
    raydium_log("%s",raydium_file_log_fopen[i]);

}

FILE *raydium_file_fopen(char *file, char *mode)
{
#ifdef PHP_SUPPORT
FILE *fp;
int i;
char found=0;
char *complete_path;
char path[] = ".:media:..:../media";
char *test;
char *context;

if(!file || !strlen(file))
    return NULL;

//explicit absolute path
if (file[0] == '/')
  complete_path = file;
//explicit relative path
else if (file[0] == '.')
{
   complete_path = malloc(sizeof(char) * (strlen(getcwd(NULL, 0)) + strlen(file) + 2));
   strcpy(complete_path, getcwd(NULL, 0));
   strcat(complete_path, "/");
   strcat(complete_path, file);
}

//no path given :
else 
{
  test = strtok_r(path, ":", &context);
  while (test) 
    {
      complete_path = malloc(sizeof(char) * (strlen(file) + strlen(test) + 2));
      strcpy(complete_path, test);
      strcat(complete_path, "/");
      strcat(complete_path, file);
      if (access(complete_path, R_OK) == 0)
	{
	  break;
	} 
      else
	{
	  free(complete_path);
	}
      test = strtok_r(NULL, ":", &context);
    }
}
 
if (strlen(complete_path) == 0)
     complete_path=file;
     
for(i=0;i<raydium_file_log_fopen_index;i++)
    if(!strcmp(raydium_file_log_fopen[i],complete_path))
	{
	found=1;
	break;
	}

if(!found) strcpy(raydium_file_log_fopen[raydium_file_log_fopen_index++],file);

if(strchr(mode,'w') || raydium_init_cli_option("repository-disable",NULL))
    {
    return fopen(complete_path,mode);
    }

if( !raydium_init_cli_option("repository-refresh",NULL) && 
    !raydium_init_cli_option("repository-force",NULL) )
{
 fp=fopen(complete_path,mode);
 if(fp) return fp;
}

//if (access("font2.tga", R_OK) == 0)
//    raydium_osd_printf(2,98,16,0.5,"font2.tga","Loading %s", file);
if (raydium_rayphp_repository_file_get(complete_path))
{
  // caution, complete_path is now garbage, 
  // let's use file because we dl in "." directory anyways
  return fopen(file,mode);
}
else 
return NULL;
#else
return fopen(file,mode);
#endif
}

void dump_vertex_to(char *filename)
{
FILE *fp;
//GLuint tex;
GLuint i;
char text[256];
char bl;

fp=fopen(filename,"wt");
if(!fp) { printf("cannot write to file \"%s\", fopen() failed\n",filename); return; }

fprintf(fp,"1\n");

/*
for(tex=0;tex<raydium_texture_index;tex++)
{
// fprintf(fp,"%s\n",raydium_texture_name[tex]);
// fprintf(fp,"%i\n",j);
 for(i=0;i<raydium_vertex_index;i++)
  if(raydium_vertex_texture[i]==tex)
  fprintf(fp,"%f %f %f %f %f %s\n",
  raydium_vertex_x[i],raydium_vertex_y[i],raydium_vertex_z[i],
  raydium_vertex_texture_u[i],raydium_vertex_texture_v[i],raydium_texture_name[tex]);
}
*/
for(bl=0;bl<2;bl++)
{
 for(i=0;i<raydium_vertex_index;i++)
 if( (raydium_texture_blended[raydium_vertex_texture[i]]?1:0) == bl)
 {
  if(raydium_vertex_texture_multi[i])
  {
   sprintf(text,"%s;%f|%f|%s",raydium_texture_name[raydium_vertex_texture[i]],
			      raydium_vertex_texture_multi_u[i],
			      raydium_vertex_texture_multi_v[i],
			      raydium_texture_name[raydium_vertex_texture_multi[i]]);
  }
  else
  strcpy(text,raydium_texture_name[raydium_vertex_texture[i]]);

#ifdef DONT_SAVE_DUMMY_TEXTURE 
  if(raydium_vertex_texture[i]) 
#endif
  fprintf(fp,"%f %f %f %f %f %f %f %f %s\n",
  raydium_vertex_x[i],raydium_vertex_y[i],raydium_vertex_z[i],
  raydium_vertex_normal_visu_x[i], raydium_vertex_normal_visu_y[i], raydium_vertex_normal_visu_z[i],
  raydium_vertex_texture_u[i],raydium_vertex_texture_v[i],
  text);
 }
}

fclose(fp);
printf("saved.\n");
}

// sorts alpha textures
void dump_vertex_to_alpha(char *filename)
{
FILE *fp;
GLuint tex;
GLuint i;
char text[256];
int bl;


raydium_log("WARNING: 'dump_vertex_to_alpha' function is deprecated, since regular 'dump_vertex_to' function now sorts alpha textures");


fp=fopen(filename,"wt");
if(!fp) { printf("cannot write to file \"%s\", fopen() failed\n",filename); return; }
fprintf(fp,"1\n");

for(bl=0;bl<2;bl++)
{
for(tex=0;tex<raydium_texture_index;tex++)
if( (raydium_texture_blended[tex]?1:0) == bl)
{
  printf("%s\n",raydium_texture_name[tex]);
  strcpy(text,raydium_texture_name[tex]);
// fprintf(fp,"%i\n",j);
 for(i=0;i<raydium_vertex_index;i++)
  if(raydium_vertex_texture[i]==tex )
  fprintf(fp,"%f %f %f %f %f %f %f %f %s\n",
  raydium_vertex_x[i],raydium_vertex_y[i],raydium_vertex_z[i],
  raydium_vertex_normal_visu_x[i], raydium_vertex_normal_visu_y[i], raydium_vertex_normal_visu_z[i],
  raydium_vertex_texture_u[i],raydium_vertex_texture_v[i],
  text);
}
printf("----\n");
}


fclose(fp);
printf("saved.\n");
}


#define MULTI_SEP ';'
#define MULTI_UVSEP '|'
#define ENVMAP_SEP '#'
int raydium_file_set_textures(char *name)
{
char *sep;
char *sep2=NULL;
char *sep_env;
char texname[RAYDIUM_MAX_NAME_LEN];

sep = strchr(name,MULTI_SEP);
sep_env = strchr(name,ENVMAP_SEP);
if(sep) sep2 = strchr(sep+1,MULTI_UVSEP);

if(sep_env)
{
  raydium_texture_current_multi=0;
  raydium_texture_current_env=raydium_texture_find_by_name(sep_env+1);
  *sep_env=0;
  raydium_texture_current_set_name(name);
  *sep_env=ENVMAP_SEP;
  return 3;
}


// 2 textures + 1 uv
if(sep && sep2)
{
  sscanf(sep+1,"%f|%f|%s\n", &raydium_texture_current_multi_u,
			    &raydium_texture_current_multi_v,
			    texname);
  raydium_texture_current_env=0;
  raydium_texture_current_multi=raydium_texture_find_by_name(texname);
  *sep=0;
  raydium_texture_current_set_name(name);
  *sep=MULTI_SEP;
  return 2;
} 
 

// 2 textures, but 0 uv
if(sep && !sep2)
{
  raydium_texture_current_env=0;
  raydium_texture_current_multi=raydium_texture_find_by_name(sep+1);
  *sep=0;
  raydium_texture_current_set_name(name);
  *sep=MULTI_SEP;
  raydium_texture_current_multi_u=-99999;
  raydium_texture_current_multi_v=-99999;
  return 1;
} 
  
// 1 texture and 0 uv
if(!sep && !sep2)
{ 
  raydium_texture_current_env=0;
  raydium_texture_current_multi=0;
  raydium_texture_current_set_name(name);
  return 0;
}

return -1; // should never reach this
}



void read_vertex_from(char *filename)
{
GLfloat x,y,z,nx,ny,nz,u,v;
int i,ii;
GLuint save;
GLint visu;
FILE *fp;
char name[RAYDIUM_MAX_NAME_LEN];

fp=raydium_file_fopen(filename,"rt");
if(!fp) { printf("cannot read from file \"%s\", fopen() failed\n",filename); return; }
fscanf(fp,"%i\n",&visu);


raydium_log("Object: loading \"%s\", version %i",filename,visu);

// test here version 2 (anims)
if(visu==2)
    {
    int j,k;
    fscanf(fp,"%i %i\n",&j,&k);
    
    if(j>RAYDIUM_MAX_OBJECT_ANIMS)
	{
	raydium_log("object: too much anims for this fime ! (%i max)",RAYDIUM_MAX_OBJECT_ANIMS);
	j=RAYDIUM_MAX_OBJECT_ANIMS; // will no work ;) (fixme)
	}
    
    raydium_object_anims[raydium_object_index]=j;
    raydium_object_anim_len[raydium_object_index]=k;
    raydium_object_anim_instance_current[raydium_object_index]=0;
    raydium_object_anim_default_anim[raydium_object_index]=0;

    for(ii=0;ii<RAYDIUM_MAX_OBJECT_ANIM_INSTANCES;ii++)
	{
	raydium_object_anim_current[raydium_object_index][ii]=0;
	raydium_object_anim_frame_current[raydium_object_index][ii]=0;
	raydium_object_anim_previous[raydium_object_index][ii]=-1;
	raydium_object_anim_frame_previous[raydium_object_index][ii]=0;
	raydium_object_anim_frame_previous_timeout[raydium_object_index][ii]=0;
	raydium_object_anim_punctually_flag[raydium_object_index][ii]=-1;
	}


    for(i=0;i<raydium_object_anims[raydium_object_index];i++)
	{
	fscanf(fp,"%i %i %s\n",&j,&k,name);
	raydium_object_anim_start[raydium_object_index][i]=j;
	raydium_object_anim_end[raydium_object_index][i]=k;
	raydium_object_anim_automatic_factor[raydium_object_index][i]=0;
	strcpy(raydium_object_anim_names[raydium_object_index][i],name);
	}

    // build "current transformed model" space
    for(i=0;i<raydium_object_anim_len[raydium_object_index];i++)
	{
	raydium_vertex_add(0,0,0);
	raydium_vertex_texture[raydium_vertex_index-1]=0;
	}

    fscanf(fp,"%i\n",&visu);
    raydium_log("object: anim: %i frame(s) with %i vertice per frame (ver %i)",raydium_object_anims[raydium_object_index],raydium_object_anim_len[raydium_object_index],visu);
    }
// ...

save=raydium_texture_current_main;
i=0;

if(visu>0)
{
 while( fscanf(fp,"%f %f %f %f %f %f %f %f %s\n",&x,&y,&z,&nx,&ny,&nz,&u,&v,name)!=EOF )
 {
  raydium_file_set_textures(name);
  raydium_vertex_uv_normals_add(x,y,z,nx,ny,nz,u,v);
  i++;
 }
}
else if(visu==0)
{
 while( fscanf(fp,"%f %f %f %f %f %s\n",&x,&y,&z,&u,&v,name)!=EOF )
 {
  raydium_file_set_textures(name);
  raydium_vertex_uv_add(x,y,z,u,v);
  i++;
 }
}
else if(visu<0)
{
 while( fscanf(fp,"%f %f %f %s\n",&x,&y,&z,name)!=EOF )
 {
  raydium_file_set_textures(name);
  raydium_vertex_add(x,y,z);
  i++;
 }

}

if(i%3) 
    {
    printf("ERROR with object %s ... must be *3 !",filename);
    // and generate dummy vertices ?
    }

fclose(fp);

raydium_texture_current_multi=0;
raydium_texture_current_set(save);
//printf("loaded.\n");
}

unsigned long raydium_file_sum_simple(char *filename)
{
unsigned long total=0;
unsigned long cpt=0;
int c;
FILE *fp;

fp=raydium_file_fopen(filename,"rb");
if(!fp)
    {
    raydium_log("file simple sum: error: cannot open file '%s'",filename);
    return 0;
    }

while( (c=fgetc(fp))!=EOF )
    {
    c*=cpt;
    total+=c;
    cpt++;
    }

fclose(fp);
return total;
}
