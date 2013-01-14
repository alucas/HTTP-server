#ifndef GETCONTENTTYPE_H
#define GETCONTENTTYPE_H

#include "requete.h"
#include "global.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

extern void getContentType(request_t *requ, reponse_t *rep, struct stat *file_stat);

extern int isDir(struct stat *file_stat);

#endif
