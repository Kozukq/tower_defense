#ifndef FICHIER_H
#define FICHIER_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "carte.h"
#include "scenario.h"

carte lire_carte(char * filename);
scenario lire_scenario(char * filename);
#endif
