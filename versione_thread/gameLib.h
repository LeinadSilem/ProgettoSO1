#ifndef GATCHA_CROSSY_ROAD
#define GATCHA_CROSSY_ROAD

#include "entities.h"
#include <fcntl.h>
#include <pthread.h>

#define NUM_LOGS 3
#define NUM_RIVERS 3
#define NUM_LANES 3
#define NUM_CARS 3
#define NUM_DENS 5
#define OUCH 5
#define ENEMY_CHANCE 10
#define ATTACK_CHANCE 40

#define MIN_ROW_CAR 16
#define MAX_ROW_CAR 22
#define MIN_ROW_LOG 4
#define MAX_ROW_LOG 10
#define ZONES 4
#define WIDTH_DENS 9
#define MAXX 83
#define MAXY 28
#define PHROG_STARTING_LIVES 3
#define DELAY 3000
#define TESTING 1

//structures and type definitions

typedef struct den{
	Hitbox area;
	_Bool visited;
}Den;

typedef struct gamestate
{
	Entity player;
	Entity carTable[NUM_LANES][NUM_CARS];
	Entity logs[NUM_LOGS];
	Entity spiders[NUM_LOGS];
	Den Dens[NUM_DENS];
	int zoneLimitY[ZONES];
	WINDOW *gameWin, *statWin; 
}Gamestate;

int gameStart(int startingLives, _Bool dRegister[]);
void* phrog(void* param);
void initializeData(_Bool dRegister[]);
void screenRefresh();
void* roadsAndPonds(void* param);
int calcRow(int playerRow);
void drawMap();
void translateDirection(Direction dir);
int denCollisions();

#endif

