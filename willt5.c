#include "raydium/index.c"

GLfloat cam_angle_x = 0;
GLfloat cam_angle_y = 90;

GLfloat cam_pos_x = 0;
GLfloat cam_pos_y = 0;
GLfloat cam_pos_z = 0;

GLfloat speed = 0.1;
GLint sensibilite = 3;

GLint lacet = 0;

GLfloat light_color[] = {1.0, 0.9, 0.8, 1.0};

char draw_debug=1;

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

    if(raydium_key_last==1032)
	raydium_php_exec("test.php");

    if(raydium_key_last==1116) draw_debug*=-1;
    
    delta_x = raydium_mouse_x - (raydium_window_tx/2);
    cam_angle_x += (delta_x*sensibilite*0.1f); 

    delta_y = raydium_mouse_y - (raydium_window_ty/2);
    cam_angle_y += (delta_y*sensibilite*0.1f); 

    raydium_mouse_move(raydium_window_tx/2, raydium_window_ty/2);
    
    raydium_light_position[0][0]=100;
    raydium_light_position[0][1]=100;
    raydium_light_position[0][2]=100;
    raydium_light_position[0][3]=1.0;

    raydium_background_color_change(light_color[0],light_color[1],light_color[2],light_color[3]);

    
    raydium_clear_frame();
    raydium_camera_place(cam_pos_x,cam_pos_y,cam_pos_z,cam_angle_x,cam_angle_y,0);

    raydium_camera_replace();
    
    raydium_ode_draw_all(0);
    if(draw_debug>0)
       raydium_ode_draw_all(1);

    raydium_osd_logo("logo.tga");
    
    raydium_rendering_finish();
    raydium_ode_network_element_send_iterative(5);
}




int main(int argc, char **argv)
{
    char server[128];
    raydium_init_args(argc,argv);
    
    raydium_window_create(640,480,RAYDIUM_RENDERING_WINDOW,"Test 5 network viewer");
    raydium_texture_filter=RAYDIUM_TEXTURE_FILTER_TRILINEAR;
    raydium_projection_near=0.01;
    raydium_projection_far=2500;
    raydium_projection_fov=60;
    raydium_fog_disable();
    raydium_window_view_update();
    
    raydium_light_on(0);
    memcpy(raydium_light_color[0],light_color,raydium_internal_size_vector_float_4);
    raydium_light_intensity[0] = 1000000;
    raydium_light_update_all(0);
    
//    raydium_osd_cursor_set("BOXcursor.tga",4,4);

    raydium_window_view_update();

if(raydium_init_cli_option("server",server))
     if(!raydium_network_client_connect_to(server)) 
        exit(1);

    raydium_ode_ground_set_name("a.tri");
    raydium_ode_object_box_add("boite",0,0.1,RAYDIUM_ODE_AUTODETECT,0,0,RAYDIUM_ODE_STANDARD,0,"crate.tri");
    raydium_ode_element_move_name_3f("boite",10,10,10);

    raydium_callback(&display);
    return(0);
}
// EOF
