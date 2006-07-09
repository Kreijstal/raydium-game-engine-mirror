/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#include "raydium/index.c"

GLfloat cam_angle_x = 0;
GLfloat cam_angle_y = 90;

GLfloat cam_pos_x = 0;
GLfloat cam_pos_y = 0;
GLfloat cam_pos_z = 0;

GLfloat speed = 0.1;
GLint sensibilite = 3;

GLint lacet = 0;

dReal *lpos;

void display(void)
{
int delta_x, delta_y;
raydium_joy_key_emul();

lpos=raydium_ode_element_pos_get_name("light");
raydium_light_move_3f(0,lpos[0],lpos[1],lpos[2]);

cam_pos_z += (raydium_trigo_sin(cam_angle_x+90)*raydium_joy_y*speed*raydium_trigo_sin(90-cam_angle_y));
cam_pos_x += (raydium_trigo_cos(cam_angle_x+90)*raydium_joy_y*speed*raydium_trigo_sin(90-cam_angle_y));
cam_pos_y += (raydium_trigo_cos(90-cam_angle_y)*speed*raydium_joy_y);
    
cam_pos_x -= (raydium_trigo_cos(cam_angle_x)*raydium_joy_x*speed);
cam_pos_z -= (raydium_trigo_sin(cam_angle_x)*raydium_joy_x*speed);
    
if(raydium_key_last==1027)
    exit(0);

if(raydium_key[GLUT_KEY_F1]) { raydium_projection_fov/=(1.04); raydium_window_view_update(); }
if(raydium_key[GLUT_KEY_F2]) { raydium_projection_fov*=(1.04); raydium_window_view_update(); }

delta_x = raydium_mouse_x - (raydium_window_tx/2);
cam_angle_x += (delta_x*sensibilite*0.1f); 

delta_y = raydium_mouse_y - (raydium_window_ty/2);
cam_angle_y += (delta_y*sensibilite*0.1f); 

raydium_mouse_move(raydium_window_tx/2, raydium_window_ty/2);

raydium_clear_frame();
raydium_camera_place(cam_pos_x,cam_pos_y,cam_pos_z,cam_angle_x,cam_angle_y,0);
raydium_ode_draw_all(0);
raydium_rendering_finish();
}


int main(int argc, char **argv)
{
int obj;

raydium_init_args(argc,argv);
raydium_window_create(640,480,RAYDIUM_RENDERING_WINDOW,"Small shader test");

raydium_texture_filter_change(RAYDIUM_TEXTURE_FILTER_TRILINEAR);
raydium_window_view_perspective(60,0.01,2500); // fov 60 + near and far planes

raydium_fog_disable();    
raydium_light_enable();
raydium_light_on(0);

raydium_light_conf_7f(0,0,0,0,1000000,1,0.9,0.7); // id, pos, intensity and color (RGB)
raydium_background_color_change(1,0.9,0.7,1);

raydium_sky_box_cache();

raydium_register_variable(&speed, RAYDIUM_REGISTER_FLOAT, "speed");

raydium_ode_ground_set_name("crate_bump.tri");
obj=raydium_ode_object_find("GLOBAL");
raydium_ode_object_box_add("coch",obj,1,RAYDIUM_ODE_AUTODETECT,RAYDIUM_ODE_AUTODETECT,RAYDIUM_ODE_AUTODETECT,RAYDIUM_ODE_STANDARD,0,"coch2.tri");
raydium_ode_object_sphere_add("light",obj,0.01,RAYDIUM_ODE_AUTODETECT,RAYDIUM_ODE_STANDARD,0,"ballon.tri");
raydium_ode_element_move_name_3f("light",0,0,0.25);
raydium_ode_joint_attach_universal("chand",RAYDIUM_ODE_JOINT_FIXED,raydium_ode_element_find("light"),0,0,0.5, 1,0,0, 0,1,0);
raydium_ode_element_addforce_name_3f("light",10,1,0);

raydium_shader_load("shader1","toon.vert","toon.frag");
raydium_shader_var_f_name("shader1","Edge",0.64);
raydium_shader_var_f_name("shader1","Phong",0.954);
raydium_shader_var_3f_name("shader1","DiffuseColor",0.35,0,0.25);
raydium_shader_var_3f_name("shader1","PhongColor",0.94,0.7,0.85);

raydium_shader_load("shader2","bump.vert","bump.frag");
raydium_shader_var_i_name("shader2","colorMap",0);
raydium_shader_var_i_name("shader2","normalMap",1);
raydium_shader_var_4f_name("shader2","specular",0.3,0.3,0.3,1);
raydium_shader_var_f_name("shader2","shininess",40);

raydium_shader_attach_texture_name("shader1","rgb(0.94,0.7,0.85)");
raydium_shader_attach_texture_name("shader2","crate_cmap.tga");

raydium_shadow_enable();

raydium_callback(&display);
return 0;
}
// EOF