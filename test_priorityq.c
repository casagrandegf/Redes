#include <stdio.h>
#include "priorityq.h"

int main () {
  // push(3, 4);
  // push(4, 3);
  // push(2, 6);
  struct pair heap[123];
  heap[0].size = 1;
  // push(heap, 1, 5);
  // printf("%d %d\n", heap[1].first, heap[1].second);
  // struct pair teste;
  // new(&teste);

  // teste.push(&teste, 4, 5);

  // printf("%d %d\n", teste.first, teste.second);
  //
  //
  int i, j;
  for (i = 10, j = 12; i > 0; i--, j += 5)
    push(heap, i + j, i);
  //
  struct pair b;
  for (i = 0; i < 10; i++)
    b = top(heap),
    printf("%d %d\n", b.first, b.second),
    pop(heap);

  return 0;
}
