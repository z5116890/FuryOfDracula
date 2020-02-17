//TODO


// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "string.h"
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Queue.h"
#include "LocSet.h"

// #include "Map.h" ... if you decide to use the Map ADT

struct hunterView {
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView currentGameView;
};


// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    HunterView hunterView = malloc(sizeof(struct hunterView));
    hunterView->currentGameView = newGameView(pastPlays, messages);
    return hunterView;
}


// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    //dispose the gameView that was created
    disposeGameView(toBeDeleted->currentGameView);
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int round = getRound(currentView->currentGameView);
    return round;
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    // call the function that returns what round it is. This is yet to be created in Game.c
    PlayerID currPlayer = getCurrentPlayer(currentView->currentGameView);
    return currPlayer;
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int score = getScore(currentView->currentGameView);
    return score;
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    // assuming the hunters can see Dracula's health
    int health = getHealth(currentView->currentGameView, player);
    return health;
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int location = getLocation(currentView->currentGameView, player);
    return location;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player, LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    // pass the array into the trail generator in GameView
    getHistory(currentView->currentGameView, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    // find the current position of the player
    Round currentRound = giveMeTheRound(currentView);
    PlayerID currentHunter = whoAmI(currentView);
    LocationID currentHunterPosition = whereIs(currentView, currentHunter);
    LocationID *possibleLocations = connectedLocations(currentView->currentGameView, numLocations, currentHunterPosition, currentHunter, currentRound, road, rail, sea);
    return possibleLocations;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations, PlayerID player, int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    // assert that the player isn't Dracula
    assert(player != PLAYER_DRACULA);
    Round currentRound = giveMeTheRound(currentView);
    LocationID currentHunterPosition = whereIs(currentView, player);
    LocationID *possibleLocations = connectedLocations(currentView->currentGameView, numLocations, currentHunterPosition, player, currentRound, road, rail, sea);
    return possibleLocations;
}

// Joshua's extra code
LocationID *whereCouldTheyHaveGone(HunterView currentView, int *numLocations, PlayerID player, int road, int rail, int sea, int roundNum, LocationID position){
    LocationID *possibleLocations = connectedLocations(currentView->currentGameView, numLocations, position, player, roundNum, road, rail, sea);
    return possibleLocations;
}
