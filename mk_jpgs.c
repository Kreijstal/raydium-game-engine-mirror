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

signed char is_jpg(char *name)
{
int l,m;
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
DIR *dir;
struct dirent *tmp;
long offset;

if(argc!=5)
    {
    printf("This program will create a movie file (JPGS) for Raydium 3D Game Engine\n");
    printf("using all jpg files in the current directory.\n");
    printf("usage: %s fps sizex sizey output\n",argv[0]);
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


if (!(dir = opendir(".")))
    {
    printf("cannot list current dir\n");
    exit(2);
    }

total=0;
while ((tmp = readdir(dir)))
    {
    if(is_jpg(tmp->d_name))
        total++;
    }

offset=0;
sprintf(head,"%f %i %i %i|",fps,sx,sy,total);
fprintf(fpo,"%s",head);

// header, second part
offset=0;
rewinddir(dir);

while ((tmp = readdir(dir)))
    {
    if(!is_jpg(tmp->d_name))
        {
        continue;
        }

    fprintf(fpo,"%i|",offset);
    offset+=filesize(tmp->d_name);
    }

// body
rewinddir(dir);

while ((tmp = readdir(dir)))
    {
    if(!is_jpg(tmp->d_name))
        {
        continue;
        }

    copy(tmp->d_name,fpo);
    }

fclose(fpo);
printf("%s created\n",out);
}
