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
