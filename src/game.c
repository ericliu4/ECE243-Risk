#include "game.h"
#include "draw.h"
#include "enums.h"
#include "globals.h"
#include "helper.h"

#include "../resources/riskMap.h"
#include "../resources/titleScreen.h"
#include "../resources/tutorialScreen.h"

void drawMap(currPlayer playerNameOnLoc[], int numTroopsOnLoc[], int locX[], int locY[], currPlayer currTurn, currPhase currAction){

    //TODO - This should put all of this on the back buffer, then flip the buffers and "wait for vsync"

    //draw main map
    drawScreen(*riskMap, 222);

    //draw current player and stuff
    drawString(16, 228, "Current Player: ");
    switch(currTurn){
        case PLAYER1:
        //TODO: Maybe draw a rectangle behind this of the color?
            drawString(80, 228, "Player 1 (Red)");
            break;
        case PLAYER2:
            drawString(80, 228, "Player 2 (Blue)");
            break;
        case PLAYER3:
            drawString(80, 228, "Player 3 (Green)");
            break;
        case PLAYER4:
            drawString(80, 228, "Player 4 (Magenta)");
            break;
        default:
            drawString(80, 228, "ERROR (improper player)");
            break;
    }

    drawString(16, 235, "Current Action: ");
    switch(currAction){
        case PLACETROOPS:
            drawString(80, 235, "Place Troops");
        case ATTACKPHASE:
            drawString(80, 235, "Attack!");
        case MOVETROOPPHASE:
            drawString(80, 235, "Move Troops");
        default:
            drawString(80, 235, "ERROR (Invalid turn phase)");
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
        drawRect(x_i -2, y_i -2, x_i +6, y_i +6, col);

        //draw number of troops at location
        if(numTroopsOnLoc[i] < 0)
            numTroopsOnLoc[i] = 0; //error checking in case eric's code breaks... just in case :)
        if(numTroopsOnLoc[i] >9){
            char left = numTroopsOnLoc[i] %10 +48;
            char right = numTroopsOnLoc[i] /10 +48;
            drawChar(locX[i]-2, locY[i]-2, left);
            drawChar(locX[i]+2, locY[i]+2, right);
        }     
        else
            drawChar(locX[i], locY[i], numTroopsOnLoc[i]+48);
    }

    swapBuffers();
    wait_for_vsync();
}

void drawTitleScreen(){
    drawScreen(*titleScreen, 240);

    swapBuffers();
    wait_for_vsync();
}
void drawTutorialScreen(){
    drawScreen(*tutorialScreen, 201);

    swapBuffers();
    wait_for_vsync();
}


static inline void drawPixel(int x, int y, uWord C){
    //struct fb_t* buffer = ((struct videoStruct*)VIDEO_BASE)->fbp;
    //fbp->pixels[y][x] = C;
    ((struct videoStruct*)VIDEO_BASE)->bfbp->pixels[y][x] = C;
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
            for(int y = y_cursor - size; y = y_cursor + size; ++y){
                drawPixel(x_cursor, y, titleScreen[y][x_cursor]);
            }
            return;
        case TUTORIAL: 
            for(int x = x_cursor - size; x = x_cursor + size; ++x){
                drawPixel(x, y_cursor, tutorialScreen[y_cursor][x]);
            }
            for(int y = y_cursor - size; y = y_cursor + size; ++y){
                drawPixel(x_cursor, y, tutorialScreen[y][x_cursor]);
            }
        default:
            for(int x = x_cursor - size; x = x_cursor + size; ++x){
                drawPixel(x, y_cursor, riskMap[y_cursor][x]);
            }
            for(int y = y_cursor - size; y = y_cursor + size; ++y){
                drawPixel(x_cursor, y, riskMap[y][x_cursor]);
            }
    }

}