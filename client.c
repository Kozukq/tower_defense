#include "network.h"
#include "interface.h"

#include <stdlib.h> 	/* EXIT_SUCCESS, EXIT_FAILURE */
#include <stdio.h> 		/* fprintf() */
#include <sys/socket.h>	/* socket() */
#include <string.h> 	/* strerror(), memset() */
#include <errno.h> 		/* errno */
#include <arpa/inet.h> 	/* htons(), inet_pton() */
#include <unistd.h> 	/* close() */

/* déclarations globales */
struct interface interface;
struct game game;

int main(int argc, char* argv[]) {

	int udp_sockfd;
	int tcp_sockfd;
	int status;
	int msg;
	int response;
	pthread_t interface_thread;
	int games[MAX_GAMES];
	char config_list[256];
	struct sockaddr_in server;
	struct sockaddr_in tcpserver;
	struct map map;
	struct scenario scenario;

	/* vérification des arguments */
	if(argc != 3) {
		fprintf(stderr,"Wrong argument : IPv4 address, UDP port required\n");
		exit(EXIT_FAILURE);
	}

	/* ouverture de la socket UDP */
	udp_sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(udp_sockfd == -1) {
		fprintf(stderr,"%s (socket opening)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* création de l'adresse du serveur */
	memset(&server,0,sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	status = inet_pton(AF_INET,argv[1],&server.sin_addr);
	if(status == -1) {
		fprintf(stderr,"%s (server address creation)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* envoi serveur : établissement connexion */
	if(DEBUG) fprintf(stdout,"sending..\n");
	msg = BEGIN_CONNECTION;
	status = sendto(udp_sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&server,sizeof(struct sockaddr_in));
	if(status == -1) {
		fprintf(stderr,"%s (sending to the server)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}	

	/* menu principal */
	fprintf(stdout,"┌------MENU------┐\n");
	fprintf(stdout,"|  New Game (1)  |\n");
	fprintf(stdout,"|    Join (2)    |\n");
	fprintf(stdout,"└----------------┘\n");
	fprintf(stdout,">");
	fscanf(stdin,"%d",&msg);
	if(!((msg == 1) || (msg == 2))) {
		fprintf(stderr,"Wrong input\n");
		exit(EXIT_FAILURE);
	}
	if(msg == 1) msg = NEW_GAME;
	else msg = JOIN_GAME;

	/* envoi serveur : choix menu */
	if(DEBUG) fprintf(stdout,"sending..\n");
	status = sendto(udp_sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&server,sizeof(struct sockaddr_in));
	if(status == -1) {
		fprintf(stderr,"%s (sending to the server)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}

	/* réception serveur : acknowledgment */
	if(DEBUG) fprintf(stdout,"..waiting\n");
	status = recvfrom(udp_sockfd,&response,sizeof(response),0,NULL,NULL);
	if(status == -1) {
		fprintf(stderr,"%s (receiving from the server)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}

	/* traitement */
	if(response == DECLINE) {
		fprintf(stderr,"The server rejected the query\n");
		exit(EXIT_FAILURE);
	}

	if(msg == NEW_GAME) {
		
		/* réception serveur : liste de configuration */
		if(DEBUG) fprintf(stdout,"..waiting\n");
		status = recvfrom(udp_sockfd,config_list,sizeof(config_list),0,NULL,NULL);
		if(status == -1) {
			fprintf(stderr,"%s (receiving from the server)\n",strerror(errno));
			exit(EXIT_FAILURE);		
		}

		/* choix de la configuration */
		fprintf(stdout,"%s\n",config_list);
		fprintf(stdout,">");
		fscanf(stdin,"%d",&msg);
		
		/* envoi serveur : choix de configuration */
		if(DEBUG) fprintf(stdout,"sending..\n");
		status = sendto(udp_sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&server,sizeof(struct sockaddr_in));
		if(status == -1) {
			fprintf(stderr,"%s (sending to the server)\n",strerror(errno));
			exit(EXIT_FAILURE);		
		}		
	}
	else if(msg == JOIN_GAME) {

		/* réception serveur : liste des parties */
		if(DEBUG) fprintf(stdout,"..waiting\n");
		status = recvfrom(udp_sockfd,games,sizeof(int) * MAX_GAMES,0,NULL,NULL);
		if(status == -1) {
			fprintf(stderr,"%s (receiving from the server)\n",strerror(errno));
			exit(EXIT_FAILURE);		
		}

		/* affichage des parties */
		status = show(games);
		if(status == FALSE) {
			fprintf(stdout,"Exiting\n");
			exit(EXIT_FAILURE);
		}

		/* choix de la partie */
		fprintf(stdout,">");
		fscanf(stdin,"%d",&msg);
	
		/* envoi serveur : choix de partie */
		if(DEBUG) fprintf(stdout,"sending..\n");
		status = sendto(udp_sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&server,sizeof(struct sockaddr_in));
		if(status == -1) {
			fprintf(stderr,"%s (sending to the server)\n",strerror(errno));
			exit(EXIT_FAILURE);		
		}	
	}
	
	/* réception serveur : port TCP */
	if(DEBUG) fprintf(stdout,"..waiting\n");
	status = recvfrom(udp_sockfd,&msg,sizeof(msg),0,NULL,NULL);
	if(status == -1) {
		fprintf(stderr,"%s (receiving from the server)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}

	/* ouverture de la socket TCP */
	tcp_sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(tcp_sockfd == -1) {
		fprintf(stderr,"%s (socket opening)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* création de l'adresse du serveur TCP */
	memset(&tcpserver,0,sizeof(struct sockaddr_in));
	tcpserver.sin_family = AF_INET;
	tcpserver.sin_port = htons(msg);
	status = inet_pton(AF_INET,argv[1],&tcpserver.sin_addr);
	if(status == -1) {
		fprintf(stderr,"%s (server address creation)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* connexion avec le serveur TCP */
	status = connect(tcp_sockfd,(struct sockaddr*)&tcpserver,sizeof(struct sockaddr_in));
	if(status == -1) {
		fprintf(stderr,"%s (socket connecting)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* réception serveur : carte de jeu */
	if(DEBUG) fprintf(stdout,"..waiting\n");
	status = read(tcp_sockfd,&map,sizeof(struct map));
	if(status == -1) {
		fprintf(stderr,"%s (receiving from the server)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}	

	/* réception serveur : scénario de jeu */
	if(DEBUG) fprintf(stdout,"..waiting\n");
	status = read(tcp_sockfd,&scenario,sizeof(struct scenario));
	if(status == -1) {
		fprintf(stderr,"%s (receiving from the server)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	fprintf(stdout,"map : %s\n",map.description);
	fprintf(stdout,"scenario : %s\n",scenario.description);

	/* initialisation du mutex positionné sur l'interface */
	status = pthread_mutex_init(&interface.mutex,NULL);
	if(status != 0) {
		fprintf(stderr,"error:%d (mutex initialization)\n",status);
		exit(EXIT_FAILURE);
	}

	/* initialisation du mutex positionné sur la partie */
	status = pthread_mutex_init(&game.mutex,NULL);
	if(status != 0) {
		fprintf(stderr,"error:%d (mutex initialization)\n",status);
		exit(EXIT_FAILURE);
	}

	/* initialisation de la partie */
	pthread_mutex_lock(&game.mutex);
	initialize_game(&game,&map);
	pthread_mutex_unlock(&game.mutex);

	/* création du thread gérant l'interface */
	status = pthread_create(&interface_thread,NULL,interface_behaviour,NULL);
	if(status != 0) {
		fprintf(stderr,"error:%d (creating interface thread)\n",status);
		exit(EXIT_FAILURE);		
	}

	/* boucle principale */
	while(1) {

		/* scénario de jeu */

		break;
	}

	/* destruction du thread gérant l'interface */
	status = pthread_join(interface_thread,NULL);
	if(status != 0) {
		fprintf(stderr,"error:%d (destroying interface thread)\n",status);
		exit(EXIT_FAILURE);		
	}

	/* destruction du mutex positionné sur l'interface */
	status = pthread_mutex_destroy(&interface.mutex);
	if(status != 0) {
		fprintf(stderr,"error:%d (mutex destruction)\n",status);
		exit(EXIT_FAILURE);
	}

	/* destruction du mutex positionné sur la partie */
	status = pthread_mutex_destroy(&game.mutex);
	if(status != 0) {
		fprintf(stderr,"error:%d (mutex destruction)\n",status);
		exit(EXIT_FAILURE);
	}

	/* fermeture de la socket TCP */
	status = close(tcp_sockfd);
	if(status == -1) {
		fprintf(stderr,"%s (socket closing)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* fermeture de la socket UDP */
	status = close(udp_sockfd);
	if(status == -1) {
		fprintf(stderr,"%s (socket closing)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

void* interface_behaviour(void* arg) {

	int exit_request;
	int input_key;

	/* initialisation de ncurses */
	ncurses_initialiser();
	ncurses_souris();
	ncurses_couleurs(); 
	palette();
	clear();
	refresh();
	exit_request = FALSE;

	/* vérification des dimensions du terminal */
	if((COLS < LARGEUR) || (LINES < HAUTEUR)) {
		
		ncurses_stopper();
		
		fprintf(stderr,
		"Les dimensions du terminal sont insufisantes : l=%d,h=%d au lieu de l=%d,h=%d\n", 
	    COLS, LINES, LARGEUR, HAUTEUR);
    
    	exit(EXIT_FAILURE);
	}

	/* création de l'interface */
	pthread_mutex_lock(&interface.mutex);
	pthread_mutex_lock(&game.mutex);
	interface = interface_creer(&game);
	pthread_mutex_unlock(&interface.mutex);
	pthread_mutex_unlock(&game.mutex);

	/* boucle principale */
	while(exit_request == FALSE) {

		input_key = getch();

		if((input_key == 'Q') || (input_key == 'q')) {
			exit_request = TRUE;
		}
		else {
			pthread_mutex_lock(&interface.mutex);
			pthread_mutex_lock(&game.mutex);
			interface_main(&interface,&game,input_key);
			pthread_mutex_unlock(&interface.mutex);
			pthread_mutex_unlock(&game.mutex);
		}
	}

	/* suppression de l'interface */
	pthread_mutex_lock(&interface.mutex);
	pthread_mutex_lock(&game.mutex);
	interface_supprimer(&interface);
	pthread_mutex_unlock(&interface.mutex);
	pthread_mutex_unlock(&game.mutex);

	/* terminaison de ncurses */
	ncurses_stopper();

	pthread_exit(NULL);
}

int show(int* games) {

	int i;
	int games_running;

	games_running = 0;

	for(i = 0; i < MAX_GAMES; i++) {
		if(games[i] == RUNNING) {
			fprintf(stdout,"Game %d is running\n",i+1);
			games_running++;
		}
	}

	if(games_running == 0) {
		fprintf(stdout,"No game started\n");
		return FALSE;
	}

	return TRUE;
}
