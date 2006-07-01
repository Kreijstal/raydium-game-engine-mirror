#ifndef _SHADER__H
#define _SHADER__H
#include "../shader.h"

__rayapi void raydium_shader_init(void);
__rayapi signed char raydium_shader_isvalid(int shader);
__rayapi int raydium_shader_find(char *name);
__rayapi void raydium_shader_infolog(GLhandleARB shader);
__rayapi int raydium_shader_load(char *name, char *file_vert, char *file_frag);

__rayapi int raydium_shader_variable(int shader, char *name);
// non "_name" functions can work only on the current shader !
__rayapi signed char raydium_shader_var_i(int var_id, int value);
__rayapi signed char raydium_shader_var_i_name(char *shader, char *variable, int value);
__rayapi signed char raydium_shader_var_f(int var_id, float value);
__rayapi signed char raydium_shader_var_f_name(char *shader, char *variable, float value);
__rayapi signed char raydium_shader_var_2f(int var_id, float value1, float value2);
__rayapi signed char raydium_shader_var_2f_name(char *shader, char *variable, float value1, float value2);
__rayapi signed char raydium_shader_var_3f(int var_id, float value1, float value2, float value3);
__rayapi signed char raydium_shader_var_3f_name(char *shader, char *variable, float value1, float value2, float value3);
__rayapi signed char raydium_shader_var_4f(int var_id, float value1, float value2, float value3, float value4);
__rayapi signed char raydium_shader_var_4f_name(char *shader, char *variable, float value1, float value2, float value3, float value4);

__rayapi signed char raydium_shader_current(int shader);
__rayapi signed char raydium_shader_current_name(char *shader);

__rayapi signed char raydium_shader_attach_texture(int shader, int texture);
__rayapi signed char raydium_shader_attach_texture_name(char *shader, char *texture);

#endif
