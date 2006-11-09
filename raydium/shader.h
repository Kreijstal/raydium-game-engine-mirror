/*
    Raydium - CQFD Corp.
    http://raydium.org/
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef SHADER_H
#define SHADER_H

#define RAYDIUM_SHADER_TANGENT_ATTRIB_NAME "tangent"

typedef struct raydium_shader_Shader
{
    int id;
    char  name[RAYDIUM_MAX_NAME_LEN];
    signed char state;
    GLhandleARB vert;
    GLhandleARB frag;
    GLhandleARB prog;
    GLint attrib_tangent; // -1 means "no tangent info asked in this shader"
} raydium_shader_Shader;

__global raydium_shader_Shader raydium_shader_shaders[RAYDIUM_MAX_SHADERS];
__global signed char raydium_shader_support;
__global int raydium_shader_active;
#endif
