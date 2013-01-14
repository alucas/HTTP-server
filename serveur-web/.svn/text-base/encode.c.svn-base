#define _BSD_SOURCE

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "encode.h"

static void ajouter(buffer_t *ouptut, char *entete, int entete_size, char *valeur, int valeur_size);
static void getTime(char *date, time_t time);

/* Fonction qui forme l'entete de la reponse HTTP */
void encoder(buffer_t *output, reponse_t *rep, request_t *req)
{
  output->ptr[0] = '\0';
  output->use = 0;

  if(req->protocol == 1){
    ajouter(output, "HTTP/1.0", 8, rep->header.ptr, strlen(rep->header.ptr));
  }else{
    ajouter(output, "HTTP/1.1", 8, rep->header.ptr, strlen(rep->header.ptr));
  }

  if(rep->code < 400){
    /*Date*/
    char date[50] = {0};
    getTime(date, rep->time);
    ajouter(output, "Date: ", 6, date, 32);

    /*Server*/
    ajouter(output, "Server: ", 8, gl_name.ptr, gl_name.use);
    
    /*Content-type*/
    if (rep->content_type != NULL && req->index != 2){
      ajouter(output, "Content-Type: ", 14, rep->content_type, strlen(rep->content_type));
    }
  }
  /*Authenticate*/
  if (rep->code == 401){
    ajouter(output, "WWW-Authenticate: ", 18, "Basic realm=\"serveur de test\"", 29);
  }
  output->use = strlen(output->ptr);
}

/* Ajoute l'entete et sa valeur dans le buffer output */
static void ajouter(buffer_t *output, char *entete, int entete_size, char *valeur, int valeur_size)
{
  bufstrcat(output,entete,entete_size);
  bufstrcat(output,valeur,valeur_size);
  bufstrcat(output,"\r\n", 2);
}

/* Formate time dans date */
static void getTime(char *date, time_t time)
{
  struct tm gmtime;
  gmtime_r(&time,&gmtime);
  snprintf(date, 32,
	   "%s, %02d %s %04d %02d:%02d:%02d GMT",
	   day[gmtime.tm_wday],
	   gmtime.tm_mday,
	   mon[gmtime.tm_mon],
	   gmtime.tm_year + 1900,
	   gmtime.tm_hour,
	   gmtime.tm_min,
	   gmtime.tm_sec);
}
