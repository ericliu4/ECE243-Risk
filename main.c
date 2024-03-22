#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef enum{
    STARTSCREEN,
    TUTORIAL,
    INGAME,
    ENDSCREEN,
} gameStates;

typedef enum{
    PLAYER1,
    PLAYER2,
    PLAYER3,
    PLAYER4,
} currPlayer;

typedef enum{
    PLACETROOPS,
    ATTACKPHASE,
    MOVETROOPPHASE,
} currPhase;

gameStates currState;
currPlayer currTurn;
currPhase currAction;

//global variables for game state
#define numCountries 42
currPlayer playerNameOnTerritory[numCountries];
int numTroopsOnTerritory[numCountries];

//if arr[x][y] == 1. that means x is connected to y
int isTerritoriesConnected[numCountries][numCountries];

//location of each territory
int locationTerritories[numCountries][numCountries];


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
void loadConnections(void){
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
    isTerritoriesConnected[20][19] = 1;
    isTerritoriesConnected[20][21] = 1;
    isTerritoriesConnected[21][20] = 1;
    isTerritoriesConnected[21][26] = 1;
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
    printf("Loading connections complete\n\n");
}

/* ATTACK FUNCTION
    - Returns true is attack went ahead
    - Returns false if attack own territoy or attacking territory only has 1 troop
    - Calls diceroll and simulate attack*/
bool attack(int location1, int location2){
    if (numTroopsOnTerritory[location1] == 1 || playerNameOnTerritory[location1] == playerNameOnTerritory[location2]){
        return false;
    } 
    //Simulate attack by using dicerolls
    diceRolls(location1, location2);
    return true;
}

/*Calculate the number of troops given to player at start of turn
    - minimum 3 + 1 for every 3 occupying territories.*/
bool calculateNumTroopz(currPlayer turnPlayer){
    int troops = 3;
    int count = 0;
    for (int currTerritory = 0; currTerritory < numCountries; currTerritory++){
        if (turnPlayer == playerNameOnTerritory[currTerritory]){
            count += 1;
        }
    }
    return troops + count/3;
}
/*Move troops from player's territory 1 to territory 2
    - returns true if move is valid
    - return false if not (from and to territory does not match up with curr player)
    Simulate move
*/
bool moveTroopsEndofTurn(int location1, int location2){
    if (currTurn != playerNameOnTerritory[location1] || location1 != location2){
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
    numTroopsOnTerritory[location] += numberToPlace;
    return true;
}

/* This sets up the map
    - randomly generate starting territoies for players
    - randomly generate starting troops for each territory
*/
void initialBoardSetup(void){
    //randomly generate starting territories. 
    //array of numbers 0 to 41 all randomly placed
}

int main(void){
    srand((unsigned int)time(NULL));

    currState = STARTSCREEN;
    currTurn = PLAYER1;
    currAction = ATTACKPHASE;

    loadConnections();



    //testing dice roll function
    numTroopsOnTerritory[0] = 10;
    numTroopsOnTerritory[1] = 10;
    playerNameOnTerritory[0] = 2; // Assuming player names are represented as integers
    playerNameOnTerritory[1] = 3;

    // Call the diceRolls function for territories 0 and 1
    bool result = diceRolls(0, 1);

    // Print the result
    printf("Result of the battle: %s\n", result ? "Victory" : "Defeat");
    printf("Troops on territory 0: %d\n", numTroopsOnTerritory[0]);
    printf("Troops on territory 1: %d\n", numTroopsOnTerritory[1]);
    printf("Player owning territory 0: %d\n", playerNameOnTerritory[0]);
    printf("Player owning territory 1: %d\n", playerNameOnTerritory[1]);


    printf("done!!");
    return 0;
}



