#ifndef SCENARIO_H
#define SCENARIO_H

typedef struct entree_t{
	long temps;
	unsigned char type;
	char[255] msg;
	unsigned int donnees;
} entree;

typedef struct carte_t{
	entree * entrees;
} carte;

#endif
