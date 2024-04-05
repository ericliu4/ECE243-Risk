#ifndef GLOBALS_H
#define GLOBALS_H

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240



#define C_BLACK 0x0000 //eventually find a way to convert from rbg to this
#define C_WHITE 0xFFFF

#define C_BLUE 0x001F
#define C_RED 0xF800
#define C_GREEN 0x07E0
#define C_CYAN 0x07FF
#define C_MAGENTA 0xF81F
#define C_YELLOW 0xFFE0


typedef unsigned int uint32;
typedef unsigned short uWord;

#define CHAR_BASE 0x09000000
#define VIDEO_BASE 0xFF203020
#define PS2_addr 0xFF200100



#endif /*GLOBALS_H*/