#include <stdlib.h> 	/* EXIT_SUCCESS, EXIT_FAILURE */
#include <stdio.h> 		/* fprintf() */
#include <sys/socket.h> /* socket() */
#include <string.h> 	/* strerror(), memset() */
#include <errno.h> 		/* errno */
#include <arpa/inet.h> 	/* htons(), htonl() */
#include <unistd.h> 	/* close() */
#include "network.h"
#include "config.h"

int main(int argc, char* argv[]) {
	
	int sockfd;
	int status;
	int msg;
	int is_launched;
	socklen_t addrlen;
	char buffer[256];
	char config_list[256];
	struct sockaddr_in server;
	struct sockaddr_in client;

	/* test chargement de la config */
	int i;
	struct config config;
	load("./",&config);
	for(i = 0; i < 15; i++) {
		if(config.scenarios[2].events[i].type == 0) {
			fprintf(stdout,"%llu,%d,%s\n",config.scenarios[2].events[i].duration,config.scenarios[2].events[i].type,config.scenarios[2].events[i].value.text);
		}
		else {
			fprintf(stdout,"%llu,%d,%d\n",config.scenarios[2].events[i].duration,config.scenarios[2].events[i].type,config.scenarios[2].events[i].value.number);
		}
	}

	/* initialisations */
	is_launched = FALSE;
	addrlen = sizeof(struct sockaddr_in);

	/* vérification des arguments */
	if(argc != 2) {
		fprintf(stderr,"Wrong arguments : UDP port required\n");
		exit(EXIT_FAILURE);
	}

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

	/* chargement des données */
	memset(config_list,0,sizeof(config_list));
	strcat(config_list,"Maps : easy(1), hard(2), medium(3)\n");
	strcat(config_list,"Scenarios : infini(1), pognon(2), tranquille(3)");

	/* réception client : choix menu */
	if(DEBUG) fprintf(stdout,"..waiting connection\n");
	addrlen = sizeof(struct sockaddr_in);
	status = recvfrom(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,&addrlen);
	if(status == -1) {
		fprintf(stderr,"%s (receiving from the client)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}

	if(msg == NEW_GAME) {

		/* vérification */
		if(is_launched) {
			msg = DECLINE;
		}
		else {
			msg = ACCEPT;
		}

		/* envoi client : acknowledgment */
		if(DEBUG) fprintf(stdout,"sending..\n");
		status = sendto(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,sizeof(struct sockaddr_in));
		if(status == -1) {
			fprintf(stderr,"%s (sending to the client)\n",strerror(errno));
			exit(EXIT_FAILURE);		
		}

		/* en cas de refus */
		if(is_launched) {
			fprintf(stderr,"There is already a game launched\n");
			exit(EXIT_FAILURE);
		}

		/* envoi client : liste de configuration */
		if(DEBUG) fprintf(stdout,"sending..\n");
		status = sendto(sockfd,config_list,sizeof(config_list),0,(struct sockaddr*)&client,sizeof(struct sockaddr_in));
		if(status == -1) {
			fprintf(stderr,"%s (sending to the client)\n",strerror(errno));
			exit(EXIT_FAILURE);		
		}

		/* réception client : choix de configuration */
		if(DEBUG) fprintf(stdout,"..waiting\n");
		status = recvfrom(sockfd,&msg,sizeof(&msg),0,(struct sockaddr*)&client,&addrlen);
		if(status == -1) {
			fprintf(stderr,"%s (receiving from the client)\n",strerror(errno));
			exit(EXIT_FAILURE);		
		}

		fprintf(stdout,"%d\n",msg);
	}
	else if(msg == JOIN_GAME) {
		
		/* vérification */
		if(is_launched) {
			msg = ACCEPT;
		}
		else {
			msg = DECLINE;
		}

		/* envoi client : acknowledgment */
		if(DEBUG) fprintf(stdout,"sending..\n");
		status = sendto(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&client,sizeof(struct sockaddr_in));
		if(status == -1) {
			fprintf(stderr,"%s (sending to the client)\n",strerror(errno));
			exit(EXIT_FAILURE);		
		}

		/* en cas de refus */
		if(!is_launched) {
			fprintf(stderr,"No game launched yet\n");
			exit(EXIT_FAILURE);
		}
	}

	/* envoi client : port TCP */


	/* fermeture de la socket */
	status = close(sockfd);
	if(status == -1) {
		fprintf(stderr,"%s (socket closing)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
