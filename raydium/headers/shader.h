#ifndef _SHADER__H
#define _SHADER__H
#include "../shader.h"

void raydium_shader_init(void);
signed char raydium_shader_isvalid(int shader);
int raydium_shader_find(char *name);
void raydium_shader_infolog(GLhandleARB shader);
int raydium_shader_load(char *name, char *file_vert, char *file_frag);

int raydium_shader_variable(int shader, char *name);
// non "_name" functions can work only on the current shader !
signed char raydium_shader_var_i(int var_id, int value);
signed char raydium_shader_var_i_name(char *shader, char *variable, int value);
signed char raydium_shader_var_f(int var_id, float value);
signed char raydium_shader_var_f_name(char *shader, char *variable, float value);
signed char raydium_shader_var_2f(int var_id, float value1, float value2);
signed char raydium_shader_var_2f_name(char *shader, char *variable, float value1, float value2);
signed char raydium_shader_var_3f(int var_id, float value1, float value2, float value3);
signed char raydium_shader_var_3f_name(char *shader, char *variable, float value1, float value2, float value3);
signed char raydium_shader_var_4f(int var_id, float value1, float value2, float value3, float value4);
signed char raydium_shader_var_4f_name(char *shader, char *variable, float value1, float value2, float value3, float value4);

signed char raydium_shader_current(int shader);
signed char raydium_shader_current_name(char *shader);

signed char raydium_shader_attach_texture(int shader, int texture);
signed char raydium_shader_attach_texture_name(char *shader, char *texture);

#endif
