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

int main(int argc, char *argv[]) {
  int i;
  struct roteamento rotConf;
  struct tplg topology;

  rotConf.id_no = atoi(argv[1]);
  ini_list(topology.adjList, MAX); //zera vetor de listas

  printf("Id node: %d\n", rotConf.id_no);
  getRot(rotConf.conf);
  getEnl(&topology);
  dijkstra(rotConf.id_no, topology, rotConf.tab);

  //limpa lista de adjacencias
  for (i = 0; i <= topology.n; i++)
    clear_list(topology.adjList[i]);

  //Threads which send and receive messages
  pthread_t tids[2];
  pthread_create(&tids[0], NULL, (void *)sender, &rotConf);
  pthread_create(&tids[1], NULL, (void *)receiver, &rotConf);
  pthread_join(tids[0], NULL);
  pthread_join(tids[1], NULL);
  return 0;
}


void die(char *s) {
  perror(s);
  exit(1);
}

//Reads a message and sends to the destiny
void *sender(struct roteamento *rotConf) {
  while(1) {
    struct msg message;
    //Initializes the struct message
    message.dest = 0;
    message.origin = (*rotConf).id_no;
    memset(message.message,'\0', sizeof(message.message));
    printf("\n>>>Enter id destiny and message:\n");
    scanf("%d", &message.dest);
    fgets(message.message, 100, stdin);
    message.message[strlen(message.message) - 1] = '\0';
    //Calls a function which sends the message to the destiny
    send_n(message, &(*rotConf));
  }
  pthread_exit(NULL);
}

//function used to send the message to the destiny
void send_n(struct msg message, struct roteamento *rotConf) {
  int sockt;
  struct sockaddr_in si_dest;
  unsigned int slen = sizeof(si_dest);

  //Create a UDP socket
  if ((sockt=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    die("socket");
  memset((char *) &si_dest, 0, sizeof(si_dest));
  si_dest.sin_family = AF_INET;
  //Gets the port which will be send the message
  int port = (int)(*rotConf).conf[(*rotConf).tab[message.dest].second].port;

  si_dest.sin_port = htons(port);
  if (inet_aton((*rotConf).conf[(*rotConf).tab[message.dest].second].ip , &si_dest.sin_addr) == 0) //address to number
    fprintf(stderr, "inet_aton() send_n() failed\n"),
    exit(1);
  //Sets the timeout and the number of attempts in case of fail
  int attempts = ATTEMPTS_STD;
  enum msg_type timeo = NONE;
  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 10;

  //Tries send the message until a confirmation is get
  while (attempts--) {
    struct msg ack;

    memset(ack.message, '\0', sizeof(ack.message));
    ack.type = NONE;

    //sends the message
    if (sendto(sockt, &message, sizeof(message), 0 , (struct sockaddr *) &si_dest, slen)==-1)
      die("\nsendto() send_n()\n");

    //sets the timeout of current socket
    setsockopt(sockt, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));

    //Wait for acknowledgement
    if (recvfrom(sockt, &ack, sizeof(ack), 0, (struct sockaddr *) &si_dest, &slen) == -1)
      printf("\n....Failed to send. Sending message again...\n");

    //In case of success, get the confirmation and leave this function
    timeo = ack.type;
    if (ack.type == ACK) break;
  }
  if (timeo == ACK) printf("\n....Message %s.\n", message.origin == (*rotConf).id_no ? "delivered" : "forwarded");
  if (timeo == NONE) printf("\n....Timeout. Failed to send.\n");

  close(sockt);
}

//Function used for receive or forward a message
void* receiver(struct roteamento *rotConf) {
  struct sockaddr_in si_me, si_other;
  int sockt, recv_len;
  int myport = (*rotConf).conf[(*rotConf).id_no].port;
  unsigned slen = sizeof(si_other);

  //create a UDP socket
  if ((sockt=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    die("socket");

  // zero out the structure
  memset((char *) &si_me, 0, sizeof(si_me));

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(myport); //ntohs();
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  //Bind socket to port
  if( bind(sockt , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    die("bind");

  //Keep listening for data
  while(1) {
    struct msg message;

    //Sets the message struct to default values
    message.dest = message.origin = 0;
    message.type = NONE;
    memset(message.message,'\0', sizeof(message.message));
    fflush(stdout);

    //Try to receive some data
    if ((recv_len = recvfrom(sockt, &message, sizeof(message), 0, (struct sockaddr *) &si_other, &slen)) == -1)
      die("\nrecvfrom() receiver()\n");

    //Test if the message will be forwarded or showed in this router
    if (message.dest == (*rotConf).id_no) {
      printf("\n....Received packet from router %d\n....With message: %s\n\n",  message.origin, message.message);
      
      //Now reply with acknowledgement
      struct msg ack;
      memset(ack.message, '\0', sizeof(ack.message));
      ack.type = ACK;
      if (sendto(sockt, &ack, sizeof(ack), 0, (struct sockaddr*) &si_other, slen) == -1)
      die("\nsendto() receiver()\n");
    }
    else {
      printf("\nRouter %d forwarding message with: destiny: %d origin: %d\n", (*rotConf).id_no, message.dest, message.origin);
      send_n(message, &(*rotConf));
    }
    
  }

  close(sockt);
  pthread_exit(NULL);
}
