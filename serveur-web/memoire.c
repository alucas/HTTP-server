#include <stdio.h>
#include <stdlib.h>

#include "memoire.h"

#ifndef NDEBUG

static int niveau = 0;
static bool trace = true;

static void tracer(char type, void *p){
  if(trace)
    printf(" [memoire %c : %2d (%p)]\n", type, niveau, p);
}

void *memoire_allouer(size_t size){
  if(size == 0){
    fprintf(stderr, "alocation de 0octets\n");
    return NULL;
  }

  void *p = malloc(size);
  
  if(p != NULL){
    niveau++;
    tracer('+', p);
  }
  
  return p; 
}

void *memoire_reallouer(void *p, size_t size){
  void *q = realloc(p, size);

  if(p == NULL && q != 0){
    niveau++;
    tracer('+', q);
    return q;
  }

  if(p != NULL && size == 0){
    fprintf(stderr, "free via un realloc\n");
    niveau--;
    tracer('-', q);
    return q;
  }

  tracer(' ', q);
  return q;
}

void memoire_liberer(void *p){
  if(p == NULL){
    fprintf(stderr, "free de NULL\n");
    return;
  }

  free(p);

  niveau--;
  tracer('-', p);
}

int memoire_balance(){
  return niveau;
}

void memoire_trace(bool valeur){
  trace = valeur;
}

#endif /* NDEBUG */
