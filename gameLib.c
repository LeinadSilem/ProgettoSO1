#include "gameLib.h"

Gamestate game;

// WORK IN PROGRESS
void phrog(int pipewrite)
{
    char input;
    Entity player;

    player.box.topleft.x = MAXX/2;
    player.box.topleft.y = MAXY-3;

    player.box.botright.x = player.box.topleft.x + PHROG_SIZE-1;
    player.box.botright.y = player.box.topleft.y + PHROG_SIZE-1;
    player.lives = 1;
    player.dir = FIXED;
    player.et = PHROG;
    player.pid = getpid();
    player.color = RED;

    while(true) {
        input=getch();
        switch(input) {
            case UP:
                if(player.box.topleft.y > 0) {
                    player.box.topleft.y -=3;
                    player.dir = N;
                }                
            break;

            case DOWN:
                if(player.box.botright.y < MAXY) {
                    player.box.topleft.y += 3;
                    player.dir = S;
                }
            break;

        	case LEFT:
        		if(player.box.topleft.x > 0){
        			player.box.topleft.x -= 1;
        			player.dir = W;
        		}
        	break;

        	case RIGHT:
        		if(player.box.botright.x < MAXX){
        			player.box.botright.x += 1;
        			player.dir = E;
        		}
        	break;
        }

        player.box.botright.x = player.box.topleft.x + PHROG_SIZE-1;
        player.box.botright.y = player.box.topleft.y + PHROG_SIZE-1;

        write(pipewrite,&player,sizeof(Entity));
    }
}

void mammarrancaFields(int piperead, int pipewrite)
{
    initializeGameElements();
    Entity tempEntity, tempSpider;

    drawFieldBorder();
    
    while(game.running)
    {   
        drawFieldBorder();
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
                game.player.dir = tempEntity.dir;
                if(game.player.lives > 0){
                    printer(tempEntity);
                }
            break;

            case CAR:
                clearer(game.carTable[tempEntity.row][tempEntity.col]);
                game.carTable[tempEntity.row][tempEntity.col].box.topleft.x = tempEntity.box.topleft.x;
                game.carTable[tempEntity.row][tempEntity.col].box.topleft.y = tempEntity.box.topleft.y;
                game.carTable[tempEntity.row][tempEntity.col].box.botright.x = tempEntity.box.botright.x;
                game.carTable[tempEntity.row][tempEntity.col].box.botright.y = tempEntity.box.botright.y;
                game.carTable[tempEntity.row][tempEntity.col].dir = tempEntity.dir;
                game.carTable[tempEntity.row][tempEntity.col].pid = tempEntity.pid;                              
                //carCollisions(game.carTable[tempEntity.row][tempEntity.col]);
                //checkBorderProximity(game.carTable[tempEntity.id][tempEntity.col].box.topleft.x);               
                printer(tempEntity);
            break;
            
            case LOG:
                clearer(game.logs[tempEntity.row]); 
                game.logs[tempEntity.row].box.topleft.x = tempEntity.box.topleft.x;
                game.logs[tempEntity.row].box.topleft.y = tempEntity.box.topleft.y;
                game.logs[tempEntity.row].box.botright.x = tempEntity.box.botright.x;
                game.logs[tempEntity.row].box.botright.y = tempEntity.box.botright.y;
                game.logs[tempEntity.row].dir = tempEntity.dir;
                game.logs[tempEntity.row].pid = tempEntity.pid;                
                //shipCollisions(game.aliensLVL2[tempEntity.id]);
                //checkBorderProximity(game.aliensLVL2[tempEntity.id].box.topleft.x);
                //if(game.aliensLVL2[tempEntity.id].lives > 0){
                printer(tempEntity);
                //}
            break;
        
            case SPIDER:
                clearer(tempSpider);
                tempSpider.box.topleft.x = tempSpider.box.botright.x = tempEntity.box.topleft.x;
                tempSpider.box.topleft.y = tempSpider.box.botright.y = tempEntity.box.botright.y;
                tempSpider.pid = tempEntity.pid;
                tempSpider.dir = tempEntity.dir;
                tempSpider.et = tempEntity.et;                            
                //shipCollisions(tempSpider);
                //if(tempSpider.box.topleft.x > 1){
                printer(tempSpider);
                //}               
            break;
        }
        
        refresh();

        if (game.running && game.player.lives > 0 && getVisitedDens() == 5)
        {
        	game.win = true;
            game.running = false;
            refresh();
        }  
    }
}

void initializeGameElements()
{
	int i,j;

	// init colors
	init_pair(WHITE,COLOR_WHITE,COLOR_BLACK);
    init_pair(RED,COLOR_RED,COLOR_BLACK);
    init_pair(YELLOW,COLOR_YELLOW,COLOR_BLACK);
    init_pair(GREEN,COLOR_GREEN,COLOR_BLACK);
    init_pair(BLUE,COLOR_BLUE,COLOR_BLACK);
    init_pair(MAGENTA,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(CYAN,COLOR_CYAN,COLOR_BLACK);

	// initPlayer
	game.player.lives = 3;
	game.player.color = RED;
	game.player.row = game.player.col = 0;


	// init car & log directions
	for(i = 0; i < NUM_LANES; i++){
		if(rand()%2 == 0){
			game.dirLanes[i] = E;
		}else{
			game.dirLanes[i] = W;
		}
	}

	for(i = 0; i < NUM_LOGS; i++){
		if(rand()%2 == 0){
			game.dirRivers[i] = W;
		}else{
			game.dirRivers[i] = E;
		}
	}

	// init cars
	for(i = 0; i < NUM_LANES; i++){
		for(j = 0; j < NUM_CARS; j++){
			game.carTable[i][j].row = i;
			game.carTable[i][j].col = j;
			game.carTable[i][j].dir = game.dirLanes[j]; 
		}
	}

	// init logs
	for(i = 0; i < NUM_LOGS; i++){
		game.logs[i].row = i;
		game.logs[i].dir = game.dirRivers[i];
	}

	// init dens
	for(i = 0; i < NUM_DENS; i++){
		game.visitedDens[i].coords.x = 0 + (i*PHROG_SIZE*2);
		game.visitedDens[i].coords.y = 0;
		game.visitedDens[i].visited = false;
	}
}

void carGenerator(int pipewrite)
{
	int i,j;
	Entity car[NUM_LANES][NUM_CARS];
	
	for(i = 0; i < NUM_LANES; i++){
		for(j = 0; j < NUM_CARS; j++){

			// deciding map position and direction
			car[i][j].row = i;
			car[i][j].col = j;

			// deciding characteristics
			car[i][j].length = 4 +rand()%(6);
			if(car[i][j].length > 6){
				car[i][j].color = BLUE;
			}else {
				car[i][j].color = CYAN;
			}

			// deciding starting methods and hitbox measurements
			if(car[i][j].dir == W){
				car[i][j].box.topleft.x = 0 - car[i][j].length;
			}else{
				car[i][j].box.topleft.x = MAXX;
			}

			car[i][j].box.topleft.y = MIN_ROW_CAR + (i*PHROG_SIZE);
			
			car[i][j].box.botright.x = car[i][j].box.topleft.x + car[i][j].length;
			car[i][j].box.botright.y = car[i][j].box.topleft.y + PHROG_SIZE;
		}
	}

	// timed interval generation
}

void logGenerator(int pipewrite)
{
	int i;
	Log log[NUM_LOGS];

	for(i = 0; i < NUM_LOGS; i++){
		// deciding map position and direction
		log[i].data.row = i;

		// deciding characteristics
		log[i].data.length = 4 +rand()%(6);
		if(log[i].data.length > 6){
			log[i].data.color = BLUE;
		}else {
			log[i].data.color = CYAN;
		}

		// deciding starting methods and hitbox measurements
		if(log[i].data.dir == W){
			log[i].data.box.topleft.x = 0 - log[i].data.length;
		}else{
			log[i].data.box.topleft.x = MAXX;
		}

		log[i].data.box.topleft.y = MIN_ROW_CAR + (i*PHROG_SIZE);
		
		log[i].data.box.botright.x = log[i].data.box.topleft.x + log[i].data.length;
		log[i].data.box.botright.y = log[i].data.box.topleft.y + PHROG_SIZE;

		// chance of enemy spawning

		// send log
	}
}

// COMPLETED

int getVisitedDens(){
	int i,count;
	for(i = 0; i < NUM_DENS; i++){
		if(game.visitedDens[i].visited){
			count +=1;
		}
	}
	return count;
}

void printer(Entity ent)
{
	int i,j,x,y;
    // matrice giocatore...
    char phrogBody [PHROG_SIZE][PHROG_SIZE] = {
        "\\-/",
        "(O)",
        "/-\\"
    };

    char carFrontSectionRight [PHROG_SIZE][2] = {
    	"o-",
    	"|C",
    	"o-"
    };

    char carFrontSectionLeft [PHROG_SIZE][2] = {
    	"-o",
    	"D|",
    	"-o"
    };

    char carBackSectionRight [PHROG_SIZE][2] = {
    	"-o",
    	"K|",
    	"-o"
    };

    char carBackSectionLeft [PHROG_SIZE][2] = {
    	"o-",
    	"|H",
    	"o-"
    };

    char carMiddleSection [PHROG_SIZE][1] = {
    	"-",
    	" ",
    	"-"
    };

    y = ent.box.topleft.y;
    x = ent.box.topleft.x;
    
    switch(ent.et){
		case PHROG:
			attron(COLOR_PAIR(ent.color));
			for(i = 0; i < PHROG_SIZE; i++){
				for(j = 0; j < PHROG_SIZE; i++){
					mvaddch(y+i,x+j,phrogBody[i][j]);
				}
			}
			attroff(COLOR_PAIR(ent.color));
		break;
	}
}

void clearer(Entity ent)
{
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

_Bool collisionDetection(Hitbox a, Hitbox b)
{
	if(((a.topleft.x >= b.topleft.x && a.topleft.x <= b.botright.x) || (a.botright.x >= b.topleft.x && a.botright.x <= b.botright.x)) &&
       ((a.topleft.y >= b.topleft.y && a.topleft.y <= b.botright.y) || (a.botright.y >= b.topleft.y && a.botright.y <= b.botright.y))) {
        return true;
    } else {
        return false;
    }
}

void drawFieldBorder()
{
    int i = 0;
    int j = 0;
    for (j; j <= MAXY; j++) {
        i = 0;
        if (j >= 1 && j < MAXY) {
            mvaddch(j, i, ACS_VLINE);
            mvaddch(j, MAXX, ACS_VLINE);

        } else if (j == 0 || j == MAXY) {
            for (i = 0; i < MAXX; i++) {
                mvaddch(j, i, ACS_HLINE);
                mvaddch(j, MAXX, ACS_HLINE);
            }
        }
    }
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, MAXX, ACS_URCORNER);
    mvaddch(MAXY, MAXX, ACS_LRCORNER);
    mvaddch(MAXY, 0, ACS_LLCORNER);
}

/*
	void Phrog(int pipewrite)
	{
		Entity phrog;

		char c;
		phrog.lives = 3;
		phrog.box.topleft.x = MAXX/2;
		phrog.box.topleft.y = MAXY/2;
		phrog.box.botright.x = (MAXX/2)+2;
		phrog.box.botright.y = (MAXY/2)+2;
		phrog.color = RED;
		phrog.dir = N;
		phrog.et = PHROG;
		phrog.pid = getpid();

		while(phrog.lives > 0){
			c = getch();
			switch(c){
				case UP:
					if(phrog.box.topleft.y > 0){
						phrog.box.topleft.y -= 3;
						phrog.box.botright.y -= 3;
						phrog.row -=1;
						phrog.dir = N;
					}	
				break;

				case DOWN:
					if(phrog.box.botright.y < MAXY){
						phrog.box.topleft.y += 3;
						phrog.box.botright.y += 3;
						phrog.row +=1;
						phrog.dir = S;
					}				
				break;

				case LEFT:
					if(phrog.box.topleft.x > 0){
						phrog.box.topleft.x -= 1;
						phrog.box.botright.x -= 1;
						phrog.dir = W;
					}			
				break;

				case RIGHT:
					if(phrog.box.botright.x < MAXX){
						phrog.box.topleft.x += 1;
						phrog.box.botright.x += 1;
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

		mvprintw(MAXY+2,MAXX+1,"a casa delle nutrie\n");
	    Entity tempEntity, tempSpit, tempCar;
	    Log tempLog;

	    game.player.lives = 3;
	    game.player.color = RED;

	    game.running = true;
	    game.loss = false;
	    game.win = false;

	    drawFieldBorder();
	 
	    while(game.running)
	    {   
	        drawFieldBorder();
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
*/