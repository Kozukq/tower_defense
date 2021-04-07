#ifndef THREADS_H
#define THREADS_H

#include "fenetre.h"
#include "jeu.h"
#include "interface.h"
#include "unite.h"

typedef struct thread_tour_arg_t{
  tour * tour;
  interface_t * interface;
  jeu_t * jeu;
} thread_tour_arg;
void * thread_tour(void * arg);
void * thread_unite(void * arg);

#endif
