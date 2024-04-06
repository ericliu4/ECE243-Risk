#ifndef KEYBOARD_H
#define KEYBOARD_H

#define HEX3_HEX0_BASE 0xFF200020  
#define HEX5_HEX4_BASE 0xFF200030 
#define PS2_BASE 0xFF200100
	
#include "structs.h"

extern cursor_t cursor;

/* function prototypes */

//to be called at start, sends reset signal to keyboard
void keyboardSetup();

//for debugging, displays the three bytes from ps/2 to the hex displays
void HEX_PS2(char, char, char);

//to be called by main fn - polls ps/ for bytes 
//TODO Does this need to be called 3 times?
void pollKeyboard();

#endif // KEYBOARD_H