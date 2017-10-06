#include "priorityq.h"

// Add a element in the pair
void Push(struct pair *d, int a, int b) {
  (*d).first = a;
  (*d).second = b;
}
// Add a element in the pair
void new(struct pair *d) {
  (*d).push = Push;
}

// Swap two values of position
void swap(struct pair *a, struct pair *b) {
  (*a).first ^= (*b).first;
  (*b).first ^= (*a).first;
  (*a).first ^= (*b).first;

  (*a).second ^= (*b).second;
  (*b).second ^= (*a).second;
  (*a).second ^= (*b).second;

}

// While the child's value is less than dad, swap father with child and rises
void editUp(struct pair *heap[], int i) {
  for (; i > 1 && (*heap)[i].first < (*heap)[PAI(i)].first; i = PAI(i))
  swap(&(*heap)[i], &(*heap)[PAI(i)]);

}
void editDown(struct pair heap[]) {//heapify(index)
  // Repeat while the index still has child
  int i = 1;
  while (ESQ(i) < heap[0].size)
  {
    // Select the child with lowest value (left or right)
    int filho = ESQ(i);
    if (DIR(i) < heap[0].size && (heap)[DIR(i)].first < (heap)[ESQ(i)].first)
    filho = DIR(i);

    // If the father's value is less than the value of child, it's done
    if ((heap)[i].first < (heap)[filho].first)
    break;

    // Else, swap father with the child
    swap(&(heap)[i], &(heap)[filho]);
    i = filho;
  }
}

// Add an element in the priority queue
void push(struct pair heap[], int i, int j) {
  (heap)[heap[0].size].first = i;
  (heap)[heap[0].size].second = j;
  editUp(&(heap), heap[0].size);
  heap[0].size++;
}

// Removes an element in the priority queue
void pop(struct pair heap[]) {
  if (heap[0].size > 0) {
    heap[0].size--;
    swap(&(heap)[1], &(heap)[heap[0].size]);
    editDown((heap));
  }
}
// Returns the lowest element in the priority queue
struct pair top(struct pair d[]) {
  return d[1];
}
