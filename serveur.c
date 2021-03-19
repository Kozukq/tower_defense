#include "serveur.h"

int main(int argc, char* argv[]){
  int port;
  if(argc != 2){
    printf("Veuillez préciser un numéro de port UDP (n'importe quel numéro fonctionne)\n");
    exit(EXIT_FAILURE);
  }
  port = atoi(argv[1]);
  printf("%d\n",port);
  exit(EXIT_SUCCESS);
}
