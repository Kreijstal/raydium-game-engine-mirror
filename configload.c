/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#include "raydium/index.c"

void display(void)
{
    if(raydium_key_last==1027)
        exit(0);           
    raydium_camera_freemove();
    raydium_clear_frame();
    /* [ draw here ] */ raydium_ode_draw_all(0);
    raydium_rendering_finish();
}

int main(int argc, char **argv)
{
    raydium_init_args(argc,argv);
    raydium_init_load("exampleconfig.txt");
    /* [ place base scene here ] */ raydium_ode_ground_set_name("cocorobix.tri");
    raydium_callback(&display);
    
    return(0);
}

// EOF
