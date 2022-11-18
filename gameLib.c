#include "gameLib.h"

char carBody1 [3][4] = 
{
	{'o','-','-','o'},
	{'|','E','D',')'},
	{'o','-','-','o'}
};

char carBody2 [3][5] =
{
	{'o','-','-','-','o'},
	{'|','E','H','D',')'},
	{'o','-','-','-','o'}
};

char carBody3 [3][6] =
{
	{'o','-','-','-','-','o'},
	{'|','E','H','H','D',')'},
	{'o','-','-','-','-','o'}
};

char logBody1 [3][4] = 
{
	{'(','-','-',')'},
	{'(',' ',' ',')'},
	{'(','-','-',')'}
};

char logBody2 [3][5] = 
{
	{'(','-','-','-',')'},
	{'(',' ',' ',' ',')'},
	{'(','-','-','-',')'}
};

char logBody3 [3][6] = 
{
	{'(','-','-','-','-',')'},
	{'(',' ',' ',' ',' ',')'},
	{'(','-','-','-','-',')'}
};

char phrogBody[3][3] = 
{
	{'\\','_','/'},
	{'(','O',')'},
	{'/','-','\\'}
};

void Phrog(int pipewrite){

	Entity phrog;

	phrog.lives = 3;
	phrog.box.topleft.x = 0;
	phrog.box.topleft.y = 0;
	phrog.box.botright.x = 0;
	phrog.box.botright.y = 0;
	phrog.color = RED;
	phrog.dir = N;
	phrog.et = PHROG;

	write(pipewrite,&phrog,sizeof(Entity));

	Hitbox* phrogbox = &phrog.box;

	while(1){
		char c = getch();
		switch(c){
			case UP:
				phrogbox->topleft.y -= 1;
				phrogbox->botright.y -= 1;
			break;

			case DOWN:
				phrogbox->topleft.y += 1;
				phrogbox->botright.y += 1;
			break;

			case LEFT:
				phrogbox->topleft.x += 1;
				phrogbox->botright.x += 1;
			break;

			case RIGHT:
				phrogbox->topleft.x -= 1;
				phrogbox->botright.x -= 1;
			break;

			case ' ':
				//spit();
			break;

			case 'q':
				exit(0);
			break;
		}

		write(pipewrite,&phrog,sizeof(Entity));
	}
}

void mammarrancaFields(int piperead, int pipewrite){
	_Bool gamestate = true;
	Entity tempEntity;

	while(gamestate){
		read(piperead,&tempEntity,sizeof(Entity));
		switch(tempEntity.et){
			case PHROG:
				clearer(tempEntity);
				printer(tempEntity);
			break;
		}
	}
}

void printer(Entity tempEntity){
	
	int i = 0,j = 0;

	switch(tempEntity.et){
		case PHROG:
			attron(COLOR_PAIR(tempEntity.color));
			for(i = 0; i<PHROG_SIZE; i++){
				for(j = 0; j<PHROG_SIZE; i++){
					mvaddch(tempEntity.box.topleft.y+i,tempEntity.box.topleft.x+j,phrogBody[i][j]);
				}
			}
			attroff(COLOR_PAIR(tempEntity.color));
		break;
	}
}

void clearer(Entity tempEntity){

	int i = 0, j = 0, adjX = 0, adjY = 0;

	switch(tempEntity.et){
		case PHROG:

			switch(tempEntity.dir){
				case UP:
					adjY = 1;				
				break;

				case DOWN:
					adjY = -1;
				break;

				case LEFT:
					adjX = 1;
				break;

				case RIGHT:
					adjX = -1;
				break;
			}

			for(i = 0; i<PHROG_SIZE; i++){
				for(j = 0; j<PHROG_SIZE; i++){
					mvaddch(tempEntity.box.topleft.y+i+adjY,tempEntity.box.topleft.x+j+adjX,' ');
				}
			}
		break;
	}
}