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
}cursor_t;


#endif /*STRUCTS_H*/