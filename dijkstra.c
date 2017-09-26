#include "dijkstra.h"


//dist.first = weight dist.second = origin vertix
void dijkstra(int inicio, struct tplg topology, struct pair resp[]) {
  struct pair heap[MAX], dist[MAX];
  heap[0].size = 1;
  int i, u;

  for (i = 0; i < MAX; i++) //inicializa distancias com infinito
    dist[i].first = INF,
    dist[i].second = -1;
  (dist)[inicio].first = 0;
  dist[inicio].second = inicio;

  push(heap, 0, inicio); //primeiro no

  while(heap[0].size > 0) {
    struct pair front;
    front = top(heap);
    pop(heap);
    int d = front.first, u = front.second;
    if (d > dist[u].first) continue;

    // //percorre a lista dos vizinhos atualizando distancias acumuladas
    while(topology.adjList[u] != NULL) {
      if ((dist)[u].first + topology.adjList[u]->first < (dist)[topology.adjList[u]->second].first) {
        (dist)[topology.adjList[u]->second].first = (dist)[u].first + topology.adjList[u]->first;
        dist[topology.adjList[u]->second].second = u;//resalva
        push(heap, dist[topology.adjList[u]->second].first, topology.adjList[u]->second);
      }
      topology.adjList[u] = topology.adjList[u]->prox;
    }
  }

  //Ajusta tabela de roteamento
  for (u = 0; u < topology.n; u++) {
    int i = u;
    if (i != inicio)
      while (dist[i].second != inicio && dist[i].second > -1)
        if (dist[i].second != inicio) i = dist[i].second;
      resp[u].second = i,
      resp[u].first = dist[u].first;
 
  }
  resp[inicio].first = 0;
  resp[inicio].second = inicio;
  // for (i = 0; i < topology.n; i++)
  //   printf("i: %d saida = %d dist = %d\n", i, resp[i].second, resp[i].first);
}
