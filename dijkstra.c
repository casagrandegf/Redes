#include "dijkstra.h"

void dijkstra(int inicio, struct tplg topology, int dist[]) {
  struct pair heap[MAX];
  heap[0].size = 1;
  int i;

  for (i = 0; i < MAX; i++) //inicializa distancias com infinito
    dist[i] = INF;
  (dist)[inicio] = 0;
  push(heap, 0, inicio); //primeiro no

  while(heap[0].size > 0) {
    struct pair front;
    front = top(heap);
    pop(heap);
    int d = front.first, u = front.second;
    if (d > dist[u]) continue;

    // //percorre a lista dos vizinhos
    while(topology.adjList[u] != NULL) {
      if ((dist)[u] + topology.adjList[u]->first < (dist)[topology.adjList[u]->second]) {
        (dist)[topology.adjList[u]->second] = (dist)[u] + topology.adjList[u]->first;
        push(heap, dist[topology.adjList[u]->second], topology.adjList[u]->second);
      }
      topology.adjList[u] = topology.adjList[u]->prox;
    }
  }
}
