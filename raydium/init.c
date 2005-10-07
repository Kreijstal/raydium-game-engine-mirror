/*
    Raydium - CQFD Corp.
    http://raydium.cqfd-corp.org
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/init.h"

#endif 

// proto
void raydium_ode_init(void);
void raydium_register_api(void);
#ifndef WIN32
void raydium_live_init(void);
#endif
void raydium_video_init(void);
void raydium_internal_live_close(void);

int raydium_init_cli_option(char *option, char *value)
{
int i;
int found;
char full_option[RAYDIUM_MAX_NAME_LEN];
char head[3];

strcpy(full_option  ,"--");
strcpy(full_option+2,option);

found=0;

for(i=1;i<raydium_init_argc;i++)
    if(!strcasecmp(full_option,raydium_init_argv[i]))
	found=i;

if(!found) return 0; 	// option not found

if((found+1) >= raydium_init_argc)
    {
    if(value) strcpy(value,"");
    return 1;	// found, no value (last option of cli)
    }

strncpy(head,raydium_init_argv[found+1],3);
head[2]=0;

if(!strcasecmp(head,"--"))
    {
    if(value) strcpy(value,"");
    return 1;	// found, no value (value starts with -- : is a option)
    }

if(value) strcpy(value,raydium_init_argv[found+1]);
return 1;
}


int raydium_init_cli_option_default(char *option, char *value, char *default_value)
{
if(!raydium_init_cli_option(option,value))
    strcpy(value,default_value);
return 1;
}

void raydium_init_lights(void)
{
GLuint i;

for(i=0;i<RAYDIUM_MAX_LIGHTS;i++)
raydium_light_reset(i);
raydium_log("lights: OK");
}

void raydium_init_objects(void)
{
GLuint i;

raydium_object_anim_time_factor=1.f;

for(i=0;i<RAYDIUM_MAX_OBJECTS;i++)
raydium_object_reset(i);
raydium_log("objects: OK");
}


void raydium_init_key(void)
{
if(raydium_window_mode==RAYDIUM_RENDERING_NONE)
    return;
glutIgnoreKeyRepeat(1);
memset(raydium_key,0,RAYDIUM_KEYBOARD_SIZE);
raydium_key_last=0;
raydium_log("keyboard: OK");
}

// NEVER tested as it should be ! (used once only for now)
void raydium_init_reset(void)
{
GLuint i;

/*
for(i=1;i<raydium_texture_index;i++) // free all texture buffers
free(raydium_texture_ptr[i]);
*/

raydium_init_lights();
raydium_init_objects();
raydium_network_init();
raydium_timecall_init();
raydium_particle_init();
raydium_camera_path_init_all();
raydium_osd_fade_init();
raydium_console_init();
raydium_gui_init();
#ifndef WIN32
raydium_live_init();
#endif
raydium_video_init();

// Must find a way to delete textures from video card's memory, too...

for(i=0;i<RAYDIUM_MAX_TEXTURES;i++) // reset all textures
{
 raydium_texture_name[i][0]=0;
 raydium_texture_blended[i]=0;
 raydium_texture_islightmap[i]=0;
 raydium_texture_rgb[0][i]=-1.f;
 raydium_texture_rgb[1][i]=-1.f;
 raydium_texture_rgb[2][i]=-1.f;
 raydium_texture_rgb[3][i]=1.f;
}
raydium_vertex_index=0;
raydium_vertex_offset_triangle=0;

strcpy(raydium_texture_name[0],"dummy.null");

raydium_texture_index=1; // no more texture loaded (0 is not a texture)
raydium_texture_current_main=0; // sets an "invalid" current texture
raydium_texture_current_multi=0; // sets an "invalid" current texture
raydium_texture_current_multi_u=0;
raydium_texture_current_multi_v=0;
raydium_texture_to_replace=0; // No texture to erase.. just load it :)
raydium_texture_used_memory=0;
//raydium_texture_filter=RAYDIUM_TEXTURE_FILTER_NONE;
raydium_texture_filter_change(RAYDIUM_TEXTURE_FILTER_NONE);
raydium_rendering_rgb_normal();
raydium_rendering_displaylists_enable();
raydium_render_rgb_force_tag=0;
raydium_render_lightmap_color_value[0]=1;
raydium_render_lightmap_color_value[1]=1;
raydium_render_lightmap_color_value[2]=1;
raydium_render_lightmap_color_value[3]=1;
raydium_vertex_counter=0;
raydium_projection=RAYDIUM_PROJECTION_PERSPECTIVE;
raydium_projection_fov=60;
raydium_projection_near=1;
raydium_projection_far=1000;
raydium_projection_left=0;
raydium_projection_right=0;
raydium_projection_bottom=0;
raydium_projection_top=0;
raydium_camera_pushed=0;
raydium_camera_look_at_roll=0;
raydium_camera_path_reset_flag=1;
raydium_camera_rumble_amplitude=0;
raydium_camera_rumble_evolution=0;
raydium_camera_rumble_remaining=0;
raydium_window_view_update();
raydium_internal_vertex_next_extras=0;
raydium_sky_force=0;
raydium_osd_logo_angle=0;
raydium_osd_cursor_texture=0;
raydium_register_variable_index=0;
raydium_register_function_index=0;
raydium_file_log_fopen_index=0;
raydium_frame_time=0;

for(i=0;i<4;i++)
raydium_osd_color[i]=1.f;

raydium_background_color_change(1,1,1,1);
raydium_fog_mode();
raydium_fog_enable();

glEnable(GL_TEXTURE_2D);
glEnable(GL_DEPTH_TEST);
glEnable(GL_COLOR_MATERIAL);
glShadeModel(GL_SMOOTH);
glDepthFunc(GL_LESS);
glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


raydium_light_enable();
glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);  // mmmmm...
glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
raydium_log("Raydium engine reseted to original state");
}



void raydium_init_engine(void)
{
#ifdef PHP_SUPPORT
char autoexec[RAYDIUM_MAX_NAME_LEN];
#endif

raydium_signal_install_trap();
#ifdef WIN32
// init Win32 OpenGL ARB ext.
raydium_arb_win32_init();
#endif
raydium_internal_size_vector_float_4=sizeof(GLfloat)*4;
raydium_log("Platform \"4xfloat\" vector size is: %i byte(s) long",raydium_internal_size_vector_float_4);
glGetIntegerv(GL_MAX_TEXTURE_SIZE, &raydium_texture_size_max);
raydium_log("OpenGL implementation maximum texture size: %ix%i",raydium_texture_size_max,raydium_texture_size_max); 
glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &raydium_texture_units);
raydium_log("OpenGL hardware providing %i texture unit(s)",raydium_texture_units); 
raydium_vertex_x=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_y=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_z=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_normal_x=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_normal_y=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_normal_z=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_normal_visu_x=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_normal_visu_y=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_normal_visu_z=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_texture_u=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_texture_v=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_texture=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLuint));
raydium_vertex_texture_multi=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLuint));
raydium_vertex_texture_multi_u=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_texture_multi_v=malloc(RAYDIUM_MAX_VERTICES*sizeof(GLfloat));
raydium_vertex_tag=malloc(RAYDIUM_MAX_VERTICES);
// must test more than just the last "big" malloc result..
if(!raydium_vertex_texture) { raydium_log("Out of memory..."); exit(29); }
raydium_log("vertex arrays memory: OK");
raydium_random_randomize();
raydium_init_key();
raydium_mouse_init();
raydium_joy_init(); // and not init_joy, since defined in joy.c, not init.c
raydium_sound=0;
raydium_sound_init();
raydium_callback_set();
#ifdef PHP_SUPPORT
raydium_php_init();
#endif

atexit(raydium_sound_close);
atexit(raydium_joy_close);
atexit(raydium_network_close);
atexit(raydium_internal_dump);
atexit(raydium_console_history_save);
#ifndef WIN32
atexit(raydium_internal_live_close);
#endif
raydium_log("atexit functions: OK");
raydium_init_reset();
#ifdef ODE_SUPPORT
raydium_ode_init();
#endif
raydium_register_api();
raydium_log("Engine is now ready.\n\t -----------------------------------------------------------");

#ifdef PHP_SUPPORT
if(raydium_init_cli_option("autoexec",autoexec))
    raydium_php_exec(autoexec);
#endif
}


void raydium_init_args(int argc, char **argv)
{
int i;
char logfile[RAYDIUM_MAX_NAME_LEN];


raydium_init_argc=argc;
raydium_init_argv=malloc(argc*sizeof(char *));

for(i=0;i<argc;i++)
    {
      raydium_init_argv[i]=malloc(strlen(argv[i])+1);
      strcpy(raydium_init_argv[i],argv[i]);
    }
raydium_log("Using Raydium %s",raydium_version);

if(raydium_init_cli_option("logfile",logfile))
    {
    raydium_log_file=fopen(logfile,"wt");
    if(!raydium_log_file) raydium_log("init: Warning: cannot open logfile (%s) with rw mode",logfile);
    }
else raydium_log_file=NULL;

raydium_log("command line args: OK");
}

