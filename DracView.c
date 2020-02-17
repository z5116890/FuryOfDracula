// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Map.h" //... if you decide to use the Map ADT
#include "Queue.h"
#include "Play.h"

#define MAX_LENGTH 14680

struct dracView {
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    char play[MAX_LENGTH];
    PlayerMessage messages[MESSAGE_SIZE];
    GameView gv;
};


// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    DracView dracView = malloc(sizeof(struct dracView));
    strcpy(dracView->play, pastPlays);
    dracView -> gv = newGameView(dracView -> play,dracView -> messages);
    
    return dracView;
}


// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
    Round currentR = getRound(currentView -> gv);
    return currentR;
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    int currentScore = getScore(currentView -> gv);
    return currentScore;
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    int playerHealth = getHealth(currentView -> gv, player);
    return playerHealth;
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    
    LocationID playerLoc = getLocation(currentView -> gv, player);
    return playerLoc;
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
              LocationID *start, LocationID *end)
{
    LocationID trail[5];
    getHistory(currentView -> gv, player, trail);
    // NEED TO CHECK
    *start = trail[1];
    *end = trail[0];
    
    
    
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                int *numTraps, int *numVamps)
{
    *numTraps = 0;
    *numVamps = 0;
     LocationID trail[TRAIL_SIZE];
    printf("numVamps = %d\n", *numVamps);
     giveMeTheTrail(currentView, PLAYER_DRACULA, trail);
     printf("numVamps = %d\n", *numVamps);
     int locFound = FALSE;

     //current round - 1 gives you recent trail element
    Round roundN = giveMeTheRound(currentView) - 1;
    
     for(int i = 0; i < TRAIL_SIZE; i++){
     if(trail[i] == where) locFound = TRUE;
     }
    
     if (where != UNKNOWN_LOCATION && locFound == TRUE){

         for(int i = 0; i < TRAIL_SIZE; i++){
             if(trail[i] == where){
                 if(roundN%13 == 0){
                     *numVamps = *numVamps + 1;
                 }else{
                     *numTraps = *numTraps + 1;
                 }
             }
             roundN--;
         }
         printf("numVamps = %d\n", *numVamps);

     }
    

}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                    LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView -> gv, player, trail);
//    for(int i = 0; i < TRAIL_SIZE; i++){
 //       printf("Historytrail[%d] = %d\n", i, trail[i]);
  //  }
    
    if(player == PLAYER_DRACULA){
        printf("yo\n");
        for(int i = 0; i < TRAIL_SIZE; i++){
            print
            switch(trail[i]){
                case DOUBLE_BACK_1: trail[i] = trail[i + 1]; break;
                case DOUBLE_BACK_2: trail[i] = trail[i + 2]; break;
                case DOUBLE_BACK_3: trail[i] = trail[i + 3]; break;
                case DOUBLE_BACK_4: trail[i] = trail[i + 4]; break;
                case DOUBLE_BACK_5: trail[i] = trail[i + 5]; break;
                case HIDE: trail[i] = trail[i+1]; break;
            }
        }
    }

}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{

    //access current trail so we can look for HIDE or DOUBLE BACK
    LocationID dracTrail[TRAIL_SIZE] = {0};
    giveMeTheTrail(currentView, PLAYER_DRACULA, dracTrail);
    
    
    
    //get connectedLocations without taking into consideration drac's trail
    Round roundNum = giveMeTheRound(currentView);
    LocationID whereIsDrac = whereIs(currentView, PLAYER_DRACULA);
    LocationID *connectedPlaces = connectedLocations(currentView -> gv,numLocations, whereIsDrac,
                                                     PLAYER_DRACULA, roundNum, road, 0, sea);

    
    
    int finalSize = *numLocations;
    int count = 0;
    
    //check if hide and/or double back is in trail
    int hide = FALSE;
    int doubleb = FALSE;
    
    //find if drac's trail has double back or hide in it
    int t;
    for(t = 0; t < TRAIL_SIZE; t++){
        if(dracTrail[t] == DOUBLE_BACK_1 || dracTrail[t] == DOUBLE_BACK_2 || dracTrail[t] == DOUBLE_BACK_3 ||
           dracTrail[t] == DOUBLE_BACK_4 ||dracTrail[t] == DOUBLE_BACK_5) doubleb = TRUE;
        if(dracTrail[t] == HIDE) hide = TRUE;
    }
    
    //mark with -2 to denote removal of item
    //if hide already in trail then current location cant be part of his trail again
    //remove his current location from connectedLocations
    if(hide == TRUE){
        for(int p = 0; p < finalSize; p++){
            if(connectedPlaces[p] == whereIsDrac){
                connectedPlaces[p] = -2;
                count++;
            }
        }
    }
    printf("finalSize = %d\n", finalSize);
    
    //drac cannot go anywhere that is already in trail if doubleback is used
    //remove any locations in connectedPlaces that are in his trail
    if(doubleb == TRUE){
        for(int i = 0; i < TRAIL_SIZE; i++){
            for(int j = 0; j < finalSize; j++){
                if(dracTrail[i] == connectedPlaces[j]){
                    count++;
                    connectedPlaces[j] = -2;
                }
            }
        }
    }
    //numLocations should be smaller if there was DOUBLE BACK or HIDE
    *numLocations = finalSize - count;
    //allocate memory for new array
    LocationID *realConnect = malloc(sizeof(LocationID) * (*numLocations));
   
    //if connectedPlaces[j] = -2 then dont include in the new array
    int placesCount = 0, j = 0;
    while(placesCount < finalSize){
        if(connectedPlaces[j] != -2){
            realConnect[placesCount] = connectedPlaces[j];
            placesCount++;
        }
        j++;
    }


    return realConnect;
    
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    Round roundNum = giveMeTheRound(currentView);
    LocationID whereIsPlayer = whereIs(currentView, player);
    
    if(whereIsPlayer == PLAYER_DRACULA){
        int *connectedPlaces = whereCanIgo(currentView, numLocations, road, sea);
        return connectedPlaces;
    }else{
        int *connectedPlaces = connectedLocations(currentView -> gv,numLocations, whereIsPlayer,
                                                  player, roundNum, road, rail, sea);
        
        return connectedPlaces;
    }
}

