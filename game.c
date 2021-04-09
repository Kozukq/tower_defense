#include "game.h"
#include "interface.h"

#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

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
		/*Cancellation point*/
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

	/*Définition des attributs de l'unite*/
	struct soldier_state * soldier_state = (struct soldier_state * ) arg;
	int minY;
	int maxY;
	int minX;
	int maxX;
	
	/*Pour nanosleep*/
	struct timespec req, rem;
	req.tv_sec = soldier_state->soldier->speed / 1000;
  	req.tv_nsec = soldier_state->soldier->speed % 1000;
	
	/*Définition de la posibilité de déplacement de l'unité*/
	minY = soldier_state->soldier->position[0] - 1;
	if(minY < 0){
		minY = 0;
	}
	maxY = soldier_state->soldier->position[0] + 1;
	if(maxY > 14){
		maxY = 14;
	}
	minX = soldier_state->soldier->position[1] - 1;
	if(minX < 0){
		minX = 0;
	}
	maxX = soldier_state->soldier->position[1] + 1;
	if(maxX > 14){
		maxX = 14;
	}

	/*Boucle principale*/
  while(soldier_state->soldier->health > 0){
    int i,j, has_moved = 0;
  	nanosleep(&req, &rem);
		for(i=minY; i <= maxY && has_moved == 0; i = i+1){
			for(j = minX; j <= maxX && has_moved == 0; j = j+1){
    		pthread_mutex_lock(&soldier_state->game->board[i][j].mutex);
				if(soldier_state -> game -> board[i][j].background >= CASE_MIN_CHEMIN && soldier_state -> game -> board[i][j].background <= CASE_MIN_CHEMIN && soldier_state -> game -> board[i][j].unit_type == UNITE_AUCUNE ){
					if(soldier_state -> game -> board[i][j].background >= soldier_state -> game ->board[soldier_state->soldier->position[0]][soldier_state->soldier->position[1]].background){
						pthread_mutex_lock(&soldier_state -> game ->board[soldier_state->soldier->position[0]][soldier_state->soldier->position[1]].mutex);
						/*mvwaddch(soldier_state -> interface->map->interieur, i, j, 'U');*/
						mvwaddch(soldier_state -> interface->map->interieur, soldier_state->soldier->position[0], soldier_state->soldier->position[1], ' ');
						soldier_state -> game ->board[i][j].unit_type = soldier_state -> game ->board[soldier_state->soldier->position[0]][soldier_state->soldier->position[1]].unit_type;
						soldier_state -> game ->board[soldier_state->soldier->position[0]][soldier_state->soldier->position[1]].unit_type = UNITE_AUCUNE;
						soldier_state -> game ->board[i][j].soldier = soldier_state -> game ->board[soldier_state->soldier->position[0]][soldier_state->soldier->position[1]].soldier;
						pthread_mutex_unlock(&soldier_state -> game ->board[soldier_state->soldier->position[0]][soldier_state->soldier->position[1]].mutex);
						soldier_state->soldier->position[0] = i;
						soldier_state->soldier->position[1] = j;
						has_moved = 1;
					}
					
				}
				pthread_mutex_unlock(&soldier_state->game->board[i][j].mutex);
			}
		}
  }
  pthread_exit(NULL);
}
