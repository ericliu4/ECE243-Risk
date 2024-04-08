#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "game.h"
#include "structs.h"
#include "enums.h"
#include "keyboard.h"
#include "draw.h"
#include "audio.h"

gameStates prevState;
gameStates nextState;
gameStates currState;
currPlayer currTurn;
currPhase currAction;
//extern mouse_movement movement;

cursor_t cursor;


int cursorLoc[2];
int prevCursorLoc[2];

//global variables for game state
#define numCountries 42
currPlayer playerNameOnTerritory[numCountries];
int numTroopsOnTerritory[numCountries];

//if arr[x][y] == 1. that means x is connected to y
int isTerritoriesConnected[numCountries][numCountries];

//location of each territory
int locationTerritoriesX[numCountries];
int locationTerritoriesY[numCountries];


//Helper functions
int findMin(int a, int b){
    return a < b ? a : b;
}

/* Simulate the dice roll and change the board accordingly
    returns true if territory is taken over
    returns false if not */
bool diceRolls(int attackTerritory, int defendTerritory){
    int attackTroops = numTroopsOnTerritory[attackTerritory];
    int defendTroops = numTroopsOnTerritory[defendTerritory];

    //Keep looping until attackTroops = 1 or territory is taken over
    while (attackTroops > 1){
        
        //2 scenarios: defend with 1 or defend with 2 soldiers
        int attackNumDice = findMin(attackTroops-1, 3);
        int defendNumDice = findMin(defendTroops, 2);

        //1 will be the highest
        int attackDice1 = -1;
        int attackDice2 = -1;
        int defendDice1 = -1;
        int defendDice2 = -1;

        int generated;
        for (int i = 0; i < attackNumDice; i++){
            generated = rand()%6;
            if (generated >= attackDice1){
                attackDice2 = attackDice1;
                attackDice1 = generated;
            } else if (generated >= attackDice2){
                attackDice2 = generated;
            } 
        }
        for (int j = 0; j < defendNumDice; j++){
            generated = rand()%6;
            if (generated >= defendDice1){
                defendDice2 = defendDice1;
                defendDice1 = generated;
            } else {
                defendDice2 = generated;
            }
        }
        
        //find min combats
        int minDice = findMin(attackNumDice, defendNumDice);
        int attackLost = 0;
        if (minDice == 1){
            if (attackDice1 > defendDice1){
                numTroopsOnTerritory[defendTerritory]--;
            } else {
                attackLost++;
                numTroopsOnTerritory[attackTerritory]--;
            }
        } else {
            if (attackDice1 > defendDice1){
                numTroopsOnTerritory[defendTerritory]--;
            } else {
                attackLost++;
                numTroopsOnTerritory[attackTerritory]--;
            }
            if (attackDice2 > defendDice2){ 
                numTroopsOnTerritory[defendTerritory]--;
            } else {
                attackLost++;
                numTroopsOnTerritory[attackTerritory]--;
            }
        }
        if (numTroopsOnTerritory[defendTerritory] <= 0){
            int numTroopsLeft = attackNumDice - attackLost;

            //populate defendTerritory with remain attackTroops
            numTroopsOnTerritory[attackTerritory]-= numTroopsLeft;
            numTroopsOnTerritory[defendTerritory] = numTroopsLeft;
            playerNameOnTerritory[defendTerritory] = playerNameOnTerritory[attackTerritory];
            return true;
        }
        attackTroops = numTroopsOnTerritory[attackTerritory];
        defendTroops = numTroopsOnTerritory[defendTerritory];
    } 
    return false;
}
/*LOAD CONNECTIONS FUNCTION
    - Loads all possible map connections into 2D array (isTerritoriesConnected)
    - 0 between [x][y] means they are not connected
    - 1 between [x][y] menas they have one connections */
void loadConnections(){
    for (int firstCountry = 0; firstCountry < numCountries; firstCountry++){
        for (int secondCountry = 0; secondCountry < numCountries; secondCountry++){
            isTerritoriesConnected[firstCountry][secondCountry] = 0;
        }
    }
    // manually load up all connections
    // map and numbering system found in connections
    isTerritoriesConnected[0][1] = 1;
    isTerritoriesConnected[1][0] = 1;
    isTerritoriesConnected[0][2] = 1;
    isTerritoriesConnected[2][0] = 1;
    isTerritoriesConnected[0][36] = 1;
    isTerritoriesConnected[36][0] = 1;
    isTerritoriesConnected[2][1] = 1;
    isTerritoriesConnected[1][2] = 1;
    isTerritoriesConnected[1][6] = 1;
    isTerritoriesConnected[6][1] = 1;
    isTerritoriesConnected[8][1] = 1;
    isTerritoriesConnected[1][8] = 1;
    isTerritoriesConnected[2][6] = 1;
    isTerritoriesConnected[6][2] = 1;
    isTerritoriesConnected[2][3] = 1;
    isTerritoriesConnected[3][2] = 1;
    isTerritoriesConnected[3][4] = 1;
    isTerritoriesConnected[4][3] = 1;
    isTerritoriesConnected[3][5] = 1;
    isTerritoriesConnected[5][3] = 1;
    isTerritoriesConnected[3][6] = 1;
    isTerritoriesConnected[6][3] = 1;
    isTerritoriesConnected[4][5] = 1;
    isTerritoriesConnected[5][4] = 1;
    isTerritoriesConnected[4][23] = 1;
    isTerritoriesConnected[23][4] = 1;
    isTerritoriesConnected[5][6] = 1;
    isTerritoriesConnected[6][5] = 1;
    isTerritoriesConnected[5][7] = 1;
    isTerritoriesConnected[7][5] = 1;
    isTerritoriesConnected[6][7] = 1;
    isTerritoriesConnected[7][6] = 1;
    isTerritoriesConnected[6][8] = 1;
    isTerritoriesConnected[8][6] = 1;
    isTerritoriesConnected[7][8] = 1;
    isTerritoriesConnected[8][7] = 1;
    isTerritoriesConnected[8][9] = 1;
    isTerritoriesConnected[9][8] = 1;
    isTerritoriesConnected[9][10] = 1;
    isTerritoriesConnected[10][9] = 1;
    isTerritoriesConnected[9][13] = 1;
    isTerritoriesConnected[13][9] = 1;
    isTerritoriesConnected[10][11] = 1;
    isTerritoriesConnected[11][10] = 1;
    isTerritoriesConnected[10][12] = 1;
    isTerritoriesConnected[12][10] = 1;
    isTerritoriesConnected[10][13] = 1;
    isTerritoriesConnected[13][10] = 1;
    isTerritoriesConnected[11][12] = 1;
    isTerritoriesConnected[12][11] = 1;
    isTerritoriesConnected[11][17] = 1;
    isTerritoriesConnected[17][11] = 1;
    isTerritoriesConnected[11][15] = 1;
    isTerritoriesConnected[15][11] = 1;
    isTerritoriesConnected[12][15] = 1;
    isTerritoriesConnected[15][12] = 1;
    isTerritoriesConnected[12][13] = 1;
    isTerritoriesConnected[13][12] = 1;
    isTerritoriesConnected[12][14] = 1;
    isTerritoriesConnected[14][12] = 1;
    isTerritoriesConnected[13][14] = 1;
    isTerritoriesConnected[14][13] = 1;
    isTerritoriesConnected[14][15] = 1;
    isTerritoriesConnected[15][14] = 1;
    isTerritoriesConnected[14][26] = 1;
    isTerritoriesConnected[26][14] = 1;
    isTerritoriesConnected[14][40] = 1;
    isTerritoriesConnected[40][14] = 1;
    isTerritoriesConnected[14][41] = 1;
    isTerritoriesConnected[41][14] = 1;
    isTerritoriesConnected[15][16] = 1;
    isTerritoriesConnected[16][15] = 1;
    isTerritoriesConnected[15][26] = 1;
    isTerritoriesConnected[26][15] = 1;
    isTerritoriesConnected[16][17] = 1;
    isTerritoriesConnected[17][16] = 1;
    isTerritoriesConnected[16][21] = 1;
    isTerritoriesConnected[21][16] = 1;
    isTerritoriesConnected[16][26] = 1;
    isTerritoriesConnected[26][16] = 1;
    isTerritoriesConnected[17][18] = 1;
    isTerritoriesConnected[18][17] = 1;
    isTerritoriesConnected[17][21] = 1;
    isTerritoriesConnected[21][17] = 1;
    isTerritoriesConnected[17][22] = 1;
    isTerritoriesConnected[22][17] = 1;
    isTerritoriesConnected[18][19] = 1;
    isTerritoriesConnected[19][18] = 1;
    isTerritoriesConnected[18][21] = 1;
    isTerritoriesConnected[21][18] = 1;
    isTerritoriesConnected[19][20] = 1;
    isTerritoriesConnected[19][21] = 1;
    isTerritoriesConnected[20][19] = 1;
    isTerritoriesConnected[20][21] = 1;
    isTerritoriesConnected[21][20] = 1;
    isTerritoriesConnected[21][26] = 1;
    isTerritoriesConnected[21][19] = 1;
    isTerritoriesConnected[26][21] = 1;
    isTerritoriesConnected[22][23] = 1;
    isTerritoriesConnected[23][22] = 1;
    isTerritoriesConnected[22][24] = 1;
    isTerritoriesConnected[24][22] = 1;
    isTerritoriesConnected[22][25] = 1;
    isTerritoriesConnected[25][22] = 1;
    isTerritoriesConnected[23][24] = 1;
    isTerritoriesConnected[24][23] = 1;
    isTerritoriesConnected[24][25] = 1;
    isTerritoriesConnected[25][24] = 1;
    isTerritoriesConnected[26][27] = 1;
    isTerritoriesConnected[27][26] = 1;
    isTerritoriesConnected[26][41] = 1;
    isTerritoriesConnected[41][26] = 1;
    isTerritoriesConnected[27][28] = 1;
    isTerritoriesConnected[28][27] = 1;
    isTerritoriesConnected[27][33] = 1;
    isTerritoriesConnected[33][27] = 1;
    isTerritoriesConnected[27][41] = 1;
    isTerritoriesConnected[41][27] = 1;
    isTerritoriesConnected[28][29] = 1;
    isTerritoriesConnected[29][28] = 1;
    isTerritoriesConnected[28][33] = 1;
    isTerritoriesConnected[33][28] = 1;
    isTerritoriesConnected[29][32] = 1;
    isTerritoriesConnected[32][29] = 1;
    isTerritoriesConnected[29][30] = 1;
    isTerritoriesConnected[30][29] = 1;
    isTerritoriesConnected[30][31] = 1;
    isTerritoriesConnected[31][30] = 1;
    isTerritoriesConnected[30][32] = 1;
    isTerritoriesConnected[32][30] = 1;
    isTerritoriesConnected[31][32] = 1;
    isTerritoriesConnected[32][31] = 1;
    isTerritoriesConnected[33][34] = 1;
    isTerritoriesConnected[34][33] = 1;
    isTerritoriesConnected[33][39] = 1;
    isTerritoriesConnected[39][33] = 1;
    isTerritoriesConnected[33][40] = 1;
    isTerritoriesConnected[40][33] = 1;
    isTerritoriesConnected[33][41] = 1;
    isTerritoriesConnected[41][33] = 1;
    isTerritoriesConnected[34][35] = 1;
    isTerritoriesConnected[35][34] = 1;
    isTerritoriesConnected[34][38] = 1;
    isTerritoriesConnected[38][34] = 1;
    isTerritoriesConnected[34][36] = 1;
    isTerritoriesConnected[36][34] = 1;
    isTerritoriesConnected[34][39] = 1;
    isTerritoriesConnected[39][34] = 1;
    isTerritoriesConnected[35][36] = 1;
    isTerritoriesConnected[36][35] = 1;
    isTerritoriesConnected[36][37] = 1;
    isTerritoriesConnected[37][36] = 1;
    isTerritoriesConnected[36][38] = 1;
    isTerritoriesConnected[38][36] = 1;
    isTerritoriesConnected[37][38] = 1;
    isTerritoriesConnected[38][37] = 1;
    isTerritoriesConnected[37][39] = 1;
    isTerritoriesConnected[39][37] = 1;
    isTerritoriesConnected[38][39] = 1;
    isTerritoriesConnected[39][38] = 1;
    isTerritoriesConnected[39][40] = 1;
    isTerritoriesConnected[40][39] = 1;
    isTerritoriesConnected[40][41] = 1;
    isTerritoriesConnected[41][40] = 1;
}

/*dimensions
*/
void loadLocations(){
    locationTerritoriesX[0] = 23;
    locationTerritoriesY[0] = 34;
    locationTerritoriesX[1] = 51;
    locationTerritoriesY[1] = 34;
    locationTerritoriesX[2] = 49;
    locationTerritoriesY[2] = 51;
    locationTerritoriesX[3] = 49;
    locationTerritoriesY[3] = 75;
    locationTerritoriesX[4] = 53;
    locationTerritoriesY[4] = 100;    
    locationTerritoriesX[5] = 75;
    locationTerritoriesX[5] = 80;
    locationTerritoriesY[5] = 75;

    locationTerritoriesX[6] = 70;
    locationTerritoriesY[6] = 54;
    locationTerritoriesX[7] = 93;
    locationTerritoriesY[7] = 54;
    locationTerritoriesX[8] = 111;
    locationTerritoriesY[8] = 23;
    locationTerritoriesX[9] = 135;
    locationTerritoriesY[9] = 42;
    locationTerritoriesX[10] = 128;
    locationTerritoriesY[10] = 68;
    locationTerritoriesX[11] = 136;
    locationTerritoriesY[11] = 94;
    locationTerritoriesX[12] = 158;
    locationTerritoriesY[12] = 72;
    locationTerritoriesX[13] = 161;
    locationTerritoriesY[13] = 40;
    locationTerritoriesX[14] = 189;
    locationTerritoriesY[14] = 55;
    locationTerritoriesX[15] = 162;
    locationTerritoriesY[15] = 92;

    locationTerritoriesX[16] = 171;
    locationTerritoriesY[16] = 121;
    locationTerritoriesX[17] = 146;
    locationTerritoriesY[17] = 134;
    locationTerritoriesX[18] = 171;
    locationTerritoriesY[18] = 159;
    locationTerritoriesX[19] = 172;
    locationTerritoriesY[19] = 187;
    locationTerritoriesX[20] = 203;
    locationTerritoriesY[20] = 187;
    locationTerritoriesX[21] = 193;
    locationTerritoriesY[21] = 152;
    locationTerritoriesX[22] = 102;
    locationTerritoriesY[22] = 139;
    locationTerritoriesX[23] = 77;
    locationTerritoriesY[23] = 119;
    locationTerritoriesX[24] = 69;
    locationTerritoriesY[24] = 142;
    locationTerritoriesX[25] = 80;
    locationTerritoriesY[25] = 179;

    locationTerritoriesX[26] = 198;
    locationTerritoriesY[26] = 111;
    locationTerritoriesX[27] = 232;
    locationTerritoriesY[27] = 115;
    locationTerritoriesX[28] = 260;
    locationTerritoriesY[28] = 122;
    locationTerritoriesX[29] = 263;
    locationTerritoriesY[29] = 157;
    locationTerritoriesX[30] = 273;
    locationTerritoriesY[30] = 190;
    locationTerritoriesX[31] = 300;
    locationTerritoriesY[31] = 175;
    locationTerritoriesX[32] = 293;
    locationTerritoriesY[32] = 144;
    locationTerritoriesX[33] = 253;
    locationTerritoriesY[33] = 95;
    locationTerritoriesX[34] = 259;
    locationTerritoriesY[34] = 75;
    locationTerritoriesX[35] = 296;
    locationTerritoriesY[35] = 80;

    locationTerritoriesX[36] = 281;
    locationTerritoriesY[36] = 35;
    locationTerritoriesX[37] = 260;
    locationTerritoriesY[37] = 27;
    locationTerritoriesX[38] = 256;
    locationTerritoriesY[38] = 56;
    locationTerritoriesX[39] = 236;
    locationTerritoriesY[39] = 38;
    locationTerritoriesX[40] = 220;
    locationTerritoriesY[40] = 52;
    locationTerritoriesX[41] = 214;
    locationTerritoriesY[41] = 80;

}
/* ATTACK FUNCTION
    - Returns true is attack went ahead
    - Returns false if attack own territoy or attacking territory only has 1 troop
    - Calls diceroll and simulate attack*/
bool attack(int location1, int location2){
    if (numTroopsOnTerritory[location1] == 1 || playerNameOnTerritory[location1] == playerNameOnTerritory[location2] || isTerritoriesConnected[location1][location2] == 0){
        return false;
    } 
    //Simulate attack by using dicerolls
    printf("Got to dice rolls\n");
    printf("%d, %d \n", location1, location2);
    diceRolls(location1, location2);
    return true;
}

/*Calculate the number of troops given to player at start of turn
    - minimum 3 + 1 for every 3 occupying territories.
    -if player owns a continent, get continental bonus*/
int calculateNumTroops(currPlayer turnPlayer){
    int troops = 0;
    int count = 0;
    for (int currTerritory = 0; currTerritory < numCountries; currTerritory++){
        if (turnPlayer == playerNameOnTerritory[currTerritory]){
            count += 1;
        }
    }

    bool bonus = true;
    // NORTH AMERICA
    for (int i = 0; i < 9; i++){
        if (playerNameOnTerritory[i] != turnPlayer){
            bonus = false;
            break;
        }
    }
    if (bonus){
        troops += 5;
    }
    //EUROPE
    bonus = true;
    for (int i = 9; i < 16; i++){
        if (playerNameOnTerritory[i] != turnPlayer){
            bonus = false;
            break;
        }
    }
    if (bonus){
        troops += 6;
    }
    //AFRICA
    bonus = true;
    for (int i = 16; i < 22; i++){
        if (playerNameOnTerritory[i] != turnPlayer){
            bonus = false;
            break;
        }
    }
    if (bonus){
        troops += 4;
    }
    //SOUTH AMERICA
    bonus = true;
    for (int i = 22; i < 26; i++){
        if (playerNameOnTerritory[i] != turnPlayer){
            bonus = false;
            break;
        }
    }
    if (bonus){
        troops += 3;
    }
    //AUSTRALIA
    bonus = true;
    for (int i = 29; i < 33; i++){
        if (playerNameOnTerritory[i] != turnPlayer){
            bonus = false;
            break;
        }
    }
    if (bonus){
        troops += 2;
    }
    //ASIA
    bonus = true;
    for (int i = 26; i < 29; i++){
        if (playerNameOnTerritory[i] != turnPlayer){
            bonus = false;
            break;
        }
    }
    for (int i = 33; i < 42; i++){
        if (playerNameOnTerritory[i] != turnPlayer){
            bonus = false;
            break;
        }
    }
    if (bonus){
        troops += 7;
    }
    if (count/3 <= 3){
        count = 9;
    }
    int total = troops + count/3;
    printf("ADDED %d Troops\n", total);
    return total;
}
/*Move troops from player's territory 1 to territory 2
    - returns true if move is valid
    - return false if not (from and to territory does not match up with curr player)
    Simulate move
*/
bool moveTroopsEndofTurn(int location1, int location2){
    if (currTurn != playerNameOnTerritory[location1] || currTurn != playerNameOnTerritory[location1]){
        return false;
    }
    int toMove = numTroopsOnTerritory[location1]-1;
    numTroopsOnTerritory[location1] = 1;
    numTroopsOnTerritory[location2] += toMove;
    return true;
}
/* at the start of player turn, player is given troops
    - returns true is player chose an valid location
    - returns false if player chose an invalid location
    -simulate place troops on territory
*/
bool placeTroopsStartOfTurn(int location, int numberToPlace){
    if (playerNameOnTerritory[location] != currTurn){
        return false;
    }
    printf("%d BEFORE, %d PLACED \n",numTroopsOnTerritory[location], numberToPlace);
    numTroopsOnTerritory[location] = numTroopsOnTerritory[location] + numberToPlace;
    return true;
}

/* This sets up the map
    - randomly generate starting territoies for players
    - randomly generate starting troops for each territory*/
void initialBoardSetup(){
    //randomly generate starting territories. 
    //array of numbers 0 to 41 all randomly placed
    int randomTerritoryNumbers[numCountries];
    for (int i = 0; i < numCountries; i++){
        randomTerritoryNumbers[i] = i;
    }
    //printf("HEREEEE");
    //randomly swap order of array
    for (int firstIndex = 41; firstIndex > 0; firstIndex--){
        int secondIndex = rand()%(firstIndex + 1);
        int temp = randomTerritoryNumbers[firstIndex];
        randomTerritoryNumbers[firstIndex] = randomTerritoryNumbers[secondIndex];
        randomTerritoryNumbers[secondIndex] = temp;
    }
    //printf("HEREEEE");
    for (int i = 0; i < numCountries; i++){
        if (i <= 10){
            playerNameOnTerritory[randomTerritoryNumbers[i]] = PLAYER1;
        } else if (i <= 21) {
            playerNameOnTerritory[randomTerritoryNumbers[i]] = PLAYER2;
        } else if (i <= 31){
            playerNameOnTerritory[randomTerritoryNumbers[i]] = PLAYER3;
        } else {
            playerNameOnTerritory[randomTerritoryNumbers[i]] = PLAYER4;
        }
    }
    

    //set up random amount of troops starting
    for (int i = 0; i < numCountries; i++){
        //numTroopsOnTerritory[i] = rand()%5+1;
        numTroopsOnTerritory[i] = rand()%5 + 1;
    }
}



/*Check if all the territories is occupied by the same player
*/
bool checkEndGame(void){
    currPlayer curr = playerNameOnTerritory[0];
    for (int i = 0; i < numCountries; i++){
        if (curr != playerNameOnTerritory[i]);
        return false;
    }
    return true;
}

/*Temporary Function. Will be replaced by Nick's code
Returns the selected territory index
*/
int getSelectedTerritory(){
    // int a;
    // scanf("%d", &a);
    // return a;
    while(1){
        // poll keyboard

        updateCursor();

        if(cursor.q){
            cursor.q = false;
            return -1;
        }

        if(cursor.clicked){
            cursor.clicked = false;
            for(int i = 0; i < 42; ++i){
                if(numTroopsOnTerritory[i] < 10){
                    if(cursor.xPos <= locationTerritoriesX[i] +4 && cursor.xPos >= locationTerritoriesX[i] -4){
                        if(cursor.yPos <= locationTerritoriesY[i] +4 && cursor.yPos >= locationTerritoriesY[i] -4){
                            printf("Found a territory");
                            return i;
                        }
                    }
                } else {
                    if(cursor.xPos <= locationTerritoriesX[i] +10 && cursor.xPos >= locationTerritoriesX[i] -4){
                        if(cursor.yPos <= locationTerritoriesY[i] +10 && cursor.yPos >= locationTerritoriesY[i] -4){
                            printf("Found a territory");
                            return i;
                        }
                    }
                }

            }
        printf("No territory found");
        }

    }

    //return -1;
}

/* Prints the current Map
For each territories: name of index, who occupies the territory and num troops on territory
*/
void printMap(void){
    for (int i = 0; i < numCountries; i++){
        currPlayer PLAYER = playerNameOnTerritory[i];
        int playerNum = 0;
        if (PLAYER == PLAYER2){
            playerNum = 1;
        } else if (PLAYER == PLAYER3){
            playerNum = 2;
        } else if (PLAYER == PLAYER4){
            playerNum = 3;
        }
        printf("%d, PLAYER: %d, NUM TROOPS: %d\n", i, playerNum, numTroopsOnTerritory[i]);
    }
}

/* Function that walks through the player's actions
Initial place troops, attack phase and move at end of turn*/
void playerTurn(void){
    //phase 1. place troops

    printf("Phase 1: place troops\n");
    int placeTroopsIndex = getSelectedTerritory();
    int numPlacedTroops = calculateNumTroops(PLAYER1);
    printf("NUM PLACED TROOPZ VAR %d \n", numPlacedTroops);
    bool checkValidPlaceTroops = placeTroopsStartOfTurn(placeTroopsIndex, numPlacedTroops);
    while (!checkValidPlaceTroops){
        printf("ILLEGAL MOVE!! TRY AGAIN\n");
        placeTroopsIndex = getSelectedTerritory();
        checkValidPlaceTroops = placeTroopsStartOfTurn(placeTroopsIndex, numPlacedTroops);
    }

    currAction = ATTACKPHASE;
    drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);
    drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);
    //printMap();
    //printMap();

    //phase 2. attack
    printf("Phase 2: attack\n");
    bool valid = true;
    while(valid){
        int getFirstLocation = getSelectedTerritory();
        if (getFirstLocation == -1){
            valid = false;
            break;
        } else if (playerNameOnTerritory[getFirstLocation] != PLAYER1){
            printf("Illegal Move TERRITORY 1!!\n");
            continue;
        }
        //but a box around selected tutorial
        int x_i = locationTerritoriesX[getFirstLocation] /4 *4;
        int y_i = locationTerritoriesY[getFirstLocation] /4 *4;
        for(int i = 0; i < 2; ++i){
            drawHollowRect(x_i -2, y_i -2, x_i +6, y_i +6, C_WHITE);
            swapBuffers();
            wait_for_vsync();
        }

        int getSecondLocation = getSelectedTerritory();

        currPlayer defender = playerNameOnTerritory[getSecondLocation];

        printf("GOT TO ATTACK, TYPE 99 to end attack phase\n");
        bool success = attack(getFirstLocation, getSecondLocation);
        if (success == false){
            printf("Illegal Move TERRITORY 22222!!\n");
            continue;
        } else {

            drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);
            drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);

            //only play sound for the player turn not the computer
            if(defender == playerNameOnTerritory[getSecondLocation]){
                playWompWomp();
            } else {
                playVictory();
            }

        }
    }


    currAction = MOVETROOPPHASE;
    drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);
    drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);
    //printMap();

    printf("Phase 3: Move Troops");
    valid = true;
    while(valid){
        int getFirstLocation = getSelectedTerritory();
        if (getFirstLocation == -1){
            break;
        } else if (playerNameOnTerritory[getFirstLocation] != PLAYER1) {
            printf("Illegal Move!!\n");
            continue;
        }
                //but a box around selected tutorial
        int x_i = locationTerritoriesX[getFirstLocation] /4 *4;
        int y_i = locationTerritoriesY[getFirstLocation] /4 *4;
        for(int i = 0; i < 2; ++i){
            drawHollowRect(x_i -2, y_i -2, x_i +6, y_i +6, C_WHITE);
            swapBuffers();
            wait_for_vsync();
        }
        int getSecondLocation = getSelectedTerritory();
        bool success = moveTroopsEndofTurn(getFirstLocation, getSecondLocation);
        if (success == false){
            printf("Illegal Move!!\n");
            continue;
        } else {
            valid = false;
        }
    }

    drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);
    drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);
    //printMap();
    //printMap();
}
/*
First check if this specific bot is still in the game, if not exit
then follow the same 3 steps
1. place troops
2. attack
3. move at end of turn
*/
void machineTurn(){
    printf("machine turn\n");
    //first check if this currPlayer still in game
    bool inGame = false;
    int currAddTroopIndex = -1;
    for (int i = 0; i < numCountries; i++){
        if (playerNameOnTerritory[i] == currTurn){
            inGame = true;
            if (currAddTroopIndex == -1){
                currAddTroopIndex = i;
            } else {
                //25% chance to replace the index
                //randomness factor here
                if ((rand()%100) < 25){
                    currAddTroopIndex = i;
                }
            }
        }
    }
    //exit function if current ai already lost
    if (inGame == false){
        return;
    }

    //since the machine move is 100% legal, directly call attack function
    int numPlacedTroops = calculateNumTroops(currTurn);
    bool success = placeTroopsStartOfTurn(currAddTroopIndex, numPlacedTroops);
    printf("Successfully placed troops");

    currAction = ATTACKPHASE;
    drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);
    drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);

    //attack phase. machine will only attack 0 or 1 territories
    if ((rand()%100) < 50){
        printf("AI will not attack this round\n");
    } else {
        printf("AI will attack this round\n");

        int attackTerritory = -1;
        int defendTerritory = -1;
        for (int i = 0; i < numCountries; i++){
            if (playerNameOnTerritory[i] == currTurn){
                //means a valid starting points.
                for (int j = 0; j < numCountries; j++){
                    //for each valid second attacking country. there is a 10% chance of switching
                    //add to randomness factor
                    if (playerNameOnTerritory[j] != currTurn && isTerritoriesConnected[i][j]){
                        if (attackTerritory == -1){
                            attackTerritory = i;
                            defendTerritory = j;
                        } else {
                            if ((rand()%100)< 11){
                                attackTerritory = i;
                                defendTerritory = j;
                            }
                        }
                    }
                }
            }
        }
                //but a box around selected tutorial
        int x_i = locationTerritoriesX[attackTerritory] /4 *4;
        int y_i = locationTerritoriesY[attackTerritory] /4 *4;
        for(int i = 0; i < 2; ++i){
            drawHollowRect(x_i -2, y_i -2, x_i +6, y_i +6, C_WHITE);
            swapBuffers();
            wait_for_vsync();
        }

                        //but a box around selected tutorial
        x_i = locationTerritoriesX[defendTerritory] /4 *4;
        y_i = locationTerritoriesY[defendTerritory] /4 *4;
        for(int i = 0; i < 2; ++i){
            drawHollowRect(x_i -2, y_i -2, x_i +6, y_i +6, C_BLACK);
            swapBuffers();
            wait_for_vsync();
        }
        bool attackedworked = attack(attackTerritory, defendTerritory);
    }

    currAction = MOVETROOPPHASE;
    drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);
    drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);
    //complete attack phase

    //for move phase, ignore
}

/*
Only called at endgame stage. Check if the player won the game
*/
bool checkIfPlayerWIn(void){
    //This function only called if there is only 1 player left
    return PLAYER1 == playerNameOnTerritory[0];
}

int main(void){
    printf("Start MAIN\n");
    srand((unsigned int)time(NULL));

    //swapBuffers();

    bool mapChanged;

    prevState = ENDSCREEN;
    currState = STARTSCREEN;
    currTurn = PLAYER1;
    currAction = ATTACKPHASE;

    loadConnections();
    initialBoardSetup();
    loadLocations();

    cursor.xPos = SCREEN_WIDTH /2;
    cursor.yPos = SCREEN_HEIGHT /2; //start in center;

    clearCharBuffer();


    //FSM for gane state
    while(1){
        if (currState == STARTSCREEN){
            //printf("State: StartScreen\n");
            if(prevState != STARTSCREEN)
                drawTitleScreen();   


            //start screen polling
            if(cursor.clicked){ //use this for title screen polling checj
                printf("Exit StartScreen");
                nextState = TUTORIAL;
                cursor.clicked = false;
            }

            
        }
        else if (currState == TUTORIAL){
            if(prevState != TUTORIAL){
                printf("State: Tutorial\n");
                drawTutorialScreen();
            }
            if(cursor.clicked){ //use this for polling from space bar
                printf("Exiting Tutorial\n");
                nextState = INGAME;
                mapChanged = true;
                cursor.clicked = false;
            }
            

        }

         else if (currState == INGAME){
            //in game function
            currAction = PLACETROOPS;

            /*if(mapChanged){
                printf("State: InGame\n");

                drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);
                drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction); //call twice so it draws on both buffers
                mapChanged = false;
            }*/
            drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction);
            drawMap(playerNameOnTerritory, numTroopsOnTerritory, locationTerritoriesX, locationTerritoriesY, currTurn, currAction); //call twice so it draws on both buffers
            //updateCursor();
            //inGameScreenPolling();
            //gameLogic
            bool endGame = checkEndGame();

            if (currTurn == PLAYER1){
                playerTurn();
                currTurn = PLAYER2;
            } else if (currTurn == PLAYER2){
                machineTurn();
                currTurn = PLAYER3;
            } else if (currTurn == PLAYER3){
                machineTurn();
                currTurn = PLAYER4;
            } else {
                machineTurn();
                currTurn = PLAYER1;
            }

            if(endGame){ 
                nextState = ENDSCREEN;
            }

            //either player or ai turn
            //right now for testing purposes. always player turn

        } 
        else {
            //end screen function
            printf("State: EndGame\n");
            //drawEndGame(); <- make sure to pass through if the player won or lost
            bool playerWon = checkIfPlayerWIn();
            clearCharBuffer();
            if(playerWon){
                drawWinScreen();
                playVictory();
            } else {
                drawLoseScreen();
                playWompWomp();
            }
            //drawEndGame(playerWon); <- make sure to pass through if the player won or lost
            if(cursor.clicked){ 
                printf("Exit EndGame Stage");
                nextState = STARTSCREEN;
                cursor.clicked = false;
            }     
        }

        //update FSM
        prevState = currState;
        currState = nextState;

        pollKeyboard();

        clearFifo();
        //poll keyboard for any input
        // if(prevCursorLoc[0] == cursor.xPos && prevCursorLoc[1] == cursor.yPos){
        //     continue;
        // }
        // //eraseCursor(prevCursorLoc[0], prevCursorLoc[1], 2, currState);
        // prevCursorLoc[0] = cursor.xPos;
        // prevCursorLoc[1] = cursor.yPos;
        // pollKeyboard();
        // drawCursor(cursor.xPos, cursor.yPos, C_WHITE, 5);
        // swapBuffers();
    
        // clearFifo();

    }

    
    return 0;
}



