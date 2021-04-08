#define _POSIX_C_SOURCE 200809L
#include "threads.h"
#include <time.h>
#include "unistd.h"

/*Routine de thread pour une tour*/
void * thread_tour(void * arg){
	/*Définition des attributs de la tour*/
	thread_tour_arg * t = (thread_tour_arg * ) arg;
	int portee = t-> tour -> portee, departY, finY, departX, finX;
	
	/*Pour nanosleep*/
	struct timespec req, rem;
	req.tv_sec = t->tour->vitesse / 1000;
  req.tv_nsec = t->tour->vitesse % 1000;
	
	/*Définition du rayon d'action de la tour*/
	departY = t->tour->posY - portee;
	if(departY < 0){
		departY = 0;
	}
	finY = t->tour->posY + portee;
	if(finY > 14){
		finY = 14;
	}
	departX = t->tour->posX - portee;
	if(departX < 0){
		departX = 0;
	}
	finX = t->tour->posX + portee;
	if(finX > 14){
		finX = 14;
	}

	/*Boucle principale*/
  while(1==1){
    int i,j, tir;
  	nanosleep(&req, &rem);
		for(i=departY; i <= finY; i = i+1){
			for(j = departX; j <= finX ; j = j+1){
    		pthread_mutex_lock(&t -> jeu -> carte[i][j].mutex);
				if(t -> jeu -> carte[i][j].element != CASE_FORT && t -> jeu -> carte[i][j].type_unite != UNITE_AUCUNE && t -> jeu -> carte[i][j].type_unite != UNITE_TOUR ){
					tir = (rand() % ( t-> tour -> tir_max - t-> tour ->tir_min)) + t-> tour -> tir_min;
					t -> jeu -> carte[i][j].unite.vie = t -> jeu -> carte[i][j].unite.vie - tir;
				}
				pthread_mutex_unlock(&t -> jeu -> carte[i][j].mutex);
			}
		}
  }
  pthread_exit(NULL);
}

/*Routine de thread pour une unite*/
void * thread_unite(void * arg){
	/*Définition des attributs de l'unite*/
	thread_unite_arg * u = (thread_unite_arg * ) arg;
	int departY, finY, departX, finX;
	
	/*Pour nanosleep*/
	struct timespec req, rem;
	req.tv_sec = u->unite->vitesse / 1000;
  req.tv_nsec = u->unite->vitesse % 1000;
	
	/*Définition de la posibilité de déplacement de l'unité*/
	departY = u->unite->posY - 1;
	if(departY < 0){
		departY = 0;
	}
	finY = u->unite->posY + 1;
	if(finY > 14){
		finY = 14;
	}
	departX = u->unite->posX - 1;
	if(departX < 0){
		departX = 0;
	}
	finX = u->unite->posX + 1;
	if(finX > 14){
		finX = 14;
	}

	/*Boucle principale*/
  while(u->unite->vie > 0){
    int i,j;
  	nanosleep(&req, &rem);
		for(i=departY; i <= finY; i = i+1){
			for(j = departX; j <= finX ; j = j+1){
    		pthread_mutex_lock(&u -> jeu -> carte[i][j].mutex);
				if(u -> jeu -> carte[i][j].element >= CASE_MIN_CHEMIN && u -> jeu -> carte[i][j].type_unite == UNITE_AUCUNE ){
					
				}
				pthread_mutex_unlock(&u -> jeu -> carte[i][j].mutex);
			}
		}
  }
  pthread_exit(NULL);
  pthread_exit(NULL);
}
