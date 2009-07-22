#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

char EXT[]=".jpg";
#define BUFFSIZE  4096

void copy(char *name,FILE *fpo)
{
FILE *fp;
char buf[BUFFSIZE];
size_t nmemb;

fp=fopen(name,"rb");
if(!fp)
    {
    printf("cannot open %s\n",name);
    exit(3);
    }


//while(fread(buf,BUFFSIZE,1,fp))
 while ((nmemb=fread(buf, 1, sizeof(buf), fp)) > 0)
    {
    fwrite(buf,1,nmemb,fpo);
    }

fclose(fp);
}

long filesize(char *name)
{
FILE *fp;
long ret;

fp=fopen(name,"rb");
if(!fp)
    {
    printf("cannot open %s\n",name);
    exit(3);
    }
fseek(fp,0,SEEK_END);
ret=ftell(fp);
fclose(fp);
return ret;
}

int is_jpg(const struct dirent *ent)
{
int l,m;
const char *name;

name=ent->d_name;
l=strlen(EXT);
m=strlen(name);

if(l<strlen(EXT))
    return 0;

if( name[m-l+0]==EXT[0] &&
    name[m-l+1]==EXT[1] &&
    name[m-l+2]==EXT[2] &&
    name[m-l+3]==EXT[3] )
        return 1;

return 0;
}

int main(int argc, char **argv)
{
float fps;
int sx,sy,total;
char out[512];
char head[100];
FILE *fpo;
struct dirent **namelist;
int i;
long offset;

if(argc!=5)
    {
    printf("This program will create a movie file (JPGS) for Raydium 3D Game Engine\n");
    printf("using all jpg files in the current directory.\n");
    printf("usage: %s fps sizex sizey output.jpgs\n",argv[0]);
    exit(0);
    }

fps=atof(argv[1]);
sx=atoi(argv[2]);
sy=atoi(argv[3]);
strcpy(out,argv[4]);

fpo=fopen(out,"wb");
if(!fpo)
    {
    printf("cannot open %s for writing\n",out);
    exit(1);
    }

total=scandir(".", &namelist, is_jpg, alphasort);
if(total<0)
    {
    perror("scandir");
    exit(15);
    }

printf("%i files found, creating header ...\n",total);
offset=0;
sprintf(head,"%f %i %i %i|",fps,sx,sy,total);
fprintf(fpo,"%s",head);

// header, second part
offset=0;
for(i=0;i<total;i++)
    {
    fprintf(fpo,"%i|",offset);
    offset+=filesize(namelist[i]->d_name);
    }

// body
printf("creating file ...\n");
for(i=0;i<total;i++)
    {
    copy(namelist[i]->d_name,fpo);
    }

fclose(fpo);
printf("%s created\n",out);
}
