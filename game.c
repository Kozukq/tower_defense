#include "game.h"

#include <stdlib.h>
#include <stdio.h>

void initialize_game(struct game* game, struct map* map) {

	int i;
	int j;
	int status;

	game->health = VIE_DEFAUT;
	game->money = ARGENT_DEFAUT;
	game->freeze = FREEZE_DEFAUT;
	game->unfreeze = UNFREEZE_DEFAUT;

	for(i = 0; i < 3; i++) {
		game->opponents_health[i] = VIE_DEFAUT;
	}

	for(i = 0; i < 15; i++) {
		for(j = 0; j < 15; j++) {

			status = pthread_mutex_init(&game->board[i][j].mutex,NULL);
			if(status != 0) {
				fprintf(stderr,"error:%d (mutex initialization)\n",status);
				exit(EXIT_FAILURE);
			}

			game->board[i][j].background = map->board[i][j];
			game->board[i][j].unit_type = UNITE_AUCUNE;
		}
	}
}

void* tower_behaviour(void* arg) {

	int range;
	int min_x;
	int min_y;
	int max_x;
	int max_y;
	struct timespec time;
	struct tower_state* tower_state;

	/* récupération de l'argument */
	tower_state = (struct tower_state*)arg;

	/* initialisations */
	range = tower_state->tower->range;
	time.tv_sec = tower_state->tower->speed / 1000;
	time.tv_nsec = tower_state->tower->speed % 1000;

	/* définition du rayon d'action de la tour */
	min_x = tower_state->tower->position[X] - range;
	if(min_x < 0){
		min_x = 0;
	}

	min_y = tower_state->tower->position[Y] - range;
	if(min_y < 0){
		min_y = 0;
	}

	max_x = tower_state->tower->position[X] + range;
	if(max_x > 14){
		max_x = 14;
	}

	max_y = tower_state->tower->position[Y] + range;
	if(max_y > 14){
		max_y = 14;
	}

	/* boucle principale */
	while(1) {

		int i;
		int j;
		int damage;

		nanosleep(&time,NULL);

		for(i = min_y; i <= max_y; i++) {
			for(j = min_x; j <= max_x; j++) {

				pthread_mutex_lock(&tower_state->game->board[i][j].mutex);

				if(tower_state->game->board[i][j].background != CASE_FORT
				&& tower_state->game->board[i][j].unit_type != UNITE_AUCUNE
				&& tower_state->game->board[i][j].unit_type != UNITE_TOUR) {	

					damage = ( rand() % (tower_state->tower->damage[MAX] - tower_state->tower->damage[MIN]) ) + tower_state->tower->damage[MIN];
					tower_state->game->board[i][j].soldier.health -= damage; 
				}

				pthread_mutex_unlock(&tower_state->game->board[i][j].mutex);
			}
		}
	}

	pthread_exit(NULL);
}

void* soldier_behaviour(void* arg) {

	pthread_exit(NULL);
}
