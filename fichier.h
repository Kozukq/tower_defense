#ifndef FICHIER_H
#define FICHIER_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "carte.h"
#include "scenario.h"

void lire_carte(char * filename, carte * c);
void lire_scenario(char * filename, scenario * s);
#endif
