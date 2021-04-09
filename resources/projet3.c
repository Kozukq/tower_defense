/**
 * Ce programme propose un exemple d'interface pour le projet n°3.
 * @author Cyril Rabat
 **/

#define _POSIX_C_SOURCE 199506L
#include <stdlib.h>     /* Pour EXIT_FAILURE */

#include "ncurses.h"
#include "fenetre.h"
#include "jeu.h"
#include "interface.h"

freeze_p freeze = {FREEZE_DEFAUT, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
freeze_p unfreeze = {UNFREEZE_DEFAUT, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

interface_p interface;
jeu_p jeu;

/*Routine de création et de gestion d'interface*/
void * creation_interface(void * arg){
  int ch;
  bool quitter = FALSE;
  /* Initialisation de ncurses */
  ncurses_initialiser();
  ncurses_souris();
  ncurses_couleurs(); 
  palette();
  clear();
  refresh();  

  /* Vérification des dimensions du terminal */
  if((COLS < LARGEUR) || (LINES < HAUTEUR)) {
    ncurses_stopper();
    fprintf(stderr, 
	    "Les dimensions du terminal sont insufisantes : l=%d,h=%d au lieu de l=%d,h=%d\n", 
	    COLS, LINES, LARGEUR, HAUTEUR);
    exit(EXIT_FAILURE);
  }
  
  /* Création de l'interface*/
  pthread_mutex_lock(&interface.mutex);
  pthread_mutex_lock(&jeu.mutex);
  interface.interface = interface_creer(&jeu.jeu);
  pthread_mutex_unlock(&interface.mutex);
  pthread_mutex_unlock(&jeu.mutex);
  
  /* Boucle principale */
  while(quitter == FALSE) {
    ch = getch();
    if((ch == 'Q') || (ch == 'q'))
      quitter = true;
    else{
      pthread_mutex_lock(&interface.mutex);
      pthread_mutex_lock(&jeu.mutex);
      interface_main(&interface.interface, &jeu.jeu, ch);
      pthread_mutex_unlock(&interface.mutex);
      pthread_mutex_unlock(&jeu.mutex);
    }
  }
  
  /* Suppression de l'interface */
	pthread_mutex_lock(&interface.mutex);
  interface_supprimer(&interface.interface);
	pthread_mutex_unlock(&interface.mutex);  

  /* Arrêt de ncurses */
  ncurses_stopper();
  return (void*) NULL;
  
}

/*Fonction principale - Appelle le thread de création d'interface */
int main() {
  int status, retour;
  pthread_t thread_affichage;
  
  /*Création des mutex de l'interface et du jeu*/
  retour = pthread_mutex_init(&interface.mutex, NULL);
  if(retour != 0){
    fprintf(stderr,"Problème lors de l'initialisation du mutex de l'interface\n");
    exit(EXIT_FAILURE);
  }
  retour = pthread_mutex_init(&jeu.mutex, NULL);
  if(retour != 0){
    fprintf(stderr,"Problème lors de l'initialisation du mutex du jeu\n");
    exit(EXIT_FAILURE);
  }

  /*Initialisation du plateau*/
  pthread_mutex_lock(&jeu.mutex);
  initialiser_plateau(&jeu.jeu);
  pthread_mutex_unlock(&jeu.mutex);
  status = pthread_create(&thread_affichage, NULL, creation_interface, NULL);
  if(status != 0){
    fprintf(stderr, "Erreur lors de la création du thread d'affichage");
    perror("PTHREAD_CREATE");
    exit(EXIT_FAILURE);
  }
  pthread_join(thread_affichage, NULL);
  
  return EXIT_SUCCESS;
}
