#ifndef __LIST__
#define __LIST__

#include <stdlib.h>

struct list{
	int first, second; //second = neighbor//first = weight
	struct list *prox;
};

void insere_list(struct list **l, int v, int w);
void clear_list(struct list *l);
void ini_list(struct list *l[], int n);
#endif
