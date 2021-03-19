#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#define CARTE 0
#define SCENARIO 1
#define DEMARRAGE 2
#define DEMARREES 3
#define ENTRER 4
#define MAX_CHAR 20

/*pour 0, 1 et 3*/
typedef struct demande_simple_t{
  int type;
} demande_simple;

typedef struct demande_demarrage_t{
  int type;
  char carte[MAX_CHAR];
  char scenario[MAX_CHAR];
} demande_demarrage;

typedef struct demande_entrer_t{
  int type;
  int port;
} demande_entrer; /* n'implique pas de réponse de la part du serveur*/

/* pour 0 et 1*/
typedef struct reponse_simple_t{
  int type;
  int nb_fichiers;
  char** noms_fichiers; /* nb_fichiers * MAX_CHAR char*/
} reponse_simple;

typedef struct reponse_demarrage_t{
  int type;
  int port;
} reponse_demarrage;

typedef struct reponse_demarrees_t{
  int type;
  /* les parties déjà commencées*/
} reponse_demarrees;

#endif

