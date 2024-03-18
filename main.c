#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

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
    ATTACKPHASE,
    MOVETROOPPHASE,
} currPhase;

gameStates currState;
currPlayer currTurn;
currPhase currAction;

//global variables for game state
#define numCountries 10
int playerNameOnTerritory[numCountries];
int numTroopsOnTerritory[numCountries];

//if arr[x][y] == 1. that means x is connected to y
int isTerritoriesConnected[numCountries][numCountries];


//Helper functions
int findMin(int a, int b){
    return a < b ? a : b;
}


// returns true if territory is taken over
// returns false if not
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
        if (numTroopsOnTerritory[defendTerritory] == 0){
            int numTroopsLeft = attackNumDice - attackLost;

            //populate defendTerritory with remain attackTroops
            numTroopsOnTerritory[attackTerritory]-=numTroopsLeft;
            numTroopsOnTerritory[defendTerritory] = numTroopsLeft;
            playerNameOnTerritory[defendTerritory] = playerNameOnTerritory[attackTerritory];
            return true;
        }
        attackTroops = numTroopsOnTerritory[attackTerritory];
        defendTroops = numTroopsOnTerritory[defendTerritory];
    } 
    return false;
}

int main(void){
    srand((unsigned int)time(NULL));

    currState = STARTSCREEN;
    currTurn = PLAYER1;
    currAction = ATTACKPHASE;

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



