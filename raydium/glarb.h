/*
    Raydium - CQFD Corp.
    http://raydium.cqfd-corp.org
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

/* OpenGL ARB ext (only needed) for WIN32 and OpenGL <= 1.1 */

// We need a few #ifdef here, since newer versions of DevCPP
// seems to provides GL > 1.1

#define GL_ACTIVE_TEXTURE_ARB               0x84E0
#define GL_MAX_TEXTURE_UNITS_ARB	    0x84E2
#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1
#define GL_TEXTURE2_ARB                     0x84C2
#define GL_TEXTURE3_ARB                     0x84C3
#define GL_TEXTURE4_ARB                     0x84C4
#define GL_TEXTURE5_ARB                     0x84C5
#define GL_TEXTURE6_ARB                     0x84C6
#define GL_TEXTURE7_ARB                     0x84C7
#define GL_TEXTURE8_ARB                     0x84C8
#define GL_COMBINE_EXT			    0x8570
#define GL_RGB_SCALE_EXT                    0x8573
#define GL_LIGHT_MODEL_COLOR_CONTROL        0x81F8
#define GL_SEPARATE_SPECULAR_COLOR          0x81FA
#define GL_CLAMP_TO_EDGE		    0x812F

// You can comment thoses two lines if any problem
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum texture, GLfloat s, GLfloat t);

PFNGLACTIVETEXTUREARBPROC       glActiveTextureARB     = NULL;
PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2fARB   = NULL;

void raydium_arb_win32_init(void)
{
glActiveTextureARB=(PFNGLACTIVETEXTUREARBPROC) 		wglGetProcAddress( "glActiveTextureARB" );
glMultiTexCoord2fARB=(PFNGLMULTITEXCOORD2FARBPROC)   	wglGetProcAddress( "glMultiTexCoord2fARB" );
}
