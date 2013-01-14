#define _BSD_SOURCE

#include "getContentType.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define isExecutable(file_stat) (((file_stat)->st_mode & S_IXUSR) == S_IXUSR)

/* Retourne 1 s'il existe un index.{htm,html} dans le dossier */
static int indexExistInDir(char *path);
/* Retourne 1 si le path contient la chaine /cgi-bin/ et 0 sinon */
static int isInCgiDir(char *path);
/* Cherche l'extention du fichier dans path et l'ecrit dans ext,
   Retourne 1 si une extension a été trouvée et 0 sinon */
static int getExtension(char *path, char *ext);
/* Ecrit dans ct le type mime correspondant a l'extension contenue dans ext 
   et change la valeur de l'index (voir global.h) si necessaire,
   Retourne 1 si le type mime est connu et correctement modifié et 0 sinon */
static int setContentType(char *ext, char *ct, int *index);


void getContentType(request_t *requ, reponse_t *rep, struct stat *file_stat)
{
  if(isDir(file_stat)){
    strcpy(rep->content_type,"text/html");
    if(!indexExistInDir(requ->path)){
      requ->index = 1;
    }
    return;
  }

  if(isInCgiDir(requ->path)){
    if(isExecutable(file_stat)){
      strcpy(rep->content_type,"text/html");
      requ->index = 2;
      return;
    }
  }

  char ext[8] = {0};
  if(getExtension(requ->path,ext)){
    if(!setContentType(ext,rep->content_type,&requ->index)){
      strcpy(rep->content_type,"text/html");
    }
  }else{
    strcpy(rep->content_type,"text/html");
  }
}

static int indexExistInDir(char *path)
{
  int i = 0;
  FILE *f;
  buffer_t index_path = bufinit(NULL, BUFFER_SIZE, path, strlen(path));
  bufstrcat(&index_path,"/index.htm",10);
  if((f = fopen(index_path.ptr,"r")) != NULL){
    strncpy(path,index_path.ptr,index_path.siz);
    fclose(f);
    i = 1;
  }
  strncat(index_path.ptr,"l",1);
  if((f = fopen(index_path.ptr,"r")) != NULL){
    strncpy(path,index_path.ptr,index_path.siz);
    fclose(f);
    i = 1;
  }
  bufkill(&index_path);
  return i; 
}

static int isInCgiDir(char *path)
{
  return (strncmp(path+gl_www.use ,"/cgi-bin/",9) == 0);
}

int isDir(struct stat *file_stat)
{

  return ((file_stat->st_mode & S_IFDIR) == S_IFDIR);
}

static int getExtension(char *path, char *ext)
{
  char *point = NULL;
  if((point = strrchr(path,'.')) != NULL){
    if(point-1 == strrchr(path,'/'))
      return 0;
    strncpy(ext,point+1,strlen(point+1));
    return 1;
  }else{
    return 0;
  }
}

/* ext = extension, ct = Content-Type */
static int setContentType(char *ext, char *ct, int *index)
{
  if(!strcmp(ext,"htm")){
    strcpy(ct,"text/html");
    return 1;
  }else if(!strcmp(ext,"cpp")){
    strcpy(ct,"text/html");
    *index = 3;
    return 1;
  }else if(!strcmp(ext,"gif")){
    strcpy(ct,"image/gif");
    return 1;
  }else if(!strcmp(ext,"svg")){
    strcpy(ct,"image/png");
    return 1;
  }else if(!strcmp(ext,"png") || !strcmp(ext,"ico")){
    strcpy(ct,"image/png");
    return 1;
  }else if(!strncmp(ext,"jp",2)){
    strcpy(ct,"image/jpeg");
    return 1;
  }else if(!strcmp(ext,"pdf")){
    strcpy(ct,"application/pdf");
    return 1;
  }else if(!strcmp(ext,"txt") || !strcmp(ext,"c") || !strcmp(ext,"h")){
    strcpy(ct,"text/plain");
    return 1;
  }
  return 0;
}
