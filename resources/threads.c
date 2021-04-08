#define _POSIX_C_SOURCE 200809L
#include "threads.h"
#include <time.h>
#include "unistd.h"

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
				if(t -> jeu -> carte[i][j].type_unite != UNITE_AUCUNE){
					tir = (rand() % ( t-> tour -> tir_max - t-> tour ->tir_min)) + t-> tour -> tir_min;
					t -> jeu -> carte[i][j].unite.vie = t -> jeu -> carte[i][j].unite.vie - tir;
				}
				pthread_mutex_unlock(&t -> jeu -> carte[i][j].mutex);
			}
		}
  }
  pthread_exit(NULL);
}

void * thread_unite(void * arg){
  pthread_exit(NULL);
}
