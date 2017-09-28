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

#include "utility.h"
// #include "dijkstra.h"

// #define MAX 1123
#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data
/* You can compile this program with:
 * gcc -Wall -D_REENTRANT -o thread thread.c<\n>
 * -lpthread */
/* We always need to include this header file for<\n>
 * the threads */


int main(int argc, char *argv[]) {
  int i;
  struct roteamento rotConf;
  struct tplg topology;
  // struct pair dist[MAX];


  rotConf.id_no = atoi(argv[1]);
  ini_list(topology.adjList, MAX); //zera vetor de listas

  getRot(rotConf.conf);
  getEnl(&topology);
  dijkstra(rotConf.id_no, topology, rotConf.tab);

  //limpa lista de adjacencias
  for (i = 0; i <= topology.n; i++)
    clear_list(topology.adjList[i]);


// #define TOPOLOGY
#ifdef TOPOLOGY
  int i, j;
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

// #define DIST
#ifdef DIST
  int i;
  for (i = 0; i <= topology.n; i++)
    printf("i: %d saida = %d dist = %d\n", i, dist[i].second, dist[i].first);
#endif


  pthread_t tids[2];
  pthread_create(&tids[0], NULL, (void *)client, &rotConf);
  pthread_create(&tids[1], NULL, (void *)server, &rotConf);
  //
  // //
  // // //  for(i=0; i<2; i++) {
  pthread_join(tids[0], NULL);
  pthread_join(tids[1], NULL);
  // printf("Threads returned\n");
  //  }
  return 0;
}

  int stop = 1;




void die(char *s)
{
    perror(s);
    exit(1);
}

void *client(struct roteamento *rotConf)
{
    struct sockaddr_in si_other;
    int s;
    char buf[BUFLEN];
    char message[BUFLEN];
    unsigned int slen=sizeof(si_other);
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
        int dest = 0;

        scanf("%d", &dest);
        // if (dest < 0) stop = -1;
        // if (stop < 0) break;
        fgets(message, 100, stdin);
        message[strlen(message) - 1] = '\0';
        // puts('\n');
        int port = (int)(*rotConf).conf[(*rotConf).tab[dest].second].port;


        printf("\n");
        // printf("%s\n", (*rotConf).conf[(*rotConf).id_no].ip);
        // printf("port = %d\nip: %s\n", (*rotConf).id_no, (char *)(*rotConf).conf[(*rotConf).tab[dest].first].ip);
        si_other.sin_port = htons(port);
        if (inet_aton((*rotConf).conf[(*rotConf).tab[dest].second].ip , &si_other.sin_addr) == 0) //address to number
          fprintf(stderr, "inet_aton() failed\n"),
          exit(1);

        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
            die("sendto()");

        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        // if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
          // die("recvfrom()");
          // printf("passou\n");


				if (!strcmp(buf, "exit")) close(s), pthread_exit(NULL);
        // puts(buf);
    }

    close(s);
		pthread_exit(NULL);
}

void* server(struct roteamento *rotConf) //(intptr_t)rotConf[id_no].port
{
    struct sockaddr_in si_me, si_other;

    int s, recv_len;
    char buf[BUFLEN];
    unsigned slen = sizeof(si_other);

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons((*rotConf).conf[(*rotConf).id_no].port);//my port
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
      // printf("SERVEN\n");
    //keep listening for data
    while(1)
    {
        if (stop < 0) break;
        // printf("Waiting for data...");
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        fflush(stdout);
        memset(buf,'\0', BUFLEN);
        // fflush(stdin);

        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }

        //print details of the client/peer and the data received
        printf("Server>> Received packet from %s:%d \n           with data: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
        // printf("Server>> Data: %s\n\n" , buf);

        //now reply the client with the same data
        // if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        // {
        //     die("sendto()");
        // }
        printf("SErver:   ");
    }

    close(s);
		pthread_exit(NULL);
}
