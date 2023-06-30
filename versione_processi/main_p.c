#include "gameLib_p.h"

void initialize();
void gameEnd();
void gameLoop();
int gameStart(int startingLives, _Bool dRegister[]);

int main(){
	initialize();
	gameLoop();
	gameEnd();
	return 0;
}

void initialize(){
	
	initscr();
	noecho();
	curs_set(0);
	erase();
	start_color();

	srand(time(NULL));
}

void gameEnd(){	
	erase();
	curs_set(1);
	endwin();
}


int gameStart(int startingLives, _Bool dRegister[]){

	int gamePipe[2];
	int result;

	pid_t carPid;
	pid_t logPid;
	pid_t playerPid;

	if (pipe(gamePipe) == -1)
	{
		perror("eee la fin de la gamePipe");
		exit(-1);
	}

	switch(playerPid = fork()){
		case -1:
			perror("eee la fin del fork player");
			exit(-1);
		break;

		case 0:
			close(gamePipe[0]);
			phrog(startingLives,gamePipe[1]);
			exit(1);
		break;
		//default order of things happening	
		default:
			carPid = fork();
			switch(carPid){
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
					logPid = fork();
					switch(logPid){
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
							result = roadsAndPonds(gamePipe[0],gamePipe[1],dRegister);	
						break;
					}					
				break;
			}		
		break;
	}

	if(result > -1){
		kill(playerPid,1);
		kill(carPid,1);
		kill(logPid,1);
	}

	return result;
}


void gameLoop(){

	int totalLives = PHROG_STARTING_LIVES;
	int densVisited = 0;
	int resultOfMatch;
	_Bool denRegister[NUM_DENS] = {false,false,false,false,false};
	
	while(densVisited < 5 && totalLives > 0){
		resultOfMatch = gameStart(totalLives,denRegister);

		//verify the result
		if(resultOfMatch < NUM_DENS && !denRegister[resultOfMatch]){
			densVisited +=1;
			denRegister[resultOfMatch] = true;
		}else{
			totalLives -= 1;
		}

		//controls for game ending
		if(totalLives < 1 || densVisited == NUM_DENS){
			if(totalLives <= 0){
				printf("game over, you suck");
			}else{
				printf("you win, you saved the frogs in california");
			}
		}
	}
}
