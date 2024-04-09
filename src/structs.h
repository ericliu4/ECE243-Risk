#ifndef STRUCTS_H
#define STRUCTS_H

#include "globals.h"
#include <stdbool.h>

//#define uint32 unsigned int
//#define uWord unsigned short

struct fb_t {
    unsigned short volatile pixels[256][512]; //don't ask me why its like this, it just is...
};

struct videoStruct {
    struct fb_t *volatile fbp; 
    struct fb_t *volatile bfbp; 
    unsigned int volatile resolution; 
    unsigned int volatile StatusControl;
};

typedef struct {
    short xPos;
    short yPos;

    bool clicked;
    bool q; //goes to 1 when q is clicked
}cursor_t;


struct audio_t {
    volatile unsigned int control;
    volatile unsigned char rarc;
    volatile unsigned char ralc;
    volatile unsigned char wsrc;
    volatile unsigned char wslc;
    volatile unsigned int ldata;
    volatile unsigned int rdata;
};

struct timer_t{
    volatile unsigned int status;
    volatile unsigned int control;
    volatile unsigned int periodLo;
    volatile unsigned int periodHi;
    volatile unsigned int snapLo;
    volatile unsigned int snapHi;
};



#endif /*STRUCTS_H*/