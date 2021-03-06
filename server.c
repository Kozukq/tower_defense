#include "network.h"
#include "game.h"

#include <stdlib.h> 	/* EXIT_SUCCESS, EXIT_FAILURE */
#include <stdio.h> 		/* fprintf() */
#include <sys/socket.h> /* socket() */
#include <string.h> 	/* strerror(), memset() */
#include <errno.h> 		/* errno */
#include <arpa/inet.h> 	/* htons(), htonl() */
#include <unistd.h> 	/* close() */

int main(int argc, char* argv[]) {
	
	int sockfd;
	int status;
	int msg;
	socklen_t client_addrlen;
	int games[MAX_GAMES];
	pthread_t gamethreads[MAX_GAMES]; 
	char buffer[256];
	char config_list[256];
	struct sockaddr_in server;
	struct sockaddr_in client;
	struct config config;
	struct game_config* game_config;

	/* initialisations */
	memset(games,0,sizeof(games));
	game_config = NULL;

	/* vérification des arguments */
	if(argc != 2) {
		fprintf(stderr,"Wrong arguments : UDP port required\n");
		exit(EXIT_FAILURE);
	}

	/* chargement de tous les choix de configuration */	
	load("./",&config);

	/* ouverture de la socket */
	sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(sockfd == -1) {
		fprintf(stderr,"%s (socket opening)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* création de l'adresse du serveur */
	memset(&server,0,sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	memset(&client,0,sizeof(struct sockaddr_in));

	/* nommage de la socket */
	status = bind(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr_in));
	if(status == -1) {
		fprintf(stderr,"%s (socket binding)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* affichage des infos de connexion au serveur */
	if(inet_ntop(AF_INET,&server.sin_addr,buffer,INET_ADDRSTRLEN) == NULL) {
		fprintf(stderr,"%s (server address to char*)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(DEBUG) fprintf(stdout,"Access this server via %s:%d\n",buffer,atoi(argv[1]));

	/* boucle principale */
	while(1) {

		/* réception client : signal */
		if(DEBUG) fprintf(stdout,"..waiting connection\n");
		status = recvfrom(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,&client_addrlen);
		if(status == -1) {
			fprintf(stderr,"%s (receiving from the client)\n",strerror(errno));
			exit(EXIT_FAILURE);		
		}

		/* traitement du signal */
		if(msg == BEGIN_CONNECTION) {

			/* réception client : choix menu */
			if(DEBUG) fprintf(stdout,"..waiting connection\n");
			status = recvfrom(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,&client_addrlen);
			if(status == -1) {
				fprintf(stderr,"%s (receiving from the client)\n",strerror(errno));
				exit(EXIT_FAILURE);		
			}

			if(msg == NEW_GAME) {

				/* vérification du nombre de parties en cours */
				if(check_limit(games) == TRUE) {
					msg = DECLINE;
				}
				else {
					msg = ACCEPT;
				}

				/* envoi client : acknowledgment */
				if(DEBUG) fprintf(stdout,"sending..\n");
				status = sendto(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,client_addrlen);
				if(status == -1) {
					fprintf(stderr,"%d:%s (sending to the client)\n",errno,strerror(errno));
					exit(EXIT_FAILURE);		
				}

				/* en cas de refus */
				if(msg == DECLINE) {
					fprintf(stderr,"There is too much games running : %d/%d\n",MAX_GAMES,MAX_GAMES);
					continue;
				}

				/* préparation de la liste de configuration */
				memset(config_list,0,sizeof(config_list));
				strcat(config_list,"Maps : easy(1), hard(2), medium(3)\n");
				strcat(config_list,"Scenarios : infini(1), pognon(2), tranquille(3)");

				/* envoi client : liste de configuration */
				if(DEBUG) fprintf(stdout,"sending..\n");
				status = sendto(sockfd,config_list,sizeof(config_list),0,(struct sockaddr*)&client,client_addrlen);
				if(status == -1) {
					fprintf(stderr,"%s (sending to the client)\n",strerror(errno));
					exit(EXIT_FAILURE);		
				}

				/* réception client : choix de configuration */
				if(DEBUG) fprintf(stdout,"..waiting\n");
				status = recvfrom(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,&client_addrlen);
				if(status == -1) {
					fprintf(stderr,"%s (receiving from the client)\n",strerror(errno));
					exit(EXIT_FAILURE);		
				}

				/* génération de la partie */
				game_config = (struct game_config*)malloc(sizeof(struct game_config));
				game_config->map = config.maps[msg / 10];
				game_config->scenario = config.scenarios[msg % 10];
				game_config->tcp_port = new_game(games,atoi(argv[1]));

				/* envoi client : port TCP */
				msg = game_config->tcp_port;
				if(DEBUG) fprintf(stdout,"sending..\n");
				status = sendto(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,client_addrlen);
				if(status == -1) {
					fprintf(stderr,"%s (sending to the client)\n",strerror(errno));
					exit(EXIT_FAILURE);	
				}

				/* création du thread gérant le serveur TCP */
				status = pthread_create(&gamethreads[game_config->tcp_port - atoi(argv[1]) + 1],NULL,tcp_server,game_config);
				if(status != 0) {
					fprintf(stderr,"error:%d (creating game thread)\n",status);
					exit(EXIT_FAILURE);		
				}

				fprintf(stdout,"Game %d has launched\n",game_config->tcp_port - atoi(argv[1]) + 1);
			}
			else if(msg == JOIN_GAME) {

				/* envoi client : acknowledgment */
				if(DEBUG) fprintf(stdout,"sending..\n");
				msg = ACCEPT;
				status = sendto(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,client_addrlen);
				if(status == -1) {
					fprintf(stderr,"%s (sending to the client)\n",strerror(errno));
					exit(EXIT_FAILURE);		
				}

				/* envoi client : liste des parties */
				if(DEBUG) fprintf(stdout,"sending..\n");
				status = sendto(sockfd,games,sizeof(int) * MAX_GAMES,0,(struct sockaddr*)&client,client_addrlen);
				if(status == -1) {
					fprintf(stderr,"%s (sending to the client)\n",strerror(errno));
					exit(EXIT_FAILURE);		
				}

				/* réception client : choix de partie */
				if(DEBUG) fprintf(stdout,"..waiting connection\n");
				status = recvfrom(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,&client_addrlen);
				if(status == -1) {
					fprintf(stderr,"%s (receiving from the client)\n",strerror(errno));
					exit(EXIT_FAILURE);		
				}

				/* préparation des données */
				msg = msg + atoi(argv[1]);

				/* envoi client : port TCP pour rejoindre la partie */
				if(DEBUG) fprintf(stdout,"sending..\n");
				status = sendto(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,client_addrlen);
				if(status == -1) {
					fprintf(stderr,"%s (sending to the client)\n",strerror(errno));
					exit(EXIT_FAILURE);		
				}
			}

		}
		else if(msg == CLOSE_SERVER) {

			/* envoi client : acknowledgment */
			if(DEBUG) fprintf(stdout,"sending..\n");
			msg = ACCEPT;
			status = sendto(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,client_addrlen);
			if(status == -1) {
				fprintf(stderr,"%s (sending to the client)\n",strerror(errno));
				exit(EXIT_FAILURE);		
			}

			/* réception client : port TCP concerné par la fermeture */
			if(DEBUG) fprintf(stdout,"..waiting\n");
			status = recvfrom(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,&client_addrlen);
			if(status == -1) {
				fprintf(stderr,"%s (receiving from the client)\n",strerror(errno));
				exit(EXIT_FAILURE);		
			}

			/* traitement de la terminaison du thread */
			msg = msg - atoi(argv[1]) + 1;
			status = pthread_join(gamethreads[msg],(void*)game_config);
			if(status != 0) {
				fprintf(stderr,"error:%d (creating game thread)\n",status);
				exit(EXIT_FAILURE);		
			}

			/* mise à jour des parties */
			games[msg] = FREE;

			/* libération de la mémoire */
			free(game_config);

			fprintf(stdout,"Server %d has been closed\n",msg);
		}

	}

	/* fermeture de la socket */
	status = close(sockfd);
	if(status == -1) {
		fprintf(stderr,"%s (socket closing)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

void* tcp_server(void* arg) {

	int i;
	int sockfd;
	int fd;
	int status;
	socklen_t client_addrlen;
	struct sockaddr_in tcpserver;
	struct sockaddr_in client;
	struct game_config* game_config;

	/* récupération de la configuration de la partie */
	game_config = (struct game_config*)arg;

	/* ouverture de la socket */
	sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sockfd == -1) {
		fprintf(stderr,"%s (socket opening)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* création de l'adresse du serveur */
	memset(&tcpserver,0,sizeof(struct sockaddr_in));
	tcpserver.sin_family = AF_INET;
	tcpserver.sin_port = htons(game_config->tcp_port);
	tcpserver.sin_addr.s_addr = htonl(INADDR_ANY);

	/* nommage de la socket */
	status = bind(sockfd,(struct sockaddr*)&tcpserver,sizeof(struct sockaddr_in));
	if(status == -1) {
		fprintf(stderr,"%s (socket binding)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* mise en écoute du serveur */
	status = listen(sockfd,4);
	if(status == -1) {
		fprintf(stderr,"%s (socket listening)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* attente des 4 clients */
	for(i = 0; i < 4; i++) {
		
		/* connexion avec un client */
		fd = accept(sockfd,(struct sockaddr*)&client,&client_addrlen);
		if(fd == -1) {
			fprintf(stderr,"%s (socket accepting)\n",strerror(errno));
			exit(EXIT_FAILURE);
		}

		/* envoi client : carte de jeu */
		status = write(fd,&game_config->map,sizeof(struct map));
		if(status == -1) {
			fprintf(stderr,"%s (sending to the client)\n",strerror(errno));
			exit(EXIT_FAILURE);
		}

		/* envoi client : scénario de jeu */
		status = write(fd,&game_config->scenario,sizeof(struct scenario));
		if(status == -1) {
			fprintf(stderr,"%s (sending to the client)\n",strerror(errno));
			exit(EXIT_FAILURE);
		}

		/* déconnexion */
		close(fd);
	}

	/* boucle principale */
	while(1) {
		break;
	}

	/* fermeture de la socket */
	status = close(sockfd);
	if(status == -1) {
		fprintf(stderr,"%s (socket closing)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	return game_config;
}

int check_limit(int* games) {

	int i;
	int games_running;

	games_running = 0;

	for(i = 0; i < MAX_GAMES; i++) {
		if(games[i] == RUNNING) {
			games_running++;
		}
	}

	return games_running == MAX_GAMES;
}

int new_game(int* games, int port) {

	int i = 0;

	while((games[i] == RUNNING) && (i < MAX_GAMES)) {
		i++;
	}

	games[i] = RUNNING;

	return port + i + 1;
}
