#ifndef CASES_H
#define CASES_H

typedef struct case_t {
  int element;
  pthread_t *unite;
  pthread_mutex_t mutex;
} case_t;

/* Types de case -> element*/
#define CASE_VIDE         0
#define CASE_MIN_JOUEUR   1
#define CASE_MAX_JOUEUR   3
#define CASE_MIN_CHEMIN   4
#define CASE_MAX_CHEMIN 229
#define CASE_ORDI       254
#define CASE_FORT       255

/* Couleurs des cases */
#define COULEUR_VIDE      2
#define COULEUR_FORT      7
#define COULEUR_CHEMIN    3
#define COULEUR_JOUEUR    6
#define COULEUR_ORDI      6

#endif
