#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE */
#include <stdio.h> /* fprintf() */
#include <sys/socket.h> /* socket() */
#include <string.h> /* strerror(), memset() */
#include <errno.h> /* errno */
#include <arpa/inet.h> /* htons(), inet_pton() */
#include <unistd.h> /* close() */
#include "network.h"

int main(int argc, char* argv[]) {

	int sockfd;
	int status;
	int value;
	int i;
	char* map_list[3];
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
	fprintf(stdout,"MENU\n1 : Start a game\n2 : See maps\n3 : See scenarios\n4 : See lobbies\n");
	fscanf(stdin,"%d",&value);

	/* envoi au serveur : choix menu */
	fprintf(stdout,"sending..\n");
	status = sendto(sockfd,&value,sizeof(value),0,(struct sockaddr*)&server,sizeof(struct sockaddr_in));
	if(status == -1) {
		fprintf(stderr,"%s (sending to the server)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}

	/* réception du serveur */
	fprintf(stdout,"..waiting\n");
	status = recvfrom(sockfd,&map_list,sizeof(map_list),0,NULL,NULL);
	if(status == -1) {
		fprintf(stderr,"%s (receiving from the server)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}

	/* affichage */
	for(i = 0; i < 3; i++) {
		fprintf(stdout,"%s\n",map_list[i]);
	}

	/* fermeture de la socket */
	status = close(sockfd);
	if(status == -1) {
		fprintf(stderr,"%s (socket closing)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
