#include "helper.h"

unsigned short rbgToColour(int r, int g, int b){
    return (r << 11) | (g << 5) | b;
}

void wait_for_vsync(void* vp){ //I don't fully understand how this works but whatever
    *(int *)vp = 1;
    while ((*((int *)vp +3) & 1) != 0);
}

void swap(int *a, int *b){
    int c = *b;
    *b = *a;
    *a = c;

}