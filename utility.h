#ifndef __UTILITY__
#define __UTILITY__

#include <stdio.h>
#include <string.h>
#include "dijkstra.h"

#define max(a, b) (a > b ? a : b)
#define ATTEMPTS_STD 5
#define MSGLEN 512  //Max length of message

struct conf {
  int port;
  char ip[20];
};

struct roteamento {
 int id_no;
 struct conf conf[MAX];
 struct pair tab[MAX]; //out: second; distance: first
};
enum msg_type{
   NONE, MESSAGE, ACK,
};

struct msg {
  char message[100];
  int dest, origin;
  enum msg_type type;
};

void *sender(struct roteamento *rotConf);
void *receiver(struct roteamento *rotConf);
void send_n(struct msg message, struct roteamento *rotConf);

void getEnl(struct tplg *topology);
void getRot(struct conf rotConf[]);



#endif
