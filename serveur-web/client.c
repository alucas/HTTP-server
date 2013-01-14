#define _GNU_SOURCE

#include "client.h"
#include "global.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <term.h>
#include <curses.h>

void initClients(client_t *client){
  client->sock = -1;
  client->serv[0] = '\0';
  client->host[0] = '\0';
  client->ip[0] = '\0';
  client->requete.ptr = NULL;
  client->requete.siz = 0;
  client->requete.use = 0;
  client->reponse.ptr = NULL;
  client->reponse.siz = 0;
  client->reponse.use = 0;
  client->path.ptr = NULL;
  client->path.siz = 0;
  client->path.use = 0;
  client->vars.ptr = NULL;
  client->vars.siz = 0;
  client->vars.use = 0;
}

void initClientsBuffer(client_t *client){
  if(!client->requete.ptr) client->requete = bufinit(NULL, BUFFER_SIZE, "", 0);
  if(!client->reponse.ptr) client->reponse = bufinit(NULL, BUFFER_SIZE, "", 0);
  if(!client->path.ptr)    client->path    = bufinit(NULL, BUFFER_SIZE, "", 0);
  if(!client->vars.ptr)    client->vars    = bufinit(NULL, BUFFER_SIZE, "", 0);
}

int connexionClient(client_t *client, int sock){
  struct sockaddr_storage addr;
  socklen_t sizeAddr = sizeof(addr);
  memset(&addr, 0, sizeof(addr));

  client->sock = accept(sock, (struct sockaddr*)(&addr), &sizeAddr);
  if(client->sock == -1){
    perror("accept");
    return 1;
  }

  if(getnameinfo((struct sockaddr *)(&addr),				\
		 sizeAddr,						\
		 client->host, SIZE_HOST,				\
		 client->serv, SIZE_SERV,				\
		 NI_NUMERICHOST) == 0){
    
    /* \e[36m
     * utiliser la libtermcap <term.h>
     * utiliser new_term, puis tparm(set_a_foreground, 36) et utiliser
     * tputs pour l'envoyer sur stdout
     * tparm(set_foreground, 36)
     * 
     * tputs(orig_pair) pour remettre par défaut
     *  man 5 terminfo pour tous les détails
     */
    
    printf("\e[36mconnexion du client (%s):%s (socket %d)\e[0m\n",	\
	   client->host,						\
	   client->serv,						\
	   client->sock);
  }else{
    perror("getnameinfo");
    printf("\e[36mconnexion d'un client (socket %d)\e[0m\n",	\
	   client->sock);
  }

  if(gl_close != 0){
    client->uptime = time(NULL);
  }

  return 0;
}

void deconnexionClient(client_t *client){
  if(close(client->sock) != 0){
    perror("close");
  }

  client->sock = -1;
  client->requete.use = 0;
}
