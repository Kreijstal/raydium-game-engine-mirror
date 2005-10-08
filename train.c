#include "raydium/index.c"

GLfloat cam_angle_x = 0;
GLfloat cam_angle_y = 90;

GLfloat cam_pos_x = 0;
GLfloat cam_pos_y = 0;
GLfloat cam_pos_z = 0;

GLfloat speed = 0.1;
GLint sensibilite = 3;

GLint lacet = 0;
float force=0;

GLfloat light_color[] = {1.0, 0.9, 0.8, 1.0};


void display(void)
{
    
int delta_x, delta_y;
// Touches
raydium_joy_key_emul();

cam_pos_z += (raydium_trigo_sin(cam_angle_x+90)*raydium_joy_y*speed*raydium_trigo_sin(90-cam_angle_y));
cam_pos_x += (raydium_trigo_cos(cam_angle_x+90)*raydium_joy_y*speed*raydium_trigo_sin(90-cam_angle_y));
cam_pos_y += (raydium_trigo_cos(90-cam_angle_y)*speed*raydium_joy_y);

cam_pos_x -= (raydium_trigo_cos(cam_angle_x)*raydium_joy_x*speed);
cam_pos_z -= (raydium_trigo_sin(cam_angle_x)*raydium_joy_x*speed);
    
if(raydium_key_last==1027)
    exit(0);

{
float speed=0;
if(raydium_key[GLUT_KEY_HOME]) speed=force;
if(raydium_key[GLUT_KEY_END]) speed=-force;
raydium_ode_motor_speed_name("train_moteur",speed);
}

if(raydium_key_last==1097) raydium_ode_time_change(0);
if(raydium_key_last==1122) raydium_ode_time_change(10);
if(raydium_key_last==1101) raydium_ode_time_change(100);


if(raydium_key_last==1032)
    {
//    raydium_particle_generator_load("smoke.prt","smoke");
//    raydium_particle_generator_load("clouds.prt","clouds");
//    raydium_particle_generator_load("test.prt","durt_smoke");
//    raydium_particle_generator_load("fountain.prt","fountain");
//    raydium_particle_generator_load("explosion.prt","explo");
//    raydium_particle_generator_load("explosmoke.prt","explo_smoke");
    
//    raydium_particle_generator_move_name_3f("explo",1,0,0);
    raydium_php_exec("train.php");
    }

    
delta_x = raydium_mouse_x - (raydium_window_tx/2);
cam_angle_x += (delta_x*sensibilite*0.1f); 
delta_y = raydium_mouse_y - (raydium_window_ty/2);
cam_angle_y += (delta_y*sensibilite*0.1f); 

raydium_mouse_move(raydium_window_tx/2, raydium_window_ty/2);
    

raydium_light_position[0][0]=100;
raydium_light_position[0][1]=100;
raydium_light_position[0][2]=100;
raydium_light_position[0][3]=1.0;
raydium_light_update_position(0); 

raydium_background_color_change(light_color[0],light_color[1],light_color[2],light_color[3]);

    
raydium_clear_frame();

raydium_camera_place(cam_pos_x,cam_pos_y,cam_pos_z,cam_angle_x,cam_angle_y,0);
//raydium_ode_element_camera_inboard_name("train_train",0,1,0,-3,0,-0.5);
//raydium_ode_element_camera_inboard_name("train_train",0,0.3,-0.3,-3,0,-0.5);
raydium_camera_replace();
raydium_object_draw_name("rail_world_background.tri");
    
raydium_ode_draw_all(0);
if(raydium_key[GLUT_KEY_F1])
    raydium_ode_draw_all(1);

raydium_osd_logo("logo.tga");
    
raydium_rendering_finish();
}




int main(int argc, char **argv)
{
    raydium_init_args(argc,argv);
    
    raydium_window_create(640,480,RAYDIUM_RENDERING_WINDOW,"train's test");
    raydium_texture_filter=RAYDIUM_TEXTURE_FILTER_TRILINEAR;
    raydium_projection_near=0.01;
    raydium_projection_far=2500;
    raydium_projection_fov=60;
    raydium_fog_disable();
    raydium_window_view_update();
    
//    raydium_light_disable();
    raydium_light_on(0);
    memcpy(raydium_light_color[0],light_color,raydium_internal_size_vector_float_4);
    raydium_light_intensity[0] = 1000000;
    raydium_light_update_all(0);
    
//    raydium_osd_cursor_set("BOXcursor.tga",4,4);

    raydium_window_view_update();

    raydium_ode_ground_set_name("rail_world.tri");
    raydium_register_variable(&force,RAYDIUM_REGISTER_FLOAT,"force");

    // preload tri files and textures
    raydium_object_find_load("train1.tri");
    raydium_object_find_load("train_bw.tri");
    raydium_object_find_load("wagon.tri");
    raydium_object_find_load("train_a.tri");
    raydium_php_exec("train.php");
    raydium_sky_atmosphere_enable();
    raydium_callback(&display);
    return(0);
}
// EOF
