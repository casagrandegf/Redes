#include "list.h"

void insere_list(struct list **l, int v, int w) {
	struct list *new = (struct list *) malloc(sizeof(struct list));
	new->first = w;
	new->second = v;
	new->prox = *l;
	*l = new;
}

void clear_list(struct list *l) {
	if (l == NULL) return;
	if (l->prox != NULL) clear_list(l->prox);
	free(l);
}

void ini_list(struct list *l[], int n) {
	int i;
	for (i = 0; i < n; i++)
		l[i] = NULL;
}
