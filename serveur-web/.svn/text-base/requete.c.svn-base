#define _XOPEN_SOURCE 500

#include "global.h"
#include "requete.h"
#include "sendrecv.h"
#include "getContentType.h"
#include "authentification.h"
#include "makeLog.h"
#include "decode.h"
#include "encode.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define valider_access(mode) (((mode) & S_IROTH) == S_IROTH)
#define min(x, y) (((x)<(y))?(x):(y))

#define SIZE_PATH 1024
#define SIZE_IP 32

#define PRINT_ERREUR 1

/* Fonction qui gere les erreurs */
static void erreur(int code, reponse_t *rep)
{
  switch(code)
    {
    case 400 :
      if(PRINT_ERREUR)
	printf("Erreur 400 : Bad Request !\n");
      bufstrcpy(&rep->header," 400 Bad Request",17);
      break;
    case 401 :
      if(PRINT_ERREUR)
	printf("Erreur 401 : Unautorized !\n");
      bufstrcpy(&rep->header," 401 Unauthorized",17);
      break;
    case 403 :
      if(PRINT_ERREUR)
	printf("Erreur 403 : Forbidden\n");
      bufstrcpy(&rep->header," 403 Forbidden",14);
      break;
    case 404 :
      if(PRINT_ERREUR)
	printf("Erreur 404 : File not found ! (Try Again)\n");
      bufstrcpy(&rep->header," 404 Not Found",14);
      break;
    case 413 :
      if(PRINT_ERREUR)
	printf("Erreur 413 : Request Entity Too Large\n");
      bufstrcpy(&rep->header," 413 Request Entity Too Large",29);
      break;
    case 414 :
      if(PRINT_ERREUR)
	printf("Erreur 414 : Request-URI Too Long\n");
      bufstrcpy(&rep->header," 414 Request-URI Too Long",25);
      break;
    case 500 :
      if(PRINT_ERREUR)
	printf("Erreur 500 : Internal Server Error\n");
      bufstrcpy(&rep->header," 500 Internal Server Error",26);
      break;
    case 501 :
      if(PRINT_ERREUR)
	printf("Erreur 501 : Commande non suportÃ©e\n");
      bufstrcpy(&rep->header," 501 Not Implemented",20);
      break;
    default :
      if(PRINT_ERREUR)
	printf("Erreur interne : Code d'erreur inconnu !\n");
      bufstrcpy(&rep->header,"",0);
    }
  rep->code = code;
}

static int valider_path(request_t *req, reponse_t *rep)
{
  char complete_path[PATH_MAX] = {0};
  char resolved_path[PATH_MAX] = {0};

  /* On obtient le chemin complet de la ressource */
  strncpy(complete_path, gl_www.ptr, gl_www.use);
  strcat (complete_path, req->path);

  /* On evalue le path complet de la ressource */
  if(realpath(complete_path, resolved_path) == NULL){
    return 404;
  }
  
  if(strncmp(resolved_path, gl_www.ptr, gl_www.use)){
    return 404;
  }
  
  strncpy(req->path, resolved_path, strlen(resolved_path));

  return 0; /* Bon path */
}


static void getVarGET(request_t *req, buffer_t *var)
{
  int size = strlen(req->path);
  char *pos = strchr(req->path, '?');
  
  if(pos != NULL){
    bufstrcpy(var, pos+1, size-(pos-req->path)-1);
    
    req->path[pos-req->path] = '\0';
  }
}

static void get_encoding(char *p, char *output)
{
  int n = 0;
  int i = 0;
  while(p[i++] != '\0'){
    n++;

    if(p[i] == ',' || p[i] == '\0'){
      if(!strncmp(&p[i-n],"gzip",4)){
	strncpy(output,&p[i-n],n);
      }else if (!strncmp(&p[i-n],"bzip2",5)){
	strncpy(output,&p[i-n],n);
      }

      n = 0;
      if(p[i++] == '\0'){
	break;
      }
    }
  }
}

/* Fonction qui interprete une requette passee en parametre dans input,
   fournit le chemin du fichier demandÃ© dans path,
   fournit le header dans output,
   index 
   prÃ©cise s'il faut conserver la connection dans keep_alive (0/1) */
static int interpreter(client_t *client, int *index, int *keep_alive, char *encoding, int *endRequete)
{
  request_t requete = {0, {0}, {0}, client->ip, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, 0, 0, 0};
  reponse_t reponse = {200, {0}, {NULL, 0, 0}, 0, 10};
  reponse.header = bufinit(&reponse.header, BUFFER_SIZE, " 200 OK", 9);
  reponse.header.use = 9;
  reponse.time = time(NULL);

#ifndef NDEBUG
  printf("<interpreter> requete entrante :\n'%s'\n", client->requete.ptr);
#endif

  int ret = decode(&client->requete, &requete);
  if(ret != 0){
    erreur(ret, &reponse);
  }

  if(requete.method == 1){ // GET
    getVarGET(&requete, &client->vars);
  }else if(requete.method == 3){ // POST
    int size = min(requete.content_length, client->requete.use - *endRequete);
    bufstrcpy(&client->vars, client->requete.ptr+(*endRequete), size);
  }

  printf("variables : %s\n", client->vars.ptr);

  /* Validation du path */
  if(valider_path(&requete, &reponse)){
    erreur(404, &reponse);
  }

  
  struct stat file_stat;
  if(reponse.code == 200){
    if(stat(requete.path, &file_stat) == -1){
      perror("stats");
      erreur(500, &reponse);
    }
  }
    
  /*droit de lecture */
  if(reponse.code == 200){
    if(!valider_access(file_stat.st_mode)){
      erreur(403, &reponse);
    }
  }

  /* Authentification */
  if(reponse.code == 200){
    if(passFileExistInDir(requete.path)){
      char login[30] = {0};
      char password[15] = {0};
      
      if(!getAuthorization(requete.authorization, login, password) &&
	 !getLoginAndPassword(requete.query, login, password)){
	erreur(401,&reponse);
	requete.keep_alive = 0;
      }else{
	char pass_path[SMALL_BUFF];
	getPassFilePath(requete.path,pass_path,isDir(&file_stat));
	if(!authentifier(login,password,pass_path)){
	  
	  erreur(401,&reponse);
	  requete.keep_alive = 0;
	}
      }
    }
    
    /* Get content type*/
    getContentType(&requete, &reponse, &file_stat);
    if(requete.method == 2){
      *index = 4; // methode HEAD 
    }else{
      *index = requete.index;
    }
  }
  
  bufstrcpy(&client->path, requete.path, strlen(requete.path));
  
  /* On creer le header dans output */
  encoder(&client->reponse, &reponse, &requete);

  if(strlen(requete.encoding) > 0){
    get_encoding(requete.encoding,encoding);
  }

  makeLog(&requete,&reponse);

  *keep_alive = requete.keep_alive;
  *endRequete += requete.content_length;

  bufkill(&reponse.header);

  return reponse.code;
}

static void supprimerRequete(buffer_t *requete, int posFin){
  assert(posFin <= requete->use);

  memmove(requete->ptr, requete->ptr + posFin, requete->use - posFin);
  requete->use -= posFin; 
}

static int traiterRequete(client_t *client, int ret, int *keep_alive, int end){
  int index = 0;
  char encoding[16] = {'\0'};
  client->vars.ptr[0] = '\0';
  client->vars.use = 0;

  if(ret == 0){
    ret = interpreter(client,						\
		      &index,						\
		      keep_alive,					\
		      encoding,
		      &end);
    supprimerRequete(&client->requete, end);
  }else{
    client->requete.use = 0;
  }

  if(ret >= 400){
    sendErreur(&client->reponse, ret, client->sock);
  }else if(ret >= 0){
    sendReponse(&client->reponse, &client->path, client->sock, index, &client->vars, encoding);
  }
  
  if(gl_close != 0){
    client->uptime = time(NULL); 
  }

  return 0;
}

void *requeteThread(void *param){
  client_t *client = (client_t *)param;
  int keep_alive = 0;
  int ret = 0;
  int endRequete = 0;

  initClientsBuffer(client);
  
  if(pthread_detach(pthread_self()) != 0){
    perror("pthread_detach");
    return NULL;
  }

  client->requete.use = 0;
  while((ret = readRequete(&client->requete, client->sock, &endRequete, true)) != -1){
    
    traiterRequete(client, ret, &keep_alive, endRequete);

    if(!keep_alive){
      break;
    }
  }

  if(client->sock != -1){
    printf("\e[31mdéconnexion du client (%s):%s (socket %d)\e[0m\n", client->host, client->serv, client->sock);
    deconnexionClient(client);
  }

  return NULL;
}

int requete(client_t *client){
  int keep_alive = 0;
  int ret = 0;
  int endRequete = 0;

  initClientsBuffer(client);
 
  ret = readRequete(&client->requete, client->sock, &endRequete, false);

  if(ret == 100){
    return 100;
  }
  
  traiterRequete(client, ret, &keep_alive, endRequete);
  
  if(ret == -1 || !keep_alive){
    printf("\e[31mdéconnexion du client (%s):%s (socket %d)\e[0m\n", client->host, client->serv, client->sock);
    deconnexionClient(client);
  }

  return 0;
}
