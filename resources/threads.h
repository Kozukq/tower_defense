#ifndef THREADS_H
#define THREADS_H

#include "fenetre.h"
#include "jeu.h"
#include "interface.h"
#include "unite.h"

typedef struct thread_tour_arg_t{
  tour * tour;
  jeu_t * jeu;
} thread_tour_arg;

typedef struct thread_unite_arg_t{
  unite * unite;
	interface_t * interface;
  jeu_t * jeu;
} thread_unite_arg;

void * thread_tour(void * arg);
void * thread_unite(void * arg);

#endif
