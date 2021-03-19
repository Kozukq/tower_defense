#include "client.h"

int main(int argc, char* argv[]){
  if(argc != 3){
    printf("Veuillez indiquer l'adresse IP ET le port UDP du serveur (n'importe quelles valeurs fonctionnent)");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
