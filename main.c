#include "gameLib.h"

void initialize();
void gameStart();
void gameEnd();

int main(){
	initialize();
	gameStart();
	gameEnd();
	printf("game over!");
	return 0;
}

void initialize(){
	
	initscr();
	noecho();
	curs_set(0);
	erase();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);

	srand(time(NULL));
}

void gameEnd(){	
	erase();
	curs_set(1);
	endwin();
}

void gameStart(){

	int gamePipe[2];

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
			phrog(PHROG_STARTING_LIVES,gamePipe[1]);
			exit(1);
		break;
		
		default:
			switch(fork()){
				case -1:
					perror("eee la fin del fork alienGenerator");
					exit(-1);
				break;

				case 0:
					close(gamePipe[0]);
					carGenerator(gamePipe[1]);
					exit(1);
				break;

				default:
						switch(fork()){
							case -1:
								perror("eee la fin del fork alienGenerator");
								exit(-1);
							break;

							case 0:
								close(gamePipe[0]);
								logGenerator(gamePipe[1]);
								exit(1);
							break;

							default:
								roadsAndPonds(gamePipe[0], gamePipe[1]);	
							break;
						}					
				break;
			}
			
		break;
	}
}


