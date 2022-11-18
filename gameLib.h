#ifndef GATCHA_CROSSY_ROAD
#define GATCHA_CROSSY_ROAD

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
#define PHROG_SIZE 3
#define NUM_LOGS 3
#define NUM_CARS 3
#define NUM_DENS 5
#define CAR_LANES 5
#define LOG_LANES 3
#define DEN1X
#define DEN2X
#define DEN3X
#define DEN4X
#define DEN5X
#define MAXX 21
#define MAXY 38

typedef enum {RED,YELLOW,GREEN,CYAN,BLUE,MAGENTA,WHITE}Color;
typedef enum {N,S,W,E}Direction;
typedef enum {PHROG,SPIDER,CAR,LOG}EntityType;

typedef struct position
{
	int x;
	int y;
}Position;

typedef struct hitbox
{
	Position topleft;
	Position botright;
}Hitbox;

typedef struct data
{
	Color color;
	Direction dir;
	EntityType et;
	Hitbox box;
	int lives;
	int row;
	pid_t pid;
}Entity;

typedef struct logs
{
	Entity data;
	Entity *container;
}Log;

typedef struct gamestate
{
	Entity player;
	Log lane2[NUM_LOGS];
	Log lane3[NUM_LOGS];
	Log lane4[NUM_LOGS];
	Entity lane6[NUM_CARS];
	Entity lane7[NUM_CARS];
	Entity lane8[NUM_CARS];
	Entity lane9[NUM_CARS];
	Entity lane10[NUM_CARS];
	_Bool visitedDens[NUM_DENS];
	_Bool running,loss,win;
}Gamestate;

void printMap();
_Bool collisionDetection(Hitbox a, Hitbox b);
void Phrog(int pipewrite);
void mammarrancaFields(int piperead, int pipewrite);
void printer(Entity tempEntity);
void clearer(Entity tempEntity);
void initGamestateData();

#endif