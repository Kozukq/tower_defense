#include "fichier.h"

carte lire_carte(char * filename){
  carte c;
  int fd, retour, i;
  unsigned char bytes[sizeof(int)];
  fd = open(filename, O_RDONLY);
  if(fd == -1){
    printf("Erreur lors de l'ouverture de la carte\n");
    perror("OPEN");
    exit(EXIT_FAILURE);
  }
  retour = read(fd, bytes, 8);
  if(retour < 0){
    printf("Erreur lors de la lecture des octets dans le fichier\n");
    perror("READ");
    exit(EXIT_FAILURE);
  }
  for(i = 0; i < 8; i = i+1){
    printf("%X.", bytes[i]);
  }
  return c;
}
scenario lire_scenario(char * filename){
  scenario s;
  return s;
}
