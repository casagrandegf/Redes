#include "dijkstra.h"


// Dist.first = weight dist.second = origin vertix
void dijkstra(int inicio, struct tplg topology, struct pair resp[]) {
  struct pair heap[MAX], dist[MAX];
  heap[0].size = 1;
  int i, u;

  // Initializes the distances with infinite
  for (i = 0; i <= topology.n; i++)
    dist[i].first = INF,
    dist[i].second = -1;
  (dist)[inicio].first = 0;
  dist[inicio].second = inicio;

  // Add the first node in the priority queue
  push(heap, 0, inicio);

  // Relaxation of the edges
  while(heap[0].size > 0) {
    struct pair front;
    front = top(heap);
    pop(heap);
    int d = front.first, u = front.second;
    if (d > dist[u].first) continue;

    // Scroll through the adjacency list updating the accumulated distances
    while(topology.adjList[u] != NULL) {
      if ((dist)[u].first + topology.adjList[u]->first < (dist)[topology.adjList[u]->second].first) {
        (dist)[topology.adjList[u]->second].first = (dist)[u].first + topology.adjList[u]->first;
        dist[topology.adjList[u]->second].second = u;//resalva
        push(heap, dist[topology.adjList[u]->second].first, topology.adjList[u]->second);
      }
      topology.adjList[u] = topology.adjList[u]->prox;
    }
  }

  // With backtraking on the vector of distances, creates a routing table
  for (u = 0; u <= topology.n; u++) {
    int i = u;
    if (i != inicio)
      while (dist[i].second != inicio && dist[i].second > -1)
        if (dist[i].second != inicio) i = dist[i].second;
      resp[u].second = i,
      resp[u].first = dist[u].first;

  }
  resp[inicio].first = 0;
  resp[inicio].second = inicio;

  // Clear the adjacency list of the graph
  for (i = 0; i <= topology.n; i++)
    clear_list(topology.adjList[i]);

  // for (i = 0; i <= topology.n; i++)
  //   printf("i: %d saida = %d dist = %d\n", i, resp[i].second, resp[i].first);
}
