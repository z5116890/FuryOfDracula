// Implementation for the Queue type.
// Author: Ricky Qu
// Date: 14 August 2016

#include <stdlib.h>
#include "Queue.h"

typedef struct _qNode * QNode;

typedef struct _qNode {
   Play current;
   QNode next;
} qNode;

typedef struct _queueRep {
   int numItems;
   QNode start;
   QNode end;
}queueRep;

static void destroyNode(QNode n);

// Allocates a new queue. It is up to the user to check for NULL.
Queue newQueue() 
{
   Queue q = malloc(sizeof(queueRep));
   q->start = NULL;
   q->end = NULL;
   q->numItems = 0;
   return q;
}

void disposeQueue(Queue q)
{
   if (q == NULL)
      return;
   if (q->start != NULL){
      while (q->start != NULL){
         QNode temp = q->start->next;
         destroyNode(q->start);
         q->start = temp;
      }
   }
   free(q);
}

// Failure is returned as FAILURE. 
int addToQueue(Queue q, Play p)
{
   QNode newNode = malloc(sizeof(qNode));
   if (newNode == NULL)
      return FAILURE;
   else if (p == NULL)
      return FAILURE;
   newNode->current = copyPlay(p);
   newNode->next = NULL;
   if (q->start == NULL)
      q->start = newNode;
   else
      q->end->next = newNode;
   q->end = newNode;
   q->numItems++;
   return SUCCESS;
}

Play leaveQueue(Queue q)
{
   QNode temp = q->start;
   q->start = temp->next;
   Play out = copyPlay(temp->current);
   destroyNode(temp);
   q->numItems--;
   return out;
}

int numItems(Queue q)
{
   return q->numItems;
}

Play getElementAt(Queue q, int location)
{
   if (location >= numItems(q) || location < 0)
      return NULL;
   int i;
   QNode temp = q->start;
   for (i = 0; i< location; i++)
      temp = temp->next;
   return copyPlay(temp->current);
}

static void destroyNode(QNode n)
{
   free(n->current);
   free(n);
}