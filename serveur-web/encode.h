#ifndef ENCODE_H
#define ENCODE_H

#include "buffer.h"
#include "requete.h"
#include "global.h"

extern void encoder(buffer_t *output, reponse_t *rep, request_t *req);

char *day[7];
char *mon[12];

#endif
