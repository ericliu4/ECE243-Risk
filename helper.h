#ifndef HELPER_H
#define HELPER_H

//converts expected RBG values to the 16 bit color format
unsigned short rgbToColour(unsigned char r, unsigned char g, unsigned char b);

//whatever this does idk but its from the most recent lab
void wait_for_vsync(void* vp);

void swap(int *a, int *b);
#endif /*HELPER_H*/