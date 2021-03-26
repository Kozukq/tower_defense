#ifndef CARTE_H
#define CARTE_H

typedef struct cases_t{
	int type;
} cases;

typedef struct carte_t{
	size_t taille;
	char * description;
	cases * lignes;
} carte;

#endif
