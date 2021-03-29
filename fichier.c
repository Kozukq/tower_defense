#include "fichier.h"

void lire_carte(char * filename, carte * c){
  int fd, retour, i;
  unsigned char bytesTaille[sizeof(size_t)];
  fd = open(filename, O_RDONLY);
  if(fd == -1){
    printf("Erreur lors de l'ouverture de la carte\n");
    perror("OPEN");
    exit(EXIT_FAILURE);
  }
  retour = read(fd, bytesTaille, sizeof(size_t));
  if(retour < 0){
    printf("Erreur lors de la lecture des octets dans le fichier\n");
    perror("READ");
    exit(EXIT_FAILURE);
  }
  size_t taille = (int) bytesTaille[0];
  printf("taille : %d", taille);
  c-> taille = taille;
}


void lire_scenario(char * filename, scenario * s){
  int fd, retour, i;
  unsigned char bytesTaille[sizeof(size_t)];
  fd = open(filename, O_RDONLY);
  if(fd == -1){
    printf("Erreur lors de l'ouverture du scénario\n");
    perror("OPEN");
    exit(EXIT_FAILURE);
  }
  retour = read(fd, bytesTaille, sizeof(size_t));
  if(retour < 0){
    printf("Erreur lors de la lecture des octets dans le fichier\n");
    perror("READ");
    exit(EXIT_FAILURE);
  }
  size_t taille = (int) bytesTaille[0];
  printf("taille : %d", taille);
  s-> taille = taille;
}
