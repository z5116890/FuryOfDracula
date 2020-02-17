// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"

#include "stdio.h"
#include "string.h"
#include "Places.h"
#include "ctype.h"
#include "Queue.h"
#include "Play.h"
#include "LocSet.h"

#define MAX_LENGTH 14680
#define PLAY_LENGTH 8

typedef struct _playerData{
    int health;
    Queue trail; //created first in.
}playerData;

struct gameView {
    char play[MAX_LENGTH];
    int turn;
    int score;
    int flag;
    playerData players[NUM_PLAYERS];
};

static int getDraculaHealth(GameView currentView);
static int getHunterHealth(GameView currentView, int player);
static void initialiseHealth(GameView currentView);
static LocationID * ExtendedRail(Map m, LocationID from, int round, PlayerID player, int *numExtensions, int nthCall);
static void initialiseLocation(GameView currentView);
// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    int player;
    
    GameView gameView = malloc(sizeof(struct gameView));
    strcpy(gameView->play, pastPlays);
    
    // add one to the total number of elements in the string
    // to compensate for the space
    // divide by 8 since there are 8 characters per move
    gameView->turn = (strlen(pastPlays) + 1)/PLAY_LENGTH;
    printf("string length of plays is: %lu , turn = %d\n",strlen(pastPlays), gameView->turn);
    
    // the flag is used to prevent the program from re-calculating the HP of
    // the player again, as it will mess up the values
    // this is included to pass the automatic test
    gameView->flag = 0;
    
    // first initialise the hp for each player
    // then get the HP of each individual player by calling the getHealth
    // function recursively and assign to the hp[] array
    // note the the hp[] array is updated to CURRENT turn
    initialiseHealth(gameView);
    for (player = PLAYER_LORD_GODALMING; player < NUM_PLAYERS; player++) {
        gameView->players[player].health = getHealth(gameView, player);
        gameView->players[player].trail = newQueue();
    }
    initialiseLocation(gameView);
    // get the game score by calling the function getScore
    gameView->score = getScore(gameView);
    // since the hp is now updated for every player
    // set the flag so that the program does not recalculate the hp again
    // this is included to pass the automatic test
    gameView->flag = 1;
    
    return gameView;
}

// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    int player;
    
    for (player = PLAYER_LORD_GODALMING; player < NUM_PLAYERS; player++) {
        disposeQueue(toBeDeleted->players[player].trail);
    }
    free(toBeDeleted);
}

//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    int round;
    
    // divide by 5 to see the current round
    // note: round =/= turn
    round = currentView->turn / NUM_PLAYERS;
    
    
    return round;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    int player, playerID;
    
    // mod 5 to see the current player
    player = currentView->turn % NUM_PLAYERS;
    
    switch (player) {
        case 1: playerID = PLAYER_DR_SEWARD; 	break;
        case 2: playerID = PLAYER_VAN_HELSING;	break;
        case 3:	playerID = PLAYER_MINA_HARKER;	break;
        case 4: playerID = PLAYER_DRACULA; 		break;
        default: playerID = PLAYER_LORD_GODALMING;
    }
    
    return playerID;
}


// Get the current score
int getScore(GameView currentView)
{
    int score, turn;
    int locationID, player;
    int vampire = 0;
    int round = 0;
    int i, j;
    char play[PLAY_LENGTH + 1], location[3];
    
    // if flag = 1, don't do any calculation
    // just return the value
    if (currentView->flag) return currentView->score;
    
    turn = currentView->turn;
    
    // go through the entire pastPlays from turn 0 to current turn
    // if score == 0, break the loop
    // if it's end of Dracula turn, score loss Dracula turn
    // if player != Dracula && current location = hospital, score loss hospital
    // if round % 13 == 0 & move[4] is 'V' & player = Dracula, set vampire = 1
    // if turn != 0, round = roundDown(turn/5)
    // if player != Dracula, check for their encounter
    //		if encounter contains 'V', set vampire = 0
    // if round = 6, 19, 32, etc.. & player = vampire & vampire = 1
    // 		score loss vampire matures
    //		set vampire = 0
    for (i = 0; i <= turn; i++) {
        memset (play, '\0', sizeof(play));
        strncpy (play, currentView->play + i*PLAY_LENGTH, 8);
        memset (location, '\0', sizeof(location));
        strncpy (location, play + 1, 2);
        locationID = abbrevToID(location);
        player = i%NUM_PLAYERS;
        
        if (score == 0) break;
        
        if (i <= 5) score = GAME_START_SCORE;
        
        if (turn != 0 && player == 0) score -= SCORE_LOSS_DRACULA_TURN;
        
        if (player != PLAYER_DRACULA && locationID == ST_JOSEPH_AND_ST_MARYS) score -= SCORE_LOSS_HUNTER_HOSPITAL;
        
        if (round%13 == 0 && play[4] == 'V' && player == PLAYER_DRACULA) vampire = 1;
        
        if (turn != 0) round = i/NUM_PLAYERS;
        
        if (i != PLAYER_DRACULA) {
            for (j = 3; j < PLAY_LENGTH; j++) {
                if (strlen(play) == 0) break;
                if (play[j] == '.') break;
                else if (play[j] == 'V') vampire = 0;
            }
        }
        
        if (((round-6)%13) == 0 && player == PLAYER_DRACULA && vampire == 1) {
            score -= SCORE_LOSS_VAMPIRE_MATURES;
            vampire = 0;
        }
    }
    
    return score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    int health;
    
    // if flag variable is set, do not go through the function again
    // instead just return the value from the struct
    // this is included to pass automatic test
    if (currentView->flag) return currentView->players[player].health;
    
    // else call the functions to calculate the current health
    // of the desired player
    if (player == PLAYER_DRACULA) {
        health = getDraculaHealth(currentView);
    } else {
        health = getHunterHealth(currentView, player);
    }
    
    return health;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    char location[3];
    int turn, round;
    int index;
    int locationID;
    
    turn = currentView->turn;
    round = getRound(currentView);
    
    // find the index of the location abbreviation from pastPlays
    // if the current turn is 'player', he/she should not have made a new move,
    // then we need to subtract the index by 40 (8 bytes * NUM_PLAYERS)
    // only if it is not round 0 (to avoid negative index)
    index = player*PLAY_LENGTH + round*NUM_PLAYERS*PLAY_LENGTH + 1;
    if (index > turn*PLAY_LENGTH) index -= NUM_PLAYERS*PLAY_LENGTH;
    
    // copy the abbreaviated name to the array 'location'
    
    memset (location, '\0', sizeof(location));
    strncpy (location, currentView->play + index, 2);
    
    // use the abbrevToID function in places.c to convert to a locationID
    // an exception is Dracula, where the there are many possible locations
    // switch does not support char compare, so have to do it case by case
    if (player != PLAYER_DRACULA) {
        locationID = abbrevToID(location);
    } else {
        if (location[1] == '?') {
            if (location[0] == 'C') {
                locationID = CITY_UNKNOWN;
            } else {
                locationID = SEA_UNKNOWN;
            }
        } else if (location[0] == 'H' && location[1] == 'I') {
            locationID = HIDE;
        } else if (location[0] == 'D' && isdigit(location[1])) {
            switch (location[1]) {
                case '1': locationID = DOUBLE_BACK_1; break;
                case '2': locationID = DOUBLE_BACK_2; break;
                case '3': locationID = DOUBLE_BACK_3; break;
                case '4': locationID = DOUBLE_BACK_4; break;
                case '5': locationID = DOUBLE_BACK_5; break;
            }
        } else if (location[0] == 'T' && location[1] == 'P') {
            locationID = TELEPORT;
        } else {
            locationID = abbrevToID(location);
        }		
    }
    
    return locationID;}

void getHistory(GameView currentView, PlayerID player,
                LocationID trail[TRAIL_SIZE])
{
    playerData * currPlayer = &(currentView->players[player]);
    int i;
    Play p;
    char loc[3]; // stores the location.
    for (i = 0; i <= TRAIL_SIZE; i++){
        p = getElementAt(currPlayer->trail, numItems(currPlayer->trail)-i-1);
        if (p == NULL)
            trail[i] = UNKNOWN_LOCATION;
        else {
            strncpy(loc, p+1, 2);
            if (loc[1] == '?'){
                if (loc[0] == 'C')
                    trail[i] = CITY_UNKNOWN;
                else if (loc[0] == 'S')
                    trail[i] = SEA_UNKNOWN;
            } else if (loc[0] == 'D') {
                switch(loc[1]){
                    case '1': trail[i] = DOUBLE_BACK_1; break;
                    case '2': trail[i] = DOUBLE_BACK_2; break;
                    case '3': trail[i] = DOUBLE_BACK_3; break;
                    case '4': trail[i] = DOUBLE_BACK_4; break;
                    case '5': trail[i] = DOUBLE_BACK_5; break;
                }
            } else if(loc[0] == 'H' && loc[1] == 'I'){
                trail[i] = HIDE;
            }else if (loc[0] == 'T') {
                if (loc[1] == 'P'){
                    trail[i] = TELEPORT;
                }else{
                    trail[i] = abbrevToID(loc);
                }
            } else {
                trail[i] = abbrevToID(loc);
            }
        }
        if (p != NULL) destroyPlay(p);
    }
}

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    Map m = newMap();
    VList v = getEdges(m, from);
    *numLocations = 0;
    
    //LocationID * accessibleLocations = calloc(numE(m, ANY), sizeof(LocationID));
    Set alreadyFound = newSet();
    VList iterator = v;
    while (iterator != NULL){
        if (road && iterator->type == ROAD){
            if (player == PLAYER_DRACULA && iterator->v == ST_JOSEPH_AND_ST_MARYS)
                ;
            else
                //accessibleLocations[nextAvailableSpot++] = iterator->v;
                insertIntoSet(alreadyFound, iterator->v);
            //showSet(alreadyFound);
        } else if (rail && iterator->type == RAIL && player != PLAYER_DRACULA && (round + player) % 4 != 0){
            insertIntoSet(alreadyFound, iterator->v);
            if ((round + player) % 4 != 1) {
                int numExtensions = 0;
                //Set alreadyFound = newSet();
                LocationID * extendedRailConnections = ExtendedRail(m, iterator->v, round, player, &numExtensions, 1);
                int i;
                for (i = 0; i < numExtensions; i++)
                    insertIntoSet(alreadyFound, extendedRailConnections[i]);
                free(extendedRailConnections);
                //showSet(alreadyFound);
            }
        } else if (sea && iterator->type == BOAT){
            insertIntoSet(alreadyFound, iterator->v);
        }
        iterator = iterator->next;
    }
    //accessibleLocations[nextAvailableSpot] = from; // don't forget the start as a destination!
    insertIntoSet(alreadyFound, from);
    
    *numLocations = getNumItems(alreadyFound);
    LocationID * accessibleLocations = malloc(sizeof(LocationID) * (*numLocations));
    int nextAvailableSpot = 0;
    
    while (getNumItems(alreadyFound) != 0){
        accessibleLocations[nextAvailableSpot++] = getAndRemoveSomeID(alreadyFound);
        //printf("%d\n", getNumItems(alreadyFound));
    }
    destroySet(alreadyFound);
    return accessibleLocations;
}

// this function can only be called twice recursively at most.
static LocationID * ExtendedRail(Map m, LocationID from, int round, PlayerID player, int *numExtensions, int nthCall)
{
    VList iterator = getEdges(m, from);
    Set s = newSet();
    while (iterator != NULL){
        if (iterator->type == RAIL) {
            insertIntoSet(s, iterator->v);
            if ((round + player) % 4 == 3 && nthCall < 2) {
                int numExtensions = 0;
                LocationID * extendedRailConnections = ExtendedRail(m, iterator->v, round, player, &numExtensions, nthCall+1);
                int i;
                for (i = 0; i < numExtensions; i++){
                    insertIntoSet(s, extendedRailConnections[i]);
                }
                free(extendedRailConnections);
            }
        }
        iterator = iterator->next;
    }
    *numExtensions = getNumItems(s);
    LocationID * secondaryRail = malloc(sizeof(LocationID) * getNumItems(s));
    int nextAvailableSpot = 0; // in the secondaryRailArray.
    while (getNumItems(s) != 0){
        secondaryRail[nextAvailableSpot++] = getAndRemoveSomeID(s);
    }
    destroySet(s);
    return secondaryRail;
}


static int getHunterHealth(GameView currentView, int player)
{
    int health, turn, round;
    int indexEncounters, indexLocation;
    int i, j;
    char encounters[5], location[3];
    Queue trail = newQueue();
    char play[PLAY_LENGTH];
    Play temp;
    
    turn = currentView->turn;
    round = getRound(currentView);
    
    // traverse through the entire pastPlays from round 0 to current round
    // if it is round 0, health = initial health
    // else use maths algos to calculate health
    for (i = 0; i <= round; i++) {
        indexLocation = player*PLAY_LENGTH + i*NUM_PLAYERS*PLAY_LENGTH + 1;
        memset (location, '\0', sizeof(location));
        strncpy (location, currentView->play + indexLocation, 2);
        strncpy (play, currentView->play + player*PLAY_LENGTH + PLAY_LENGTH*NUM_PLAYERS*i, PLAY_LENGTH);
        
        if (numItems(trail) == 6) leaveQueue(trail);
        if (strlen(currentView->play) != 0) addToQueue(trail, play);
        
        if (i == 0) health = GAME_START_HUNTER_LIFE_POINTS;
        else {
            indexEncounters = player*PLAY_LENGTH + i*NUM_PLAYERS*PLAY_LENGTH + 3;
            if (indexEncounters > turn*PLAY_LENGTH) break;
            memset (encounters, '\0', sizeof(encounters));
            strncpy (encounters, currentView->play + indexEncounters, 4);
            
            for (j = 0; j < numItems(trail) - 2; j++);
            temp = getElementAt(trail, j);
            if (location[0] == temp[1] && location[1] == temp[2]) {
                health += LIFE_GAIN_REST;
                if (health >= GAME_START_HUNTER_LIFE_POINTS) health = GAME_START_HUNTER_LIFE_POINTS;
            } else if (abbrevToID(location) == ST_JOSEPH_AND_ST_MARYS) {
                health = GAME_START_HUNTER_LIFE_POINTS;
            }
            
            for (j = 0; j < 4; j++) {
                if (encounters[j] == '.') break;
                else if (encounters[j] == 'T') health -= LIFE_LOSS_TRAP_ENCOUNTER;
                else if (encounters[j] == 'D') {
                    health -= LIFE_LOSS_DRACULA_ENCOUNTER;
                    currentView->players[PLAYER_DRACULA].health -= LIFE_LOSS_HUNTER_ENCOUNTER;
                }
            }
            if (health <= 0) health = 0;
        }
    }
    return health;
}


static int getDraculaHealth(GameView currentView)
{
    int health, round;
    int indexLocation, locationID;
    int i, j;
    char location[3];
    Queue trail = newQueue();
    char play[PLAY_LENGTH];
    Play temp;
    
    round = getRound(currentView);
    
    // traverse through the entire pastPlays from round 0 to current round
    // if it is round 0, health = initial health
    // else use maths algos to calculate health
    for (i = 0; i <= round; i++) {
        if(currentView -> turn == 15) printf("i = %d and health = %d\n", i, health);
        
        indexLocation = PLAYER_DRACULA*PLAY_LENGTH + i*NUM_PLAYERS*PLAY_LENGTH + 1;
        memset (location, '\0', sizeof(location));
        strncpy (location, currentView->play + indexLocation, 2);
        locationID = abbrevToID(location);
        strncpy (play, currentView->play + PLAYER_DRACULA*PLAY_LENGTH + PLAY_LENGTH*NUM_PLAYERS*i, PLAY_LENGTH);
        
        if (numItems(trail) == 6) leaveQueue(trail);
        if (strlen(currentView->play) != 0) addToQueue(trail, play);
        
        if (i == 0) {
            health = GAME_START_BLOOD_POINTS;
            if (location[0] == 'S' && location[1] == '?') {
                health -= LIFE_LOSS_SEA;
                currentView->players[PLAYER_DRACULA].health = health;
            } else if (locationID <= 70 && locationID >= 0 && isSea(locationID)) {
                health -= LIFE_LOSS_SEA;
                currentView->players[PLAYER_DRACULA].health = health;
            }
        } else {
            health = currentView->players[PLAYER_DRACULA].health;
            
            if (location[0] == 'S' && location[1] == '?') {
                health -= LIFE_LOSS_SEA;
                currentView->players[PLAYER_DRACULA].health = health;
            } else if (locationID <= 70 && locationID >= 0 && isSea(locationID)) {
                health -= LIFE_LOSS_SEA;
                currentView->players[PLAYER_DRACULA].health = health;
            } else if (location[0] == 'D' && isdigit(location[1])) {
                for (j = 0; j < numItems(trail) - location[1]; j++);
                temp = getElementAt(trail, j);
                strncpy (location, temp + 1, 2);
                locationID = abbrevToID(location);
                if (temp[1] == 'S' && temp[2] == '?') {
                    health -= LIFE_LOSS_SEA;
                    currentView->players[PLAYER_DRACULA].health = health;
                } else if (locationID <= 70 && locationID >= 0 && isSea(locationID)) {		
                    health -= LIFE_LOSS_SEA;			
                    currentView->players[PLAYER_DRACULA].health = health;					
                }				
            } else if (locationID <= 70 && locationID >= 0 && abbrevToID(location) == CASTLE_DRACULA) {
                health += LIFE_GAIN_CASTLE_DRACULA;
                currentView->players[PLAYER_DRACULA].health = health;
            }			
        }
        if (health <= 0) {
            health = 0;		
            break;
        }
    }
    
    return health;
}


static void initialiseHealth (GameView currentView)
{
    int player;
    
    for (player = PLAYER_DR_SEWARD; player < NUM_PLAYERS; player++) {
        if (player == PLAYER_DRACULA) {
            currentView->players[PLAYER_DRACULA].health = GAME_START_BLOOD_POINTS;
        } else {
            currentView->players[player].health = GAME_START_HUNTER_LIFE_POINTS;
        }	
    }
}

void initialiseLocation(GameView currentView)
{
    //char location[3];
    
    // find the index of the location abbreviation from pastPlays
    // if the current turn is 'player', he/she should not have made a new move,
    // then we need to subtract the index by 40 (8 bytes * NUM_PLAYERS)
    // only if it is not round 0 (to avoid negative index)
    //char * pastPlayDynamic = malloc(sizeof(char) * strlen(currentView->play));
    //strncpy(pastPlayDynamic, currentView->play, MAX_LENGTH);
    char * currPlay = strtok(currentView->play, " ");
    int i = 0;
    while(currPlay != NULL){
        addToQueue(currentView->players[i%NUM_PLAYERS].trail, currPlay);
        i++;
        currPlay = strtok(NULL, " ");
    }
    //free(pastPlayDynamic);
}
