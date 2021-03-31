/*Ce fichier est un ficher temporaire destiné aux tests de la lecture de fichiers. Il ne doit pas être présent dans le rendu final.
Il est également une référence pour savoir comment s'utilisent les fonctions de lecture de cartes et de scénarios*/

#include <stdio.h>
#include <stdlib.h>
#include "carte.h"
#include "scenario.h"
#include "fichier.h"

int main(int argc, char* argv[]){
  carte * c = malloc(sizeof(carte));
  scenario * s = malloc(sizeof(scenario));
  if(argc != 2){
    printf("Veuillez indiquer le chemin fichier de carte à lire");
    exit(EXIT_FAILURE);
  }
  lire_carte(argv[1], c);
  printf("taille : %d\n", c-> taille);
  printf("description : %s\n", c-> description);
  printf("ligne 1 : %d %d %d %d ...\n", c-> plateau[0][0], c-> plateau[0][1], c-> plateau[0][2], c-> plateau[0][3]);
}

