/*
    Raydium - CQFD Corp.
    http://raydium.cqfd-corp.org
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/trigo.h"
#endif 

GLfloat raydium_trigo_cos(GLfloat i)
{
return( (GLfloat)cos(i*PI/180) );
}

GLfloat raydium_trigo_sin(GLfloat i)
{
return( (GLfloat)sin(i*PI/180) );
}

GLfloat raydium_trigo_cos_inv(GLfloat i)
{
return(acos(i)*180/PI);
}

GLfloat raydium_trigo_sin_inv(GLfloat i)
{
return(asin(i)*180/PI);
}

void raydium_trigo_rotate(GLfloat *p, GLfloat rx, GLfloat ry, GLfloat rz, GLfloat *res)
{
res[0]= (p[0]*raydium_trigo_cos(ry)+(p[2]*raydium_trigo_cos(rx)+p[1]*raydium_trigo_sin(rx))*raydium_trigo_sin(ry))*raydium_trigo_cos(rz) + (p[1]*raydium_trigo_cos(rx)-p[2]*raydium_trigo_sin(rx))*raydium_trigo_sin(rz);
res[1]=-(p[0]*raydium_trigo_cos(ry)+(p[2]*raydium_trigo_cos(rx)+p[1]*raydium_trigo_sin(rx))*raydium_trigo_sin(ry))*raydium_trigo_sin(rz) + (p[1]*raydium_trigo_cos(rx)-p[2]*raydium_trigo_sin(rx))*raydium_trigo_cos(rz);
res[2]= (p[2]*raydium_trigo_cos(rx)+ p[1]*raydium_trigo_sin(rx))*raydium_trigo_cos(ry)-p[0]*raydium_trigo_sin(ry);
}


// pos: GLfloat[3], m: GLfloat[16]
void raydium_trigo_pos_to_matrix(GLfloat *pos, GLfloat *m)
{
m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = pos[0];
m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = pos[1];
m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = pos[2];
m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

// res: GLfloat[3]
void raydium_trigo_pos_get_modelview(GLfloat *res)
{
GLfloat tmp[16];
glGetFloatv(GL_MODELVIEW_MATRIX,tmp);
res[0]=tmp[12];
res[1]=tmp[13];
res[2]=tmp[14];
}

/* Unfinished !
// pos == res is safe
void raydium_trigo_carth_to_sphere(GLfloat *pos, GLfloat *res)
{
GLfloat r,G,T;

r=sqrt(pos[0]*pos[0] + pos[1]*pos[1] + pos[2]*pos[2]);
G=atan(pos[1]/pos[0]);
T=acos(pos[2]/r);

//printf("%f\n",r);
r=1;

if(pos[0]<0)
    res[0]=-r*cos(G);
else
    res[0]=+r*cos(G);

//res[1]=r*sin(G);
if(pos[0]<0)
    res[0]=-r*sin(T)*cos(G);
else
    res[0]=r*sin(T)*cos(G);
res[1]=r*sin(G)*cos(G);
res[2]=r*cos(T);

if(isnan(res[0])) res[0]=0;
if(isnan(res[1])) res[1]=0;
if(isnan(res[2])) res[2]=0;

}
*/
