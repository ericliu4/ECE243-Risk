#include <stdio.h>

typedef enum{
    STARTSCREEN,
    INGAME,
    ENDSCREEN,
} gameStates;

typedef enum{
    PLAYER1,
    PLAYER2,
    PLAYER3,
    PLAYER4,
} currPlayer;

gameStates currState;
currPlayer currTurn;

int main(){
    currState = STARTSCREEN;
    currTurn = PLAYER1;
    printf("done!!");
    return 0;
}

