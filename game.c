#include "game.h"
#include "riskMap.h"
#include "draw.h"
#include "enums.h"
#include "globals.h"
#include "helper.h"

void drawMap(currPlayer playerNameOnLoc[], int numTroopsOnLoc[], int locX[], int locY[], currPlayer currTurn, currPhase currAction){

    //TODO - This should put all of this on the back buffer, then flip the buffers and "wait for vsync"



    //draw main map
    drawScreen(*image_data);

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
            drawString(80, 228, "Player 4 (Yellow)");
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
                col = C_YELLOW;
                break;
            default:
                col = C_BLACK; //for debugging
        }

        //draw rectangle at location
        drawRect(locX[i] -4, locY[i]-2, locX[i] +4, locY[i]+2, col - 0x841);

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