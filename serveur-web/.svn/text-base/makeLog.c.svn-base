#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "makeLog.h"

char *day[7] = {"Sun","Mon","Tue","Wen","Thu","Fri","Sat"};
char *mon[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};


//Date, addresse IP, nom de domaine du client,
//User-Agent, fichier demandÃ©, resultat
void makeLog(request_t *requ, reponse_t *rep)
{
  FILE *f;
  struct tm gmtime;
  char *logpath;
  if (rep->code >= 400)
    logpath = gl_logError.ptr;
  else
    logpath = gl_logAccess.ptr;
  if(rep->time == 0)
    rep->time = time(NULL);
  gmtime_r(&rep->time,&gmtime);
  char log_time[100] = {0};
  sprintf(log_time,
	  "[%s, %02d %s %04d %02d:%02d:%02d GMT]",
	  day[gmtime.tm_wday],
	  gmtime.tm_mday,
	  mon[gmtime.tm_mon],
	  gmtime.tm_year + 1900,
	  gmtime.tm_hour,
	  gmtime.tm_min,
	  gmtime.tm_sec);


  if((f = fopen(logpath,"a")) != NULL){
    int write_result[30] = {};
    int i = 0;
    write_result[i++] = fwrite(log_time,sizeof(char),strlen(log_time),f);

    write_result[i++] = fwrite(" <",sizeof(char),2,f);
    write_result[i++] = fwrite(requ->ip,sizeof(char),strlen(requ->ip),f);

    write_result[i++] = fwrite("> ",sizeof(char),2,f);
    if(strlen(requ->host) == 0)strcpy(requ->host,"Unknow");
    write_result[i++] = fwrite(requ->host,sizeof(char),strlen(requ->host),f);

    write_result[i++] = fwrite(" \"",sizeof(char),2,f);
    if(strlen(requ->agent) == 0)strcpy(requ->agent,"Unknow");
    write_result[i++] = fwrite(requ->agent,sizeof(char),strlen(requ->agent),f);

    write_result[i++] = fwrite("\" ",sizeof(char),2,f);
    write_result[i++] = fwrite(requ->path,sizeof(char),strlen(requ->path),f);

    write_result[i++] = fwrite(" ",sizeof(char),2,f);
    write_result[i++] = fwrite(rep->header.ptr,sizeof(char),rep->header.use,f);

    write_result[i++] = fwrite("\r\n",sizeof(char),2,f);

    fclose(f);
    for (int n = 0; n < i; n++){
      if(write_result[n] == -1)
	perror("write");
    }
  }else{
    perror("Erreur fopen du fichier de log");
  }
}
