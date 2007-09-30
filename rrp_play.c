/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

// RRP records player. See raydium_ode_capture_record() for creating a replay.

#include "raydium/index.c"


void display(void)
{
raydium_joy_key_emul();

if(raydium_key_last==1027)
    exit(0);

raydium_clear_frame();
raydium_camera_freemove(RAYDIUM_CAMERA_FREEMOVE_NORMAL);
raydium_ode_draw_all(0);
raydium_rendering_finish();
}


int main(int argc, char **argv)
{
char file[RAYDIUM_MAX_NAME_LEN];

raydium_init_args(argc,argv);
raydium_window_create(640,480,RAYDIUM_RENDERING_WINDOW,"RRP player");

raydium_texture_filter_change(RAYDIUM_TEXTURE_FILTER_TRILINEAR);
raydium_window_view_perspective(60,0.01,2500); // fov 60 + near and far planes

raydium_fog_disable();    
raydium_light_enable();
raydium_light_on(0);

raydium_light_conf_7f(0,50,150,200,1000000,1,0.9,0.7); // id, pos, intensity and color (RGB)
raydium_background_color_change(1,0.9,0.7,1);

raydium_sky_box_cache();
raydium_shadow_enable();


if(!raydium_init_cli_option("file",file))
    {
    printf("******* Usage: rrp_play --file file.rrp\n");
    exit(0);
    }

raydium_ode_capture_play(file,1);

raydium_callback(&display);
return(0);
}

// EOF
