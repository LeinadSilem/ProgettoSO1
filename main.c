#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#include "gameLib.h"

void initializeScreen();
void gameStart();
void terminateGame();

int main()
{
	initializeScreen();
	gameStart();
	terminateGame();
	printf("Bye!");
	return 0;
}

void initializeScreen()
{
	/** Initial settings: screen and colours **/
	initscr();
	noecho();
	curs_set(0);
	erase();
	start_color();
	init_pair(RED, COLOR_RED, COLOR_BLACK);

	srand(time(NULL)); // Rand initialization
}

void gameStart(){

	int gamePipe[2];
	pid_t pidPhrog;

	if (pipe(gamePipe) == -1)
	{
		perror("eee la fin de la gamePipe");
		exit(-1);
	}

	
	switch(fork()){
		case -1:
			perror("eee la fin del fork ship");
			exit(-1);
		break;

		case 0:
			close(gamePipe[0]);
			Phrog(gamePipe[1]);
			exit(1);
		break;
		
		default:
			mammarrancaFields(gamePipe[0],gamePipe[1]);
		break;
	}
}

void terminateGame()
{
	/** Final settings: screen **/
	erase();
	curs_set(1);
	endwin();
}
