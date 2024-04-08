#include "globals.h"
#include "structs.h"
#include "draw.h"
#include "helper.h"
#include "enums.h"


#include <stdbool.h>
#include <stdlib.h>

struct fb_t bb;
struct fb_t fb;

//set buffer variable to back buffer
struct fb_t* buffer = &bb;
struct fb_t* display = &fb;



void clearScreen(){
    //struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->bfbp;
    for (int y = 0; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
            buffer->pixels[y][x] = C_BLACK;
}

void fillScreen(uWord c){
    //struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->bfbp;
    for (int y = 0; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
            buffer->pixels[y][x] = c;
}


void drawLine(int x0, int y0, int x1, int y1, int color){
    bool isSteep = abs(y1-y0) > abs(x1-x0);
    if (isSteep){
        swap(&x0, &y0);
        swap(&x1, &y1);
    }
    if (x0 > x1){
        swap(&x0, &x1);
        swap(&y0, &y1);
    }
    int deltaX = x1 - x0;
    int deltaY = abs(y1 - y0);
    int error = -(deltaX / 2);
    int y = y0;
    int y_step;
    if (y0 < y1) {
        y_step = 1 ;
    } else {y_step = -1;}

    for (int x = x0; x < x1; x++){
        if (isSteep){
            drawPixel(y, x, color);
        } else {
            drawPixel(x, y, color);
        }
        error = error + deltaY;
        if (error > 0){
            y = y + y_step;
            error -= deltaX;
        }
    }
}

void drawRect(int x_i, int y_i, int width, int height, uWord c){
	//struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->bfbp;
	for(int y = y_i; y < height; ++y) {
		for(int x = x_i; x < width; ++x) {
			buffer->pixels[y][x] = c;	//could modify to use drawPixel but I don't think it makes a difference
		}
	}
}

void drawHollowRect(int x_i, int y_i, int width, int height, uWord c){
    //struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->bfbp;
    for(int y = y_i; y <= height; ++y) {
        buffer->pixels[y][x_i] = c;
        buffer->pixels[y][width] = c;
    }
    for(int x = x_i; x <= width; ++x) {
        buffer->pixels[y_i][x] = c;
        buffer->pixels[height][x] = c;
    }
}

void drawRectInBackdrop(unsigned short img[], int x_i, int y_i, int width, int height, uWord c){
	//struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->bfbp;
	for(int y = y_i; y < height; ++y) {
		for(int x = x_i; x < width; ++x) {
			img[y * SCREEN_WIDTH + x] = c;	//could modify to use drawPixel but I don't think it makes a difference
		}
	}
}

void drawHollowRectInBackdrop(unsigned short img[], int x_i, int y_i, int width, int height, uWord c){
    //struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->bfbp;
    for(int y = y_i; y < height; ++y) {
        buffer->pixels[y][x_i] = c;
        buffer->pixels[y][width -1] = c;
    }
    for(int x = x_i; x < width; ++x) {
        buffer->pixels[y_i][x] = c;
        buffer->pixels[height -1][x] = c;
    }
}


static inline void drawPixel(int x, int y, uWord C){
    //struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->fbp;
    //fbp->pixels[y][x] = C;
    //((struct videoStruct*)VIDEO_BASE)->bfbp->pixels[y][x] = C;
    buffer->pixels[y][x] = C;
}

void drawChar(int x, int y, char c){
    //fixes bc chars are 4 pixels wide
    x = x /4;
    y = y /4;
    volatile char* cBuffer = (char *)CHAR_BASE;
	int offset = (y << 7) +x;
	*(cBuffer + offset) = c;
}

void drawString(int x, int y, char* str){
    while(*str){
        drawChar(x, y, *str);
        x += 4;
        //wraparound checking just in case
        if(x > 319){
            x = 0;
            y+= 4;
        } 
        if(y > 239){
            y = 0;
        }
        str++;
    }
}

void clearCharBuffer(){
    for(int y = 0; y < SCREEN_HEIGHT; ++y){
        for(int x = 0; x < SCREEN_WIDTH; ++x){
            drawChar(x,y,0);
        }
    }
}


void drawScreen(unsigned short img[], int height){
    for(size_t y = 0; y < height; ++y){
        for(size_t x = 0; x < SCREEN_WIDTH; ++x){
            drawPixel(x,y,img[y * SCREEN_WIDTH + x]);
            //++*img;
        }
    }
    for(int y = height; y < SCREEN_HEIGHT; ++y){
        drawHLine(C_BLACK, y);
    }

}


void wait_for_vsync(){ //I don't fully understand how this works but whatever
    volatile int* pixel_ctrl_ptr = (int*) 0xff203020;
    *pixel_ctrl_ptr = 1;
    while ((*(pixel_ctrl_ptr+3) & 1) != 0);
}


void swapBuffers(){
    volatile int* pixel_ctrl_ptr = (int*) 0xff203020;
    //swap values of display and buffer
    struct fb_t* temp = display;
    display = buffer;
    buffer = temp;
    *(pixel_ctrl_ptr +1) = (int)(display->pixels);


    // struct fb_t* temp = ((struct videoStruct*)VIDEO_BASE)->fbp;
    // ((struct videoStruct*)VIDEO_BASE)->fbp = ((struct videoStruct*)VIDEO_BASE)->bfbp;
    // ((struct videoStruct*)VIDEO_BASE)->bfbp = temp;
}

