#ifndef MEMOIRE_H
#define MEMOIRE_H

#include <stddef.h>
#include <stdbool.h>

#ifndef NDEBUG

extern void *memoire_allouer(size_t size);
extern void *memoire_reallouer(void *p, size_t size);
extern void memoire_liberer(void *p);
extern int memoire_balance();
extern void memoire_trace(bool valeur);

#else

#define memoire_allouer(size) (malloc(size))
#define memoire_reallouer(p, size) (realloc((p), (size)))
#define memoire_liberer(p) (free(p))
#define memoire_balance() ((void *)NULL)
#define memoire_trace(valeur) ((void *)NULL)

#endif /* NDEBUG */

#endif  /* MEMOIRE */
