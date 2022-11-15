#ifndef GATCHA_CROSSY_ROAD
#define GATCHA_CROSSY_ROAD

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

typedef enum {RED,YELLOW,GREEN,CYAN,BLUE,MAGENTA,WHITE}Color;
typedef enum {UP,DOWN,LEFT,RIGHT}Direction;
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
	Color c;
	Direction d;
	EntityType et;
	Hitbox h;
	int lives;
}Data;

typedef struct entity
{
	Data data;
	Data *container;
}Entity;

void printMap();
_Bool collisionDetection(Hitbox a, Hitbox b);

#endif