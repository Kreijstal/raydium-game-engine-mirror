/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

// Small "in-game" model viewer, with mouse look. Use --model option.

#define NO_ODE_SUPPORT
#include "raydium/index.c"

GLfloat cam_angle_x = 0;
GLfloat cam_angle_y = 90;

GLfloat cam_pos_x = 0;
GLfloat cam_pos_y = 0;
GLfloat cam_pos_z = 0;

GLfloat speed = 0.1;
GLint sensibilite = 3;

GLint lacet = 0;

char model[RAYDIUM_MAX_NAME_LEN];

GLfloat light_color[] = {1.0, 0.9, 0.8, 1.0};
//    GLfloat back_color[] = {1, 1, 1, 1};
GLfloat  *back_color=light_color;
//    GLfloat light_color[] = {0, 0, 0, 1};


void display(void)
{
    
    int delta_x, delta_y;
    raydium_joy_key_emul();

    cam_pos_z += (raydium_trigo_sin(cam_angle_x+90)*raydium_joy_y*speed*raydium_trigo_sin(90-cam_angle_y));
    cam_pos_x += (raydium_trigo_cos(cam_angle_x+90)*raydium_joy_y*speed*raydium_trigo_sin(90-cam_angle_y));
    cam_pos_y += (raydium_trigo_cos(90-cam_angle_y)*speed*raydium_joy_y);
    
    cam_pos_x -= (raydium_trigo_cos(cam_angle_x)*raydium_joy_x*speed);
    cam_pos_z -= (raydium_trigo_sin(cam_angle_x)*raydium_joy_x*speed);
    
    if(raydium_key_last==1027)
	exit(0);


    if(raydium_key[GLUT_KEY_F1]) { raydium_projection_fov/=(1.04); raydium_window_view_update(); }
    if(raydium_key[GLUT_KEY_F2]) { raydium_projection_fov*=(1.04); raydium_window_view_update(); }

    if(raydium_key_last==3)
	raydium_light_disable();
    if(raydium_key_last==4)
	raydium_light_enable();

    
    delta_x = raydium_mouse_x - (raydium_window_tx/2);
    cam_angle_x += (delta_x*sensibilite*0.1f); 

    delta_y = raydium_mouse_y - (raydium_window_ty/2);
    cam_angle_y += (delta_y*sensibilite*0.1f); 

    raydium_mouse_move(raydium_window_tx/2, raydium_window_ty/2);
    


    raydium_light_position[0][0]=50;
    raydium_light_position[0][1]=150;
    raydium_light_position[0][2]=200;
    raydium_light_position[0][3]=1.0;
    
    raydium_clear_frame();
    
    raydium_camera_place(cam_pos_x,cam_pos_y,cam_pos_z,cam_angle_x,cam_angle_y,0);

    raydium_camera_replace();
    
    
    raydium_object_draw_name(model);

//    raydium_osd_logo("logo.tga");
    
    raydium_rendering_finish();
}




int main(int argc, char **argv)
{
    raydium_init_args(argc,argv);
    
    // creation de la fenetre et de la scene
    raydium_window_create(640,480,RAYDIUM_RENDERING_WINDOW,"Willou's test");
    raydium_texture_filter=RAYDIUM_TEXTURE_FILTER_TRILINEAR;
    raydium_projection_near=0.01;
    raydium_projection_far=2500;
    raydium_projection_fov=60;
    raydium_fog_disable();
    raydium_window_view_update();
    
    raydium_light_enable();
    raydium_light_on(0);
    memcpy(raydium_light_color[0],light_color,raydium_internal_size_vector_float_4);
    raydium_light_intensity[0] = 10000000;
    raydium_light_update_all(0);
    
//    raydium_osd_cursor_set("BOXcursor.tga",4,4);

    raydium_window_view_update();
    raydium_background_color_change(back_color[0],back_color[1],back_color[2],back_color[3]);

    raydium_register_variable(&speed, RAYDIUM_REGISTER_FLOAT, "speed");

    strcpy(model,"a.tri");
    raydium_init_cli_option("model",model);
    
    raydium_callback(&display);
    return(0);
}
// EOF
