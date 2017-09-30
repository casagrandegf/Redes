#ifndef __DIJKSTRA__
#define __DIJKSTRA__

#include <stdio.h>
#include "priorityq.h"
#include "list.h"

#define INF 1123465789
#define MAX 11234

//Structure with an adjacency list of the graph
struct tplg {
  struct list *adjList[MAX];
  int n;
};

void dijkstra(int inicio, struct tplg topology, struct pair dist[]) ;

#endif
