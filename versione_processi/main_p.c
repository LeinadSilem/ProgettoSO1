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

	WINDOW* tryAgainWin;
	int totalLives = PHROG_STARTING_LIVES;
	int densVisited = 0;
	int resultOfMatch,i;
	_Bool denRegister[NUM_DENS] = {false,false,false,false,false};
	char playerSelection = 'y';

	tryAgainWin  = newwin(MAXY,MAXX,0,0);
	
	do{
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
			box(tryAgainWin,0,0);
			if(totalLives <= 0){			
				mvwprintw(tryAgainWin,MAXY/2,MAXX/2 - 21,  "you only reached %d/5 dens, wanna try again?",densVisited);
				mvwprintw(tryAgainWin,MAXY/2+1,MAXX/2 - 21,"   [y]:try for fame   [any]:lose in shame   ");
			}else{
				mvwprintw(tryAgainWin,MAXY/2,MAXX/2 - 26,  "you reached all dens, bravo! how about another try?");
				mvwprintw(tryAgainWin,MAXY/2+1,MAXX/2 - 26,"[y]:go for more success  [any]:retire like a coward");
			}
			
			// Restta i valori per una nuova partita
			densVisited = 0;
			totalLives = PHROG_STARTING_LIVES;	
			for(i = 0; i < NUM_DENS; i ++){
				denRegister[i] = false;
			}			
				
			wrefresh(tryAgainWin);
			playerSelection = getch();
			werase(tryAgainWin);
		}else{
			playerSelection = 'y';
		}
	}while(playerSelection == 'y');
}
