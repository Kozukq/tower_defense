#include <stdlib.h> 	/* EXIT_SUCCESS, EXIT_FAILURE */
#include <stdio.h> 		/* fprintf() */
#include <dirent.h> 	/* opendir() */
#include <string.h> 	/* strerror(), strcat() */
#include <errno.h> 		/* errno */
#include <sys/stat.h> 	/* open() */
#include <fcntl.h> 		/* open() */
#include <unistd.h> 	/* close() */
#include "config.h"

void load(char* dirpath, struct config* config) {

	int i;
	int status;
	DIR* dir;
	char maps_path[256];
	char scenarios_path[256];
	char filepath[256];
	struct dirent* file;

	/* initialisations */
	strcat(maps_path,dirpath);
	strcat(maps_path,"maps/");
	strcat(scenarios_path,dirpath);
	strcat(scenarios_path,"scenarios/");

	/* ouverture du répertoire stockant les cartes */
	dir = opendir(maps_path);
	if(dir == NULL){
		fprintf(stderr,"%s (maps directory opening)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* ignorement des sous-répertoires . et .. */
	for(i = 0; i < 2; i++) {
		file = readdir(dir);
		if(file == NULL){
			fprintf(stderr,"%s (maps directory reading)\n",strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	/* lecture et chargement des cartes */
	for(i = 0; i < 3 ; i++) {
		
		file = readdir(dir);
		if(file == NULL){
			fprintf(stderr,"%s (maps directory reading)\n",strerror(errno));
			exit(EXIT_FAILURE);
		}

		memset(filepath,'\0',sizeof(filepath));
		strcat(filepath,maps_path);
		strcat(filepath,file->d_name);

		load_map(filepath,&config->maps[i]);

		if(DEBUG) fprintf(stdout,"%s loaded\n",filepath);
	}

	/* fermeture du répertoire */
	status = closedir(dir);
	if(status == -1) {
		fprintf(stderr,"%s (maps directory closing)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* ouverture du répertoire stockant les scénarios */
	dir = opendir(scenarios_path);
	if(dir == NULL){
		fprintf(stderr,"%s (scenarios directory opening)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* ignorement des sous-répertoires . et .. */
	for(i = 0; i < 2; i++) {
		file = readdir(dir);
		if(file == NULL){
			fprintf(stderr,"%s (maps directory reading)\n",strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	/* lecture et chargement des scénarios */
	for(i = 0; i < 3 ; i++) {
		
		file = readdir(dir);
		if(file == NULL){
			fprintf(stderr,"%s (scenarios directory reading)\n",strerror(errno));
			exit(EXIT_FAILURE);
		}

		memset(filepath,'\0',sizeof(filepath));
		strcat(filepath,scenarios_path);
		strcat(filepath,file->d_name);

		load_scenario(filepath,&config->scenarios[i]);

		if(DEBUG) fprintf(stdout,"%s loaded\n",filepath);
	}

	/* fermeture du répertoire */
	status = closedir(dir);
	if(status == -1) {
		fprintf(stderr,"%s (scenarios directory closing)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void load_map(char* filepath, struct map* map) {

	int i, j;
	int fd;
	int status;
	ssize_t bytes_read;
	uint64_t description_length;
	uint8_t board_line[15];

	/* ouverture du fichier */
	fd = open(filepath,O_RDONLY);
	if(fd == -1) {
		fprintf(stderr,"%s (map file opening)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}

	/* lecture de la taille de description */
	bytes_read = read(fd,&description_length,sizeof(uint64_t));
	if(bytes_read == -1) {
		fprintf(stderr,"%s (map file description length reading)\n",strerror(errno));
		exit(EXIT_FAILURE);				
	}

	/* lecture de la description */
	bytes_read = read(fd,&map->description,sizeof(char) * description_length);
	if(bytes_read == -1) {
		fprintf(stderr,"%s (map file description reading)\n",strerror(errno));
		exit(EXIT_FAILURE);				
	}

	/* lecture du plateau de jeu */
	for(i = 0; i < 15; i++) {
		
		bytes_read = read(fd,board_line,15);
		if(bytes_read == -1) {
			fprintf(stderr,"%s (map file board reading)\n",strerror(errno));
			exit(EXIT_FAILURE);				
		}

		for(j = 0; j < 15; j++) {
			map->board[14-i][14-j] = board_line[j];
		}
	}

	/* fermeture du fichier */
	status = close(fd);
	if(fd == -1) {
		fprintf(stderr,"%s (map file opening)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}
}

void load_scenario(char* filepath, struct scenario* scenario) {

	int i;
	int fd;
	int status;
	ssize_t bytes_read;
	uint64_t description_length;

	/* ouverture du fichier */
	fd = open(filepath,O_RDONLY);
	if(fd == -1) {
		fprintf(stderr,"%s (scenario file opening)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}

	/* lecture de la taille de description */
	bytes_read = read(fd,&description_length,sizeof(uint64_t));
	if(bytes_read == -1) {
		fprintf(stderr,"%s (scenario file description length reading)\n",strerror(errno));
		exit(EXIT_FAILURE);				
	}

	/* lecture de la description */
	bytes_read = read(fd,&scenario->description,sizeof(char) * description_length);
	if(bytes_read == -1) {
		fprintf(stderr,"%s (scenario file description reading)\n",strerror(errno));
		exit(EXIT_FAILURE);				
	}

	/* lecture des évènements */
	i = 0;
	while(1) {

		/* durée de l'évènement (fin de lecture si aucun octet lu) */
		bytes_read = read(fd,&scenario->events[i].duration,sizeof(uint64_t));
		if(bytes_read == -1) {
			fprintf(stderr,"%s (scenario file event duration reading)\n",strerror(errno));
			exit(EXIT_FAILURE);				
		}
		else if(bytes_read == 0) {
			scenario->events[i].duration = 0;
			break;
		}

		/* type d'évènement */
		bytes_read = read(fd,&scenario->events[i].type,sizeof(uint8_t));
		if(bytes_read == -1) {
			fprintf(stderr,"%s (scenario file event type reading)\n",strerror(errno));
			exit(EXIT_FAILURE);				
		}

		/* valeur ou texte lié à l'évènement */
		if(scenario->events[i].type == 0) {
			bytes_read = read(fd,&scenario->events[i].value,sizeof(char) * 255);
			if(bytes_read == -1) {
				fprintf(stderr,"%s (scenario file event uint32_t value reading)\n",strerror(errno));
				exit(EXIT_FAILURE);				
			}	
		}
		else {
			bytes_read = read(fd,&scenario->events[i].value,sizeof(uint32_t));
			if(bytes_read == -1) {
				fprintf(stderr,"%s (scenario file event char[255] value reading)\n",strerror(errno));
				exit(EXIT_FAILURE);				
			}
		}

		i++;
	}

	/* fermeture du fichier */
	status = close(fd);
	if(fd == -1) {
		fprintf(stderr,"%s (scenario file opening)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}
}
