#include "helper.h"
#include "structs.h"

unsigned short rbgToColour(int r, int g, int b){
    return (r << 11) | (g << 5) | b;
}

void wait_for_vsync(){ //I don't fully understand how this works but whatever
    volatile int* pixel_ctrl_ptr = (int*) 0xff203020;
    *pixel_ctrl_ptr = 1;
    while ((*(pixel_ctrl_ptr+3) & 1) != 0);
}

void swap(int *a, int *b){
    int c = *b;
    *b = *a;
    *a = c;

}

void swapBuffers(){
    struct fb_t* temp = ((struct videoStruct*)VIDEO_BASE)->fbp;
    ((struct videoStruct*)VIDEO_BASE)->fbp = ((struct videoStruct*)VIDEO_BASE)->bfbp;
    ((struct videoStruct*)VIDEO_BASE)->bfbp = temp;
}