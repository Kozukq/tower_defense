#include "jeu.h"
#include <string.h>

void initialiser_plateau(jeu_t * jeu) {
  int i, j;
  /*La grille devra être lue depuis le serveur*/
  int grille[15][15] ={
    {   0,   1,   0,   2,   0,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
    {   0,   4,   5,   6,   5,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
    {   0,   0,   0,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
    {   0,   0,   0,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,   0 },
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  19,   0 },
    {   0,   0,   0,   0,   0,   0,   0,  26,  25,  24,  23,  22,  21,  20,   0 },
    {   0,   0,   0,   0,   0,   0,   0,  27,   0,   0,   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,  41,  42,  43, 255,   0,   0,   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,  40,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,  39,   0,   0,  24,  23,  22,   0,  16,  15,  14,   0 },
    {   0,  35,  36,  37,  38,   0,   0,  25,   0,  21,   0,  17,   0,  13,   0 },
    {   0,  34,   0,   0,   0,  28,  27,  26,   0,  20,  19,  18,   0,  12,   0 },
    {   0,  33,  32,  31,  30,  29,   0,   0,   0,   0,   0,   0,   0,  11,   0 },
    {   0,   0,   0,   0,   0,   0,   0,   4,   5,   6,   7,   8,   9,  10,   0 },
    {   0,   0,   0,   0,   0,   0,   0, 254,   0,   0,   0,   0,   0,   0,   0 }};
  jeu -> argent = ARGENT_DEFAUT;
  jeu -> vies = VIE_DEFAUT;
  jeu -> freeze = FREEZE_DEFAUT;
  jeu -> unfreeze = UNFREEZE_DEFAUT;
  for(i = 0; i < 3; i = i+1){
    jeu -> adv[i] = VIE_DEFAUT;
  }
  
  for (i = 0; i < NB_LIG_CARTE; i++) {	/* Initialisation des cases du plateau */
    for (j = 0; j < NB_COL_CARTE; j++) {
      case_t case_plateau;
      case_plateau.element = grille[i][j];
      /*case_plateau.unite = NULL;*/
      pthread_mutex_init(&case_plateau.mutex, NULL);
      jeu -> carte[i][j] = case_plateau;
    }
  }
}

void * thread_tour(void * arg){
  pthread_exit(NULL);
}

void * thread_unite(void * arg){
  pthread_exit(NULL);
}

void creation_thread(int type_unite, jeu_t * jeu, int posY, int posX){
  /*int status;
  pthread_mutex_lock(&jeu->carte[posY][posX].mutex);
  status = pthread_create(jeu->carte[posY][posX].unite, NULL, thread_tour, NULL);
  pthread_mutex_unlock(&jeu->carte[posY][posX].mutex);
  if(status != 0){
    fprintf(stderr,"\nErreur lors de la création d'un thread");
    perror("PTHREAD_CREATE");
    exit(-1);
    }*/
}
