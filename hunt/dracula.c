// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

#define TRUE 1
#define FALSE 0

LocationID analyseMoves(DracView gameState, LocationID * whereToGo, int numLocations);
int inTrail(DracView gameState, LocationID place);

void decideDraculaMove(DracView gameState)
{
    int numLocations = 0;
    if (whereIs(gameState, PLAYER_DRACULA) == UNKNOWN_LOCATION) {
        registerBestPlay(idToName(GENEVA),"Going to Geneva cause I have nowhere else to go.");
        return;
    }
    int * whereToGo = whereCanIgo(gameState, &numLocations, 1, 1);
    int bestPlaceToGo = analyseMoves(gameState, whereToGo, numLocations);
    registerBestPlay(idToName(bestPlaceToGo),"Mwuhahahaha");
}

LocationID analyseMoves(DracView gameState, LocationID * whereToGo, int numLocations)
{
    int bestPlaceToGo = NOWHERE;
    /*if (whereToGo[0] != whereIs(gameState, PLAYER_DRACULA)){
     bestPlaceToGo = whereToGo[0];
     } else {*/
    int i;
    //printf("%d\n", numLocations);
    printf("Dracula can go to the following places:\n");
    for (i = 0; i < numLocations; i++) {
        printf("%d\n", whereToGo[i]);
    }
    for (i = 0; i < numLocations && inTrail(gameState, whereToGo[i]); i++);
    bestPlaceToGo = whereToGo[i];
    printf("i is %d, numLocations is %d\n", i, numLocations);
    //}
    return bestPlaceToGo;
}

int inTrail(DracView gameState, LocationID place)
{
    LocationID trail[TRAIL_SIZE];
    giveMeTheTrail(gameState, PLAYER_DRACULA, trail);
    int i;
    printf("trail is \n");
    for (i = 0; i < TRAIL_SIZE; i++) {
        printf("%d\n", trail[i]);
        if (trail[i] == place) return TRUE;
    }
    return FALSE;
}
