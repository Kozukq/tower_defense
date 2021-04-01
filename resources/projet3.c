/**
 * Ce programme propose un exemple d'interface pour le projet n°3.
 * @author Cyril Rabat
 **/

#include <stdlib.h>     /* Pour EXIT_FAILURE */

#include "ncurses.h"
#include "fenetre.h"
#include "jeu.h"
#include "interface.h"

int main() {
  int ch;
  interface_t interface;
  bool quitter = FALSE;
  jeu_t jeu;
  initialiser_plateau(&jeu);

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

  /* Création de l'interface */
  interface = interface_creer(&jeu);
    
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
