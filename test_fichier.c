/*Ce fichier est un ficher temporaire destiné aux tests de la lecture de fichiers. Il ne doit pas être présent dans le rendu final.
Il est également une référence pour savoir comment s'utilisent les fonctions de lecture de cartes et de scénarios*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include "carte.h"
#include "scenario.h"
#include "fichier.h"

int main(int argc, char* argv[]){
  int retour;
  carte *c = malloc(sizeof(carte));
  scenario *s = malloc(sizeof(scenario));
  DIR *repCarte;
  DIR *repScenario;
  struct dirent *carte;
  struct dirent *scenario; 
  repCarte = opendir("resources/cartes");
  if(repCarte == NULL){
    fprintf(stderr,"Erreur lors de l'ouverture du repertoire des cartes");
    perror("OPENDIR");
    exit(EXIT_FAILURE);
  }
  repScenario = opendir("resources/scenarios");
    if(repScenario == NULL){
    fprintf(stderr,"Erreur lors de l'ouverture du repertoire des scenarios");
    perror("OPENDIR");
    exit(EXIT_FAILURE);
  }
  carte = readdir(repCarte);
  printf("%s", carte -> d_name);
  lire_carte("resources/cartes/easy.bin", c);
  printf("taille : %d\n", c-> taille);
  printf("description : %s", c-> description);
  lire_scenario("resources/scenarios/infini.bin", s);
  printf("taille : %d\n", s-> taille);
  printf("description : %s\n", s-> description);
  retour = closedir(repCarte);
  if(retour !=0){
    fprintf(stderr,"Erreur lors de la fermeture du repertoire de cartes");
    perror("CLOSEDIR");
    exit(EXIT_FAILURE);
  }
  retour = closedir(repScenario);
  if(retour !=0){
    fprintf(stderr,"Erreur lors de la fermeture du repertoire de scenarios");
    perror("CLOSEDIR");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}

