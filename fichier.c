#include "fichier.h"

void lire_carte(char * filename, carte * c){
  int fd, retour, i, j;
  unsigned char bytesTaille[sizeof(size_t)];
  unsigned char bytesDesc[255];
  unsigned char bytesLigne[sizeof(unsigned char)*TAILLE_LIGNE_CARTE];
  fd = open(filename, O_RDONLY);
  if(fd == -1){
    fprintf(stderr,"Erreur lors de l'ouverture de la carte\n");
    perror("OPEN");
    exit(EXIT_FAILURE);
  }
  retour = read(fd, bytesTaille, sizeof(size_t));
  if(retour < 0){
    fprintf(stderr,"Erreur lors de la lecture des octets dans le fichier\n");
    perror("READ");
    exit(EXIT_FAILURE);
  }
  size_t taille = (int) bytesTaille[0];
  c-> taille = taille;
  retour = read(fd, bytesDesc, taille * sizeof(char));
  if(retour < 0){
    fprintf(stderr,"Erreur lors de la lecture des octets dans le fichier\n");
    perror("READ");
    exit(EXIT_FAILURE);
  }
  c -> description = malloc(sizeof(char)*taille);
  memcpy(c->description, bytesDesc,sizeof(char)*taille);
  for(i = 0; i < 15; i = i+1){
    retour = read(fd, bytesLigne, sizeof(unsigned char)*TAILLE_LIGNE_CARTE);
    if(retour < 0){
      fprintf(stderr,"Erreur lors de la lecture des octets dans le fichier\n");
      perror("READ");
      exit(EXIT_FAILURE);
    }
    for(j = 0; j < 15; j = j+1){
      c -> plateau[14-i][14-j] = (int) bytesLigne[j];
    }
  }
  
}


void lire_scenario(char * filename, scenario * s){
  int fd, retour, i;
  unsigned char bytesTaille[sizeof(size_t)];
  unsigned char bytesDesc[255];
  fd = open(filename, O_RDONLY);
  if(fd == -1){
    fprintf(stderr,"Erreur lors de l'ouverture du scénario\n");
    perror("OPEN");
    exit(EXIT_FAILURE);
  }
  retour = read(fd, bytesTaille, sizeof(size_t));
  if(retour < 0){
    fprintf(stderr,"Erreur lors de la lecture des octets dans le fichier\n");
    perror("READ");
    exit(EXIT_FAILURE);
  }
  size_t taille = (int) bytesTaille[0];
  s-> taille = taille;
  
  retour = read(fd, bytesDesc, taille * sizeof(char));
  if(retour < 0){
    fprintf(stderr,"Erreur lors de la lecture des octets dans le fichier\n");
    perror("READ");
    exit(EXIT_FAILURE);
  }
  s -> description = malloc(sizeof(char) * taille);
  memcpy(s->description, bytesDesc,sizeof(char)*taille);
}
