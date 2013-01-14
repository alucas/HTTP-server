#ifndef CLIENT_H
#define CLIENT_H

#include "buffer.h"

#include <pthread.h>
#include <arpa/inet.h>

#define SIZE_SERV 32
#define SIZE_HOST 1024

typedef struct client{
  pthread_t          thread;
  time_t             uptime;
  int                sock;
  char               serv[SIZE_SERV];
  char               host[SIZE_HOST];
  char               ip[INET6_ADDRSTRLEN];
  buffer_t           requete;
  buffer_t           reponse;
  buffer_t           path;
  buffer_t           vars;
} client_t;

extern void initClients      (client_t *client);
extern void initClientsBuffer(client_t *client);
extern void deconnexionClient(client_t *client);
extern int  connexionClient  (client_t *client, int sock);

#endif /* CLIENT_H */
