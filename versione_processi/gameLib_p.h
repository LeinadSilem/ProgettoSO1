#ifndef GATCHA_CROSSY_ROAD
#define GATCHA_CROSSY_ROAD

#include "entities_p.h"
#include <fcntl.h>

#define NUM_LOGS 3
#define NUM_RIVERS 3
#define NUM_LANES 3
#define NUM_CARS 3
#define NUM_DENS 5
#define OUCH 5
#define ENEMY_CHANCE 3
#define ATTACK_CHANCE 20

#define MIN_ROW_CAR 16
#define MAX_ROW_CAR 22
#define MIN_ROW_LOG 4
#define MAX_ROW_LOG 10
#define ZONES 4
#define WIDTH_DENS 9
#define MAXX 83
#define MAXY 28
#define PHROG_STARTING_LIVES 3
#define DELAY_SECONDS 3
#define TIMER 90
#define TESTING 0

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

void phrog(int lives,int pipewrite);
_Bool verifyInput(char input);

void carGenerator(int pipewrite);
void moveCar(Entity car,int pipewrite);
_Bool carCollisions(Entity currentCar, Entity phrog);
void haltCar(int currentCar, int row);

void spider(Entity log,int pipewrite);
void spit(int pipewrite, Hitbox pH, EntityType et);
void moveSpitBall(int pipewrite, Entity projectile);
int spitballCollisions(Entity spit);

void logGenerator(int pipewrite);
void moveLog(Entity log, int pipewrite);
_Bool logCollisions();

void updateEntity(Entity temp, int pipewrite);
void initializeData(_Bool dRegister[]);
int roadsAndPonds(int piperead, int pipewrite, _Bool* dRegister);
int denCollisions();

void drawMap();
void clearRiverInteractions();
void translateDirection(Direction dir);
void screenRefresh();

#endif

