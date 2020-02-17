// hunter.c
// Implementation of your "Fury of Dracula" hunter AI
// Written by Joshua Cahill 5/10/2016
// This Hunter AI uses the spread out and attack randomly method



#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Game.h"
#include "HunterView.h"

int firstTurn(int currentHunter);
int checkNoHunterThere(HunterView gameState, int locationToCheck);
int isThereTrailMatch(int location, LocationID *trail, int trailSize);
int isSeaLocation(int location);
int findDrac(HunterView gameState);


void decideHunterMove(HunterView gameState){
    
    // set up variables
    int IDtoSend = -1;
    int currentHunter = whoAmI(gameState);
    
    
    //send each hunter to a different location if it's their first turn
    if(whereIs(gameState, currentHunter) == UNKNOWN_LOCATION){
        IDtoSend = firstTurn(currentHunter);
        registerBestPlay(idToAbbrev(IDtoSend), "This is my first move!");
        return;
    }
    //Josh's strategy (in order of importance)
    //Try and go to locations in dracs trail starting from those earlist in the trail (considers cross referencing of possible hunter locations with dracs)
    //If you are on three health points or less, then rest
    //Spread out around Europe by picking a city that the hunter hasn't already visited (e.i. not in the hunters trail)
    //if there are no nice spots then just go to the most recent otherwise go the nice spot we chose
    IDtoSend = findDrac(gameState);
    
    
    registerBestPlay(idToAbbrev(IDtoSend), "Coming");
    return;
}



// if there is a hunter already there returns 1
int checkNoHunterThere(HunterView gameState, int locationToCheck){
    int x;
    for(x = 0; x < 4; x++){
        if(whereIs(gameState, x) == locationToCheck) return 1;
    }
    return 0;
}

// if there is a match with the location and trail returns 1
int isThereTrailMatch(int location, LocationID *trail, int trailSize){
    int x;
    for(x = 0; x < trailSize; x++){
        if(trail[x] == location) return 1;
    }
    return 0;
}

// returns the location to go when its the hunters first turn
int firstTurn(int currentHunter){
    int sendID;
    switch(currentHunter){
        case 0 : sendID = 39; break;
        case 1 : sendID = 40; break;
        case 2 : sendID = 58; break;
        case 3 : sendID = 60; break;
    }
    return sendID;
}

// if the location is a sea it returns 1
int isSeaLocation(int location){
    if(location == 0 || location == 10 || location == 32 || location == 33 || location == 43 || location == 48 || location == 61 || location == 64) return 1;
    return 0;
}



//STRATEGIES
int findDrac(HunterView gameState){
    
    int sendID = -1;
    int currentHunter = whoAmI(gameState);
    LocationID trail[TRAIL_SIZE];
    giveMeTheTrail(gameState, currentHunter, trail);
    int originalSizeOfPossibleLocations;
    int *possibleLocations = whereCanIgo(gameState, &originalSizeOfPossibleLocations, 1, 1, 1);
    const int sizeOfPossibleLocations = originalSizeOfPossibleLocations;
    
    
    LocationID dracTrail[TRAIL_SIZE];
    giveMeTheTrail(gameState, PLAYER_DRACULA, dracTrail);


    
    // if we can go to a city in Dracula's trail, go to it
    int x;
    for(x = 0; x < TRAIL_SIZE; x++){
        if(isThereTrailMatch(dracTrail[x], possibleLocations, sizeOfPossibleLocations)){
            // if we know where dracula was before find all the spots that he could be know and see if any of those match up with our potential moves
            if(x == 1) {
                printf("Locked!\n");
                int currentRound = giveMeTheRound(gameState);
                int numDracsPotentialMoves;
                // check where drac could have gone not including rail or sea
                LocationID *dracsPotentialMoves = whereCouldTheyHaveGone(gameState, &numDracsPotentialMoves, PLAYER_DRACULA, 1, 0, 0, currentRound - x, dracTrail[x]);
                
                // perform a comparison between my potential locations and draculas potential locations and pick one
                int x;
                for(x = 0; x < sizeOfPossibleLocations; x++){
                    if(isThereTrailMatch(possibleLocations[x], dracsPotentialMoves, numDracsPotentialMoves)){
                        sendID = possibleLocations[x];
                        printf("Match! %s\n", idToName(sendID));
                        // make sure we aren't going to the same city as where he was
                        if(sendID != dracTrail[1]){
                            return sendID;
                        }
                    }
                }
            }else{
                if(x == 0) printf("Smacked!\n");
                sendID = dracTrail[x];
            }
            return sendID;
        }
    }
    
    // if we are on 4 health points or less then rest
    if(howHealthyIs(gameState, currentHunter) <= 4){
        return whereIs(gameState, currentHunter);
    }

    
    // otherwise get a list of connected locations and go to one that you have not yet visited by looking at the trail
    x = 0;
    // let x be the first spot in the possibleLocations array which is valid
    while((isThereTrailMatch(possibleLocations[x], trail, TRAIL_SIZE) || checkNoHunterThere(gameState, possibleLocations[x]) || isSeaLocation(possibleLocations[x])) && x < sizeOfPossibleLocations){
        x++;
    }
    
    
    // if there are no nice spots then just go to the most recent otherwise go the nice spot we chose
    if(x == sizeOfPossibleLocations){
        sendID = possibleLocations[x-1];
        printf("Had to return!\n");
    }else{
        sendID = possibleLocations[x];
    }
    
    return sendID;

}

