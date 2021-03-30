/*Ce fichier est un ficher temporaire destiné aux tests de la lecture de fichiers. Il ne doit pas être présent dans le rendu final.
Il est également une référence pour savoir comment s'utilisent les fonctions de lecture de cartes et de scénarios*/

#include <stdio.h>
#include <stdlib.h>
#include "carte.h"
#include "scenario.h"
#include "fichier.h"

int main(int argc, char* argv[]){
  carte * c = malloc(sizeof(carte));
  if(argc != 2){
    printf("Veuillez indiquer le chemin fichier de carte à lire");
    exit(EXIT_FAILURE);
  }
  lire_carte(argv[1], c);
  printf("taille : %d\n", c-> taille);
  /*La description ne s'affiche pas correctement*/
  printf("description : %s\n", c-> description);
}
