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
}Entity;

typedef struct logs
{
	Entity data;
	Entity *container;
}Logs;

void printMap();
_Bool collisionDetection(Hitbox a, Hitbox b);
void Phrog(int pipewrite);
void mammarrancaFields(int piperead, int pipewrite);
void printer(Entity tempEntity);
void clearer(Entity tempEntity);

#endif