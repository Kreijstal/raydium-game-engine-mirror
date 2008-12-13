/*
    Raydium - CQFD Corp.
    http://raydium.org/
    Released under both BSD license and Lesser GPL library license.
    See "license.txt" file.
*/

// Small JPGS movie player.

#include "raydium/index.c"

void display(void)
{
raydium_joy_key_emul();

if(raydium_key_last==1027)
    exit(0);

if(raydium_key_last==100)
    raydium_video_seek_rel_name("video",-10);
if(raydium_key_last==102)
    raydium_video_seek_rel_name("video",10);
if(raydium_key_last==103)
    raydium_video_seek_rel_name("video",-60);
if(raydium_key_last==101)
    raydium_video_seek_rel_name("video",60);


raydium_clear_frame();
raydium_live_texture_mask_name("video",1);
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

raydium_callback(&display);
return(0);
}

// EOF
