#ifndef STRUCTS_H
#define STRUCTS_H

#include "globals.h"

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
    short dx;
    short dy;

    short xPos;
    short yPos;

    //char left_pressed_bit;
    //char right_pressed_bit;
}cursor;


#endif /*STRUCTS_H*/