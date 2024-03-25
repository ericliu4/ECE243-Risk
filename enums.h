#ifndef ENUMS_H
#define ENUMS_H

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

#endif /*ENUMS_H*/