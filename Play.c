// Functions to manipulate the Play type.

#include <stdlib.h>
#include <string.h>
#include "Play.h"

#define TRUE 1
#define FALSE 0

#define PLAY_SIZE sizeof(char) * LEN_PLAY

Play newPlay(char * p)
{
   int i;
   Play constructedPlay = malloc(PLAY_SIZE);
   for (i = 0; i < LEN_PLAY; i++){
      if (p[i] == '\0') return NULL; 
      constructedPlay[i] = p[i];
   }
   return constructedPlay;
}

// Make a copy of the given Play.
Play copyPlay(Play p)
{
   Play newPlay = malloc(PLAY_SIZE);
   int i;
   for (i = 0; i < LEN_PLAY; i++){
      newPlay[i] = p[i];
   }
   return newPlay;
}

int checkSanity(Play p)
{
   if (p == NULL)
      return FALSE;
   else if (strlen(p) != LEN_PLAY)
      return FALSE;
   else
      return TRUE;
}

void destroyPlay(Play p) {
   free(p);
}

