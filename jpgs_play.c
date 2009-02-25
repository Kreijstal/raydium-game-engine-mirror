/*
    Raydium - CQFD Corp.
    http://raydium.org/
    Released under both BSD license and Lesser GPL library license.
    See "license.txt" file.
*/

// Small JPGS movie player.

#include "raydium/index.c"


#define ALPHA_SECONDS   3
float alpha=0;

void alpha_set(void)
{
alpha=ALPHA_SECONDS;
}

void hms(float t,int *h, int *m, int *s, int *ms)
{
*h=*m=0;
*ms=(t-(int)t)*1000;
*s=t;
while(*s>=60)
    {
    (*m)++;
    (*s)-=60;
    }

while(*m>=60)
    {
    (*h)++;
    (*m)-=60;
    }
}


void display(void)
{
int h1,m1,s1,ms1;
int h2,m2,s2,ms2;

raydium_joy_key_emul();

if(raydium_key_last==1027)
    exit(0);

if(raydium_key_last==100)
    {
    raydium_video_seek_rel_name("video",-10);
    alpha_set();
    }
if(raydium_key_last==102)
    {
    raydium_video_seek_rel_name("video",10);
    alpha_set();
    }
if(raydium_key_last==103)
    {
    raydium_video_seek_rel_name("video",-60);
    alpha_set();
    }
if(raydium_key_last==101)
    {
    raydium_video_seek_rel_name("video",60);
    alpha_set();
    }
if(raydium_key_last==1032)
    alpha_set();


raydium_clear_frame();
raydium_live_texture_mask_name("video",1);

if(alpha>0)
    {
    raydium_osd_color_rgba(1,1,1,alpha);
    hms(raydium_video_elapsed_name("video"),&h1,&m1,&s1,&ms1);
    hms(raydium_video_duration_name("video"),&h2,&m2,&s2,&ms2);
    raydium_osd_printf(50,95,32,0.5,"font2.tga","%i:%02i:%02i / %i:%02i:%02i",h1,m1,s1,h2,m2,s2);
    }

alpha-=raydium_frame_time;
if(alpha<0)
    alpha=0;



raydium_rendering_finish();
}


int main(int argc, char **argv)
{
char video[RAYDIUM_MAX_NAME_LEN];
char sound[RAYDIUM_MAX_NAME_LEN];

raydium_init_args(argc,argv);

if(!raydium_init_cli_option("file",video))
    {
    printf("******* Usage: jpgs_play --file file.jpgs [--sound file.ogg]\n");
    exit(0);
    }

sound[0]=0;
raydium_init_cli_option("sound",sound);

raydium_window_create(640,480,RAYDIUM_RENDERING_WINDOW,"JPGS movie player");
raydium_texture_filter_change(RAYDIUM_TEXTURE_FILTER_TRILINEAR);

if(raydium_video_open_with_sound(video,"video",sound) < 0)
    {
    printf("******* Unable to open '%s'\n",video);
    exit(0);
    }

raydium_render_fps_limit(120);
raydium_callback(&display);
return(0);
}

// EOF
