/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/parser.h"
#endif

// Trims string (left and right), removing ' ' and '\n' and ';'
void raydium_parser_trim(char *org)
{
int i;
char temp[RAYDIUM_MAX_NAME_LEN];
strcpy(temp,org);

for(i=0;i<strlen(temp);i++)
    if(temp[i]!=' ')
	break;
strcpy(org,temp+i); // heading spaces: ok

for(i=strlen(org);i>=0;i--)
    if(org[i]!=' ' && org[i]!='\n' && org[i]!=0 && org[i]!=';')
	break;

org[i+1]=0; // tailing chars: ok
}

signed char raydium_parser_isdata(char *str)
{
if(strlen(str)==0) return 0;
if(str[0]=='/' && str[1]=='/') return 0;
return 1;
}


signed char raydium_parser_cut(char *str,char *part1, char *part2, char separator)
{
// strstr, strok and strsep aren't that good ;)
int i;
int len;

len=strlen(str);

for(i=0;i<len;i++)
    if(str[i]==separator)
	break;

if(i==len) return 0; // not found

strcpy(part2,str+i+1);
strcpy(part1,str);
part1[i]=0;
raydium_parser_trim(part1);
raydium_parser_trim(part2);
return i+1;
}

void raydium_parser_replace(char *str,char what, char with)
{
int len,i;

len=strlen(str);

for(i=0;i<len;i++)
    if(str[i]==what)
	str[i]=with;

}


int raydium_parser_read(char *var, char *val_s, GLfloat *val_f, int *size, FILE *fp)
{
char str[RAYDIUM_MAX_NAME_LEN];
char *ret;

do {
str[0]=0;
ret=fgets(str,RAYDIUM_MAX_NAME_LEN-1,fp);
raydium_parser_trim(str);
} while(ret!=NULL && !raydium_parser_isdata(str));

if(ret==NULL)
    {
    *size=0;
    return RAYDIUM_PARSER_TYPE_EOF;
    }

raydium_parser_cut(str,var,val_s,'=');

if(val_s[0]=='[') // is raw data (RAYDIUM_MAX_NAME_LEN limit !)
    {
    int offset=0;
    int len;
    
    do {
	str[0]=0;
	ret=fgets(str,RAYDIUM_MAX_NAME_LEN-1,fp);
	if(ret==NULL)
	    break;
	if(strlen(str)>=1 && str[0]==']') // bad idea, but ... no time for now
	    break;
	len=strlen(str);
	memcpy(val_s+offset,str,len);
	offset+=len;
    } while(1);
    
    val_s[offset]=0;
    *size=offset;
    return RAYDIUM_PARSER_TYPE_RAWDATA;
    }

if(val_s[0]=='"') // is a string
    {
    raydium_parser_replace(val_s,'"',' ');    
    raydium_parser_trim(val_s);
    *size=strlen(val_s);
    return RAYDIUM_PARSER_TYPE_STRING;
    }

if(val_s[0]=='{') // is a float array
    {
    char extracted[RAYDIUM_MAX_NAME_LEN];
    char next[RAYDIUM_MAX_NAME_LEN];
    
    *size=0;
    raydium_parser_replace(val_s,'{',' ');
    raydium_parser_trim(val_s);

    while(raydium_parser_cut(val_s,extracted,next,','))
	{
	val_f[*size]=atof(extracted);
	(*size)++;
	strcpy(val_s,next);
	}
    val_f[*size]=atof(next);
    (*size)++;
    val_s[0]=0;
    return RAYDIUM_PARSER_TYPE_FLOAT;    
    }

// is a float
*val_f=atof(val_s);
*size=1;
return RAYDIUM_PARSER_TYPE_FLOAT;    

}
