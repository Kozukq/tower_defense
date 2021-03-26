#ifndef SCENARIO_H
#define SCENARIO_H

typedef struct entree_t{
	long temps;
	unsigned char type;
	char[255] msg;
	unsigned int donnees;
} entree;

typedef struct scenario_t{
	size_t taille;
	char * description;
	entree * entrees;
} scenario;

#endif
