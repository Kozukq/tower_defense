#ifndef CONFIG_H
#define CONFIG_H

#define _POSIX_C_SOURCE 200809L
#define DEBUG 1

#include <stdint.h> /* nécessaire pour gérer les types à taille fixe sur Linux */

/*Structure représentant une carte lue depuis un fichier binaire*/
struct map {
	uint8_t board[15][15];
	char description[256];
};

/*Union permettant de stocker une chaîne de 255 caractères et un entier non signé*/
union char255_uint {
	char text[255];
	uint32_t number;
};

/*Structure représentant un évènement lu depuis un fichier binaire*/
struct event {
	union char255_uint value;
	uint64_t duration;
	uint8_t type;
};

/*Structure représentant un scénario lu depuis un fichier binaire*/
struct scenario {
	struct event events[200];
	char description[256];
};

/*Structure regroupant les cartes et les scénarios lus*/
struct config {
	struct map maps[3];
	struct scenario scenarios[3];
};

/**
 * Ouvre et lit le contenu du répertoire passé en paramètre 
 * @param dirpath le chemin du répertoire à ouvrir
 * @param config un pointeur sur la structure dans laquelle placer les éléments lus
 */
void load(char* dirpath, struct config* config);
/**
 * Ouvre et lit le contenu du fichier binaire de carte passé en paramètre 
 * @param filepath le chemin du fichier binaire de carte à ouvrir
 * @param map un pointeur sur la structure dans laquelle placer la carte lue
 */
void load_map(char* filepath, struct map* map);
/**
 * Ouvre et lit le contenu du fichier binaire de scénario passé en paramètre 
 * @param filepath le chemin du fichier binaire de scénario à ouvrir
 * @param scenario un pointeur sur la structure dans laquelle placer le scénario lu
 */
void load_scenario(char* filepath, struct scenario* scenario);

#endif
