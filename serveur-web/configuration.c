#define _GNU_SOURCE 

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#include "global.h"
#include "buffer.h"
#include "configuration.h"

#define NB_VALUES 2 // doit etre >= 2 {comand, value1, ...}
#define NB_ALIAS 10

// bool pour desactiver les comandes
bool b_gl_port = false;
bool b_gl_mode = false;
bool b_gl_close = false;
bool b_gl_www = false;

static int setComand(char *comand, buffer_t values[], int nbValues){
  // modification du port
  if(!strcmp(comand, "port")){
    if(b_gl_port){
      return 0;
    }

    for(int i=0; i<nbValues; i++){
      // conversion numerique
      char *ptr = NULL;
      int res =  strtol(values[i].ptr, &ptr, 10);
      if(res > 0 && res < 65536 && ptr != values[i].ptr){
	gl_port = res;
	return 0;
      }
      
      // resolution de nom
      if(values[i].use >= 3){
	struct servent *serv = getservbyname(values[i].ptr, "tcp");
	if(serv != NULL){
	  gl_port = ntohs(serv->s_port);
	  return 0;
	}
	
	if(errno != 0){
	  perror("getservbyname");
	}
      }
    }

    fprintf(stderr, "%s: valeur de la commande invalide\n", comand);
    return 1;
  }

  // modification du mode
  if(!strcmp(comand, "mode")){
    if(b_gl_mode){
      return 0;
    }

    if(values[0].use == 0){
      fprintf(stderr, "%s: valeur null\n", comand);
      return 1;
    }

    if(!strncmp(values[0].ptr, "thread", 7)){
      gl_mode = 1;
      return 0;
    }else if(!strncmp(values[0].ptr, "select", 7)){
      gl_mode = 2;
      return 0;
    }

    fprintf(stderr, "%s: valeur de la commande invalide\n", comand);
    return 1;
  }

  // modification du temps de latence
  if(!strcmp(comand, "latence")){
    if(b_gl_close){
      return 0;
    }

    if(values[0].use == 0){
      fprintf(stderr, "%s: valeur null\n", comand);
      return 1;
    }

    char *ptr;
    int res =  strtol(values[0].ptr, &ptr, 10);
    if(res >= 0 && ptr != values[0].ptr){
      gl_close = res;
      return 0;
    }
    
    fprintf(stderr, "%s: valeur de la commande invalide\n", comand);
    return 1;
  }

  // modification de l'identifiant du serveur
  if(!strcmp(comand, "server")){
    if(values[0].use == 0){
      fprintf(stderr, "%s: valeur null\n", comand);
      return 1;
    }

    if(strchr(values[0].ptr, ':') != NULL){
      fprintf(stderr, "le caractere ':' est interdit dans l'indentifiant du serveur\n");
      return 1;
    }

    bufcpy(&gl_name, values);
    return 0;
  }

  // test des dossiers
  if(!strcmp(comand, "rootpath")){
    if(b_gl_www){
      return 0;
    }

    if(values[0].use == 0){
      fprintf(stderr, "%s: valeur null\n", comand);
      return 1;
    }
    
    int rep = open(values[0].ptr, O_DIRECTORY);
    if(rep != -1){
      close(rep);
      
      bufcpy(&gl_www, values);
      return 0;
    }

    perror("opendir");
    fprintf(stderr, "%s: valeur de la commande invalide\n", comand);
    return 1;
  }

  // test des fichiers
  if(!strcmp(comand, "errorlog") || !strcmp(comand, "accesslog")){
    if(values[0].use == 0){
      fprintf(stderr, "%s: valeur null\n", comand);
      return 1;
    }
    
    int file = open(values[0].ptr, 0);
    if(file != -1){
      close(file);
      
      if(!strcmp(comand, "errorlog")){
	bufcpy(&gl_logError, values);
      }else if(!strcmp(comand, "errorlog")){
	bufcpy(&gl_logAccess, values);
      }
      return 0;
    }

    perror("open");
    fprintf(stderr, "%s: valeur de la commande invalide\n", comand);
    return 1;
  }
  
  fprintf(stderr, "(Warning) %s: commande inconue\n", comand);
  return 0;
}

static void usage(char *comand){
  printf("\e[1musage:\e[0m\n"				\
	 "\t%s [-h]\n\n", comand);
  printf("\e[1moptions:\e[0m\n"						\
	 "\t-h, --help            | liste des commandes\n"		\
	 "\t    --port=<port>     | port d'écoute\n"			\
	 "\t    --rootpath=<path> | racine des fichiers\n"		\
	 "\t-t, --mode=thread     | mode d'execution du serveur\n"	\
	 "\t-s,       =select     |\n"					\
	 "\t    --latence=<temps> | temps de connexion max des clients\n" \
	 "\t                      | 0 = illimité\n"			\
	 "\n");
}

int confFile(char *fileAddr){
  FILE *file = fopen(fileAddr, "r");
  if(file == NULL){
    perror("fopen");
    return 1;
  }

  int size = 0;
  buffer_t buffer = bufinit(NULL, BUFFER_SIZE, "", 0);

  buffer_t *values = memoire_allouer(NB_VALUES * sizeof(buffer_t));
  for(int i=0; i<NB_VALUES; i++){
    values[i] = bufinit(NULL, BUFFER_SIZE, "", 0);
  }

  int nbValues = NB_VALUES;
  int valueId = 0;
  int valueCur = 0;

  bool comentaire = false;
  bool ligneValide = true;
  bool carVide = false;
  bool phrase = false;

  int ligne = 1;
  int nbErreur = 0;

  while((size = fread(buffer.ptr, sizeof(char), buffer.siz-1, file)) != 0){
    for(int i=0; i<size; i++){
      if(buffer.ptr[i] == '\n'){
	// '\0' pour la derniere valeur
	values[valueId].ptr[valueCur] = '\0';
	values[valueId].use = valueCur;
	if(ligneValide && valueId > 0){
	  
	  /*
	  for(int j=0; j<valueId+1; j++){
	    printf("%s, ", values[j].ptr);
	  }
	  printf("\n");
	  // */

	  if(setComand(values[0].ptr, values+1, valueId)){
	    nbErreur++;
	    fprintf(stderr, "%s, ligne %d: comande invalide\n", fileAddr, ligne);
	  }
	}
	
	comentaire = false;
	ligneValide = true;
	carVide = false;
	phrase = false;

	valueCur = 0;
	valueId = 0;
	for(int j=0; j<nbValues; j++){
	  values[j].ptr[0] = '\0';
	}
	
	ligne++;

	continue;
      }

      // si ce n'est pas un commentaire
      if(!comentaire){
	// caractere de commentaire
	if(buffer.ptr[i] == '#'){
	  comentaire = 1;
	  continue;
	}
	
	if(!phrase && (buffer.ptr[i] == ' ' || buffer.ptr[i] == '\t' || buffer.ptr[i] == '\0')){
	  carVide = true;
	  continue;
	}

	// passer a la valeur suivante
	if(carVide && values[0].ptr[0] != '\0'){
	  values[valueId].ptr[valueCur] = '\0';
	  values[valueId].use = valueCur;
	  valueId++;
	  valueCur = 0;
	  if(valueId == nbValues){
	    nbValues *= 2;
	    values = memoire_reallouer(values, nbValues * sizeof(buffer_t));
	    
	    for(int j=valueId; j<nbValues; j++){
	      values[i].ptr = memoire_allouer((BUFFER_SIZE+1)*sizeof(char));
	      values[i].ptr[0] = '\0';
	      values[i].use = 0;
	      values[i].siz = BUFFER_SIZE;
	    }
	  }
	}
	
	carVide = false;

	// debut ou fin d'une chaine
	if(buffer.ptr[i] == '"'){
	  phrase = !phrase;
	  continue;
	}

	// buffer trop petit
	if(valueCur >= values[valueId].siz){
	  values[valueId].ptr = memoire_reallouer(values[valueId].ptr, (2*values[valueId].siz+1)*sizeof(char));
	  values[valueId].siz *= 2;
	}
	
	values[valueId].ptr[valueCur] = buffer.ptr[i];
	valueCur++;
      }
    }
  } 
  
  fclose(file);

  for(int i=0; i<nbValues; i++){
    bufkill(values+i);
  }
  memoire_liberer(values);

  bufkill(&buffer);

  return nbErreur;
}


int confParams(int argc, char *argv[]){
  int alias[NB_ALIAS] = {0};
  for(int i=1; i<argc; i++){
    if(argv[i][0] == '-' && argv[i][1] != '\0'){
      // traitement des alias
      if(argv[i][1] != '-'){
	//for(int cur=1; argv[i][cur] != '\0'; cur++){
	  // liste des parametre sans argument
	//}

	if(argv[i][1] == 'h' && argv[i][2] == '\0'){
	  alias[0] = true;
	}

	if(argv[i][1] == 't' && argv[i][2] == '\0'){
	  alias[1] = true;
	}

	if(argv[i][1] == 's' && argv[i][2] == '\0'){
	  alias[2] = true;
	}
      }

      // traitement des commandes
      if(alias[0] || !strcmp(argv[i]+2, "help")){
	usage(argv[0]);
	return 1;
      }

      if(!strncmp(argv[i]+2, "port", 4)){
	if(argv[i][6] == '='){
	  argv[i] += 7;
	  int t = strlen(argv[i]);
	  buffer_t buf = {argv[i], t+1, t};
	  if(setComand("port", &buf, 1) == 1){
	    return 1;
	  }
	  
	  b_gl_port = true;
	  continue;
	}else{
	  fprintf(stderr, "parametre obligatoire pour la commande --port\n");
	  return 1;
	}
      }

      if(!strncmp(argv[i]+2, "rootpath", 8)){
	if(argv[i][10] == '='){
	  argv[i] += 11;
	  int t = strlen(argv[i]);
	  buffer_t buf = {argv[i], t+1, t};
	  if(setComand("rootpath", &buf, 1) == 1){
	    return 1;
	  }

	  b_gl_www = true;
	  continue;
	}else{
	  fprintf(stderr, "parametre obligatoire pour la commande --roorpath\n");
	  return 1;
	}
	continue;
      }
      
      // modification du mode (thread ou select)
      if(alias[1] || alias[2]){
	if(alias[1]){
	  char buffer[] = "thread";
	  buffer_t buf = {buffer, sizeof(buffer), sizeof(buffer)-1};
	  setComand("mode", &buf, 1);
	}
	
	if(alias[2]){
	  char buffer[] = "select";
	  buffer_t buf = {buffer, sizeof(buffer), sizeof(buffer)-1};
	  setComand("mode", &buf, 1);
	}
	
	b_gl_mode = true;
	continue;
      }

      if(!strncmp(argv[i]+2, "mode", 4)){
	if(argv[i][6] == '='){
	  argv[i] += 7;
	  int t = strlen(argv[i]);
	  buffer_t buf = {argv[i], t+1, t};
	  if(setComand("mode", &buf, 1) == 1){
	    return 1;
	  }

	  b_gl_mode = true;
	  continue;
	}else{
	  fprintf(stderr, "parametre obligatoire pour la commande --mode\n");
	  return 1;
	}
	continue;
      }

      if(!strncmp(argv[i]+2, "latence", 7)){
	if(argv[i][9] == '='){
	  argv[i] += 10;
	  int t = strlen(argv[i]);
	  buffer_t buf = {argv[i], t+1, t};
	  if(setComand("latence", &buf, 1) == 1){
	    return 1;
	  }

	  b_gl_close = true;
	  continue;
	}else{
	  fprintf(stderr, "parametre obligatoire pour la commande --latence\n");
	  return 1;
	}
	continue;
      }
    }

    fprintf(stderr, "parametre %d inconu : %s\n\e[1mhelp:\e[0m %s --help\n", i, argv[i], argv[0]);
    return 1;
  }

  return 0;
}
