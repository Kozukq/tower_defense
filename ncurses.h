#ifndef _NCURSES_
#define _NCURSES_

#define FALSE 0
#define TRUE 1

/**
 * Initialisation de ncurses.
 */
void ncurses_initialiser();

/**
 * Fin de ncurses.
 */
void ncurses_stopper();

/**
 * Initialisation des couleurs.
 */
void ncurses_couleurs();

/**
 * Initialisation de la souris.
 */
void ncurses_souris();

/**
 * Recupere la position x et y de la souris.
 * @param[out] x la position en x
 * @param[out] y la position en y
 * @param[out] bouton l'évenement associé au clic (ou NULL)
 * @return OK si reussite
 */
int souris_getpos(int *x, int *y, int *bouton);

#endif
