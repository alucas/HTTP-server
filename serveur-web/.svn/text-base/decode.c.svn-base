#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "decode.h"

static int getMethod(char *buffer, int *method);
static int getProtocol(char *buffer, int *protocol, int *keep_alive);
static int getEntete(char *buffer);
static void setEntete(request_t *req, char *buffer, int entete, int curseur);

/* Fonction qui convertit la chaine de caracteres en une liste de mots
   et les rentre dans la structure de requete passÃ© en parametre */
int decode(buffer_t *input, request_t *req)
{
  char buffer[BUFFER_SIZE + 1];
  int mot = 0;
  int ligne = 0;
  int curseur = 0;
  int car_vide = '\0'; // sert a suprimer les espaces consecutifs
  int entete = -1;
  int ret = 0;

  for(int i=0; i<input->use; i++){
    if(input->ptr[i] == ' ' || input->ptr[i] == '\r' || input->ptr[i] == '\n'){
      if(!car_vide){
	car_vide = input->ptr[i];
	buffer[curseur] = '\0';

	if(input->ptr[i] == '\n'){
	  entete = -1;
	  ligne++;
	}

	switch(mot){
	case 0 :
	  if((ret = getMethod(buffer, &req->method)) != 0)
	    return ret;
	  break;

	case 1 :
	  if(curseur <= BUFFER_SIZE){
	    strncpy(req->path, buffer, (curseur+1)*sizeof(char));
	    break;
	  }
	  return 414; // path trop long

	case 2 :
	  if(!getProtocol(buffer, &req->protocol, &req->keep_alive))
	    return 0;
	  break;

	default :
	  if (buffer[curseur-1] == ':'){
	    entete = getEntete(buffer);
	  }else{
	    setEntete(req, buffer, entete, curseur);
	  }
	}
	curseur = 0;
	mot++;
      }else{
	if(input->use - i < 3){
	  fprintf(stderr,					\
		  "fin du traitement de la requete"		\
		  "sans avoir trouvé les caracteres de fin\n");
	  break;
	}
	if(!strncmp(input->ptr + i - 1, "\r\n\r\n", 4)){
	  break;
	}
      }
    }else{
      car_vide = 0;
      buffer[curseur++] = input->ptr[i];
      if(curseur > BUFFER_SIZE){
	fprintf(stderr, "mot trop long (size max : %d)\n", BUFFER_SIZE);
	return 413; // mot trop long
      }
    }
  }
  if(mot < 2){
    return 400;
  }
  return 0;
}

static int getMethod(char *buffer, int *method)
{
  unsigned long int sum = 0;
  int p = 0;

  for(char *c = buffer; (*c) != '\0'; c++){
    sum += (*c-64)*pow(26,p);
    p++;
  }  
  switch(sum){
  case 13657: // GET
    *method = 1;
    break;
  case 71118: // HEAD
    *method = 2;
    break;
  case 364770: // POST
    *method = 3;
    break;
  case 1747798487: // OPTIONS (le resultat du case n'est pas exact car il dÃ©passe la case)
    return 501;
  case 1921533153: // CONNECT (le resultat du case n'est pas exact car il dÃ©passe la case)
    return 501;
  case 2338772: // TRACE
    return 501;
  case 14082: // PUT
    return 501;
  case 68642526: // DELETE
    return 501;
  default:
    fprintf(stderr, "commande inconue %s\n", buffer);
    return 400; // commande inconue
  }
  return 0;
}

static int getProtocol(char *buffer, int *protocol, int *keep_alive)
{
  if(!strncmp(buffer, "HTTP/1.", 7) && buffer[8] == '\0'){
    if(buffer[7] == '0'){
      *protocol = 1; // http 1.0
      return 1;

    }else if(buffer[7] == '1'){
      *protocol = 2; // http 1.1
      *keep_alive = 1;
      return 1;
    }
  }
  *protocol = 0; // http 0.9
  return 0;
}

static int getEntete(char *buffer)
{
  if       (!strncmp(buffer, "User-Agent", 10)){
    return 1;
  }else if (!strncmp(buffer, "Host", 4)){
    return 2;
  }else if (!strncmp(buffer, "Connection", 10)){
    return 3;
  }else if (!strncmp(buffer, "Keep-Alive", 10)){
    return 4;
  }else if (!strncmp(buffer, "Referer", 7)){
    return 5;
  }else if (!strncmp(buffer, "Accept-Language", 15)){
    return 6;
  }else if (!strncmp(buffer, "Accept-Encoding", 15)){
    return 7;
  }else if (!strncmp(buffer, "Accept-Charset", 14)){
    return 8;
  }else if (!strncmp(buffer, "Cache-Control", 13)){
    return 9;
  }else if (!strncmp(buffer, "Accept", 6)){
    return 10;
  }else if (!strncmp(buffer, "Authorization", 13)){
    return 11;
  }else if (!strncmp(buffer, "Content-Length", 14)){
    return 12;
  }else if (!strncmp(buffer, "Content-Type", 12)){
    return 13;
  }else{
    fprintf(stderr, "parametre %s inconu\n", buffer);
    return 0;
  }
}

static void setEntete(request_t *req, char *buffer, int entete, int curseur)
{
  switch(entete){
  case -1 :
    strncpy(req->query, buffer, curseur);
    break;
  case 1 :
    if(strlen(req->agent) == 0){
      strncpy(req->agent, buffer, curseur);
    }else{
      strncat(req->agent, " ", 1);
      strncat(req->agent, buffer, curseur);
    }
    break; 
  case 2 :
    strncpy(req->host, buffer, curseur);
    break;
  case 3 :
    if (!strncmp(buffer, "keep-alive", 10)){
      req->keep_alive = 1;
    }
    break;   
  case 4 :
    req->alive_time = atoi(buffer);
    break;    
  case 5 :
    strncpy(req->referer, buffer, curseur);
    break;    
  case 6 :
    strncpy(req->language, buffer, curseur);
    break;	      
  case 7 :
    strncpy(req->encoding, buffer, curseur);
    break;    
  case 8 :
    strncpy(req->charset, buffer, curseur);
    break;    
  case 9 :
    strncpy(req->cache_control, buffer, curseur);
    break;    
  case 10 :
    strncpy(req->accept, buffer, curseur);
    break;  
  case 11 :
    strncat(req->authorization, buffer, curseur);
    break;
  case 12 :
    req->content_length = atoi(buffer);
    break;
  case 13 :
    strncat(req->content_type, buffer, curseur);
    break;
  }
}
