#ifndef HELPER_H
#define HELPER_H

//converts expected RBG values to the 16 bit color format
unsigned short rgbToColour(unsigned char r, unsigned char g, unsigned char b);

//waits for main window to be finished drawing (might even be able to omit this)
void wait_for_vsync();

void swap(int *a, int *b);

//Function to swap front and back buffers (to be called just before 'wait for vsync')
void swapBuffers();

#endif /*HELPER_H*/