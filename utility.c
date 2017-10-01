#include "utility.h"


//Reads a file with the configuration of the links
void getEnl(struct tplg *topology) {
  FILE *fp;
  int u, v, w;
    // (*topology).vet[v][u] = w,

  (*topology).n = 0;

  fp = fopen("enlaces.config", "r");
  while (fscanf(fp, "%d %d %d", &u, &v, &w) != EOF)
    insere_list(&(*topology).adjList[u], v, w),
    insere_list(&(*topology).adjList[v], u, w),
    (*topology).n = max(max(u, v), (*topology).n);
    // printf("max %d\n", (*topology).n);
  fclose(fp);
}

//Reads a file with the graph
void getRot(struct conf rotConf[]) {
  FILE *fp;
  int a,b;
  char c[15];

  fp = fopen("roteador.config", "r");
  while(fscanf(fp, "%d %d %s", &a, &b, c) != EOF)
    (rotConf)[a].port = b, strcpy((rotConf)[a].ip, c);

  fclose(fp);
}
