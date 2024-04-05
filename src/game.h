#ifndef GAME_H
#define GAME_H

#include "enums.h"


//functions draws the whole image - then draws playerName and numTroops at locationsx,y, from 0 to 41
void drawMap(currPlayer playerNameOnLoc[], int numTroopsOnLoc[], int locX[], int locY[], currPlayer currTurn, currPhase currAction);

void drawTitleScreen();

void drawTutorialScreen();

#endif /*GAME_H*/