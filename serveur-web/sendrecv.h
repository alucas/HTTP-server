#ifndef SENDRECV_H
#define SENDRECV_H

#include "buffer.h"

#include <stdbool.h>

extern int sendErreur(buffer_t *reponse, int code, int sock);
// valeur d'index :
// 0 : file
// 1 : directory
// 2 : CGI
// 3 : CPP
// 4 : HEAD
extern int sendReponse(buffer_t *reponse, buffer_t *path, int sock, int index, buffer_t *var, char *encoding);
// lis sur le soket 'sock' et ecrit la valeur a la suite
// du contenu de 'req',  si 'req' est trop petit
// celui ci est agrandi
// code de retour possible :
//  -1 : la connexion du socket a ete interompu
//   0 : la chaine "\r\n\r\n" a ete trouvé
// 100 : si all == false, et que la chaine "\r\n\r\n" n'a pas ete trouvé
// 413 : la requete est trop grande
// 500 : erreur interne (realloc, recv, ...)
//
// BOGS : le protocol http 0.9 n'est pas reconu
extern int readRequete(buffer_t *req, int sock, int *end,  bool all);

#endif /* SENDRECV_H */
