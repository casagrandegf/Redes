#ifndef __PRIORITYQ__
#define __PRIORITYQ__

#include <stdio.h>

struct pair{
  int first, second, size;
  void (* push) (struct pair *d, int a, int b);
};

#define ESQ(i) (i << 1)
#define DIR(i) ((i << 1) | 1)
#define PAI(i) (i >> 1)


void Push(struct pair *d, int a, int b);
void new(struct pair *d);
void swap(struct pair *a, struct pair *b);
void editUp(struct pair *heap[], int i);
void editDown(struct pair heap[]);
void push(struct pair heap[], int i, int j);
void pop(struct pair heap[]);
struct pair top(struct pair d[]);

#endif
