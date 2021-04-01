#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE */
#include <stdio.h> /* fprintf() */
#include <sys/socket.h> /* socket() */
#include <string.h> /* strerror(), memset() */
#include <errno.h> /* errno */
#include <arpa/inet.h> /* htons(), htonl() */
#include <unistd.h> /* close() */
#include "network.h"

int main(int argc, char* argv[]) {
	
	int sockfd;
	int status;
	int value;
	socklen_t addrlen;
	char buffer[256];
	char* map_list[3];
	struct sockaddr_in server;
	struct sockaddr_in client;

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
	fprintf(stdout,"Access this server via %s:%d\n",buffer,atoi(argv[1]));

	/* chargement des données */
	strcat(map_list[0],"Easy");
	strcat(map_list[1],"Medium");
	strcat(map_list[2],"Hard");

	/* réception du client : choix menu */
	fprintf(stdout,"..waiting\n");
	addrlen = sizeof(struct sockaddr_in);
	status = recvfrom(sockfd,&value,sizeof(value),0,(struct sockaddr*)&client,&addrlen);
	if(status == -1) {
		fprintf(stderr,"%s (receiving from the client)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}

	/* traitement */
	switch(value) {
		case NEW_GAME:
			break;
		case GET_MAPS:
			fprintf(stdout,"sending..\n");
			status = sendto(sockfd,&map_list,sizeof(map_list),0,(struct sockaddr*)&client,sizeof(struct sockaddr_in));
			if(status == -1) {
				fprintf(stderr,"%s (sending to the server)\n",strerror(errno));
				exit(EXIT_FAILURE);		
			}
			break;
		case GET_SCENARIOS:
			break;
		case GET_LOBBIES:
			break;
	}

	/* fermeture de la socket */
	status = close(sockfd);
	if(status == -1) {
		fprintf(stderr,"%s (socket closing)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
