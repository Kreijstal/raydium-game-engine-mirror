/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

// This is an experimental test for a "ray car".

#include "raydium/index.c"

GLfloat sun[]={1.0,0.9,0.5,1.0};

#define RAY_LEN		0.1
#define MOTOR_SPEED	10
#define CAR_BASE_HEIGHT -0.2
#define CAR_WHEEL_ANGLE 30 // ... degrees 

#define CAR_MU		0.9
#define CAR_MU2		0.0001
#define CAR_SLIP_LONGI	0.1

#define CAR_SUSP_ERP	0.01	
#define CAR_SUSP_CFM	0.1  // more = less reactive susp

#define TYPE_CAR_BODY	7
#define TYPE_BALANCIER  6

#define BODY_DENS	0.2
#define BALANCIER_DENS	0.8

char ray_callback(int e1, int e2, dContact *n)
{
GLfloat org[]={1,0,0};
GLfloat res[3];
float wheel_rotation;
/*
// ... Raydium defaults ...
contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
dContactSoftERP | dContactSoftCFM | dContactApprox1;
contact[i].surface.mu = dInfinity;

contact[i].surface.slip1 = slip;
contact[i].surface.slip2 = slip;

contact[i].surface.soft_erp = erp;
contact[i].surface.soft_cfm = cfm;
*/

n->surface.mode &= ~dContactSlip1; // remove slip1
n->surface.mode |= dContactFDir1;
n->surface.slip2 = CAR_SLIP_LONGI;
n->surface.mu = CAR_MU;


if(raydium_ode_element[e1]._last_touched_ray<=1) // is a front wheel ?
    {
    wheel_rotation=raydium_joy_x*CAR_WHEEL_ANGLE;
    n->surface.mode |= dContactMu2;    
    n->surface.mu2 = n->surface.mu;
    n->surface.mu = CAR_MU2;
    }
else // rear wheel (where engine works)
    {
    wheel_rotation=0;
    n->surface.mode |= dContactMotion1;
    n->surface.motion1 = raydium_joy_y*MOTOR_SPEED; 
    }

raydium_trigo_rotate(org,0,0,wheel_rotation,res); 
dBodyVectorToWorld(raydium_ode_element[raydium_ode_element_find("corps")].body,
		    res[0],res[1],res[2],n->fdir1); 

n->surface.soft_erp=CAR_SUSP_ERP;
n->surface.soft_cfm=CAR_SUSP_CFM; 
//printf("%f ",n->geom.depth);
//n->geom.depth-=0.1;
n->geom.depth=(RAY_LEN-n->geom.depth);
//printf("-> %f\n",n->geom.depth);
return RAYDIUM_ODE_RAY_CONTACT_CREATE;
}

char collide(int e1, int e2, dContact *c)
{
int i1,i2;
i1=raydium_ode_element_tag_get(e1);
i2=raydium_ode_element_tag_get(e2);

if(i1==TYPE_BALANCIER || i2==TYPE_BALANCIER)
    return 0;

return 1;
}


void create_car(void)
{
int a;

#define BREAK_FORCE     200
#define ROTFRICTION     0.0005
#define ERP_CFM         0.3,0.8

raydium_ode_object_delete_name("WATURE");
                                                                                                                            
a=raydium_ode_object_create("WATURE");
raydium_ode_object_box_add("corps",a,BODY_DENS,1.2,0.6,0.4,RAYDIUM_ODE_STANDARD,TYPE_CAR_BODY,"clio.tri");
raydium_ode_element_slip_name("corps",RAYDIUM_ODE_SLIP_ICE);

raydium_ode_network_next_local_only=1;
raydium_ode_object_box_add("balancier",a,BALANCIER_DENS,0.1,0.1,0.1,RAYDIUM_ODE_STANDARD,TYPE_BALANCIER,""); // crate.tri
raydium_ode_element_move_name_3f("balancier",0,0,-0.5);
raydium_ode_joint_attach_fixed_name("balanfixed","corps","balancier");
		
raydium_ode_element_ray_attach_name("corps",RAY_LEN,0,0,-1);
raydium_ode_element_ray_pos_name_3f("corps",0,0.42,0.253,CAR_BASE_HEIGHT);

raydium_ode_element_ray_attach_name("corps",RAY_LEN,0,0,-1);
raydium_ode_element_ray_pos_name_3f("corps",1,0.42,-0.253,CAR_BASE_HEIGHT);

raydium_ode_element_ray_attach_name("corps",RAY_LEN,0,0,-1);
raydium_ode_element_ray_pos_name_3f("corps",2,-0.444,0.253,CAR_BASE_HEIGHT);

raydium_ode_element_ray_attach_name("corps",RAY_LEN,0,0,-1);
raydium_ode_element_ray_pos_name_3f("corps",3,-0.444,-0.253,CAR_BASE_HEIGHT);

raydium_ode_object_move_name_3f("WATURE",0,0,2);
}


void display(void)
{
dReal *pos; 

raydium_joy_key_emul();
 

if(raydium_key_last==1032) create_car();
 
if(raydium_key_last==1097) raydium_ode_time_change(0);
if(raydium_key_last==1122) raydium_ode_time_change(10);
if(raydium_key_last==1101) raydium_ode_time_change(100);

if(raydium_key_last==1027) exit(0);


raydium_clear_frame();

pos=raydium_ode_element_pos_get_name("corps");

if(raydium_key[GLUT_KEY_F1])
    raydium_camera_look_at(1,1,-0.4,pos[1],-pos[2],pos[0]);
else
    {
    dReal mpos[3];
    dReal *vel;
    dReal cam[3];
    // get element position
    pos=raydium_ode_element_pos_get_name("corps");
    vel=raydium_ode_element_linearvelocity_get_name("corps");

    // get position of camera in world coords
    raydium_ode_element_RelPointPos_name("corps",-1,0,0,cam);

    // correct z pos (always at top of the car, for example)
    cam[2]=pos[2]+0.4;

    mpos[0]=pos[0]+vel[0];
    mpos[1]=pos[1]+vel[1];
    mpos[2]=pos[2]+vel[2];
    
    // standard smooth lookat camera
      raydium_camera_smooth(cam[0],cam[1],cam[2],mpos[1],-mpos[2],mpos[0],
			    70,0,raydium_frame_time*5);
    }


raydium_ode_draw_all(RAYDIUM_ODE_DRAW_NORMAL);
raydium_ode_draw_all(RAYDIUM_ODE_DRAW_DEBUG);
raydium_ode_draw_all(RAYDIUM_ODE_DRAW_RAY);

raydium_rendering_finish();
}


int main(int argc, char **argv)
{
raydium_init_args(argc,argv);
raydium_window_create(800,600,RAYDIUM_RENDERING_WINDOW,"Ray driven car");
raydium_texture_filter_change(RAYDIUM_TEXTURE_FILTER_TRILINEAR);
raydium_projection_near=0.1;
raydium_projection_far=10000;
raydium_projection_fov=70;
raydium_window_view_update();

raydium_light_on(0);
memcpy(raydium_light_color[0],sun,raydium_internal_size_vector_float_4);
raydium_light_intensity[0]=1000000;

raydium_light_position[0][0]=50;
raydium_light_position[0][1]=150;
raydium_light_position[0][2]=200;

raydium_light_update_all(0);
raydium_background_color_change(sun[0],sun[1],sun[2],sun[3]);

raydium_fog_disable();
raydium_shadow_enable();

raydium_ode_ground_set_name("cocorobix.tri");
create_car();
raydium_ode_RayCallback=ray_callback;
raydium_ode_CollideCallback=collide;
raydium_ode_time_change(0);

raydium_callback(&display);

return 0;
}
