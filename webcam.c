#include "raydium/index.c"

#define FILE "/dev/video0"

#include <linux/types.h>
#include <linux/videodev.h>

#define READ_VIDEO_PIXEL(buf, format, depth, r, g, b)			\
{									\
	switch (format)							\
	{								\
		case VIDEO_PALETTE_GREY:				\
			switch (depth)					\
			{						\
				case 4:					\
				case 6:					\
				case 8:					\
					(r) = (g) = (b) = (*buf++ << 8);\
					break;				\
									\
				case 16:				\
					(r) = (g) = (b) = 		\
						*((unsigned short *) buf);	\
					buf += 2;			\
					break;				\
			}						\
			break;						\
									\
									\
		case VIDEO_PALETTE_RGB565:				\
		{							\
			unsigned short tmp = *(unsigned short *)buf;	\
			(r) = tmp&0xF800;				\
			(g) = (tmp<<5)&0xFC00;				\
			(b) = (tmp<<11)&0xF800;				\
			buf += 2;					\
		}							\
		break;							\
									\
		case VIDEO_PALETTE_RGB555:				\
			(r) = (buf[0]&0xF8)<<8;				\
			(g) = ((buf[0] << 5 | buf[1] >> 3)&0xF8)<<8;	\
			(b) = ((buf[1] << 2 ) & 0xF8)<<8;		\
			buf += 2;					\
			break;						\
									\
		case VIDEO_PALETTE_RGB24:				\
			(r) = buf[0] << 8; (g) = buf[1] << 8; 		\
			(b) = buf[2] << 8;				\
			buf += 3;					\
			break;						\
		case VIDEO_PALETTE_YUV420P:				\
			(r) = buf[0]+1.140*buf[2];		 	\
			(g) = buf[0]-0.394*buf[1]-0.581*buf[2]; 	\
			(b) = buf[0]+2.028*buf[1];			\
			buf += 3;					\
			break;						\
									\
		default:						\
			fprintf(stderr, 				\
				"Format %d not yet supported\n",	\
				format);				\
	}								\
}						

  int fd, f;
  struct video_capability cap;
  struct video_window win;
  struct video_picture vpic;

  unsigned char *buffer, *buffer2, *src;
  int bpp = 24, r, g, b;
  unsigned int i, src_depth;


/*
int get_brightness_adj(unsigned char *image, long size, int *brightness) {
  long i, tot = 0;
  for (i=0;i<size*3;i++)
    tot += image[i];
  *brightness = (128 - tot/(size*3))/3;
  return !((tot/(size*3)) >= 126 && (tot/(size*3)) <= 130);
}
*/


// YUV420P to RGB code from:
// Peopletracking with an omnicamera
// Daniel Hammarin & Mihajlo Miladinovic

/* LIMIT: convert a 16.16 fixed-point value to a byte, with clipping. */
#define LIMIT(x) ((x)>0xffffff?0xff: ((x)<=0xffff?0:((x)>>16)))
static inline void v4l_copy_420_block (int yTL, int yTR, int yBL, int yBR, 
int u, int v, int rowPixels, unsigned char * rgb, int bits)
{
const int rvScale = 91881;
const int guScale = -22553;
const int gvScale = -46801;
const int buScale = 116129;
const int yScale = 65536;
int r, g, b;
g = guScale * u + gvScale * v;
r = rvScale * v;
b = buScale * u;
yTL *= yScale; yTR *= yScale;
yBL *= yScale; yBR *= yScale;
if (bits == 24)
{
/* Write out top two pixels */
rgb[0] = LIMIT(b+yTL); rgb[1] = LIMIT(g+yTL); rgb[2] = LIMIT(r+yTL);
rgb[3] = LIMIT(b+yTR); rgb[4] = LIMIT(g+yTR); rgb[5] = LIMIT(r+yTR);
/* Skip down to next line to write out bottom two pixels */
rgb += 3 * rowPixels;
rgb[0] = LIMIT(b+yBL); rgb[1] = LIMIT(g+yBL); rgb[2] = LIMIT(r+yBL);
rgb[3] = LIMIT(b+yBR); rgb[4] = LIMIT(g+yBR); rgb[5] = LIMIT(r+yBR);
}
else if (bits == 16)
{
/* Write out top two pixels */
rgb[0] = ((LIMIT(b+yTL) >> 3) & 0x1F) | ((LIMIT(g+yTL) << 3) & 0xE0);
rgb[1] = ((LIMIT(g+yTL) >> 5) & 0x07) | (LIMIT(r+yTL) & 0xF8);
rgb[2] = ((LIMIT(b+yTR) >> 3) & 0x1F) | ((LIMIT(g+yTR) << 3) & 0xE0);
rgb[3] = ((LIMIT(g+yTR) >> 5) & 0x07) | (LIMIT(r+yTR) & 0xF8);
/* Skip down to next line to write out bottom two pixels */
rgb += 2 * rowPixels;
rgb[0] = ((LIMIT(b+yBL) >> 3) & 0x1F) | ((LIMIT(g+yBL) << 3) & 0xE0);
rgb[1] = ((LIMIT(g+yBL) >> 5) & 0x07) | (LIMIT(r+yBL) & 0xF8);
rgb[2] = ((LIMIT(b+yBR) >> 3) & 0x1F) | ((LIMIT(g+yBR) << 3) & 0xE0);
rgb[3] = ((LIMIT(g+yBR) >> 5) & 0x07) | (LIMIT(r+yBR) & 0xF8);
}
}

int v4l_yuv420p2rgb (unsigned char *rgb_out, unsigned char *yuv_in, int width, int
height, int bits)
{
const int numpix = width * height;
const unsigned int bytes = bits >> 3;
int h, w, y00, y01, y10, y11, u, v;
unsigned char *pY = yuv_in;
unsigned char *pU = pY + numpix;
unsigned char *pV = pU + numpix / 4;
unsigned char *pOut = rgb_out;
if (!rgb_out || !yuv_in)
return -1;
for (h = 0; h <= height - 2; h += 2)
{
for (w = 0; w <= width - 2; w += 2)
{
y00 = *(pY);
y01 = *(pY + 1);
y10 = *(pY + width);
y11 = *(pY + width + 1);
u = (*pU++) - 128;
v = (*pV++) - 128;
v4l_copy_420_block (y00, y01, y10, y11, u, v, width, pOut, bits);
pY += 2;
pOut += bytes << 1;
}
pY += width;
pOut += width * bytes;
}
return 0;
}



void read_v4l(void)
{

  read(fd, buffer, win.width * win.height * bpp);

// YUV420P style
  v4l_yuv420p2rgb (buffer2,buffer,win.width,win.height,bpp);

// RGB style
/*
int i,j;
  src = buffer;
  for (i = j = 0; i < win.width * win.height; i++) 
  {
    READ_VIDEO_PIXEL(src, vpic.palette, src_depth, r, g, b);
    buffer2[j++]=r>>8;
    buffer2[j++]=g>>8;
    buffer2[j++]=b>>8;
  }
*/
//  for (i = j = 0; i < win.width * win.height; i++) 
//  {
//  printf("%i,%i,%i ",buffer2[i*3],buffer2[i*3+1],buffer2[i*3+2]);
//  }
//printf("%i",raydium_texture_find_by_name("betoncontainers.tga"));
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D,raydium_texture_find_by_name("webcam.tga"));
//glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGBA8,GL_UNSIGNED_BYTE,buffer2);
glTexSubImage2D(GL_TEXTURE_2D,0,0,0,win.width,win.height,GL_RGB,GL_UNSIGNED_BYTE,buffer2);
              
              

}




GLfloat sun[]={1.0,0.9,0.5,1.0};

GLfloat camx=3.01;
GLfloat camy=3;
GLfloat camz=0;


void display(void)
{
static float rx,ry,rz;
raydium_joy_key_emul();

camx+=raydium_joy_x;
camy+=raydium_joy_y;
 
// camx+=0.01;

if(raydium_key[GLUT_KEY_PAGE_DOWN]) camz--;
if(raydium_key[GLUT_KEY_PAGE_UP]  ) camz++;

if(raydium_key_last==1027) exit(0);

rx+=5*raydium_frame_time;
ry+=5*raydium_frame_time*0.1;
rz+=5*raydium_frame_time*0.01;

read_v4l();

raydium_clear_frame();
raydium_camera_look_at(camx,camy,camz,0,0,0);
raydium_object_draw_name("cocorobix.tri");

raydium_camera_replace();
glRotatef(rz,0,0,1);
glRotatef(ry,0,1,0);
glRotatef(rx,1,0,0);
raydium_object_draw_name("webcam.tri");


raydium_rendering_finish();
}

int main(int argc, char **argv)
{


  fd = open(FILE, O_RDONLY);

  if (fd < 0) {
    perror(FILE);
    exit(1);
  }

  if (ioctl(fd, VIDIOCGCAP, &cap) < 0) {
    perror("VIDIOGCAP");
    fprintf(stderr, "(" FILE " not a video4linux device?)\n");
    close(fd);
    exit(1);
  }

  if (ioctl(fd, VIDIOCGWIN, &win) < 0) {
    perror("VIDIOCGWIN");
    close(fd);
    exit(1);
  }

  if (ioctl(fd, VIDIOCGPICT, &vpic) < 0) {
    perror("VIDIOCGPICT");
    close(fd);
    exit(1);
  }

  printf("%s (%ix%i -> %ix%i)\n",cap.name,cap.minwidth,cap.minheight,cap.maxwidth,cap.maxheight);
  printf("default : %ix%i\n",win.width,win.height);

  if (cap.type & VID_TYPE_MONOCHROME) {
    printf("mono\n");
    vpic.depth=8;
    vpic.palette=VIDEO_PALETTE_GREY;	/* 8bit grey */
    if(ioctl(fd, VIDIOCSPICT, &vpic) < 0) {
      vpic.depth=6;
      if(ioctl(fd, VIDIOCSPICT, &vpic) < 0) {
	vpic.depth=4;
	if(ioctl(fd, VIDIOCSPICT, &vpic) < 0) {
	  fprintf(stderr, "Unable to find a supported capture format.\n");
	  close(fd);
	  exit(1);
	}
      }
    }
  } else {
    printf("rgb\n");
    vpic.depth=24;
    vpic.palette=VIDEO_PALETTE_RGB24;
    
    if(ioctl(fd, VIDIOCSPICT, &vpic) < 0) {
      vpic.palette=VIDEO_PALETTE_RGB565;
      vpic.depth=16;
      
      if(ioctl(fd, VIDIOCSPICT, &vpic)==-1) {
	vpic.palette=VIDEO_PALETTE_RGB555;
	vpic.depth=15;
	
	if(ioctl(fd, VIDIOCSPICT, &vpic)==-1) {
	    vpic.palette=VIDEO_PALETTE_YUV420P;
	    vpic.depth=24;

	    if(ioctl(fd, VIDIOCSPICT, &vpic)==-1) {


		fprintf(stderr, "Unable to find a supported capture format.\n");
		return -1;
	    }
	}
      }
    }
  }
  
  buffer  = malloc(win.width * win.height * bpp);
  buffer2 = malloc(512 * 512 * 3); // RGB only
  if (!buffer) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
  }
  
/*
  do {
    int newbright;
    read(fd, buffer, win.width * win.height * bpp);
    f = get_brightness_adj(buffer, win.width * win.height, &newbright);
    printf("%i\n",f);
    if (f) {
      vpic.brightness += (newbright << 8);
      if(ioctl(fd, VIDIOCSPICT, &vpic)==-1) {
	perror("VIDIOSPICT");
	break;
      }
    }
  } while (f && 0);
*/

  fprintf(stdout, "P6\n%d %d 255\n", win.width, win.height);

  src = buffer;



raydium_init_args(argc,argv);
raydium_window_create(512,384,RAYDIUM_RENDERING_WINDOW,"Particles test");
//raydium_texture_filter_change(RAYDIUM_TEXTURE_FILTER_TRILINEAR);
raydium_projection_near=0.01;
raydium_projection_far=1000;
raydium_projection_fov=60;
raydium_window_view_update();
raydium_light_on(0);
memcpy(raydium_light_color[0],sun,raydium_internal_size_vector_float_4);
raydium_light_intensity[0]=100000;

raydium_light_position[0][0]=50;
raydium_light_position[0][1]=150;
raydium_light_position[0][2]=200;

raydium_light_update_all(0);
raydium_background_color_change(sun[0],sun[1],sun[2],sun[3]);
raydium_fog_disable();

raydium_callback(&display);

return 0;
}
