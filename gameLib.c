#include "gameLib.h"

Gamestate game;

void Phrog(int pipewrite){

	Entity phrog;

	phrog.lives = 3;
	phrog.box.topleft.x = 1;
	phrog.box.topleft.y = 1;
	phrog.box.botright.x = 3;
	phrog.box.botright.y = 3;
	phrog.color = RED;
	phrog.dir = N;
	phrog.et = PHROG;

	write(pipewrite,&phrog,sizeof(Entity));

	Hitbox* phrogbox = &phrog.box;

	while(1){
		char c = getch();
		switch(c){
			case UP:
				if(phrogbox->topleft.y > 0){
					phrogbox->topleft.y -= 3;
					phrogbox->botright.y -= 3;
					phrog.row -=1;
					phrog.dir = N;
				}	
			break;

			case DOWN:
				if(phrogbox->botright.y < MAXY){
					phrogbox->topleft.y += 3;
					phrogbox->botright.y += 3;
					phrog.row +=1;
					phrog.dir = S;
				}				
			break;

			case LEFT:
				if(phrogbox->topleft.x > 0){
					phrogbox->topleft.x -= 1;
					phrogbox->botright.x -= 1;
					phrog.dir = W;
				}			
			break;

			case RIGHT:
				if(phrogbox->botright.x < MAXX){
					phrogbox->topleft.x += 1;
					phrogbox->botright.x += 1;
					phrog.dir = E;
				}				
			break;

			case ' ':
				//spit();
			break;

			case 'q':
				phrog.lives = 0;
			break;
		}

		write(pipewrite,&phrog,sizeof(Entity));
	}
}

void mammarrancaFields(int piperead, int pipewrite)
{
    Entity tempEntity, tempSpit, tempCar;
    Log tempLog;

    //drawFieldBorder();

    initGamestateData();
    
    while(game.running)
    {   
        //drawFieldBorder();
        read(piperead, &tempEntity, sizeof(Entity));

        switch (tempEntity.et)
        {
            case PHROG:
                clearer(game.player);
                game.player.box.topleft.x = tempEntity.box.topleft.x;
                game.player.box.topleft.y = tempEntity.box.topleft.y;
                game.player.box.botright.x = tempEntity.box.botright.x;
                game.player.box.botright.y = tempEntity.box.botright.y;
                game.player.pid = tempEntity.pid;
                game.player.row = tempEntity.row;
                game.player.dir = tempEntity.dir;
                if(game.player.lives > 0){
                    printer(tempEntity);
                }
            break;
        }
        refresh(); 
    }
}

void printer(Entity ent)
{
	int i,j,x,y;
    // matrice giocatore...
    char phrogBody [3][3] = {
        "\\-/",
        "(O)",
        "/-\\"
    };

    y = ent.box.topleft.y;
    x = ent.box.topleft.x;
    
    switch(ent.et){
		case PHROG:
			attron(COLOR_PAIR(ent.color));
			for(i = 0; i<PHROG_SIZE; i++){
				for(j = 0; j<PHROG_SIZE; i++){
					mvaddch(y+i,x+j,phrogBody[i][j]);
				}
			}
			attroff(COLOR_PAIR(ent.color));
		break;
	}
}

void clearer(Entity ent){
    int size,x,y,adjy,adjx;

    y = ent.box.topleft.y;
    x = ent.box.topleft.x;
    adjy = adjx = 0;

    switch(ent.et){
        case PHROG:            
            size = PHROG_SIZE;

            switch(ent.dir){
	            case N:
	            	adjy +=1;
				break;

				case S:
					adjy -=1;
				break;

				case W:
					adjx +=1;
				break;

				case E:
					adjx -=1;
				break;
            }
        break;
    }

    for(int i = 0; i < size; i++){
        for(int j = 0 ; j < size ; j++){
            mvaddch(y+i+adjy,x+j+adjx,' ');
        }
    }
}

void initGamestateData(){
	init_pair(WHITE,COLOR_WHITE,COLOR_BLACK);
    init_pair(RED,COLOR_RED,COLOR_BLACK);
    init_pair(YELLOW,COLOR_YELLOW,COLOR_BLACK);
    init_pair(GREEN,COLOR_GREEN,COLOR_BLACK);
    init_pair(BLUE,COLOR_BLUE,COLOR_BLACK);
    init_pair(MAGENTA,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(CYAN,COLOR_CYAN,COLOR_BLACK);

    game.player.lives = 3;
    game.player.color = RED;

    game.running = true;
    game.loss = false;
    game.win = false;
}

_Bool collisionDetection(Hitbox a, Hitbox b){
	if(((a.topleft.x >= b.topleft.x && a.topleft.x <= b.botright.x) || (a.botright.x >= b.topleft.x && a.botright.x <= b.botright.x)) &&
       ((a.topleft.y >= b.topleft.y && a.topleft.y <= b.botright.y) || (a.botright.y >= b.topleft.y && a.botright.y <= b.botright.y))) {
        return true;
    } else {
        return false;
    }
}