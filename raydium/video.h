#ifndef RAY_VIDEO_H
#define RAY_VIDEO_H

#include <jpeglib.h>


typedef struct raydium_video_Video
    {
    signed char state;
    FILE *fp;
    int sizex;
    int sizey;
    int fps;
    int frames_total;
    int live_id;
    float elapsed;
    unsigned char *data;
    long start;
    long *offsets;
    int last_decoded;
    } raydium_video_Video;

raydium_video_Video raydium_video_video[RAYDIUM_MAX_VIDEOS];

#endif
