/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

// Small JPGS movie player.

#include "raydium/index.c"

void display(void)
{
raydium_joy_key_emul();

if(raydium_key_last==1027)
    exit(0);

raydium_clear_frame();
raydium_live_texture_mask_name("video",1);
raydium_rendering_finish();
}


int main(int argc, char **argv)
{
char video[RAYDIUM_MAX_NAME_LEN];

raydium_init_args(argc,argv);

if(!raydium_init_cli_option("file",video))
    {
    printf("******* Usage: jpgs_play --file file.jpgs\n");
    exit(0);
    }

raydium_window_create(640,480,RAYDIUM_RENDERING_WINDOW,"JPGS movie player");
raydium_texture_filter_change(RAYDIUM_TEXTURE_FILTER_TRILINEAR);

if(raydium_video_open(video,"video") < 0)
    {
    printf("******* Unable to open '%s'\n",video);
    exit(0);
    }

raydium_callback(&display);
return(0);
}

// EOF
