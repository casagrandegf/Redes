#include "priorityq.h"

void Push(struct pair *d, int a, int b) {
  (*d).first = a;
  (*d).second = b;
}

void new(struct pair *d) {
  (*d).push = Push;
}

void swap(struct pair *a, struct pair *b) {
 (*a).first ^= (*b).first;
 (*b).first ^= (*a).first;
 (*a).first ^= (*b).first;

 (*a).second ^= (*b).second;
 (*b).second ^= (*a).second;
 (*a).second ^= (*b).second;

}
void corrigeSubindo(struct pair *heap[], int i) {
 // Enquanto o valor do filho for menor do que o valor do pai, troca o pai com o filho e sobe
 for (; i > 1 && (*heap)[i].first < (*heap)[PAI(i)].first; i = PAI(i))
 swap(&(*heap)[i], &(*heap)[PAI(i)]);

}
void corrigeDescendo(struct pair heap[]) {// também conhecida como heapify(index)
 // Repete enquanto index ainda tiver filho
 int i = 1;
 while (ESQ(i) < heap[0].size)
 {
	 // Seleciona o filho com menor valor (esquerda ou direita?)
	 int filho = ESQ(i);
	 if (DIR(i) < heap[0].size && (heap)[DIR(i)].first < (heap)[ESQ(i)].first)
	 filho = DIR(i);

	 // Se o valor do pai é menor do que o valor do menor filho, terminamos
	 if ((heap)[i].first < (heap)[filho].first)
	 break;

	 // Caso contrário, trocamos o pai com o filho no heap e corrigimos agora para o filho
	 swap(&(heap)[i], &(heap)[filho]);
	 i = filho;
 }
}
void push(struct pair heap[], int i, int j) {
 // printf("size %d\n", size);
 (heap)[heap[0].size].first = i;
 (heap)[heap[0].size].second = j;
 corrigeSubindo(&(heap), heap[0].size);
 heap[0].size++;
}

void pop(struct pair heap[]) {
 if (heap[0].size > 0) {
   heap[0].size--;
	//  printf("pop %d %d\n", heap[1].first, heap[1].second);
	 swap(&(heap)[1], &(heap)[heap[0].size]);
	 corrigeDescendo((heap));
 }
}

struct pair top(struct pair d[]) {
  return d[1];
}
