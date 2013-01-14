#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "buffer.h"

buffer_t bufinit(buffer_t *dst, int size, char *str, int sizeStr){
  if(dst == NULL){
    buffer_t buf = {NULL, 0, 0};
    dst = &buf;
  }

  if(sizeStr > size){
    sizeStr = size;
  }

  bufsize(dst, size);
  bufstrcpy(dst, str, sizeStr);

  return *dst;
}

int bufsize(buffer_t *dst, int size){
  assert(dst != NULL && size >= 0);

  if(size != dst->siz){
    dst->ptr = memoire_reallouer(dst->ptr, (size+1)*sizeof(char));
    dst->siz = size;
    
    if(dst->use > size){
      dst->use = size;
    }
  }

  return 0;
}

int bufcpy(buffer_t *dst, buffer_t *src){
  assert(dst != NULL && src != NULL && dst->ptr != NULL && src->ptr != NULL);

  while(dst->siz < src->use){
    bufsize(dst, 2*dst->siz);
  }

  strncpy(dst->ptr, src->ptr, src->use);
  dst->use = src->use;
  dst->ptr[dst->use] = '\0';

  return 0;
}

int bufcat(buffer_t *dst, buffer_t *src){
  assert(dst != NULL && src != NULL && dst->ptr != NULL && src->ptr != NULL);

  while(dst->siz < dst->use + src->use){
    bufsize(dst, 2*dst->siz);
  }

  strncat(dst->ptr, src->ptr, src->use);
  dst->use += src->use;
  dst->ptr[dst->use] = '\0';

  return 0;
}

int bufstrcpy(buffer_t *dst, char *src, int size){
  assert(dst != NULL && src != NULL);

  while(dst->siz < dst->use + size){
    bufsize(dst, 2*dst->siz);
  }

  strncpy(dst->ptr, src, size);
  dst->use = size;
  dst->ptr[dst->use] = '\0';

  return 0;
}

int bufstrcat(buffer_t *dst, char *src, int size){
  assert(dst != NULL && src != NULL);

  while(dst->siz < dst->use + size){
    bufsize(dst, 2*dst->siz);
  }

  strncat(dst->ptr, src, size);
  dst->use += size;
  dst->ptr[dst->use] = '\0';

  return 0;
}
