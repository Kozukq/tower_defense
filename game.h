#ifndef GAME_H
#define GAME_H

#include "config.h"
#include <pthread.h>

/* Constantes d'initialisation du jeu */
#define VIE_DEFAUT          0
#define ARGENT_DEFAUT     150
#define FREEZE_DEFAUT       0
#define UNFREEZE_DEFAUT     0

/* Constantes pour les coûts des unités */
#define SOLDAT_COUT       100
#define COMMANDO_COUT     300
#define VEHICULE_COUT     700
#define MISSILE_COUT     1500
#define CHAR_COUT        3100

/* Constantes pour les vies des unités */
#define SOLDAT_VIE          5
#define COMMANDO_VIE       10
#define VEHICULE_VIE       20
#define MISSILE_VIE        50
#define CHAR_VIE          100

/* Constantes pour les déplacements des unités */
#define SOLDAT_VITESSE   1000
#define COMMANDO_VITESSE  900
#define VEHICULE_VITESSE  500
#define MISSILE_VITESSE   300
#define CHAR_VITESSE      400

/* Constantes pour les coûts des tourelles */
#define TOUR_1_COUT       100
#define TOUR_2_COUT       300
#define TOUR_3_COUT       700
#define TOUR_4_COUT      1500
#define TOUR_5_COUT      3100

/* Constantes pour les dégâts des tirs des tourelles */
#define TOUR_1_TIR_MIN      1
#define TOUR_1_TIR_MAX     10
#define TOUR_1_PORTEE       1
#define TOUR_1_VITESSE    500

#define TOUR_2_TIR_MIN      5
#define TOUR_2_TIR_MAX     20
#define TOUR_2_PORTEE       2
#define TOUR_2_VITESSE    400

#define TOUR_3_TIR_MIN     10
#define TOUR_3_TIR_MAX     40
#define TOUR_3_PORTEE       3
#define TOUR_3_VITESSE    300

#define TOUR_4_TIR_MIN     20
#define TOUR_4_TIR_MAX    100
#define TOUR_4_PORTEE       4
#define TOUR_4_VITESSE    200

#define TOUR_5_TIR_MIN     50
#define TOUR_5_TIR_MAX    200
#define TOUR_5_PORTEE       5
#define TOUR_5_VITESSE    100

/* Constante pour la durée de freeze */
#define FREEZE_DUREE     5000

/* Types de case (background) */
#define CASE_VIDE           0
#define CASE_MIN_JOUEUR     1
#define CASE_MAX_JOUEUR     3
#define CASE_MIN_CHEMIN     4
#define CASE_MAX_CHEMIN   229
#define CASE_ORDI         254
#define CASE_FORT         255
#define CASE_PRISE        256

/* Couleurs des cases */
#define COULEUR_VIDE        2
#define COULEUR_CHEMIN      3
#define COULEUR_JOUEUR      6
#define COULEUR_ORDI        6
#define COULEUR_FORT        7

/* Type d'unité */
#define UNITE_AUCUNE        0
#define UNITE_SOLDAT        1
#define UNITE_COMMANDO      2
#define UNITE_VEHICULE      3
#define UNITE_MISSILE       4
#define UNITE_CHAR          5
#define UNITE_TOUR          6
#define UNITE_FORT          7

/* Coordonnées */
#define X                   0
#define Y                   1

/* Intervalle pour le calcul des dommages */
#define MIN                 0
#define MAX                 1

struct soldier {
	int position[2];
	int speed;
	int health;
};

struct tower {
	int damage[2];
	int position[2];
	int speed;
	int range;
};

struct square {
	pthread_mutex_t mutex;
	int background;
	int unit_type;
	struct soldier soldier;
	struct tower tower;
};

struct game {
	pthread_mutex_t mutex;
	struct square board[15][15];
	int health;
	int opponents_health[3];
	int money;
	int freeze;
	int unfreeze;
};

struct tower_state {
	struct game* game;
	struct tower* tower;
};

struct soldier_state {
	struct game* game;
	struct interface* interface;
	struct soldier* soldier;
};

void initialize_game(struct game* game, struct map* map);
void* tower_behaviour(void* arg);
void* soldier_behaviour(void* arg);

#endif
