# Useful functions i store away when i reset the project

## Util functions
~~~C
// function that counts visited dens
int getVisitedDens(){
	int i,count;
	for(i = 0; i < NUM_DENS; i++){
		if(game.visitedDens[i].visited){
			count +=1;
		}
	}
	return count;
}

// the printer
void printer(Entity ent)
{
	int i,j,x,y;
    // matrice giocatore...
    char phrogBody [PHROG_SIZE][PHROG_SIZE] = {
        "\\-/",
        "(O)",
        "/-\\"
    };

    char carFrontSectionRight [PHROG_SIZE][2] = {
    	"o-",
    	"|C",
    	"o-"
    };

    char carFrontSectionLeft [PHROG_SIZE][2] = {
    	"-o",
    	"D|",
    	"-o"
    };

    char carBackSectionRight [PHROG_SIZE][2] = {
    	"-o",
    	"K|",
    	"-o"
    };

    char carBackSectionLeft [PHROG_SIZE][2] = {
    	"o-",
    	"|H",
    	"o-"
    };

    char carMiddleSection [PHROG_SIZE][1] = {
    	"-",
    	" ",
    	"-"
    };

    y = ent.box.topleft.y;
    x = ent.box.topleft.x;
    
    switch(ent.et){
		case PHROG:
			attron(COLOR_PAIR(ent.color));
			for(i = 0; i < PHROG_SIZE; i++){
				for(j = 0; j < PHROG_SIZE; i++){
					mvaddch(y+i,x+j,phrogBody[i][j]);
				}
			}
			attroff(COLOR_PAIR(ent.color));
		break;
	}
}

// the clearer
void clearer(Entity ent)
{
    int size,x,y,adjy,adjx;

    y = ent.box.topleft.y;
    x = ent.box.topleft.x;
    adjy = adjx = 0;

    switch(ent.et){
        case PHROG:            
            size = PHROG_SIZE;

            switch(ent.dir){
	            case N:
	            	adjy +=1;
				break;

				case S:
					adjy -=1;
				break;

				case W:
					adjx +=1;
				break;

				case E:
					adjx -=1;
				break;
            }
        break;
    }

    for(int i = 0; i < size; i++){
        for(int j = 0 ; j < size ; j++){
            mvaddch(y+i+adjy,x+j+adjx,' ');
        }
    }
}

// aabb collision detection
_Bool collisionDetection(Hitbox a, Hitbox b)
{
	if(((a.topleft.x >= b.topleft.x && a.topleft.x <= b.botright.x) || (a.botright.x >= b.topleft.x && a.botright.x <= b.botright.x)) &&
       ((a.topleft.y >= b.topleft.y && a.topleft.y <= b.botright.y) || (a.botright.y >= b.topleft.y && a.botright.y <= b.botright.y))) {
        return true;
    } else {
        return false;
    }
}

// draws the border
void drawFieldBorder()
{
    int i = 0;
    int j = 0;
    for (j; j <= MAXY; j++) {
        i = 0;
        if (j >= 1 && j < MAXY) {
            mvaddch(j, i, ACS_VLINE);
            mvaddch(j, MAXX, ACS_VLINE);

        } else if (j == 0 || j == MAXY) {
            for (i = 0; i < MAXX; i++) {
                mvaddch(j, i, ACS_HLINE);
                mvaddch(j, MAXX, ACS_HLINE);
            }
        }
    }
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, MAXX, ACS_URCORNER);
    mvaddch(MAXY, MAXX, ACS_LRCORNER);
    mvaddch(MAXY, 0, ACS_LLCORNER);
}
~~~

## System functions and structures
~~~C

//structures and type definitions
typedef enum {RED,YELLOW,GREEN,CYAN,BLUE,MAGENTA,WHITE}Color;
typedef enum {N,S,W,E,FIXED}Direction;
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
	int row,col,length;
	pid_t pid;
}Entity;

typedef struct logs
{
	Entity data;
	Entity *container;
}Log;

typedef struct den{
	Position coords;
	_Bool visited;
}Den;

typedef struct gamestate
{
	Entity player;
	Entity carTable[NUM_LANES][NUM_CARS];
	Entity logs[NUM_LOGS];
	Direction dirLanes[NUM_LANES];
	Direction dirRivers[NUM_LOGS];	
	Den visitedDens[NUM_DENS];
	_Bool running,loss,win;
}Gamestate;

// functions

void initializeGameElements()
{
	int i,j;

	game.running = true;
    game.loss = false;
    game.win = false;

	// init colors
	init_pair(WHITE,COLOR_WHITE,COLOR_BLACK);
    init_pair(RED,COLOR_RED,COLOR_BLACK);
    init_pair(YELLOW,COLOR_YELLOW,COLOR_BLACK);
    init_pair(GREEN,COLOR_GREEN,COLOR_BLACK);
    init_pair(BLUE,COLOR_BLUE,COLOR_BLACK);
    init_pair(MAGENTA,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(CYAN,COLOR_CYAN,COLOR_BLACK);

	// initPlayer
	game.player.lives = 3;
	game.player.color = RED;
	game.player.row = game.player.col = 0;
	mvprintw(MAXY+3,MAXX+1,"player stats initialized");


	// init car & log directions
	for(i = 0; i < NUM_LANES; i++){
		if(rand()%2 == 0){
			game.dirLanes[i] = E;
		}else{
			game.dirLanes[i] = W;
		}
	}

	for(i = 0; i < NUM_LOGS; i++){
		if(rand()%2 == 0){
			game.dirRivers[i] = W;
		}else{
			game.dirRivers[i] = E;
		}
	}
	mvprintw(MAXY+4,MAXX+1,"player stats initialized");

	// init cars
	for(i = 0; i < NUM_LANES; i++){
		for(j = 0; j < NUM_CARS; j++){
			game.carTable[i][j].row = i;
			game.carTable[i][j].col = j;
			game.carTable[i][j].dir = game.dirLanes[j]; 
		}
	}
	mvprintw(MAXY+5,MAXX+1,"player stats initialized");

	// init logs
	for(i = 0; i < NUM_LOGS; i++){
		game.logs[i].row = i;
		game.logs[i].dir = game.dirRivers[i];
	}
	mvprintw(MAXY+6,MAXX+1,"player stats initialized");

	// init dens
	for(i = 0; i < NUM_DENS; i++){
		game.visitedDens[i].coords.x = 0 + (i*PHROG_SIZE*2);
		game.visitedDens[i].coords.y = 0;
		game.visitedDens[i].visited = false;
	}
	mvprintw(MAXY+7,MAXX+1,"player stats initialized");
}

void carGenerator(int pipewrite)
{
	int i,j;
	Entity car[NUM_LANES][NUM_CARS];
	
	for(i = 0; i < NUM_LANES; i++){
		for(j = 0; j < NUM_CARS; j++){

			// deciding map position and direction
			car[i][j].row = i;
			car[i][j].col = j;

			// deciding characteristics
			car[i][j].length = 4 +rand()%(6);
			if(car[i][j].length > 6){
				car[i][j].color = BLUE;
			}else {
				car[i][j].color = CYAN;
			}

			// deciding starting methods and hitbox measurements
			if(car[i][j].dir == W){
				car[i][j].box.topleft.x = 0 - car[i][j].length;
			}else{
				car[i][j].box.topleft.x = MAXX;
			}

			car[i][j].box.topleft.y = MIN_ROW_CAR + (i*PHROG_SIZE);
			
			car[i][j].box.botright.x = car[i][j].box.topleft.x + car[i][j].length;
			car[i][j].box.botright.y = car[i][j].box.topleft.y + PHROG_SIZE;
		}
	}

	// timed interval generation
}

void logGenerator(int pipewrite)
{
	int i;
	Log log[NUM_LOGS];

	for(i = 0; i < NUM_LOGS; i++){
		// deciding map position and direction
		log[i].data.row = i;

		// deciding characteristics
		log[i].data.color = YELLOW;
		if(rand()%2 == 0){
			log[i].data.length = 6;
		}else{
			log[i].data.length = 9;
		}

		// deciding starting methods and hitbox measurements
		if(log[i].data.dir == W){
			log[i].data.box.topleft.x = 0 - log[i].data.length;
		}else{
			log[i].data.box.topleft.x = MAXX;
		}

		log[i].data.box.topleft.y = MIN_ROW_CAR + (i*PHROG_SIZE);
		
		log[i].data.box.botright.x = log[i].data.box.topleft.x + log[i].data.length;
		log[i].data.box.botright.y = log[i].data.box.topleft.y + PHROG_SIZE;

		// chance of enemy spawning

		// send log
	}
}
~~~

## Macros and libs
~~~C
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
#define PHROG_SIZE 3
#define NUM_LOGS 3
#define NUM_LANES 3
#define NUM_CARS 3
#define NUM_DENS 5
#define MAXX 27
#define MAXY 27
#define MIN_ROW_CAR 15
#define MIN_ROW_LOG 3
#define ENEMY_CHANCE 20;
~~~

## Makefile
~~~makefile
SDT: main.o gameLib.o
	gcc main.o gameLib.o -o SDP -lncurses

main.o: main.c
	gcc -c main.c

gamelib.o: gameLib.c gameLib.h
	gcc -c gameLib.c -lncurses

clean:
	rm *o SDP
~~~