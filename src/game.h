#ifndef GAME_H
#define GAME_H

#include "enums.h"
#include "globals.h"


//functions draws the whole image - then draws playerName and numTroops at locationsx,y, from 0 to 41
void drawMap(currPlayer playerNameOnLoc[], int numTroopsOnLoc[], int locX[], int locY[], currPlayer currTurn, currPhase currAction);

void drawTitleScreen();

void drawTutorialScreen();

void drawCursor(int x_cursor, int y_cursor, int colour, int size);

void eraseCursor(int x_cursor, int y_cursor, int size, gameStates gameState);

#endif /*GAME_H*/