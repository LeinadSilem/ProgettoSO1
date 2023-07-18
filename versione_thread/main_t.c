#include "gameLib_t.h"

void initialize();
void gameEnd();
void gameLoop();

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


void gameLoop(){

	WINDOW* tryAgainWin;
	int totalLives = PHROG_STARTING_LIVES;
	int densVisited = 0;
	int resultOfMatch;
	_Bool denRegister[NUM_DENS] = {false,false,false,false,false};
	char playerSelection = 'y';

	tryAgainWin  = newwin(MAXY,MAXX,0,0);
	
	do{
		if(totalLives <= 0){
			totalLives = PHROG_STARTING_LIVES;
		}
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
			wrefresh(tryAgainWin);
			playerSelection = getchar();
			werase(tryAgainWin);
		}else{
			playerSelection = 'y';
		}

	}while(playerSelection == 'y');
}
