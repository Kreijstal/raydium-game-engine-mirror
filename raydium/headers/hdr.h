#ifndef _HDR_H
#define _HDR_H

/*=
Pseudo - HDR
4300
**/
void raydium_hdr_init(void);
void raydium_hdr_enable(void);
void raydium_hdr_disable(void);
void raydium_hdr_block(signed char blocking);
void raydium_hdr_blur(unsigned char *in, unsigned char *out);
void raydium_hdr_map(void);
void raydium_hdr_map_apply(void);

signed char raydium_hdr_texture(int texture, signed char hdr);
signed char raydium_hdr_texture_name(char *texture, signed char hdr);
void raydium_hdr_texture_reset(void); // display lists !!





#endif
