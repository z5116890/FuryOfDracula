// testGameView.c ... test the GameView ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "GameView.h"

int main()
{
    int i;
    GameView gv;
    
    printf("Test basic empty initialisation\n");
    PlayerMessage messages1[] = {};
    gv = newGameView("", messages1);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 0);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);
    
    printf("Test for Dracula trail and basic functions\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GST.... SAO.... HZU.... MBB.... DC?....", messages2);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeGameView(gv);
    
    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", messages3);
    assert(getLocation(gv,PLAYER_DRACULA) == GENEVA);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 5);
    assert(getHealth(gv,PLAYER_DRACULA) == 30);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == GENEVA);
    LocationID history[TRAIL_SIZE];
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[1] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);
    
    printf("Test for Dracula doubling back at sea, and losing blood points (Hunter View)\n");
    PlayerMessage messages4[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages4);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);
    
    printf("Test for Dracula doubling back at sea, and losing blood points (Drac View)\n");
    PlayerMessage messages5[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DEC.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages5);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == ENGLISH_CHANNEL);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);
    
    

    printf ("Test from pastPlays string 1\n");
    PlayerMessage messages6[] = {".."};
    gv = newGameView("GMN....", messages6);
    assert(getRound(gv) == 0);
    assert(getCurrentPlayer(gv) == PLAYER_DR_SEWARD);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getScore(gv) == GAME_START_SCORE);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 41);
    printf("dr seward is at %d\n, ", getLocation(gv, PLAYER_DR_SEWARD));
    assert(getLocation(gv, PLAYER_DR_SEWARD) == -1);
    
    assert(getLocation(gv, PLAYER_VAN_HELSING) == -1);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == -1);
    assert(getLocation(gv, PLAYER_DRACULA) == -1);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL....", messages6);
    assert(getRound(gv) == 0);
    assert(getCurrentPlayer(gv) == PLAYER_VAN_HELSING);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getScore(gv) == GAME_START_SCORE);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 41);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == -1);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == -1);
    assert(getLocation(gv, PLAYER_DRACULA) == -1);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM....", messages6);
    assert(getRound(gv) == 0);
    assert(getCurrentPlayer(gv) == PLAYER_MINA_HARKER);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getScore(gv) == GAME_START_SCORE);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 41);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 2);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == -1);
    assert(getLocation(gv, PLAYER_DRACULA) == -1);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA....", messages6);
    assert(getRound(gv) == 0);
    assert(getCurrentPlayer(gv) == PLAYER_DRACULA);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getScore(gv) == GAME_START_SCORE);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 41);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 2);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 50);
    assert(getLocation(gv, PLAYER_DRACULA) == -1);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V...", messages6);
    assert(getRound(gv) == 1);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 1);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 41);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 2);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 50);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. GLV....", messages6);
    assert(getRound(gv) == 1);
    assert(getCurrentPlayer(gv) == PLAYER_DR_SEWARD);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 1);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 38);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 2);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 50);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO....", messages6);
    assert(getRound(gv) == 1);
    assert(getCurrentPlayer(gv) == PLAYER_VAN_HELSING);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 1);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 38);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 2);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 50);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS....", messages6);
    assert(getRound(gv) == 1);
    assert(getCurrentPlayer(gv) == PLAYER_MINA_HARKER);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 1);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 38);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 48);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 50);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... ", messages6);
    assert(getRound(gv) == 1);
    assert(getCurrentPlayer(gv) == PLAYER_DRACULA);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 1);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 38);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 48);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 60);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T...", messages6);
    assert(getRound(gv) == 2);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 2);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 38);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 48);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 60);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR....", messages6);
    assert(getRound(gv) == 2);
    assert(getCurrentPlayer(gv) == PLAYER_DR_SEWARD);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 2);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 33);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 48);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 60);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL....", messages6);
    assert(getRound(gv) == 2);
    assert(getCurrentPlayer(gv) == PLAYER_VAN_HELSING);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 2);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 33);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 48);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 60);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO....", messages6);
    assert(getRound(gv) == 2);
    assert(getCurrentPlayer(gv) == PLAYER_MINA_HARKER);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 2);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 33);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 4);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 60);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... ", messages6);
    assert(getRound(gv) == 2);
    assert(getCurrentPlayer(gv) == PLAYER_DRACULA);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 2);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 33);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 4);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 70);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT...", messages6);
    assert(getRound(gv) == 3);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 3);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 33);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 4);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 70);
    assert(getLocation(gv, PLAYER_DRACULA) == 17);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW....", messages6);
    assert(getRound(gv) == 3);
    assert(getCurrentPlayer(gv) == PLAYER_DR_SEWARD);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 3);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 61);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 4);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 70);
    assert(getLocation(gv, PLAYER_DRACULA) == 17);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO....", messages6);
    assert(getRound(gv) == 3);
    assert(getCurrentPlayer(gv) == PLAYER_VAN_HELSING);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 3);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 61);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 4);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 70);
    assert(getLocation(gv, PLAYER_DRACULA) == 17);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO.... HNS....", messages6);
    assert(getRound(gv) == 3);
    assert(getCurrentPlayer(gv) == PLAYER_MINA_HARKER);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 3);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 61);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 48);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 70);
    assert(getLocation(gv, PLAYER_DRACULA) == 17);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO.... HNS.... MFR....", messages6);
    assert(getRound(gv) == 3);
    assert(getCurrentPlayer(gv) == PLAYER_DRACULA);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 3);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 61);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 48);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 25);
    assert(getLocation(gv, PLAYER_DRACULA) == 17);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO.... HNS.... MFR.... DC?T...", messages6);
    assert(getRound(gv) == 4);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 4);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 61);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 48);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 25);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO.... HNS.... MFR.... DC?T... "
                     "GLV....", messages6);
    assert(getRound(gv) == 4);
    assert(getCurrentPlayer(gv) == PLAYER_DR_SEWARD);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 4);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 38);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 48);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 25);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO.... HNS.... MFR.... DC?T... "
                     "GLV.... SPL....", messages6);
    assert(getRound(gv) == 4);
    assert(getCurrentPlayer(gv) == PLAYER_VAN_HELSING);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 4);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 38);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 48);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 25);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO.... HNS.... MFR.... DC?T... "
                     "GLV.... SPL.... HAO....", messages6);
    assert(getRound(gv) == 4);
    assert(getCurrentPlayer(gv) == PLAYER_MINA_HARKER);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 4);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 38);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 4);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 25);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO.... HNS.... MFR.... DC?T... "
                     "GLV.... SPL.... HAO.... MZU....", messages6);
    assert(getRound(gv) == 4);
    assert(getCurrentPlayer(gv) == PLAYER_DRACULA);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 4);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 38);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 4);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 70);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO.... HNS.... MFR.... DC?T... "
                     "GLV.... SPL.... HAO.... MZU.... DC?T...", messages6);
    assert(getRound(gv) == 5);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 5);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 38);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 4);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 70);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO.... HNS.... MFR.... DC?T... "
                     "GLV.... SPL.... HAO.... MZU.... DC?T... "
                     "GSW....", messages6);
    assert(getRound(gv) == 5);
    assert(getCurrentPlayer(gv) == PLAYER_DR_SEWARD);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 5);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 61);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 51);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 4);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 70);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO.... HNS.... MFR.... DC?T... "
                     "GLV.... SPL.... HAO.... MZU.... DC?T... "
                     "GSW.... SLO....", messages6);
    assert(getRound(gv) == 5);
    assert(getCurrentPlayer(gv) == PLAYER_VAN_HELSING);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 5);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 61);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 4);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 70);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO.... HNS.... MFR.... DC?T... "
                     "GLV.... SPL.... HAO.... MZU.... DC?T... "
                     "GSW.... SLO.... HNS....", messages6);
    assert(getRound(gv) == 5);
    assert(getCurrentPlayer(gv) == PLAYER_MINA_HARKER);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 5);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 61);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 48);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 70);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    gv = newGameView("GMN.... SPL.... HAM.... MPA.... DC?.V.. "
                     "GLV.... SLO.... HNS.... MST.... DC?T... "
                     "GIR.... SPL.... HAO.... MZU.... DCDT... "
                     "GSW.... SLO.... HNS.... MFR.... DC?T... "
                     "GLV.... SPL.... HAO.... MZU.... DC?T... "
                     "GSW.... SLO.... HNS.... MMR....", messages6);
    assert(getRound(gv) == 5);
    assert(getCurrentPlayer(gv) == PLAYER_DRACULA);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 10);
    assert(getScore(gv) == GAME_START_SCORE - 5);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 61);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 39);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 48);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 42);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    printf ("passed\n");
    
    printf ("Test from pastPlays string 2\n");
    gv = newGameView("GED.... SGE.... HZU.... MCA.... DCF.V.. "
                     "GMN.... SCFVD.. HGE.... MLS.... DBOT... "
                     "GLO.... SMR.... HCF.... MMA.... DC?T... "
                     "GPL.... SMS.... HMR.... MGR.... DBAT... "
                     "GLO.... SBATD.. HMS.... MMA.... DC?T... "
                     "GPL.... SJM.... HBA.... MGR.... DC?T... "
                     "GPL.... SSJ.... HBA.... MGR.... DC?T... ", messages6);
    assert(getRound(gv) == 7);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 20);
    assert(getScore(gv) == GAME_START_SCORE - 7 - 6);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == 51);
    assert(getLocation(gv, PLAYER_DR_SEWARD) == 57);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 5);
    assert(getLocation(gv, PLAYER_MINA_HARKER) == 30);
    assert(getLocation(gv, PLAYER_DRACULA) == 100);
    disposeGameView(gv);
    
    
    
    
    

    
    
    
    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;
    gv = newGameView("", messages1);
    printf("Checking Galatz road connections\n");
    edges = connectedLocations(gv,&size,GALATZ,PLAYER_LORD_GODALMING,0,1,0,0);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);
    printf("Checking Ionian Sea sea connections\n");
    edges = connectedLocations(gv,&size,IONIAN_SEA,PLAYER_LORD_GODALMING,0,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);
    printf("Checking Athens rail connections (none)\n");
    edges = connectedLocations(gv,&size,ATHENS,PLAYER_LORD_GODALMING,0,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);
    printf("passed\n");
    disposeGameView(gv);
    
    return 0;
}
