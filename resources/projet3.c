/**
 * Ce programme propose un exemple d'interface pour le projet n°3.
 * @author Cyril Rabat
 **/

#include <stdlib.h>     /* Pour EXIT_FAILURE */

#include "ncurses.h"
#include "fenetre.h"
#include "jeu.h"
#include "interface.h"

freeze_p freeze = {FREEZE_DEFAUT, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
freeze_p unfreeze = {UNFREEZE_DEFAUT, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

interface_t interface;
jeu_t jeu;

void * creation_interface(void * arg){
  fprintf(stderr, "Demarrage du thread");
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
  interface = interface_creer(&jeu);
  return (void*) 3;
  
}

int main() {
  int ch, status;
  bool quitter = FALSE;
  pthread_t thread_affichage;
  initialiser_plateau(&jeu);
  status = pthread_create(&thread_affichage, NULL, creation_interface, NULL);
  if(status != 0){
    fprintf(stderr, "Erreur lors de la création du thread d'affichage");
    perror("PTHREAD_CREATE");
    exit(EXIT_FAILURE);
  }
  fprintf(stdout,"Après création du thread");
  pthread_join(thread_affichage, NULL);
  /* Boucle principale */
  while(quitter == FALSE) {
    ch = getch();
    if((ch == 'Q') || (ch == 'q'))
      quitter = true;
    else
      interface_main(&interface, &jeu, ch);
  }
    
    /* Suppression de l'interface */
  interface_supprimer(&interface);
    
    /* Arrêt de ncurses */
  ncurses_stopper();
  
  return EXIT_SUCCESS;
}
