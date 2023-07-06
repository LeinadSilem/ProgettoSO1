#ifndef GATCHA_CROSSY_ROAD_SPRITES
#define GATCHA_CROSSY_ROAD_SPRITES

#define PHROG_SIZE 3
#define TESTING 1

#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef enum {ROAD,WATER,
              LOGS,TRUCKS,CARS,SPIDER_COLOR,
              PHROG_ON_ROAD,PHROG_ON_GRASS,PHROG_ON_LOG,
              SPIT_ON_ROAD,SPIT_ON_GRASS,SPIT_ON_LOG,SAFE_ZONE
              }Color;
typedef enum {N,S,W,E,FIRE,FIXED}Direction;
typedef enum {PHROG,SPIDER,CAR,LOG,SPITBALL}EntityType;

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
    pthread_t id;
    _Bool isOnLog;
    _Bool hasSpider;
}Entity;

typedef struct entNode{
    struct entNode* prev;
    Entity data;
    struct entNode* next;
}entityNode;

typedef struct entList{
    entityNode* head;
    entityNode* tail;
    int len;
}entityList;

void printerLogs(Entity ent, WINDOW* win);
void printerCars(Entity ent, WINDOW* win);
void bodyClearing(Entity ent, WINDOW* win);
void bodyClearingPlayer(Entity ent, WINDOW* win);
void printerSingleEntities(Entity ent, WINDOW* win);
_Bool verifyHitbox(Hitbox a, Hitbox b);
#endif