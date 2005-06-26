/*
    Raydium - CQFD Corp.
    http://raydium.cqfd-corp.org
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

#ifndef DONT_INCLUDE_HEADERS
#include "index.h"
#else
#include "headers/capture.h"
#endif 

// Present capture code was inspired from various web files...
// Not a pure CQFD Corp. production :)

void raydium_capture_frame(char *filename)
{
unsigned char cGarbage = 0, type,mode,aux,bpp, *imageData;
short int iGarbage = 0;
GLuint i;
GLuint size;
FILE *file;

mode=3; // RGB (Bpp)
bpp=mode*8;
type=2; // Color


size=raydium_window_tx * raydium_window_ty * mode;
imageData=malloc(size+1);

glReadPixels(0, 0,raydium_window_tx,raydium_window_ty,GL_RGB,GL_UNSIGNED_BYTE,imageData);


// open file and check for errors
file = fopen(filename, "wb");
if (file == NULL) { raydium_log("Error: capture: cannot open %s for writing",filename); return; }

// write the header
fwrite(&cGarbage, sizeof(unsigned char), 1, file);
fwrite(&cGarbage, sizeof(unsigned char), 1, file);
fwrite(&type, sizeof(unsigned char), 1, file);
fwrite(&iGarbage, sizeof(short int), 1, file);
fwrite(&iGarbage, sizeof(short int), 1, file);
fwrite(&cGarbage, sizeof(unsigned char), 1, file);
fwrite(&iGarbage, sizeof(short int), 1, file);
fwrite(&iGarbage, sizeof(short int), 1, file);
fwrite(&raydium_window_tx, sizeof(short int), 1, file);
fwrite(&raydium_window_ty, sizeof(short int), 1, file);
fwrite(&bpp, sizeof(unsigned char), 1, file);
fwrite(&cGarbage, sizeof(unsigned char), 1, file);

// convert the image data from RGB(a) to BGR(A)
    for (i=0; i < size ; i+= mode) 
    {
    aux = imageData[i];
    imageData[i] = imageData[i+2];
    imageData[i+2] = aux;
    }

    // save the image data
    fwrite(imageData, sizeof(unsigned char), size, file);
    fclose(file);
    // release the memory
    free(imageData);
    raydium_log("screenshot saved as %s",filename);
}

void raydium_capture_frame_auto(void)
{
static int cpt=0;
char f[RAYDIUM_MAX_NAME_LEN];
time_t rawtime;
struct tm *ptm;
    
time(&rawtime);
ptm=gmtime(&rawtime); // localtime() ?
sprintf(f,"raycap%i-%02i-%02i-%02i%02i%02i-%02i.tga",
	ptm->tm_year+1900,
	ptm->tm_mon+1,
	ptm->tm_mday,
	ptm->tm_hour,
	ptm->tm_min,
	ptm->tm_sec,
	cpt);
raydium_capture_frame(f);
cpt++;
}
