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

  /**
   * A MODIFIER DANS LE PROJET
   * Contient la carte, le nombre de vie de l'utilisateur et des adversaires, l'argent,
   * l'énergie pour la bombe et le freeze.
   */
  jeu_t jeu = {
    {
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
            {   0,   0,   0,   0,   0,   0,   0, 254,   0,   0,   0,   0,   0,   0,   0 }
        }, 10, { 10, 10, 10} , 150, 0, 0 };

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
