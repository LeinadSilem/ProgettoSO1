#ifndef GATCHA_CROSSY_ROAD
#define GATCHA_CROSSY_ROAD

#include "entities.h"

#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68

#define NUM_LOGS 3
#define NUM_RIVERS 3
#define NUM_LANES 3
#define NUM_CARS 3
#define NUM_DENS 5
#define ENEMY_CHANCE 20;

#define MIN_ROW_CAR 16
#define MIN_ROW_LOG 4
#define ZONES 4
#define WIDTH_DENS 9
#define MAXX 82
#define MAXY 28

//structures and type definitions

typedef struct den{
	Position coords;
	_Bool visited;
}Den;

typedef struct gamestate
{
	Entity player;
	Entity carTable[NUM_LANES][NUM_CARS];
	Entity logs[NUM_LOGS];
	Den visitedDens[NUM_DENS];
	int zoneLimitY[ZONES];
	_Bool running,loss,win;
}Gamestate;

//void gameEnd();
void phrog(int pipewrite);
void spit(int pipewrite, Hitbox pH);
void moveSpitBall(int pipewrite, Entity projectile);
void moveCar(Entity car,int pipewrite);
void moveLog(Entity log, int pipewrite);
void carGenerator(int pipewrite);
void logGenerator(int pipewrite);
void roadsAndPonds(int piperead, int pipewrite);
//void projectileCollisions(Entity currentMissile, int pipewrite);
//void phrogCollisions(Entity temp);
//void checkBorderProximity(int topLeftx);
_Bool verifyHitbox(Hitbox a, Hitbox b);
void updateEntity(Entity temp, int pipewrite);
void initializeData();
void drawMap();
void translateDirection(Direction dir);

#endif
