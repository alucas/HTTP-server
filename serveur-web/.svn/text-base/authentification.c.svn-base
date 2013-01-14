#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include "authentification.h"

#define BUFFER_SIZE 1024

/* Ecrit la partie a droite de Basic dans code,
   Retourne 1 si un code a été trouvé et 0 sinon */
static int getCode(char *p,char *buffer);
/* Decrypte le code et ecrit le resultat dans result,
   Retourne 1 si la fonction s'est executée sans erreur et 0 sinon */
static int decryptCode(char *code, char *result);
/* Separe le login et le password lus dans result */
static void separerLoginEtPass(char *result, char *login, char *password);
static int verifierIdentifiants(char *source, char *identifiants);


int getAuthorization(char *p, char *login, char *password)
{
  char code[30] = {0};
  char result[50] = {0};

  if(!getCode(p, code)){
    return 0;
  }

  if(!decryptCode(code, result)){
    return 0;
  }

  separerLoginEtPass(result, login, password);

  return 1;
}


int authentifier(char *login, char *password, char *pass_path)
{
  FILE *f;
  char identifiants[50] = {0};
  char fichier[BUFFER_SIZE] = {0};
  int size;
  
  sprintf(identifiants,"%s:%s",login,password);

  if((f = fopen(pass_path,"a+")) != NULL){
    size = fread(fichier,sizeof(char),BUFFER_SIZE,f);

    do{
      if(verifierIdentifiants(fichier, identifiants)){
	return 1;
      }

      size = fread(fichier,sizeof(char),BUFFER_SIZE,f);
    }while(!feof(f) && !ferror(f));

    if(ferror(f)){
      perror("fread");
    }

    fclose(f);
  }else{
    perror("fopen");
  }
  return 0;
}

int getLoginAndPassword(char *p, char *login, char *password)
{
  char *current = p;
  char *egal = NULL;
  char *et = NULL;
  while(1){
    egal = strchr(current,'=');
    et = strchr(current,'&');
    if(!strncmp(current,"login",5)){
      strncpy(login, egal+1, strlen(et) - strlen(egal) - 1);
    }else if(!strncmp(current,"password",8)){
      strncpy(password, egal+1, strlen(et) - strlen(egal) - 1);
    }
    if(et != NULL)
      current = et+1;
    else
      break;
  }  
  return strlen(login)*strlen(password);
}

void getPassFilePath(char *path, char *pass_path, int isDir)
{
  if(!isDir){
    char *pos = strrchr(path,'/');
    strncpy(pass_path,path,strlen(path) - strlen(pos));
  }else{
    strncpy(pass_path,path,strlen(path)+1);
    strcat(pass_path,"/");
  }
  strcat(pass_path,".htpasswd");
}

int isInAuthDir(char *path)
{
  return (strncmp(path+gl_www.use ,"/auth",5) == 0);
}

int passFileExistInDir(char *path)
{
  int i = 0;
  FILE *f;
  buffer_t index_path = bufinit(NULL, BUFFER_SIZE, path, strlen(path));
  bufstrcat(&index_path,"/.htpasswd",10);
  if((f = fopen(index_path.ptr,"r")) != NULL){
    fclose(f);
    i = 1;
  }
  bufkill(&index_path);
  return i; 
}

static int getCode(char *p,char *buffer)
{
  int i = 0;
  while(p[i] != '\0'){
    if(p[i] != ' '){
      if(!strncmp(&p[i],"Basic",5)){
	i+=5;

	while(p[i] == ' ' && p[i] != '\0'){
	  i++;
	}

	strncpy(buffer,&p[i],strlen(&p[i]));
      }
      return 1;
    }
    i++;
  }
  return 0;
}

static int verifierIdentifiants(char *source, char *identifiants)
{
  char *current = source;
  char *end_line = strchr(source, '\n');
  int id_size = strlen(identifiants);
  int cur_size;
  while(1){
    cur_size = end_line - current;

    if(id_size == cur_size){
      if(strncmp(current,identifiants,cur_size-1) == 0){
	return 1;
      }
    }

    if(end_line == NULL)
      break;

    current = end_line+1;
    end_line = strchr(current, '\n');
  }
  return 0;
}

static int decryptCode(char *code, char *result)
{
  int in[2];
  int out[2];

  if(pipe(in) == -1 || pipe(out) == -1){
    perror("pipe");
    return 0;
  }

  int size;
  if((size = write(in[1],code,strlen(code))) == -1){
    perror("write");
    return 0;
  }
  close(in[1]);

  int ret = fork();

  switch(ret){
  case -1 : //erreur
    perror("fork");
    return 0;

  case 0 : //fils
    dup2(in[0],STDIN_FILENO);
    close(in[0]);
    close(out[0]);

    dup2(out[1],STDOUT_FILENO);
    close(out[1]);

    execlp("base64","base64","-d",NULL);
    perror("execlp");
    exit(EXIT_FAILURE);
  }
  //pere
  size = 0;
  while((size = read(out[0], result + size, 50)) == 0){
    if(size == -1){
      perror("read");
      return 0;

      while((size = read(out[0], result + size, 50)) != 0){
	if(size == -1){
	  perror("read");
	  return 0;
	}
      }
    }
  }
  close(in[0]);
  close(out[0]);
  close(out[1]);

  waitpid(ret,NULL,0);
  return 1;
}

static void separerLoginEtPass(char *result, char *login, char *password)
{
  int i = 0;
  while(result[i] != ':')
    i++;
  strncpy(login,result,i);
  strncpy(password,result+i+1,strlen(result+i+1));
}
