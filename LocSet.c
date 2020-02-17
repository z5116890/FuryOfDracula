// Implementation of a Set of LocationID

#include <stdlib.h>
#include <stdio.h>
#include "LocSet.h"

typedef struct _qNode * QNode;

typedef struct _qNode {
   LocationID current;
   QNode next;
} qNode;

typedef struct _setRep {
   int numItems;
   QNode start;
   QNode end;
}SetRep;

static void destroyNode(QNode n);

Set newSet()
{
   Set aSet = malloc(sizeof(SetRep));
   aSet->numItems = 0;
   aSet->start = NULL;
   aSet->end = NULL;
   return aSet;
}

void insertIntoSet(Set s, LocationID l)
{
   if (s->start == NULL){
      s->start = malloc(sizeof(qNode));
      s->end = s->start;
      s->start->current = l;
      s->start->next = NULL;   
   } else {
      QNode it = s->start; //it for iterator
      while (it != NULL) {
         if (it->current == l) return;
         it = it->next;
      }
      QNode temp = s->end;
      s->end = malloc(sizeof(qNode));
      s->end->next = NULL;
      s->end->current = l;
      temp->next = s->end;
   }
   s->numItems++;
}

LocationID getAndRemoveSomeID(Set s)
{
   QNode temp = s->start;
   s->start = s->start->next;
   s->numItems--;
   if (s->numItems == 0){
      s->start = NULL;
      s->end = NULL;
   }
   int returnValue = temp->current;
   destroyNode(temp);
   return returnValue;
}

int getNumItems(Set s)
{
   return s->numItems;
}

void destroySet(Set s)
{
   if (s == NULL)
      return;
   if (s->start != NULL){
      while (s->start != NULL){
         QNode temp = s->start->next;
         destroyNode(s->start);
         s->start = temp;
      }
   }
   free(s);
}

void showSet(Set s)
{
   QNode it = s->start;
   while (it != NULL){
      printf("%d->", it->current);
      it = it->next;   
   }
   printf("X\n");
}

static void destroyNode(QNode n)
{
   free(n);
}
