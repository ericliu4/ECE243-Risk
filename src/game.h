#ifndef GAME_H
#define GAME_H

#include "enums.h"
#include "globals.h"
#include "structs.h"

extern cursor_t cursor;

//functions draws the whole image - then draws playerName and numTroops at locationsx,y, from 0 to 41
void drawMap(currPlayer playerNameOnLoc[], int numTroopsOnLoc[], int locX[], int locY[], currPlayer currTurn, currPhase currAction);

void updateCursor();

void drawTitleScreen();

void drawTutorialScreen();

void drawWinScreen();
void drawLoseScreen();

void drawCursor(int x_cursor, int y_cursor, int colour, int size);

void eraseCursor(int x_cursor, int y_cursor, int size, gameStates gameState);

#endif /*GAME_H*/