#ifndef NETWORK_H
#define NETWORK_H

#include "config.h"

#define MAX_GAMES 10

#define BEGIN_CONNECTION 	100
#define NEW_GAME 			101
#define JOIN_GAME 			102
#define CLOSE_SERVER 		200

#define FALSE 	0
#define TRUE 	1

#define ACCEPT 	0
#define DECLINE 1

#define FREE 	0
#define RUNNING 1

struct game_config {
	struct map map;
	struct scenario scenario;
	int tcp_port;
};

void* tcp_server(void* arg);
int new_game(int* games, int port);
int check_limit(int* games);
int show(int* games);

#endif
