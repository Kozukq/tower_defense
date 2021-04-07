#define _POSIX_C_SOURCE 200809L
#include "threads.h"
#include <time.h>
#include "unistd.h"

void * thread_tour(void * arg){
  thread_tour_arg * t = (thread_tour_arg * ) arg;
  while(1==1){
    int i;
    struct timespec req, rem;
    req.tv_sec = t->tour->vitesse / 1000;
    req.tv_nsec = t->tour->vitesse % 1000;
    
    nanosleep(&req, &rem);
	for(i=0; i < t -> tour -> portee; i = i+1){
    	pthread_mutex_lock(&t -> jeu -> carte[t->tour->posY][t->tour->posX].mutex);
		pthread_mutex_unlock(&t -> jeu -> carte[t->tour->posY][t->tour->posX].mutex);
	}
    
  }
  pthread_exit(NULL);
}

void * thread_unite(void * arg){
  pthread_exit(NULL);
}
