#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define DEBUG 1

struct map {
	uint8_t board[15][15];
	char description[256];
};

union char255_uint {
	char text[255];
	uint32_t number;
};

struct event {
	union char255_uint value;
	uint64_t duration;
	uint8_t type;
};

struct scenario {
	struct event events[200];
	char description[256];
};

struct config {
	struct map maps[3];
	struct scenario scenarios[3];
};

void load(char* dirpath, struct config* config);
void load_map(char* filepath, struct map* map);
void load_scenario(char* filepath, struct scenario* scenario);

#endif
