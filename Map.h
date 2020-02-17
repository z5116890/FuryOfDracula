// Map.h ... interface to Map data type

#ifndef MAP_H
#define MAP_H

#include "Places.h"

typedef struct vNode *VList;

struct vNode {
    LocationID  v;    // ALICANTE, etc
    TransportID type; // ROAD, RAIL, BOAT
    VList       next; // link to next node
};

typedef struct edge{
    LocationID  start;
    LocationID  end;
    TransportID type;
} Edge;

// graph representation is hidden
typedef struct MapRep *Map;

// operations on graphs
Map  newMap();
void disposeMap(Map g);
void showMap(Map g);
int  numV(Map g);
int  numE(Map g, TransportID t);
VList getEdges(Map g, LocationID from);
int numConnections(VList v);

#endif
