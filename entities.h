#ifndef GATCHA_CROSSY_ROAD_SPRITES
#define GATCHA_CROSSY_ROAD_SPRITES

#define PHROG_SIZE 3

#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef enum {WHITE,RED,YELLOW,GREEN,CYAN,BLUE,MAGENTA}Color;
typedef enum {N,S,W,E,FIXED}Direction;
typedef enum {PHROG,SPIDER,CAR,LOG,SPITBALL,EMPTY}EntityType;

typedef struct position
{
    int x;
    int y;
}Position;

typedef struct hitbox
{
    Position topLeft;
    Position botRight;
}Hitbox;

typedef struct data
{
    Color color;
    Direction dir;
    EntityType et;
    Hitbox box;
    int lives;
    int row,col,length;
    pid_t pid;
    _Bool isOnLog;
}Entity;

void printerLogs(Entity ent);
void printerCars(Entity ent);
void bodyClearing(Entity ent);
void bodyClearingSingleEntities(Entity ent);
void printerSingleEntities(Entity ent);
_Bool verifyHitbox(Hitbox a, Hitbox b);
#endif