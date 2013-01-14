#ifndef REQUETE_H
#define REQUETE_H

#include "client.h"

#define SMALL_BUFF 100

typedef struct request {
  int method;
  char path[BUFFER_SIZE];
  char host[BUFFER_SIZE];
  char *ip;
  char agent[BUFFER_SIZE];
  char referer[BUFFER_SIZE];
  char language[SMALL_BUFF];
  char charset[BUFFER_SIZE];
  char encoding[SMALL_BUFF];
  char cache_control[BUFFER_SIZE];
  char accept[BUFFER_SIZE];
  char query[BUFFER_SIZE];
  char authorization[SMALL_BUFF];
  char content_type[SMALL_BUFF];
  int content_length;
  int protocol;
  int index;
  int keep_alive;
  int alive_time;
} request_t;

typedef struct reponse {
  int code;
  char content_type[30];
  buffer_t header;
  time_t time;
  int lenght;
} reponse_t;

extern void *requeteThread(void *param);
extern int   requete(client_t *client);

#endif /* REQUETE_H */
