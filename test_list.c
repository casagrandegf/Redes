#include <stdio.h>
#include "list.h"

int main () {
	struct list *l[1123];
	// insere_list(&l, 1, 4);
	// printf("%d %d\n", l->first, l->second);
	// insere_list(&l, 3, 6);
	// insere_list(&l, 2, 5);
	int i, j;

	ini_list(l, 10);
	//
	for (i = 0; i < 10; i++)
		for (j = 0; j < 4; j++)
			insere_list(&l[i], i, j);
	// for (i = 0; i < 10; i++) {
	// 	while(l[i] != NULL)
	// 		printf("%d %d\n", l[i]->first, l[i]->second),
	// 		l[i] = l[i]->prox;
	// 	printf("\n");
	//
	// }

	// struct list *l = NULL;
	// int i;
	// for (i = 0; i < 10; i++)
		// insere_list(&l, i, i);

	for (i = 0; i < 10; i++)
		clear_list(l[i]),
		printf("sdf\n");



	return 0;
}
