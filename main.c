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

	srand(time(NULL)); // Rand initialization

	mvprintw(MAXY,MAXX+1,"initializeScreen over\n");
}

void gameStart(){

	int gamePipe[2];

	if (pipe(gamePipe) == -1)
	{
		perror("eee la fin de la gamePipe");
		exit(-1);
	}

	mvprintw(MAXY+1,MAXX+1,"pipe is legal\n");

	switch(fork()){
		case -1:
			perror("eee la fin del fork ship");
			exit(-1);
		break;

		case 0:
			close(gamePipe[0]);
			phrog(gamePipe[1]);
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
