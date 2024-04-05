#include "globals.h"
#include "structs.h"
#include "draw.h"
#include "helper.h"
#include "enums.h"
#include "../resources/riskMap.h"
#include "../resources/titleScreen.h"
#include "../resources/tutorialScreen.h"

#include <stdbool.h>
#include <stdlib.h>

void clearScreen(){
    struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->bfbp;
    for (int y = 0; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
            buffer->pixels[y][x] = C_BLACK;
}

void fillScreen(uWord c){
    struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->bfbp;
    for (int y = 0; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
            buffer->pixels[y][x] = c;
}


void drawHLine(uWord c, int y){
    struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->bfbp;
    for (int x = 0; x < SCREEN_WIDTH; x++){
        buffer->pixels[y][x] = c;
    }
}
void drawVLine(uWord c, int x){
    struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->bfbp;
    for (int y = 0; y < SCREEN_HEIGHT; y++){
        buffer->pixels[y][x] = c;
    }
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
	struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->bfbp;
	for(int y = y_i; y < height; ++y) {
		for(int x = x_i; x < width; ++x) {
			buffer->pixels[y][x] = c;	//could modify to use drawPixel but I don't think it makes a difference
		}
	}
}


static inline void drawPixel(int x, int y, uWord C){
    //struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->fbp;
    //fbp->pixels[y][x] = C;
    ((struct videoStruct*)VIDEO_BASE)->bfbp->pixels[y][x] = C;
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




void drawSprite(int x_i, int y_i, int* sprite){
    //Assuming sprite is 8x8
    for(size_t y = 0; y < 8; ++y){
        for(size_t x = 0; x < 8; ++x){
            drawPixel(x, y, *sprite);
            ++sprite;
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


void drawCursor(int x_cursor, int y_cursor, int colour, int size){
  //draw_block(x_cursor, y_cursor, colour, size);
  for(int x = x_cursor - size; x < x_cursor + size; ++x){
    drawPixel(x, y_cursor, C_WHITE);
  }
  for(int y = y_cursor - size; y < y_cursor + size; ++y){
    drawPixel(x_cursor, y, C_WHITE);
  }
}

void eraseCursor(int x_cursor, int y_cursor, int size, gameStates gameState){

    switch(gameState){
        case STARTSCREEN:
            for(int x = x_cursor - size; x = x_cursor + size; ++x){
                drawPixel(x, y_cursor, titleScreen[y_cursor][x]);
            }
            return;
        case 
    }

    for(int x = x_cursor - size; x = x_cursor + size; ++x){
        drawPixel()
    }
}
