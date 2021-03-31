#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE */
#include <stdio.h> /* fprintf() */
#include <sys/socket.h> /* socket() */
#include <string.h> /* strerror(), memset() */
#include <errno.h> /* errno */
#include <arpa/inet.h> /* htons(), htonl() */
#include <unistd.h> /* close() */

int main(int argc, char* argv[]) {
	
	int fd;
	int status;
	char message[256];
	struct sockaddr_in address;

	/* vérification des arguments */
	if(argc != 2) {
		fprintf(stderr,"Wrong argument. Please specify a valid UDP port\n");
		exit(EXIT_FAILURE);
	}

	/* ouverture de la socket */
	fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(fd == -1) {
		fprintf(stderr,"%s (socket opening)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* création de l'adresse du serveur */
	memset(&address,0,sizeof(struct sockaddr_in));
	address.sin_family = AF_INET;
	address.sin_port = htons(atoi(argv[1]));
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	/* nommage de la socket */
	status = bind(fd,(struct sockaddr*)&address,sizeof(struct sockaddr_in));
	if(status == -1) {
		fprintf(stderr,"%s (socket binding)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* préparation du message pour la réception */
	memset(message,0,sizeof(message));

	/* réception depuis le client */
	status = recvfrom(fd,message,sizeof(message),0,NULL,NULL);
	if(status == -1) {
		fprintf(stderr,"%s (receiving from the client)\n",strerror(errno));
		exit(EXIT_FAILURE);		
	}

	/* affichage du message */
	fprintf(stdout,"%s\n",message);

	/* fermeture de la socket */
	status = close(fd);
	if(status == -1) {
		fprintf(stderr,"%s (socket closing)\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
