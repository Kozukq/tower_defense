all : client serveur

client : client.o
	gcc -o client client.o

serveur : serveur.o
	gcc -o serveur serveur.o

client.o : client.c client.h communication.h
	gcc -c client.c

serveur.o : serveur.c serveur.h communication.h
	gcc -c serveur.c

clean :
	rm *.o client serveur
