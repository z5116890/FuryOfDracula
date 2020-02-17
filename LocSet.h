// A set to store the LocationID of places we've seen.

#include "Places.h"

typedef struct _setRep * Set;

Set newSet();
void insertIntoSet(Set, LocationID);
LocationID getAndRemoveSomeID(Set);
int getNumItems(Set);
void destroySet(Set);
void showSet(Set);
