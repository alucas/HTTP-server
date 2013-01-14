#define _XOPEN_SOURCE 1
#define _GNU_SOURCE

#include "client.h"
#include "global.h"
#include "configuration.h"
#include "requete.h"
#include "sendrecv.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <errno.h>
#include <time.h>
#include <limits.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SIZE 10
#define MAX_CONNEXION 128 // obselete
#define MIN_CLIENT 10
#define MIN_SERVEUR 5
#define FILE_CONF "./var/serveur.conf"

typedef struct serveur{
  int sock;
} serveur_t;

static bool shut = false;

int gl_port;
int gl_mode;
int gl_close;
buffer_t gl_www;
buffer_t gl_name;
buffer_t gl_logError;
buffer_t gl_logAccess;

static void eteindre(int sig){
  shut = true;
}

int main(int argc, char *argv[]){
  setlocale(LC_ALL, "");
  
  struct sigaction sa;
  sa.sa_handler = eteindre;
  sa.sa_flags = SA_RESETHAND;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGINT, &sa, NULL);

  // valeur des global par defaut
  gl_port  = 80; // port "www"
  gl_mode  = 1; // mode thread
  gl_close = 0;
  gl_www       = bufinit(NULL, BUFFER_SIZE, "./www/", 6);
  gl_name      = bufinit(NULL, BUFFER_SIZE, "lina, v0.1", 10);
  gl_logError  = bufinit(NULL, BUFFER_SIZE, "./var/log/error.log", 19);
  gl_logAccess = bufinit(NULL, BUFFER_SIZE, "./var/log/access.log", 20);

  // getion des parametres
  if(confParams(argc, argv) != 0){
    return EXIT_FAILURE;
  }

  // getion d'un fichier de configuration
  int ret = 0;
  if((ret = confFile(FILE_CONF)) != 0){
    fprintf(stderr, "fichier de configuration invalide (%d erreur(s))\n", ret);
  }

  char buffer[PATH_MAX];
  if(realpath(gl_www.ptr, buffer) == NULL){
    perror("realpath");
  }
  bufstrcpy(&gl_www, buffer, strlen(buffer));


  // getion des multiple protocole d'ecoute
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *result, *res;
  char str[6] = {0};
  snprintf(str, 6, "%d", gl_port);
  ret = getaddrinfo(NULL, str, &hints, &result);
  if(ret != 0){ 
    gai_strerror(ret);
    return EXIT_FAILURE;
  }

  serveur_t *serveurs = memoire_allouer(sizeof(serveur_t)*MIN_SERVEUR);
  int maxAddr = MIN_SERVEUR; 
  int nbAddr = 0;

  for(res = result; res != NULL; res = res->ai_next){
    if(nbAddr >= maxAddr){
      maxAddr *= 2;
      serveurs = memoire_reallouer(serveurs, sizeof(serveur_t)*maxAddr);
    }

    serveurs[nbAddr].sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(serveurs[nbAddr].sock == -1){
      perror("socket");
      exit(EXIT_FAILURE);
    }
  
    printf("création du socket (socket %d", serveurs[nbAddr].sock);
    if(res->ai_family == 2){
      printf(", ip v4");
    }else if(res->ai_family == 10){
      printf(", ip v6");
    }
    printf(")\n");

    int boo = 1;
    if(res->ai_family == AF_INET6 && setsockopt(serveurs[nbAddr].sock, IPPROTO_IPV6, IPV6_V6ONLY, &boo, sizeof(boo)) != 0){
      perror("setsockopt");
    }
    boo = 1;
    if(setsockopt(serveurs[nbAddr].sock, SOL_SOCKET, SO_REUSEADDR, &boo, sizeof(boo)) != 0){
      perror("setsockopt");
    }

    if(bind(serveurs[nbAddr].sock, res->ai_addr, res->ai_addrlen)){
      printf("errno : %d,%d\n", errno, EINVAL);
      perror("bind");
      //exit(EXIT_FAILURE);
    }
    
    printf("bind (socket %d, port %d)\n", serveurs[nbAddr].sock, gl_port);

    if(listen(serveurs[nbAddr].sock, MAX_CONNEXION)){
      perror("listen");
      exit(EXIT_FAILURE);
    }

    nbAddr++; // ne pas oublier
  }

  int nbClients = MIN_CLIENT;
  int nbConexion = 0;
  int idClientMax = 0;
  client_t **clients = memoire_allouer(sizeof(client_t *) * nbClients);
  for(int i=0; i<nbClients; i++){
    clients[i] = memoire_allouer(sizeof(client_t));
    initClients(clients[i]);
  }
  
  fd_set set;
  int fd_max = 0;
  int i = 0;
  int j = 0;

  while(1){
    if(shut){
      break;
    }
    
    // on compte le nombre de clients connecté
    nbConexion = 0;
    for(i=0; i<nbClients; i++){
      if(clients[i]->sock != -1){
	nbConexion++;
	idClientMax = i;
      }
    }
  
    // si il n'y a plus de place libre on augmente la taille
    if(nbConexion == nbClients){
      clients = memoire_reallouer(clients, 2*nbClients*sizeof(client_t *));
      for(i=nbClients; i<(2*nbClients); i++){
	clients[i] = memoire_allouer(sizeof(client_t));
	initClients(clients[i]);
      }

      nbClients *= 2;
    }
  
    // si il y a trop de place libre on libert de la place
    if((idClientMax+1)*4 <= nbClients && nbClients > MIN_CLIENT){
      for(i=(nbClients/2); i<nbClients; i++){
	memoire_liberer(clients[i]);
	bufkill(&clients[i]->requete);
	bufkill(&clients[i]->reponse);
	bufkill(&clients[i]->path);
      }
      
      clients = memoire_reallouer(clients, (nbClients/2)*sizeof(client_t *));    
      
      nbClients /= 2;
    }

    FD_ZERO(&set);

    for(i=0; i<nbAddr; i++){
      FD_SET(serveurs[i].sock, &set);
      if(serveurs[i].sock > fd_max){
	fd_max = serveurs[i].sock;
      }
    }

    if(gl_mode == 2){ // mode select
      for(i=0; i<nbClients; i++){
	if(clients[i]->sock != -1){
	  FD_SET(clients[i]->sock, &set);
	  
	  if(clients[i]->sock > fd_max){
	    fd_max = clients[i]->sock;
	  }
	}
      }

      struct timeval timeout = {1, 0};
      if(select(fd_max+1, &set, NULL, NULL, &timeout) == -1){
	perror("select");
	break;
      }

      for(j=0; j<nbAddr; j++){
	if(FD_ISSET(serveurs[j].sock, &set)){
	  for(i=0; i<nbClients; i++){
	    if(clients[i]->sock == -1){
	      connexionClient(clients[i], serveurs[j].sock);
	      break;
	    }
	  }
	}
      }

      int timer;
      for(i=0; i<nbClients; i++){
	if(clients[i]->sock != -1){
	  timer = time(NULL);

	  if(FD_ISSET(clients[i]->sock, &set)){
	    requete(clients[i]);
	  }else if(gl_close != 0 && clients[i]->uptime + gl_close < timer){
	    printf("\e[31mdéconnexion du client (%s):%s "			\
		   "(socket %d)(trop longtemps inactif)\e[0m\n",	\
		   clients[i]->host,					\
		   clients[i]->serv,					\
		   clients[i]->sock);
	    deconnexionClient(clients[i]);
	  }
	}
      }
    }else{ // mode normal
      struct timeval timeout = {1, 0};
      if(select(fd_max+1, &set, NULL, NULL, &timeout) == -1){
	perror("select");
	break;
      }

      // on connecte le client
      for(j=0; j<nbAddr; j++){
	if(FD_ISSET(serveurs[j].sock, &set)){
	  for(i=0; i<nbClients; i++){
	    if(clients[i]->sock == -1){
	      if(connexionClient(clients[i], serveurs[j].sock) == 0){
		if(pthread_create(&(clients[i]->thread), NULL, requeteThread, clients[i]) != 0){
		  perror("pthread_create");
		}
	      }
	      break;
	    }
	  }
	}
      }
      
      if(gl_close != 0){
	// déconnexion des client inactif depuis trop longtemps
	int timer = time(NULL);
	for(i=0; i<nbClients; i++){
	  if(clients[i]->sock != -1 && clients[i]->uptime+gl_close < timer){
	    printf("\e[31mdéconnexion du client (%s):%s "		\
		   "(socket %d)(trop longtemps inactif)\e[0m\n",	\
		   clients[i]->host,					\
		   clients[i]->serv,					\
		   clients[i]->sock);
	    pthread_cancel(clients[i]->thread);
	    deconnexionClient(clients[i]);
	  }
	}
      }
    }
  } // fin du while(1);
  
  // on libert la memoire (pour detecter les fuites)
  for(i=0; i<nbClients; i++){
    if(clients[i]->sock != -1){
      printf("\e[31mdéconnexion du client (%s):%s (socket %d)\e[0m\n",	\
	     clients[i]->host,						\
	     clients[i]->serv,						\
	     clients[i]->sock);
      //pthread_cancel(clients[i]->thread);
      deconnexionClient(clients[i]);
    }

    bufkill(&clients[i]->requete);
    bufkill(&clients[i]->reponse);
    bufkill(&clients[i]->path);
    bufkill(&clients[i]->vars);
    memoire_liberer(clients[i]);
  }
  memoire_liberer(clients);

  memoire_liberer(serveurs);

  freeaddrinfo(result);
  
  bufkill(&gl_www);
  bufkill(&gl_name);
  bufkill(&gl_logError);
  bufkill(&gl_logAccess);

  return EXIT_SUCCESS;
}
