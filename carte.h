#ifndef CARTE_H
#define CARTE_H

#define TAILLE_LIGNE_CARTE 15

typedef struct carte_t{
  size_t taille;
  char * description;
  unsigned int plateau[TAILLE_LIGNE_CARTE][TAILLE_LIGNE_CARTE];
} carte;

#endif
