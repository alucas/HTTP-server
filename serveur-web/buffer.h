#ifndef BUFFER_H
#define BUFFER_H

#include "memoire.h"

#define BUFFER_SIZE 1024
#define bufkill(x)  memoire_liberer((x)->ptr)

typedef struct buffer{
  char *ptr;
  int   siz;
  int   use;
} buffer_t;

// reinitialiser un buffer avec la chaine 'str' de taille 'sizeStr'
// on peut utiliser dst = NULL pour initiliser un buffer
extern buffer_t bufinit  (buffer_t *dst, int size, char *str, int sizeStr);
// modifier la taille du buffer
extern int      bufsize  (buffer_t *dst, int size);
extern int      bufcpy   (buffer_t *dst, buffer_t *src);
extern int      bufcat   (buffer_t *dst, buffer_t *src);
extern int      bufstrcpy(buffer_t *dst, char *src, int size);
extern int      bufstrcat(buffer_t *dst, char *src, int size);

#endif /* BUFFER_H */
