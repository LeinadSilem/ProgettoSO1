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
void spawnThreads();
void terminateGame();

int main()
{
	initializeScreen();
	spawnThreads();
	terminateGame();
	printf("Bye!");
	return 0;
}

void initializeScreen()
{
	/** Initial settings: screen and colours **/
	initscr();
	noecho();
	curs_set(false);
	erase();
	start_color();
	init_pair(RED, COLOR_RED, COLOR_BLACK);

	srand(time(NULL)); // Rand initialization
}

void spawnThreads()
{
	// pthread_t tMainThread, tShip, tUfo;
	int communicationPipe[2];
	int pidMainThread, pidPhrog, pidUfo;

	if (pipe(communicationPipe) == -1)
	{ 
		perror("Error in the initialization of communication pipes!");
		exit(-1);
	}


	pidPhrog = fork();

	if (pidPhrog == 0) // This process is ship process
	{
			close(communicationPipe[0]); // Closing reading pipe
			Phrog(communicationPipe[1]);
			exit(1);	
	}

	// This is the main thread
	mammarrancaFields(communicationPipe[0], communicationPipe[1]);
}

void terminateGame()
{
	/** Final settings: screen **/
	erase();
	curs_set(1);
	endwin();
}
