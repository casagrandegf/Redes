/*
Simple udp client
*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

//threads
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

// #include "list.h"
#include "dijkstra.h"

// #define MAX 1123
#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data
/* You can compile this program with:
 * gcc -Wall -D_REENTRANT -o thread thread.c<\n>
 * -lpthread */
/* We always need to include this header file for<\n>
 * the threads */
struct roteamento {
 int port;
 char ip[20];
};
// struct tplg {
//   struct list *adjList[MAX];
//   int n;
// };
void *client(struct roteamento *rotConf);
void *server(char *port);
// pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;


void getEnl(struct tplg *topology) {
  FILE *fp;
  int u, v, w;

  // memset((*topology).adjList, 0, sizeof((*topology).vet));
  (*topology).n = 0;

  fp = fopen("enlaces.config", "r");
  while (fscanf(fp, "%d %d %d", &u, &v, &w) != EOF)
    insere_list(&(*topology).adjList[u], v, w),
    insere_list(&(*topology).adjList[v], u, w),
    (*topology).n++;
    // (*topology).vet[u][v] = w,
    // (*topology).vet[v][u] = w,
  fclose(fp);
}

void getRot(struct roteamento rotConf[]) {
  FILE *fp;
  int a,b;
  char c[15];

  fp = fopen("roteador.config", "r");
  while(fscanf(fp, "%d %d %s", &a, &b, c) != EOF)
    (rotConf)[a].port = b, strcpy((rotConf)[a].ip, c);

  fclose(fp);
}

int main(int argc, char *argv[]) {
  int id_no = atoi(argv[1]);
  // printf("AQUI\n");
  struct roteamento rotConf[100];
  struct tplg topology;
  int dist[MAX];

  ini_list(topology.adjList, MAX); //zera vetor de listas

  getRot(rotConf);
  getEnl(&topology);
  dijkstra(id_no, topology, dist);

// #define TOPOLOGY
#ifdef TOPOLOGY
  int i, j;
  // for (i = 0; i < topology.n; i++) {
  //   for (j = 0; j < topology.n; j++)
  //     printf("%d ", topology.vet[i][j]);
  //   printf("\n");
  // }
  for (i = 0; i < 10; i++) {
  		while(topology.adjList[i] != NULL)
  			printf("weight: %d neighbor: %d\n", topology.adjList[i]->first, topology.adjList[i]->second),
  			topology.adjList[i] = topology.adjList[i]->prox;
  		printf("\n");
  	}
#endif
// #define ROTCONF
#ifdef ROTCONF
  int i;
  for (i = 1; i < 7; i++)
    printf("%d das %s\n", rotConf[i].port, rotConf[i].ip);
#endif

#define DIST
#ifdef DIST
  int i;
  for (i = 0; i < topology.n; i++)
    printf("%d\n", dist[i]);
#endif


  // printf("%d\n", id_no);
  pthread_t tids[2];
  pthread_create(&tids[0], NULL, client, rotConf);
  pthread_create(&tids[1], NULL, server, rotConf[id_no].port);
  //

  //  for(i=0; i<2; i++) {
  pthread_join(tids[0], NULL);
  pthread_join(tids[1], NULL);
  printf("Threads returned\n");
  //  }
  return 0;
}






void die(char *s)
{
    perror(s);
    exit(1);
}

void *client(struct roteamento rotConf[])
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    // si_other.sin_port = htons(PORT);
    //
    // if (inet_aton(SERVER , &si_other.sin_addr) == 0) //address to number
    // {
    //     fprintf(stderr, "inet_aton() failed\n");
    //     exit(1);
    // }

    while(1)
    {

        printf("Client>> Enter message : ");
        //gets(message);
        int dest;
        scanf("%d %s", &dest, message);
				// puts('\n');
				printf("\n");
        si_other.sin_port = htons(rotConf[dest].port);
        if (inet_aton(rotConf[dest].ip , &si_other.sin_addr) == 0) //address to number
          fprintf(stderr, "inet_aton() failed\n"),
          exit(1);

        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
            die("sendto()");


        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
          die("recvfrom()");


				if (!strcmp(buf, "exit")) close(s), pthread_exit(NULL);
        // puts(buf);
    }

    close(s);
		pthread_exit(NULL);
}

void* server(char *port)
{
    struct sockaddr_in si_me, si_other;

    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }

    //keep listening for data
    while(1)
    {
        // printf("Waiting for data...");
        fflush(stdout);
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);

        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }

        //print details of the client/peer and the data received
        printf("Server>> Received packet from %s:%d\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Server>> Data: %s\n\n" , buf);

        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }

    close(s);
		pthread_exit(NULL);
}
