#ifndef INTERFACE_H
#define INTERFACE_H

#include "fenetre.h"
#include "game.h"

/* Propriétés de la fenêtre d'informations */
#define INFOS_POSX        0 /* Position en X */
#define INFOS_POSY        0 /* Position en Y */
#define INFOS_LARGEUR    60 /* Largeur avec le cadre */
#define INFOS_HAUTEUR    10 /* Hauteur avec le cadre */

/* Propriétés de la fenêtre de la carte */
#define CARTE_POSX        5 /* Position en X */
#define CARTE_POSY       10 /* Position en Y */
#define CARTE_LARGEUR    17 /* Largeur avec le cadre */
#define CARTE_HAUTEUR    17 /* Hauteur avec le cadre */

/* Propriétés de la fenêtre des outils */
#define OUTILS_POSX      23 /* Position en X */
#define OUTILS_POSY      10 /* Position en Y */
#define OUTILS_LARGEUR   15 /* Largeur avec le cadre */
#define OUTILS_HAUTEUR    9 /* Hauteur avec le cadre */

/* Propriétés de la fenêtre d'état */
#define ETAT_POSX        39 /* Position en X */
#define ETAT_POSY        10 /* Position en Y */
#define ETAT_LARGEUR     15 /* Largeur avec le cadre */
#define ETAT_HAUTEUR      9 /* Hauteur avec le cadre */

/* Propriétés de la fenêtre des attaques */
#define ATTAQUES_POSX    23 /* Position en X */
#define ATTAQUES_POSY    19 /* Position en Y */
#define ATTAQUES_LARGEUR 31 /* Largeur avec le cadre */
#define ATTAQUES_HAUTEUR  8 /* Hauteur avec le cadre */

/* Dimensions de l'interface (pour vérification avec celles du terminal) */
#define LARGEUR INFOS_LARGEUR
#define HAUTEUR ATTAQUES_POSY + ATTAQUES_HAUTEUR

/* Outils */
#define OUTIL_NONE       -1
#define OUTIL_TOUR_1      1
#define OUTIL_TOUR_2      2
#define OUTIL_TOUR_3      3
#define OUTIL_TOUR_4      4
#define OUTIL_TOUR_5      5
#define OUTIL_UNFREEZE    6

/* Structure représentant l'interface de l'application */
struct interface {
	pthread_mutex_t mutex;	/* Mutex de synchronisation */
	fenetre_t* info;		/* Fenêtre d'informations */
	fenetre_t* map;			/* Fenêtre de la carte */
	fenetre_t* tools;		/* Fenêtre des outils */
	fenetre_t* status;		/* Fenêtre d'état */
	fenetre_t* attacks;		/* Fenêtre des attaques */
	int current_tool;		/* Outil sélectionné */
};

/**
 * Définition de la palette.
 */
void palette();

/**
 * Création d'une interface.
 * @param jeu l'état du jeu
 * @return l'interface créée
 */
struct interface interface_creer(struct game* game);

/**
 * Suppression d'une interface.
 * @param interface l'interface à supprimer
 */
void interface_supprimer(struct interface* interface);

/**
 * Met à jour la fenêtre 'Attaques'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJAttaques(struct interface* interface, struct game* game);

/**
 * Met à jour la fenêtre 'Outils'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJOutils(struct interface* interface, struct game* game);

/**
 * Met à jour la fenêtre 'Etat'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJEtat(struct interface* interface, struct game* game);

/**
 * Gestion des actions dans la fenêtre des outils.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre 
 */
void interface_outils(struct interface* interface, struct game* game, int posX, int posY);

/**
 * Gestion des actions dans la fenêtre des attaques.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre 
 */
void interface_attaques(struct interface* interface, struct game* game, int posX, int posY);

/**
 * Gestion des actions dans la fenêtre de la carte.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre
 */
void interface_carte(struct interface* interface, struct game* game, int posX, int posY);

/**
 * Gestion des actions de l'utilisateur dans l'interface.
 * @param interface l'interface
 * @param jeu les paramètres de la partie
 * @param input_key la touche pressée
 */
void interface_main(struct interface* interface, struct game* game, int input_key);

void* interface_behaviour(void* arg);

#endif
