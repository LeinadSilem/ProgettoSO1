#ifndef GATCHA_CROSSY_ROAD
#define GATCHA_CROSSY_ROAD

#include "entities_t.h"
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
#define TIMER 30
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
	entityList* projectilePtr;
	Den Dens[NUM_DENS];
	int zoneLimitY[ZONES];
	_Bool gameReady;
	_Bool alreadyStarted;
	WINDOW *gameWin, *statWin; 
}Gamestate;

int gameStart(int startingLives, _Bool dRegister[]);

void* phrog(void* param);

void* carGenerator();
void* moveCar(void* param);
void haltCar(int currentCar, int row);
_Bool carCollisions(Entity currentCar, Entity phrog);

void* logGenerator();
void* moveLog(void* param);
_Bool logCollisions();

void* spider(void* param);

void spit(Entity shooter);
void* moveSpitBall(void* param);
int spitballCollisions(Entity spit);

void initializeData(_Bool dRegister[]);
int roadsAndPonds(_Bool dRegister[]);

void screenRefresh();
void drawMap();
void swap(Entity *a, Entity *b);
void translateDirection(Direction dir);
int denCollisions();
void clearMemory();

entityList* initEntityList();
entityNode *insert(Entity data, entityList *list);
entityList* eraseEntity(entityNode *forDeletion, entityList *list);

#endif

