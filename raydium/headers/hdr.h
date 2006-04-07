#ifndef _HDR_H
#define _HDR_H

/*=
Pseudo - HDR
4300
**/
void raydium_hdr_init(void);
void raydium_hdr_enable(void);
void raydium_hdr_disable(void);
void raydium_hdr_internal_window_malloc(void);
void raydium_hdr_block(signed char blocking);
void raydium_hdr_blur(unsigned char *in, unsigned char *out);
void raydium_hdr_map(void);
void raydium_hdr_map_apply(void);
void raydium_hdr_settings_color_local(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void raydium_hdr_settings_color_ambient(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void raydium_hdr_settings_eye(float speed, float alpha_max);
void raydium_hdr_settings(GLfloat *color_local, GLfloat *color_ambient, float eye_speed, float alpha_max);

signed char raydium_hdr_texture(int texture, signed char hdr);
signed char raydium_hdr_texture_name(char *texture, signed char hdr);
void raydium_hdr_texture_reset(void); // display lists !!





#endif
