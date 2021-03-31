#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE */
#include <stdio.h> /* fprintf() */
#include <sys/socket.h> /* socket() */
#include <string.h> /* strerror(), memset() */
#include <errno.h> /* errno */
#include <arpa/inet.h> /* htons(), inet_pton() */
#include <unistd.h> /* close() */

int main(int argc, char* argv[]) {

	int fd;
	int status;
	char message[256];
	struct sockaddr_in server_address;

	/* vérification des arguments */
	if(argc != 4) {
		fprintf(stderr,"Wrong argument. Please specify the IPv4 address of the server, its UDP port and a valid message\n");
		exit(EXIT_FAILURE);
	}

	/* ouverture de la socket */
	fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(fd == -1) {
		fprintf(stderr,"%s (socket opening)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* création de l'adresse du serveur */
	memset(&server_address,0,sizeof(struct sockaddr_in));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(argv[2]));
	status = inet_pton(AF_INET,argv[1],&server_address.sin_addr);
	if(status == -1) {
		fprintf(stderr,"%s (server address creation)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* préparation du message pour l'envoi */
	memset(message,0,sizeof(message));
	strcpy(message,argv[3]);

	/* envoi au serveur */
	status = sendto(fd,message,sizeof(message),0,(struct sockaddr*)&server_address,sizeof(struct sockaddr_in));
	if(status == -1) {
		fprintf(stderr,"%s (sending to the server)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}

	/* fermeture de la socket */
	status = close(fd);
	if(status == -1) {
		fprintf(stderr,"%s (socket closing)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
