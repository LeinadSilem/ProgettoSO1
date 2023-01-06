#include "gameLib.h"

void initialize();
int gameStart(int startingLives);
void gameEnd();
void gameLoop();

int main(){
	initialize();
	gameLoop();
	gameEnd();
	printf("game over hoe!");
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

int gameStart(int startingLives){

	int gamePipe[2];
	int lifeline[2];
	int result;

	if (pipe(gamePipe) == -1)
	{
		perror("eee la fin de la gamePipe");
		exit(-1);
	}

	if (pipe(lifeline) == -1 || fcntl(lifeline[0], F_SETFL, O_NONBLOCK) < 0)
	{
		perror("aw hell naw spunchbop, my lifeline died");
		exit(-1);
	}
	

	switch(fork()){
		case -1:
			perror("eee la fin del fork player");
			exit(-1);
		break;

		case 0:
			close(gamePipe[0]);
			close(lifeline[1]);
			phrog(startingLives,gamePipe[1]);
			exit(1);
		break;
		
		default:
			switch(fork()){
				case -1:
					perror("eee la fin del fork cars");
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
								perror("eee la fin del fork tronky");
								exit(-1);
							break;

							case 0:
								close(gamePipe[0]);
								logGenerator(gamePipe[1]);
								exit(1);
							break;

							default:
								roadsAndPonds(gamePipe[0],gamePipe[1]);	
							break;
						}					
				break;
			}
			
		break;
	}
}

void gameLoop(){

	int totalLives = PHROG_STARTING_LIVES;
	int densVisited = 0;
	int resultOfMatch;
	_Bool denRegister[NUM_DENS] = {false,false,false,false,false};
	

	while(true){
		//converrebbe spostare la funzione della mappa qui
		resultOfMatch = gameStart(totalLives);

		if(resultOfMatch < NUM_DENS && !denRegister[resultOfMatch]){
			densVisited +=1;
			denRegister[resultOfMatch] = true;
		}else{
			totalLives -= 1;
		}

		if(totalLives < 0 || densVisited == NUM_DENS){
			if(totalLives < 0){
				printf("game over, you suck");
			}else{
				printf("you win, you saved the frogs in california");
			}
			exit(0);
		}
	}
}