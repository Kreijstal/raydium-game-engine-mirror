/*
    Raydium - CQFD Corp.
    http://raydium.org/
    Released under both BSD license and Lesser GPL library license.
    See "license.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/path.h"
#endif

#include "path.h"

void raydium_path_package_update(void);

int raydium_path_package_find(char *name)
{
int i;
int l;

for (i=0;i<RAYDIUM_MAX_PATHS;i++)
    {
    if (!raydium_path_paths[i].state)
        continue;
    l=strlen(raydium_path_paths[i].path)-strlen(name);
    if (strcmp(&raydium_path_paths[i].path[l],name)==0)
        return i;
    }
return RAYDIUM_PATH_PACKAGE_NOT_FOUND;
}

int raydium_path_find_free(void)
{
int i;

for(i=0;i<RAYDIUM_MAX_PATHS;i++)
    if(!raydium_path_paths[i].state)
        return i;
return -1;
}

signed char raydium_path_ext(char *dir,char *ext)
{
    return raydium_path_ext_priority(dir,ext,2);// 2 is default user priority
}

signed char raydium_path_ext_priority(char *dir, char *ext,int priority)
{
int id;

id=raydium_path_find_free();
if(id<0)
    {
    raydium_log("path: ERROR: no more free path slot");
    return 0;
    }

if(dir==NULL || strlen(dir)==0)
    {
    raydium_log("path: ERROR: directory name is not valid");
    return 0;
    }

if(ext==NULL || strlen(ext)==0)
    {
    raydium_log("path: ERROR: extension is not valid for this path");
    return 0;
    }

if(strlen(ext)>=2 && ext[0]=='*' && ext[1]=='.')
    ext+=2;

strcpy(raydium_path_paths[id].ext,ext);
strcpy(raydium_path_paths[id].path,dir);

if(dir[strlen(dir)-1]=='/')
    raydium_path_paths[id].path[strlen(dir)-1]=0;

raydium_path_paths[id].mode=RAYDIUM_PATH_MODE_READ;
raydium_path_paths[id].state=1;
raydium_path_paths[id].priority=priority;
return 1;
}

signed char raydium_path_add(char *dir)
{
    return raydium_path_add_priority(dir,2); // Default high priority for user path adds
}

signed char raydium_path_add_priority(char *dir,int priority)
{
int id;

id=raydium_path_find_free();
if(id<0)
    {
    raydium_log("path: ERROR: no more free path slot");
    return 0;
    }

if(dir==NULL || strlen(dir)==0)
    {
    raydium_log("path: ERROR: directory name is not valid");
    return 0;
    }

if(strchr(dir,'*'))
    {
    char path[RAYDIUM_MAX_DIR_LEN];
    char ext[RAYDIUM_MAX_DIR_LEN];

    raydium_file_dirname(path,dir);
    raydium_file_basename(ext,dir);
    return raydium_path_ext_priority(path,ext,priority);
    }

strcpy(raydium_path_paths[id].path,dir);
if(dir[strlen(dir)-1]=='/')
    raydium_path_paths[id].path[strlen(dir)-1]=0;

raydium_path_paths[id].mode=RAYDIUM_PATH_MODE_READ;
raydium_path_paths[id].state=1;
raydium_path_paths[id].ext[0]=0;
raydium_path_paths[id].priority=priority; // High priority for user added path.
return 1;
}

signed char raydium_path_write(char *dir)
{
if(!raydium_file_directory_writable(dir))
    {
    raydium_log("path: ERROR: '%s' is not a writable directory !",dir);
    return 0;
    }

strcpy(raydium_path_write_current,dir);

if(dir[strlen(dir)-1]=='/')
    raydium_path_write_current[strlen(dir)-1]=0;

return 1;
}

signed char raydium_path_string_from(char *str)
{
// cut the string on ':'
char part[RAYDIUM_MAX_DIR_LEN];
char *c;
int end;
signed char last=0;

if(!str || strlen(str)==0)
    return 0;

raydium_path_reset();

do
 {
    if((c=strchr(str,':')))
        end=c-str;
    else
        {
        end=strlen(str);
        last=1;
        }

    strncpy(part,str,end);
    part[end]=0;
    raydium_path_add_priority(part,2);
    str+=(end+1);
 } while(!last);

return 1;
}

int raydium_path_string_to(char *out)
{
int i;

out[0]=0;
for(i=0;i<RAYDIUM_MAX_PATHS;i++)
    if( raydium_path_paths[i].state &&
        raydium_path_paths[i].mode==RAYDIUM_PATH_MODE_READ )
            {
            strcat(out,raydium_path_paths[i].path);
            if(strlen(raydium_path_paths[i].ext))
                {
                strcat(out,"/*.");
                strcat(out,raydium_path_paths[i].ext);
                }

            strcat(out,":");
            }

if(strlen(out))
    out[strlen(out)-1]=0; // trim last ':'

return strlen(out);
}

/*
signed char raydium_path_delete(char *path)
{
}
*/

void raydium_path_resolv(char *in, char *out, char mode)
{
int cur_priority;
int found;
strcpy(out,in); // this is the default behavior

// Seems than it's better to allow this "feature", in the risk
// of conflicts (example: ask for "foo/bar.txt" while a such file exists at the
// same time in foo/ (current dir) and data/foo (data/ being a search path)
//if(strchr(in,'/'))
//    return;

if(mode=='r')
    {
    char path[RAYDIUM_MAX_DIR_LEN];
    char ext[RAYDIUM_MAX_NAME_LEN];
    int i;

    // Just thinking ... should'nt we check "." after all other paths ?
    if(raydium_file_readable(in))
        return;

    // ok ... file does not exists, search paths
    raydium_file_ext(ext,in);
    found=0;
    cur_priority=1000; // Hope it is high enough.
    for(i=0;i<RAYDIUM_MAX_PATHS;i++)
        {
        if( raydium_path_paths[i].state &&
            (raydium_path_paths[i].mode==RAYDIUM_PATH_MODE_READ ||
             raydium_path_paths[i].mode==RAYDIUM_PATH_PACKAGE_READONLY ||
             raydium_path_paths[i].mode==RAYDIUM_PATH_PACKAGE_READWRITE ))
            {
            if(raydium_path_paths[i].ext[0] &&
               strcmp(raydium_path_paths[i].ext,ext))
                continue; // not the good extension

            sprintf(path,"%s/%s",raydium_path_paths[i].path,in);
            if(raydium_file_readable(path))
                {
                if (raydium_path_paths[i].priority<cur_priority)
                    {
                    cur_priority=raydium_path_paths[i].priority;
                    strcpy(out,path);
                    found=1;
                    }
                continue;
                }
            if (raydium_path_paths[i].path[0]=='.') // relative directory, look also in home directory
                {
                sprintf(path,"%s/%s/%s",raydium_homedir,raydium_path_paths[i].path,in);
                if(raydium_file_readable(path))
                    {
                    if (raydium_path_paths[i].priority<cur_priority)
                        {
                        cur_priority=raydium_path_paths[i].priority;
                        strcpy(out,path);
                        found=1;
                        }
                    continue;
                    }
                }
            }
        }
    if (found) return;

    // ok, if we're here, the file does not exists at all, let's
    // prepare R3S job:
    mode='w';
    }

if(mode=='w')
    {
    if(raydium_file_directory_writable(".") && raydium_path_write_local_dir_allowed)
        return;

    sprintf(out,"%s/%s",raydium_path_write_current,in);
    }

}

void raydium_path_dump(void)
{
char path[1024];
raydium_path_string_to(path);
raydium_log("path: read: \"%s\"",path);
raydium_log("path: write: \"%s\"",raydium_path_write_current);
}

void raydium_path_reset(void)
{
int i;

for(i=0;i<RAYDIUM_MAX_PATHS;i++)
    raydium_path_paths[i].state=0;
}

void raydium_path_init(void)
{
char path[1024];
char packages[1024];
signed char ok;

raydium_path_reset();
raydium_atexit(raydium_path_dump);
//raydium_atexit(raydium_path_package_update); // Must be defined after php atexit as we need php to be active there.
raydium_path_write_local_dir_allowed=1;

if(raydium_init_cli_option("path",path))
    raydium_path_string_from(path);

ok=0;
if(raydium_init_cli_option("write-path",path))
    ok=raydium_path_write(path);

if(!ok)
    {
    strcpy(path,raydium_file_home_path("data"));
    if(!raydium_file_directory_writable(path))
        {
        mkdir(path,0755);
        if(!raydium_file_directory_writable(path))
            {
            ok=0;
            raydium_log("ERROR: path: cannot create '%s'",path);
            }
        else
            {
            ok=1;
            raydium_log("path: created writable path '%s'",path);
            }
        }
    else ok=1;
    }

strcpy(packages,raydium_file_home_path("packages"));
if(!raydium_file_directory_writable(packages))
	{
	mkdir(packages,0755);
	if(!raydium_file_directory_writable(packages))
		raydium_log("ERROR: path: cannot create '%s'",packages);
	else
		raydium_log("path: created '%s'",packages);
	}


if(ok)
    {
    raydium_path_write(path);
    raydium_path_add_priority(path,7);
    raydium_log("path: OK");
    }
else
    raydium_log("ERROR: path: unable to find a writable path. Try using --write-path");
}

void raydium_path_write_local_deny(signed char deny)
{
raydium_path_write_local_dir_allowed=(deny?0:1);
}

signed char raydium_path_package_cache_clear(void)
{
char path[RAYDIUM_MAX_DIR_LEN];

// let's use the easy and secure way: delete the whole 'packages' directory.
raydium_file_home_path_cpy("packages",path);
if(!raydium_file_rm_rf(path))
	{
	raydium_log("ERROR: path: packages: can't delete '%s'.",path);
	return 0;
	}

// We'll let the next startup create the directory again (easy, I said)
raydium_log("path: packages: cache cleared. Exiting !");
exit(0);
return 1;
}

signed char raydium_path_package_register(char *file)
{
char tmp[RAYDIUM_MAX_NAME_LEN];
char filename[RAYDIUM_MAX_NAME_LEN];
char extract_path[RAYDIUM_MAX_DIR_LEN];
char file_path[RAYDIUM_MAX_DIR_LEN];
struct stat extract_stat;
struct stat file_stat;
signed char extract=0;
FILE *fp;

// should test ".zip" suffix ?

raydium_file_basename(filename,file);
sprintf(tmp,"packages/%s",filename);
strcpy(extract_path,raydium_file_home_path(tmp));

// let's try to fopen the file (R3S call)
fp=raydium_file_fopen(file,"r");
if(!fp)
    {
    raydium_log("Registering Empty package %s.",file);
    raydium_path_add_priority(extract_path,5);
    raydium_path_package_mode(file,RAYDIUM_PATH_PACKAGE_READONLY);
    return 0;
    }
fclose(fp);

// must revolv "file" path when calling raydium_rayphp_zip_extract()
raydium_path_resolv(file,file_path, 'r');

if(stat(file_path,&file_stat)==-1)
	{
	raydium_log("ERROR: path: can't stat / resolv '%s'",file);
	perror("stat");
	return 0;
	}

if(!raydium_file_isdir(extract_path))
	extract=1;
else
	{
	// already exists ... Is the same ?
	if(stat(extract_path,&extract_stat)==-1)
		{
		raydium_log("ERROR: path: can't stat '%s'.",extract_path);
		perror("stat");
		return 0;
		}

	if(extract_stat.st_mtime!=file_stat.st_mtime)
		{
		extract=1;
		// Whatever "svn blame" says, it's not me who erased your hard drive.
		if(!raydium_file_rm_rf(extract_path))
			{
			raydium_log("ERROR: path: can't delete '%s'.",extract_path);
			return 0;
			}
		}
	else
		raydium_log("path: package '%s' already in cache",filename);
	}

if(extract)
	{
	struct utimbuf time;

	time.actime=file_stat.st_mtime;
	time.modtime=file_stat.st_mtime;

	if(!raydium_rayphp_zip_extract(file_path,extract_path))
		return 0;

	if(raydium_file_utime(extract_path,&time))
		{
		raydium_log("ERROR: path: can't change '%s' mtime.",extract_path);
		perror("utime");
		return 0;
		}
	}

raydium_path_add_priority(extract_path,5);
raydium_path_package_mode(filename,RAYDIUM_PATH_PACKAGE_READONLY);
sprintf(tmp,"%s/RAYDIUM_PACKAGE_READWRITE",extract_path);
fp=fopen(tmp,"r");
if (fp)
    {
    fclose(fp);
    raydium_path_package_mode(filename,RAYDIUM_PATH_PACKAGE_READWRITE);
    }
return 1;
}

void raydium_path_package_update(void)
{
int i;
int pindex=-1;
char package_name[RAYDIUM_MAX_NAME_LEN];
char full_file_name[RAYDIUM_MAX_NAME_LEN];

raydium_log("********* Path package update");

for (i=0;i<RAYDIUM_MAX_PATHS;i++)
    {
    if (!raydium_path_paths[i].state)
        continue;
    if (raydium_path_paths[i].mode==RAYDIUM_PATH_PACKAGE_READWRITE)
        {
        char tmp[RAYDIUM_MAX_NAME_LEN];
        raydium_file_basename(package_name,raydium_path_paths[i].path);
        strcpy(tmp,"RAYDIUM_PACKAGE_READWRITE");
        raydium_rayphp_zip_add(package_name,tmp);
        if (pindex==-1)
            pindex=i;
        }
    if (raydium_path_paths[i].mode==RAYDIUM_PATH_PACKAGE_READONLY)
        {
        raydium_file_basename(package_name,raydium_path_paths[i].path);
        raydium_rayphp_zip_add(package_name,"RAYDIUM_PACKAGE_READONLY");
        }
    }

if (pindex!=-1) // No package to update exiting
    {
    raydium_file_basename(package_name,raydium_path_paths[pindex].path);
    raydium_log("Updating %s package file",package_name);
    for(i=0;i<raydium_file_log_fopen_index;i++)
        {
        char base_name[RAYDIUM_MAX_NAME_LEN];
        int base_name_len;

        if ( raydium_file_log_fopen_status[i]==RAYDIUM_FILE_NOT_FOUND)
            continue;
        if (!strcmp(&raydium_file_log_fopen[i][strlen(raydium_file_log_fopen[i])-4],".zip"))
            continue;

        raydium_path_resolv(raydium_file_log_fopen[i],full_file_name,'r');
        //raydium_log("File:%s",full_file_name);
        raydium_file_basename(base_name,full_file_name);
        base_name_len=strlen(base_name);
        base_name_len+=1; // Skip /
        base_name_len=base_name_len+strlen(package_name);
        base_name_len=strlen(full_file_name)-base_name_len;

        if (strncmp(&full_file_name[base_name_len],package_name,strlen(package_name))!=0)
            raydium_rayphp_zip_add(package_name,full_file_name);
        }
    }
raydium_log("********* End of Path package update");
}

unsigned char raydium_path_package_mode(char * name,unsigned char mode)
{
int pindex;

pindex=raydium_path_package_find(name);
if (pindex!=RAYDIUM_PATH_PACKAGE_NOT_FOUND)
    raydium_path_paths[pindex].mode=mode;
}
