#include "game.h"
#include "draw.h"
#include "enums.h"
#include "globals.h"
#include "helper.h"
#include "keyboard.h"
#include "structs.h"


#include "../resources/riskMap.h"
#include "../resources/titleScreen.h"
#include "../resources/tutorialScreen.h"
#include "../resources/win.h"
#include "../resources/lose.h"

extern struct fb_t* buffer;

int prevCursorLoc1[2] = {0};
int prevCursorLoc2[2] = {0};

void drawMap(currPlayer playerNameOnLoc[], int numTroopsOnLoc[], int locX[], int locY[], currPlayer currTurn, currPhase currAction){

    //TODO - This should put all of this on the back buffer, then flip the buffers and "wait for vsync"



    //draw current player and stuff
    drawString(16, 228, "Current Player: ");
    switch(currTurn){
        case PLAYER1:
        //TODO: Maybe draw a rectangle behind this of the color?
            drawString(80, 228, "Player 1 (Red)        ");
            break;
        case PLAYER2:
            drawString(80, 228, "Player 2 (Blue)       ");
            break;
        case PLAYER3:
            drawString(80, 228, "Player 3 (Green)      ");
            break;
        case PLAYER4:
            drawString(80, 228, "Player 4 (Magenta)    ");
            break;
        default:
            drawString(80, 228, "ERROR (improper player)");
            break;
    }

    drawString(16, 235, "Current Action: ");
    switch(currAction){
        case PLACETROOPS:
            drawString(80, 235, "Place Troops              ");
            break;
        case ATTACKPHASE:
            drawString(80, 235, "Attack!                   ");
            break;
        case MOVETROOPPHASE:
            drawString(80, 235, "Move Troops               ");
            break;
        default:
            drawString(80, 235, "ERROR (Invalid turn phase)");
            break;
    }
    unsigned short col;
    for(int i = 0; i < 42; ++i){
        //get color to draw
        switch(playerNameOnLoc[i]) {
            case PLAYER1:
                col = C_RED;
                break;
            case PLAYER2:
                col = C_BLUE;
                break;
            case PLAYER3:
                col = C_GREEN;
                break;
            case PLAYER4:
                col = C_MAGENTA;
                break;
            default:
                col = C_BLACK; //for debugging
        }

        //draw rectangle at location
        int x_i = locX[i] /4 *4;
        int y_i = locY[i] /4 *4;
        //drawRect(x_i -2, y_i -2, x_i +6, y_i +6, col);


        //draw number of troops at location
        if(numTroopsOnLoc[i] < 0)
            numTroopsOnLoc[i] = 0; //error checking in case eric's code breaks... just in case :)
        if(numTroopsOnLoc[i] >9){
            unsigned char left = numTroopsOnLoc[i] %10 +48;
            unsigned char right = numTroopsOnLoc[i] /10 +48;
            drawChar(locX[i], locY[i], right);
            drawChar(locX[i] +4, locY[i], left);
            drawRectInBackdrop(*riskMap, x_i -2, y_i -2, x_i +10, y_i +6, col);
        }     
        else{
            drawChar(locX[i], locY[i], numTroopsOnLoc[i]+48);
            drawChar(locX[i] +4, locY[i], 0);
            drawRectInBackdrop(*riskMap, x_i -2, y_i -2, x_i +6, y_i +6, col);
        }
    }
        //draw main map
    drawScreen(*riskMap, 222);
    clearFifo();
    pollKeyboard();
    drawCursor(cursor.xPos, cursor.yPos, C_WHITE, 2);


    swapBuffers();
    wait_for_vsync();

}

void updateCursor(){
    //erase cursor from 2 buffers ago (or don't idc)
    eraseCursor(prevCursorLoc2[0], prevCursorLoc2[1], 2, INGAME);
    prevCursorLoc2[0] = prevCursorLoc1[0];
    prevCursorLoc2[1] = prevCursorLoc1[1];
    prevCursorLoc1[0] = cursor.xPos;
    prevCursorLoc1[1] = cursor.yPos;
    pollKeyboard();
    //draw new cursor
    drawCursor(cursor.xPos, cursor.yPos, C_BLACK, 2);
    swapBuffers();
    wait_for_vsync();
}

void drawTitleScreen(){
    drawScreen(*titleScreen, 240);

    swapBuffers();
    wait_for_vsync();
    //drawScreen(*titleScreen, 240);
}
void drawTutorialScreen(){
    drawScreen(*tutorialScreen, 201);

    swapBuffers();
    wait_for_vsync();
    //drawScreen(*tutorialScreen, 201);
}

void drawWinScreen(){
    drawScreen(*win, 320);

    swapBuffers();
    wait_for_vsync();
    //drawScreen(*tutorialScreen, 201);
}

void drawLoseScreen(){
    drawScreen(*lose, 320);

    swapBuffers();
    wait_for_vsync();
    //drawScreen(*tutorialScreen, 201);
}


static inline void drawPixel(int x, int y, uWord C){
    //struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->fbp;
    //fbp->pixels[y][x] = C;
    //((struct videoStruct*)VIDEO_BASE)->bfbp->pixels[y][x] = C;
    buffer->pixels[y][x] = C;
}


void drawCursor(int x_cursor, int y_cursor, int colour, int size){
  //draw_block(x_cursor, y_cursor, colour, size);
  for(int x = x_cursor - size; x <= x_cursor + size; ++x){
    drawPixel(x, y_cursor, C_WHITE);
  }
  for(int y = y_cursor - size; y <= y_cursor + size; ++y){
    drawPixel(x_cursor, y, C_WHITE);
  }
}

void eraseCursor(int x_cursor, int y_cursor, int size, gameStates gameState){

    switch(gameState){
        case STARTSCREEN:
            for(int x = x_cursor - size; x <= x_cursor + size; ++x){
                drawPixel(x, y_cursor, titleScreen[y_cursor][x]);
            }
            for(int y = y_cursor - size; y <= y_cursor + size; ++y){
                drawPixel(x_cursor, y, titleScreen[y][x_cursor]);
            }
            return;
        case TUTORIAL: 
            for(int x = x_cursor - size; x <= x_cursor + size; ++x){
                drawPixel(x, y_cursor, tutorialScreen[y_cursor][x]);
            }
            for(int y = y_cursor - size; y <= y_cursor + size; ++y){
                drawPixel(x_cursor, y, tutorialScreen[y][x_cursor]);
            }
        default:
            for(int x = x_cursor - size; x <= x_cursor + size; ++x){
                drawPixel(x, y_cursor, riskMap[y_cursor][x]);
            }
            for(int y = y_cursor - size; y <= y_cursor + size; ++y){
                drawPixel(x_cursor, y, riskMap[y][x_cursor]);
            }
    }

}