#include <stdlib.h> 	/* EXIT_SUCCESS, EXIT_FAILURE */
#include <stdio.h> 		/* fprintf() */
#include <sys/socket.h>	/* socket() */
#include <string.h> 	/* strerror(), memset() */
#include <errno.h> 		/* errno */
#include <arpa/inet.h> 	/* htons(), inet_pton() */
#include <unistd.h> 	/* close() */
#include "network.h"
#include "config.h"

int main(int argc, char* argv[]) {

	int sockfd;
	int status;
	int msg;
	int response;
	char config_list[256];
	struct sockaddr_in server;

	/* vérification des arguments */
	if(argc != 3) {
		fprintf(stderr,"Wrong argument : IPv4 address, UDP port required\n");
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
	server.sin_port = htons(atoi(argv[2]));
	status = inet_pton(AF_INET,argv[1],&server.sin_addr);
	if(status == -1) {
		fprintf(stderr,"%s (server address creation)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* menu principal */
	fprintf(stdout,"┌------MENU------┐\n");
	fprintf(stdout,"|  New Game (1)  |\n");
	fprintf(stdout,"|    Join (2)    |\n");
	fprintf(stdout,"└----------------┘\n");
	fprintf(stdout,">");
	fscanf(stdin,"%d",&msg);
	if(!((msg == NEW_GAME) || (msg == JOIN_GAME))) {
		fprintf(stderr,"Wrong input\n");
		exit(EXIT_FAILURE);
	}

	/* envoi serveur : choix menu */
	if(DEBUG) fprintf(stdout,"sending..\n");
	status = sendto(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&server,sizeof(struct sockaddr_in));
	if(status == -1) {
			fprintf(stderr,"%s (sending to the server)\n",strerror(errno));
			exit(EXIT_FAILURE);		
	}

	/* réception serveur : acknowledgment */
	fprintf(stdout,"..waiting\n");
	status = recvfrom(sockfd,&response,sizeof(response),0,NULL,NULL);
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
		fprintf(stdout,"..waiting\n");
		status = recvfrom(sockfd,config_list,sizeof(config_list),0,NULL,NULL);
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
		status = sendto(sockfd,&msg,sizeof(msg),0,(struct sockaddr*)&server,sizeof(struct sockaddr_in));
		if(status == -1) {
				fprintf(stderr,"%s (sending to the server)\n",strerror(errno));
				exit(EXIT_FAILURE);		
		}		
	}
	
	/* réception serveur : port TCP */


	/* fermeture de la socket */
	status = close(sockfd);
	if(status == -1) {
		fprintf(stderr,"%s (socket closing)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
