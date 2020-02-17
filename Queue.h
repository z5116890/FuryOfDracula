// A custom made Queue class used to represent the trail
// for the Fury of Dracula game.

#include "Play.h"

#define SUCCESS 1
#define FAILURE 0

typedef struct _queueRep * Queue;

Queue newQueue();
void disposeQueue(Queue q);

// Add an item to the queue. Returns SUCCESS when successful
// and FAILURE when fails.
int addToQueue(Queue q, Play p);

// Returns a copy of the Play type and drops it from
// the queue;
Play leaveQueue(Queue q);

// Returns the number of items in the queue
// at that particular time.
int numItems(Queue q);

// Returns a copy of the item at a particular location.
// Zero indexed, from the fromt.
Play getElementAt(Queue q, int location);