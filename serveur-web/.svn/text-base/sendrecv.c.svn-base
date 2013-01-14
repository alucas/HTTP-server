#define _GNU_SOURCE

#include "global.h"
#include "sendrecv.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <sys/wait.h>

#define SIZE_REQUETE 1<<14
#define PATH_ERROR "./var/erreur/"

static char b1[] = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\r\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\r\n  <head>\r\n    <title>";
static char b2[] = "</title>\r\n    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\r\n  </head>\r\n  <body>\r\n    <table>\r\n";
static char b3[] = "      <tr><td><img src=\"/icons/";
static char b4[] = "\" height=\"24px\" width=\"24px\" /></td><td><a href=\"./";
static char b5[] = "\">";
static char b6[] = "</a></td></tr>\r\n";
static char b7[] = "    </table>\r\n  </body>\r\n</html>";

static char bi[] = "parent directory/";
static char bd[] = "folder.svg\" alt=\"[DIR]";
static char bf[] = "file.svg\" alt=\"[FIL]";

static int sendBuffer(buffer_t *buf, int sock){
  assert(buf != NULL && buf->ptr != NULL);
  
  int size = BUFFER_SIZE;
  for(int i=0; i<buf->use; i += BUFFER_SIZE){
    if(i + size > buf->use){
      size = buf->use - i;
    }
    
    if(send(sock, buf->ptr + i, size, MSG_MORE) == -1){
      perror("send");
      return 500; // etudier le code a retourner
    }
  } 
  
  return 0;
}


static int sendDir(buffer_t *path, int sock){
  assert(path != NULL && path->ptr != NULL);
  
  DIR* dir = opendir(path->ptr);
  if(dir == NULL){
    perror("opendir");
    return 404; // etudier le code de retour
  }
  
  buffer_t buffer = bufinit(NULL, BUFFER_SIZE, b1, sizeof(b1)-1);
  
  bufcat(&buffer, path);
  bufstrcat(&buffer, b2, sizeof(b2)-1);
  
  if(strcmp(gl_www.ptr, path->ptr) != 0){ // a coriger
    bufstrcat(&buffer, b3, sizeof(b3)-1);
    bufstrcat(&buffer, bd, sizeof(bd)-1);
    bufstrcat(&buffer, b4, sizeof(b4)-1);
    bufstrcat(&buffer, "../", 3);
    bufstrcat(&buffer, b5, sizeof(b5)-1);
    bufstrcat(&buffer, bi, sizeof(bi)-1);
    bufstrcat(&buffer, b6, sizeof(b6)-1);
  }
  
  int len = offsetof(struct dirent, d_name) + pathconf(path->ptr, _PC_NAME_MAX) + 1;
  struct dirent *result;
  struct dirent *file = memoire_allouer(len);
  
  while(readdir_r(dir, file, &result) == 0 && result != NULL){
    if(file->d_name[0] != '.'){
      int size = strlen(file->d_name);
      
      bufstrcat(&buffer, b3, sizeof(b3)-1);
      if(file->d_type == DT_DIR){
	bufstrcat(&buffer, bd, sizeof(bd)-1);
      }else{
	bufstrcat(&buffer, bf, sizeof(bf)-1);
      }
      
      bufstrcat(&buffer, b4, sizeof(b4)-1);
      bufstrcat(&buffer, file->d_name, size);
      if(file->d_type == DT_DIR)
	bufstrcat(&buffer, "/", 1);
      bufstrcat(&buffer, b5, sizeof(b5)-1);
      bufstrcat(&buffer, file->d_name, size);
      if(file->d_type == DT_DIR)
	bufstrcat(&buffer, "/", 1);
      bufstrcat(&buffer, b6, sizeof(b6)-1);
    }
  }
  
  bufstrcat(&buffer, b7, sizeof(b7)-1);
  
  char buf[30];
  sprintf(buf, "Content-Length: %d\r\n\r\n", buffer.use);
  
  send(sock, buf, strlen(buf), MSG_MORE);
  send(sock, buffer.ptr, buffer.use, 0);
  
  memoire_liberer(file);
  bufkill(&buffer);
  closedir(dir);
  return 0;
}

static int sendFile(buffer_t *path, int sock){
  assert(path != NULL && path->ptr != NULL);
  
  FILE* file = fopen(path->ptr, "r");
  if(file == NULL){
    perror("fopen");
    return 404; // etudier le code de retour
  }
  
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  char buffer[BUFFER_SIZE+1] = {0};

  sprintf(buffer, "Content-Length: %d\r\n\r\n", size);

  send(sock, buffer, strlen(buffer), MSG_MORE);

  fseek(file, 0, SEEK_SET);
  while((size = fread(buffer, sizeof(char), BUFFER_SIZE, file)) != 0){
    if(ferror(file)){
      perror("fread");
      return -1;
    }

    if(send(sock, buffer, size, MSG_MORE) == -1){
      perror("send");
      return -1;
    }
  } 
 
  fclose(file);

  return 0;
}

static int sendEXE(char *path, bool relativPath, bool isCGI, int sock, char *var, int nbrParam, ...){
  assert(path != NULL);
  assert(sock >= 0);
  assert(nbrParam >= 0);
  
  int tab[2];
  if(pipe(tab) == -1){
    perror("pipe");
    return 500; // etudier le code de retour
  }

  int ret = fork();
  if(ret == -1){
    perror("fork");
    close(tab[0]);
    close(tab[1]);
    return 500;
  }else if(ret == 0){ // fils
    close(tab[0]);
    dup2(tab[1], STDOUT_FILENO);
    close(tab[1]);
    
    va_list ap;
    va_start(ap, nbrParam);
    
    char *tab[nbrParam+1];
    for(int i=0; i<nbrParam; i++){
      tab[i] = va_arg(ap, char*);
    }
    tab[nbrParam] = NULL;
    
    va_end(ap);

    setenv("QUERY_STRING", var, 1);

    if(relativPath){
      execvp(path, tab);
    }else{
      execv(path, tab);
    }
    perror("execv");
    exit(EXIT_FAILURE);
  }
  
  close(tab[1]);
  
  int size;
  int retour = 0;
  buffer_t buffer = bufinit(NULL, BUFFER_SIZE, "", 0);
  while((size = read(tab[0], buffer.ptr + buffer.use, BUFFER_SIZE)) != 0){
    if(size == -1){
      perror("read");
      retour = -1;
      break;
    }
    
    buffer.use += size;
    if(buffer.use + BUFFER_SIZE > buffer.siz){
      bufsize(&buffer, 2*buffer.siz);
    }
  }
  
  size = buffer.use;
  char buf[30];
  char *pos;
  //if(isCGI && (((pos = strstr(buffer.ptr, "\r\n\r\n")) != NULL) || ((pos = strstr(buffer.ptr, "\n\n")) != NULL))){
  if(isCGI && (pos = strstr(buffer.ptr, "\r\n\r\n")) != NULL){
    size -= (pos - buffer.ptr) + 4;
    sprintf(buf, "Content-Length: %d\r\n", size);
  }else{
    sprintf(buf, "Content-Length: %d\r\n\r\n", size);
  }
  
  send(sock, buf, strlen(buf), MSG_MORE);
  send(sock, buffer.ptr, buffer.use, 0);
  
  waitpid(ret, NULL, 0);
  
  bufkill(&buffer);
  close(tab[0]);
  return retour;
}

int sendErreur(buffer_t *reponse, int code, int sock){
  char tmp[] = PATH_ERROR"000";
  buffer_t path = {tmp, sizeof(tmp)-1, sizeof(tmp)-1};
  
  sendBuffer(reponse, sock); // tester le code de retour
  
  switch(code){
  case 401:
  case 400:
  case 403:
  case 404:
  case 413:
  case 414:
  case 500:
  case 501:
    snprintf(tmp + sizeof(tmp)-4, 4, "%d", code);
    
    // il faudrai envoyer l'entete d'erreur ici aussi
    sendFile(&path, sock);
    
    break;
  default:
    fprintf(stderr, "Erreur %d non suportée\n", code);
    return -1;
  }

  return 0;
}

/* Je ne sait pas encore vraiment comment exploiter le QUERY_STRING */
int sendReponse(buffer_t *reponse, buffer_t *path, int sock, int index, buffer_t *var, char *encoding){

#ifndef NDEBUG
  printf("file '%s'\n", path->ptr);
#endif

  if(sendBuffer(reponse, sock) == 0){
    switch(index){
    case 0:
      if(!strcmp(encoding, "gzip")){
	printf("send file (socket %d, gzip)\n", sock);
	send(sock, "Content-Encoding: gzip\r\n", 24, MSG_MORE);
	sendEXE("gzip", true, false, sock, var->ptr, 3, "gzip", "-c", path->ptr);
      }else if(!strcmp(encoding, "bzip2")){
	printf("send file (socket %d, bzip2)\n", sock);
	send(sock, "Content-Encoding: bzip2\r\n", 25, MSG_MORE);
	sendEXE("bzip2", true, false, sock, var->ptr, 3, "bzip2", "-c", path->ptr);
      }else{
	printf("send file (socket %d)\n", sock);
	sendFile(path, sock);
      }
      break;
    case 1:
      printf("send folder (socket %d)\n", sock);
      sendDir(path, sock);
      break;
    case 2:
      printf("send executable (socket %d)\n", sock);
      sendEXE(path->ptr, false, true, sock, var->ptr, 0);
      break;
    case 3:
      printf("send .cpp (socket %d)\n", sock);
      sendEXE("cpp", true, false, sock, var->ptr, 3, "cpp", "-P", path->ptr);
      break;
    case 4:
      break;
    default:
      fprintf(stderr, "index %d inconu\n", index);
      return -1;
    }
  }

  return 0;
}

int readRequete(buffer_t *req, int sock, int *end, bool all){
  assert(req != NULL && req->ptr != NULL && req->siz > 0 && req->use >= 0 && req->use <= req->siz);

  char *pos = NULL;
  int size = 0;
  int recule = 0;
  int sizeRead = BUFFER_SIZE;
  int ret = 0;

  // si la chaine 'req' contien deja une requete
  req->ptr[req->use] = '\0';
  if((pos = strstr(req->ptr, "\r\n\r\n")) != NULL){
    *end = pos - req->ptr + 4;
    return ret;
  }

  while(1){
    // on verifie si le buffer est assez grand pour lire la requete
    if(req->use + sizeRead > req->siz){
      // si la taille maximum du buffer est ateinte
      if(2*req->siz > SIZE_REQUETE){
	// si le buffer est plein
	if(req->use >= SIZE_REQUETE){
	  ret = 413; // requete trop grande
	  // je recopie les 3dernier caractere
	  req->ptr[0] = req->ptr[req->use-3];
	  req->ptr[1] = req->ptr[req->use-2];
	  req->ptr[2] = req->ptr[req->use-1];
	  req->use = 3;
	}

	sizeRead = req->siz - req->use;
      }else{
	// on agrandi le buffer
	if(bufsize(req, 2*req->siz) != 0){
	  ret = 500; // erreur d'alocation memoire
	  break;
	}
      }
    }

    size = recv(sock, req->ptr + req->use, sizeRead, 0);

    if(size == -1){
      perror("recv");
      ret = 500; // erreur de lecture
      break;
    }
    
    // client déconecté et requete non finie
    if(size == 0){
      ret = -1; // deconexion du client
      break;
    }

    // verification de la terminaison de la requete
    // recherche du CRLFCRLF : '\r\n\r\n'
    if(req->use < 3){
      recule = req->use;
    }else{
      recule = 3;
    }
    
    req->ptr[req->use + size] = '\0';
    if((pos = strstr(req->ptr + req->use - recule, "\r\n\r\n")) != NULL){
      *end = pos - req->ptr + 4;
      // on sort de la boucle
      req->use += size;
      break;
    }
    
    req->use += size;

    // si la fonction ne demande qu'a lire un seul bloc
    if(all == false){
      if(ret == 0) ret = 100;
      break;
    }
  }

  req->ptr[req->use] = '\0';
  return ret; // tout vas bien, on continue
}
