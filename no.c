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
#define BUFLEN 512  //Max length of buffer


int main(int argc, char *argv[]) {
  int i;
  struct roteamento rotConf;
  struct tplg topology;
  // struct pair dist[MAX];


  rotConf.id_no = atoi(argv[1]);
  ini_list(topology.adjList, MAX); //zera vetor de listas

  printf("Id node: %d\n", rotConf.id_no);
  getRot(rotConf.conf);
  getEnl(&topology);
  dijkstra(rotConf.id_no, topology, rotConf.tab);

  //limpa lista de adjacencias
  for (i = 0; i <= topology.n; i++)
    clear_list(topology.adjList[i]);

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
    // struct sockaddr_in si_other;
    // int s;
    // unsigned int slen=sizeof(si_other);
    // if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    // {
    //     die("socket");
    // }
    //
    // memset((char *) &si_other, 0, sizeof(si_other));
    // si_other.sin_family = AF_INET;

    while(1) {
        struct msg message;
        message.dest = 0;
        message.origin = (*rotConf).id_no;
        memset(message.message,'\0', sizeof(message.message));
        // printf("type %d\n", message.origin);
        // printf("message origin %d %d\n", (*rotConf).id_no, message.origin);
        printf("Client>> Enter message : ");
        scanf("%d", &message.dest);
        fgets(message.message, 100, stdin);
        message.message[strlen(message.message) - 1] = '\0';
        send_n(message, &(*rotConf));
        //
        // int port = (int)(*rotConf).conf[(*rotConf).tab[message.dest].second].port;
        // printf("\n");
        // si_other.sin_port = htons(port);
        // if (inet_aton((*rotConf).conf[(*rotConf).tab[message.dest].second].ip , &si_other.sin_addr) == 0) //address to number
        //   fprintf(stderr, "inet_aton() client failed\n"),
        //   exit(1);
        //
        // //timeout
        // int attempts = ATTEMPTS_STD;
        // struct timeval timeout;
        // enum msg_type timeo = NONE;
        // timeout.tv_sec = 1;
        // timeout.tv_usec = 10;
        //
        //
        // while (attempts--) {
        //   //sends the message
        //   if (sendto(s, &message, sizeof(message) + BUFLEN , 0 , (struct sockaddr *) &si_other, slen)==-1)
        //       die("sendto()");
        //   setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));
        //
        //   struct msg ack;
        //   // memset(ack.message, '\0', BUFLEN);
        //   ack.type = NONE;
        //
        //   // wait for ack
        //   if (recvfrom(s, &ack, sizeof(ack) + BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
        //     printf("Failed to send. Sending message again\n");//die("recvfrom() Mensage Not delivered");
        //   // printf("%s\n", ack.type == ACK ? "ACK" : ack.type == MESSAGE ? "MESSAGE" : "NONE");
        //   timeo = ack.type;
        //   if (ack.type == ACK) break;
        // }
        // // printf("DESTINO NO CLIENTE: %d\n", message.dest);
        // if (timeo == NONE) printf("Timeout. Failed to send\n");
    }

    // close(s);
		pthread_exit(NULL);
}

void send_n(struct msg message, struct roteamento *rotConf) {
  int s;
  struct sockaddr_in si_other;
  unsigned int slen=sizeof(si_other);
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
      die("socket");
  }
  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;

  int port = (int)(*rotConf).conf[(*rotConf).tab[message.dest].second].port;

  si_other.sin_port = htons(port);
  if (inet_aton((*rotConf).conf[(*rotConf).tab[message.dest].second].ip , &si_other.sin_addr) == 0) //address to number
  fprintf(stderr, "inet_aton() client failed\n"),
  exit(1);
  //timeout
  int attempts = ATTEMPTS_STD;
  enum msg_type timeo = NONE;
  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 10;

  printf("firts att: %d\n", attempts);
  while (attempts--) {
    //sends the message

    if (sendto(s, &message, sizeof(message) + BUFLEN , 0 , (struct sockaddr *) &si_other, slen)==-1)
    die("sendto()");
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));

    struct msg ack;
    memset(ack.message, '\0', sizeof(ack.message));
    ack.type = NONE;

    // wait for ack
      printf("att: %d\n", attempts);
    if (recvfrom(s, &ack, sizeof(ack), 0, (struct sockaddr *) &si_other, &slen) == -1)
    printf("Failed to send. Sending message again\n");//die("recvfrom() Mensage Not delivered");
    // printf("%s\n", ack.type == ACK ? "ACK" : ack.type == MESSAGE ? "MESSAGE" : "NONE");
    timeo = ack.type;
    if (ack.type == ACK) break;
  }
  // printf("DESTINO NO CLIENTE: %d\n", message.dest);
  if (timeo == ACK) printf("\nMessage delivered\n");
  if (timeo == NONE) printf("\nTimeout. Failed to send\n");
  close(s);
}

void* server(struct roteamento *rotConf) //(intptr_t)rotConf[id_no].port
{
    struct sockaddr_in si_me, si_other;
    int cont = 0;
    int s, recv_len;
    int myport = (*rotConf).conf[(*rotConf).id_no].port;
    char buf[BUFLEN];
    unsigned slen = sizeof(si_other);

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        die("socket");

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(myport); //ntohs();
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("port %d %d\n", ntohs(si_me.sin_port), si_me.sin_port);
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
        die("bind");
    //keep listening for data
    while(1)
    {
        // if (cont > 0) { cont = 0; continue;}
        printf("Entrou server %d\n", cont++);
        struct msg message;
        message.dest = message.origin = 0;
        message.type = NONE;
        memset(message.message,'\0', sizeof(message.message));
        fflush(stdout);
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, &message, sizeof(message), 0, (struct sockaddr *) &si_other, &slen)) == -1)
            die("recvfrom()");

        if (message.dest == (*rotConf).id_no)
          printf("\n....Received packet from router %d\n....with message: %s\n\n",  message.origin, message.message);
        // printf("Received packet from %s:%d \n            with data: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), message.message);
          // printf("%d %d\n", htons(si_other.sin_port), htons(si_me.sin_port)),
        else {
          printf("\nRouter %d forwarding message with: destiny: %d origin: %d\n", (*rotConf).id_no, message.dest, message.origin);
          send_n(message, &(*rotConf));
        }

         // now reply with ACK
         struct msg ack;
         memset(ack.message, '\0', sizeof(ack.message));
         ack.type = ACK;
         if (sendto(s, &ack, sizeof(ack), 0, (struct sockaddr*) &si_other, slen) == -1)
         die("sendto()");
    }
    close(s);
		pthread_exit(NULL);
}
