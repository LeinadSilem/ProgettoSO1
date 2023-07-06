#include "gameLib_t.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
Gamestate game;

// SEZIONE GENERAZIONE E MOVIMENTO ENTITA

int gameStart(int startingLives, _Bool dRegister[])
{

	int* lives = malloc(sizeof(startingLives));
	*lives = startingLives;
	int result = 0;
	pthread_t plThread, logThread, carThread, fieldThread;

	pthread_mutex_init(&mutex,NULL);

	pthread_create(&plThread,NULL,&phrog,(void*)lives);
    pthread_create(&logThread,NULL,&logGenerator,NULL);
    pthread_create(&carThread,NULL,&carGenerator,NULL);
    result = roadsAndPonds(dRegister);


	if(result > -1){
		pthread_join(plThread,NULL);
		pthread_join(logThread,NULL);
		pthread_join(carThread,NULL);
	}

	return result;
}

// player -------------------------------

void* phrog(void* param)
{
	char input;

	game.player.lives = *((int*)param); 
	game.player.id = pthread_self();

	game.gameReady = true;
	   
    while(game.player.lives > 0) {

    	input=getchar();
        switch(input) {
            case 'w':
            	pthread_mutex_lock(&mutex);
				if(game.player.box.topLeft.y > 1){
					game.player.box.topLeft.y -= 3;
					game.player.box.botRight.y -= 3;
					game.player.dir = N;
					game.player.row--;	
				}	
				pthread_mutex_unlock(&mutex);
			break;

			case 's':
				pthread_mutex_lock(&mutex);
				if(game.player.box.botRight.y < MAXY-1){
					game.player.box.topLeft.y += 3;
					game.player.box.botRight.y += 3;
					game.player.dir = S;
					game.player.row++;	
				}
				pthread_mutex_unlock(&mutex);
			break;

			case 'a':
				pthread_mutex_lock(&mutex);
				if(game.player.box.topLeft.x > 1){
					game.player.box.topLeft.x -= 3;	
					game.player.box.botRight.x -= 3;
					game.player.dir = W;
				}
				pthread_mutex_unlock(&mutex);
			break;

			case 'd':
				pthread_mutex_lock(&mutex);
				if(game.player.box.botRight.x < MAXX-1){
					game.player.box.topLeft.x += 3;	
					game.player.box.botRight.x += 3;  
					game.player.dir = E;				
				}
				pthread_mutex_unlock(&mutex);
			break;

			case 'f':
				pthread_mutex_lock(&mutex);
				game.player.dir = FIRE;
				//spit(pipewrite,game.player.box,game.player.et);
				pthread_mutex_unlock(&mutex);
			break;

			default:
				pthread_mutex_lock(&mutex);
				game.player.dir = FIXED;
				pthread_mutex_unlock(&mutex);
			break;
	    }

        usleep(3000);
    }

    return NULL;
}

// cars ---------------------------------

void* carGenerator()
{
	int i,j;
	Direction dirLanes[NUM_LANES];


	if(rand()%2 == 0){
		dirLanes[0] = W;
	}else{
		dirLanes[0] = E;
	}

	for(i = 1; i < NUM_LANES; i++){
		switch(dirLanes[i-1]){
		case W:
			dirLanes[i] = E;
			break;
		case E:
			dirLanes[i] = W;
			break;
		}
	}
	
	for(i = 0; i < NUM_LANES; i++){
		for(j = 0; j < NUM_CARS; j++){

			// deciding map position and direction
			game.carTable[i][j].row = i;
			game.carTable[i][j].col = j;
			game.carTable[i][j].dir = dirLanes[i];
			game.carTable[i][j].et = CAR;

			// deciding characteristics
			game.carTable[i][j].length = (4 +rand()%6);
			if(game.carTable[i][j].length > 6){
				game.carTable[i][j].color = TRUCKS;
			}else {
				game.carTable[i][j].color = CARS;
			}

			// deciding starting methods and hitbox measurements
			if(game.carTable[i][j].dir == W){
				game.carTable[i][j].box.topLeft.x = MAXX+1;
			}else{				
				game.carTable[i][j].box.topLeft.x = 0 - game.carTable[i][j].length;
			}

			game.carTable[i][j].box.topLeft.y = MIN_ROW_CAR + (i*PHROG_SIZE);
			
			game.carTable[i][j].box.botRight.x = game.carTable[i][j].box.topLeft.x + game.carTable[i][j].length-1;
			game.carTable[i][j].box.botRight.y = game.carTable[i][j].box.topLeft.y + PHROG_SIZE-1;
		}
	}

	while(!game.gameReady){
		usleep(1000);
	}

	for(j = 0; j < NUM_CARS; j++){
		for(i = 0; i < NUM_LANES; i++){
			pthread_create(&game.carTable[i][j].id,NULL,&moveCar,(void*)&game.carTable[i][j]);		
		}
		sleep(6);
	}

	return NULL;
}

void* moveCar(void* param)
{

	Entity *tempCar = (Entity*)param;

	int i = tempCar->row;
	int j = tempCar->col;

	game.carTable[i][j].id = pthread_self();

	while(true){
		switch(game.carTable[i][j].dir){
			case W:
				pthread_mutex_lock(&mutex);
				if(game.carTable[i][j].box.botRight.x == 0){
					game.carTable[i][j].box.topLeft.x = MAXX+1;
				}else{
					game.carTable[i][j].box.topLeft.x -=1;
				}
				game.carTable[i][j].box.botRight.x = game.carTable[i][j].box.topLeft.x + game.carTable[i][j].length-1;
				pthread_mutex_unlock(&mutex);
			break;

			case E:
				pthread_mutex_lock(&mutex);
				if(game.carTable[i][j].box.topLeft.x == MAXX){
					game.carTable[i][j].box.topLeft.x = 0-game.carTable[i][j].length;
				}else{
					game.carTable[i][j].box.topLeft.x +=1;
				}		
				game.carTable[i][j].box.botRight.x = game.carTable[i][j].box.topLeft.x + game.carTable[i][j].length-1;
				pthread_mutex_unlock(&mutex);
			break;
		}
		usleep(100000);
	}
}

_Bool carCollisions(Entity currentCar, Entity phrog)
{
	int i;

	//for(i = 0; i < NUM_CARS; i++){
    //	  if(verifyHitbox(currentCar.box,game.carTable[currentCar.row][i].box) && currentCar.id != game.carTable[currentCar.row][i].id){
    //	  	  haltCar(currentCar.col,currentCar.row);
    //	  }
    //}

    if(verifyHitbox(currentCar.box,phrog.box)){
    	bodyClearing(game.player,game.gameWin);
    	return true;
    }  

    return false;
}

void haltCar(int currentCar, int row)
{
	switch(game.carTable[row][currentCar].dir){
		case W:
			game.carTable[row][currentCar].box.topLeft.x++;
			game.carTable[row][currentCar].box.botRight.x++;
		break;

		case E:
			game.carTable[row][currentCar].box.topLeft.x--;
			game.carTable[row][currentCar].box.botRight.x--;
		break;
	}
}

// logs ---------------------------------

void* logGenerator()
{
	int i;

	Direction dirRivers[NUM_LANES];

	if(rand()%2 == 0){
		dirRivers[0] = W;
	}else{
		dirRivers[0] = E;
	}

	for(i = 1; i < NUM_LOGS; i++){
		switch(dirRivers[i-1]){
		case W:
			dirRivers[i] = E;
			break;
		case E:
			dirRivers[i] = W;
			break;
		}
	}
	
	for(i = 0; i < NUM_LOGS; i++){
		// deciding map position and direction
		game.logs[i].row = i;
		game.logs[i].dir = dirRivers[i];
		game.logs[i].et = LOG;
		game.logs[i].color = LOGS;
		game.logs[i].hasSpider = false;

		
		// deciding characteristics
#if TESTING == 0
		if(rand()%2 == 0){
			game.logs[i].length = 6;
		}else{
			game.logs[i].length = 9;
		}
#else 
		game.logs[i].length = 18;
#endif

		// deciding starting methods and hitbox measurements
		if(game.logs[i].dir == W){
			game.logs[i].box.topLeft.x = MAXX+1;
		}else{				
			game.logs[i].box.topLeft.x = 0 - game.logs[i].length;
		}

		game.logs[i].box.topLeft.y = MIN_ROW_LOG + (i*PHROG_SIZE);
		game.logs[i].box.botRight.x = game.logs[i].box.topLeft.x + game.logs[i].length-1;
		game.logs[i].box.botRight.y = game.logs[i].box.topLeft.y + PHROG_SIZE-1;
	}

	for(i = 0; i < NUM_LOGS; i++){
		pthread_create(&game.logs[i].id,NULL,&moveLog,(void*)&game.logs[i]);			
	}
}

void* moveLog(void* param)
{
	//spitter random generation and forking

	Entity *tempLog = (Entity*)param;

	int i = tempLog->row;

	game.logs[i].id = pthread_self();

	while(true){
		switch(game.logs[i].dir){
			case W:
				pthread_mutex_lock(&mutex);
				if(game.logs[i].box.topLeft.x == 1){
					game.logs[i].dir = E;
				}else{
					game.logs[i].box.topLeft.x -=1;
					game.logs[i].box.botRight.x = game.logs[i].box.topLeft.x + game.logs[i].length-1;
				}
				pthread_mutex_unlock(&mutex);
			break;

			case E:
				pthread_mutex_lock(&mutex);
				if(game.logs[i].box.botRight.x == MAXX-1){
					game.logs[i].dir = W;
				}else{
					game.logs[i].box.topLeft.x +=1;
					game.logs[i].box.botRight.x = game.logs[i].box.topLeft.x + game.logs[i].length-1;
				}	
				pthread_mutex_unlock(&mutex);	
			break;
		}
		usleep(100000);
	}
}

_Bool logCollisions()
{
	int i,misses=0;

	for(i = 0; i < NUM_LOGS; i++){
		if(verifyHitbox(game.player.box,game.logs[i].box) && !game.logs[i].hasSpider){
			game.logs[i].isOnLog = true;
    		game.player.isOnLog = true;
			return true;
		}else{
			misses +=1;
		}
	}

	if(misses == NUM_LOGS){
		bodyClearing(game.player,game.gameWin);
		return false;
	}
}

// game --------------------------------

void initializeData(_Bool dRegister[])
{
	int i,j;

	game.gameWin = newwin(MAXY,MAXX,0,0);
	game.statWin = newwin(5,MAXX,MAXY,0);

	// init colors
	init_pair(SAFE_ZONE,COLOR_WHITE,COLOR_GREEN);
	init_pair(ROAD,COLOR_WHITE,COLOR_BLACK);
	init_pair(WATER,COLOR_WHITE,COLOR_CYAN);

    init_pair(LOGS,COLOR_YELLOW,COLOR_CYAN);    
    init_pair(TRUCKS,COLOR_BLUE,COLOR_BLACK); 
    init_pair(CARS,COLOR_CYAN,COLOR_BLACK);

    init_pair(PHROG_ON_ROAD,COLOR_RED,COLOR_BLACK);
    init_pair(PHROG_ON_GRASS,COLOR_RED,COLOR_GREEN);
    init_pair(PHROG_ON_LOG,COLOR_RED,COLOR_CYAN);

    init_pair(SPIT_ON_ROAD,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(SPIT_ON_GRASS,COLOR_MAGENTA,COLOR_GREEN);
    init_pair(SPIT_ON_LOG,COLOR_MAGENTA,COLOR_CYAN);
    init_pair(SPIDER_COLOR,COLOR_MAGENTA,COLOR_YELLOW);

    wattrset(game.gameWin, COLOR_PAIR(SAFE_ZONE));
    wattrset(game.gameWin, COLOR_PAIR(ROAD));
    wattrset(game.gameWin, COLOR_PAIR(WATER));
    wattrset(game.gameWin, COLOR_PAIR(LOGS));
    wattrset(game.gameWin, COLOR_PAIR(TRUCKS));
    wattrset(game.gameWin, COLOR_PAIR(CARS));
    wattrset(game.gameWin, COLOR_PAIR(PHROG_ON_ROAD));
    wattrset(game.gameWin, COLOR_PAIR(PHROG_ON_GRASS));
    wattrset(game.gameWin, COLOR_PAIR(PHROG_ON_LOG));
    wattrset(game.gameWin, COLOR_PAIR(SPIT_ON_ROAD));
    wattrset(game.gameWin, COLOR_PAIR(SPIT_ON_GRASS));
    wattrset(game.gameWin, COLOR_PAIR(SPIT_ON_LOG));
    wattrset(game.gameWin, COLOR_PAIR(SPIDER));

    //delimitations calculations
    game.zoneLimitY[0] = 4;
	game.zoneLimitY[1] = 4 + (NUM_RIVERS*PHROG_SIZE);
	game.zoneLimitY[2] = (MAXY-3) - (NUM_LANES*PHROG_SIZE)-1;
	game.zoneLimitY[3] = MAXY-3;

	// initPlayer
	game.player.box.topLeft.x = 1;
    game.player.box.topLeft.y = MAXY-3;
    game.player.box.botRight.x = game.player.box.topLeft.x + PHROG_SIZE-1;
    game.player.box.botRight.y = game.player.box.topLeft.y + PHROG_SIZE-1;
	game.player.color = PHROG_ON_GRASS;
	game.player.row = 8;
	game.player.isOnLog = false;

	// init cars
	// row in the case of cars is the lane in which they are found
	// col is just car number 0,1,2 etc..
	for(i = 0; i < NUM_LANES; i++){
		for(j = 0; j < NUM_CARS; j++){
			game.carTable[i][j].row = i;
			game.carTable[i][j].col = j;
			game.carTable[i][j].et = CAR;
			game.carTable[i][j].length = (4 +rand()%6);
			if(game.carTable[i][j].length > 6){
				game.carTable[i][j].color = TRUCKS;
			}else {
				game.carTable[i][j].color = CARS;
			}
		}
	}

	// init logs and spiders
	for(i = 0; i < NUM_LOGS; i++){
		game.logs[i].row = i;
		game.logs[i].et = LOG;
		game.logs[i].color = LOGS;
		game.logs[i].hasSpider = false;

		
		// deciding characteristics
#if TESTING == 0
		if(rand()%2 == 0){
			game.logs[i].length = 6;
		}else{
			game.logs[i].length = 9;
		}
#else 
		game.logs[i].length = 18;
#endif

		game.spiders[i].row = i;
		game.spiders[i].et = SPIDER;
	}

	// init dens
	for(i = 0; i < NUM_DENS; i++){
		game.Dens[i].area.topLeft.x = 1 + (i*WIDTH_DENS*2);
		game.Dens[i].area.topLeft.y = 1;
		game.Dens[i].area.botRight.x = 9 + (i*WIDTH_DENS*2);
		game.Dens[i].area.botRight.y = 3;
		game.Dens[i].visited = dRegister[i];
	}
}

void screenRefresh()
{
	wrefresh(game.gameWin);
	wrefresh(game.statWin);
	refresh();
}

int roadsAndPonds(_Bool dRegister[])
{
	//clock_t startTime;
	//double timeSpent;
    _Bool playerHit = false;
    _Bool denReached = false;
    _Bool playerIsDry;
    time_t start, end;
    int timeSpent, timeRemaining;
    int result, denId, resultOfSpitCollision,i,j;
    FILE *debugLog;
    entityNode *iter;

    debugLog = fopen("debugLog.txt","a");
    fprintf(debugLog,"----start of manche----\n");

    initializeData(dRegister);
    drawMap();
    time(&start);
    mvwprintw(game.statWin,1,1,"[·phrog lives:  ·] ");
      
    do{          
    	 	
    	time(&end);
        drawMap();
        
	 	timeSpent = difftime(end, start);
	 	timeRemaining = TIMER - timeSpent;
 	
	 	pthread_mutex_lock(&mutex);

	 	bodyClearingPlayer(game.player,game.gameWin);
	 	
	 	//	if(game.player.dir == FIRE){
	 	//		pthread_t projThread;
	 	//		pthread_create(&projThread,NULL,spit,(void*)game.player);
	 	//	}

        if(game.player.row == 0){
        	denId = denCollisions();
        	mvprintw(3,MAXX+1,"den collided:%d",denId);
            if(game.player.box.topLeft.y == 1){
                if(denId < NUM_DENS){
                	denReached = true;
                }else{
                	playerHit = true;
                }
            } 
        }

        if(game.player.row <= 3 && game.player.row >= 1){
	    	playerIsDry = logCollisions();
			
	    	if(!playerIsDry && game.logs[game.player.row-1].hasSpider){
	    		fprintf(debugLog, "attempted to jump on log with a spider in row %d\n", game.player.row);
	    		playerHit = true;
	    	}else if(!playerIsDry && !game.logs[game.player.row-1].hasSpider){
	    		fprintf(debugLog, "missed the log in row %d\n", game.player.row);
	    		playerHit = true;
	    	}else if(playerIsDry){
	    		fprintf(debugLog, "player is dry and on log in position x:%d y:%d \n", game.player.box.topLeft.x, game.player.box.topLeft.y);
	    		for(i = 0; i<NUM_LOGS; i++){
	    			if(game.logs[i].row != game.player.row-1) game.logs[i].isOnLog = false;
	    		}
	    	}
        }

	 	mvprintw(1,MAXX+1,"player x: %d, y: %d, dir:",game.player.box.topLeft.x,game.player.box.topLeft.y);
	 	translateDirection(game.player.dir);
	 	printerSingleEntities(game.player,game.gameWin);

		for(i = 0; i < NUM_LANES; i++){
			for(j = 0; j < NUM_CARS; j++){
				bodyClearing(game.carTable[i][j],game.gameWin);
				playerHit = carCollisions(game.carTable[i][j],game.player);

				if(playerHit){
					result = OUCH;
					break;
				}

				printerCars(game.carTable[i][j],game.gameWin);
			}
			if(playerHit){
				break;
			}
		}

		for(i = 0; i < NUM_LOGS; i++){
			bodyClearing(game.logs[i],game.gameWin);
			printerLogs(game.logs[i],game.gameWin);
			if(game.logs[i].hasSpider){
				bodyClearing(game.spiders[i],game.gameWin);
				printerSingleEntities(game.spiders[i],game.gameWin);
			}
			if(game.player.row-1 == game.logs[i].row){
				bodyClearing(game.player,game.gameWin);
				switch(game.logs[i].dir){
            		case W:
		    			game.player.box.topLeft.x--;	
						game.player.box.botRight.x--;
						game.player.dir = W;
		    		break;			    		
		    		case E:
		    			game.player.box.topLeft.x++;	
						game.player.box.botRight.x++;
						game.player.dir = E;
		    		break;
            	}
            	printerSingleEntities(game.player,game.gameWin);
			}
		}

	 	// (projectiles have to be put in a list)
		// 	iter = game.entityList->head;
		// 	while(iter != NULL){
		// 		bodyClearing(iter->data,game.gameWin);
		// 		// collision controls
		// 		printerSingleEntities(iter->data,game.gameWin);
		// 		iter = iter->next;
		// 	}
	 	
        mvwprintw(game.statWin,1,1,"[·phrog lives:%d·]\n", game.player.lives);       
        mvwprintw(game.statWin,2,1,"[·time left:%d·]\n", timeRemaining);       
        screenRefresh(); 

    	pthread_mutex_unlock(&mutex);
    }while(!playerHit && !denReached && timeSpent <= TIMER);

    werase(game.gameWin);
    werase(game.statWin);
    screenRefresh();
    fclose(debugLog);
    //clearRiverInteractions();

    if(playerHit || timeSpent >= TIMER){
    	result = OUCH;
    }

    if(denReached){
    	result = denId;
    }
    return result;
}

void drawMap()
{
	int i,j,k;
	//drawing border
	wattron(game.gameWin,COLOR_PAIR(SAFE_ZONE));
	wattron(game.statWin,COLOR_PAIR(ROAD));
	
	box(game.gameWin,0,0);
    
	for(j = 1; j<ZONES; j++){
    	mvwaddch(game.gameWin,game.zoneLimitY[j],0,ACS_LTEE);	
    	for(i = 1;i<MAXX;i++){
	    	mvwaddch(game.gameWin,game.zoneLimitY[j],i,ACS_HLINE);
	    }
	    mvwaddch(game.gameWin,game.zoneLimitY[j],MAXX-1,ACS_RTEE);
    }

	for(i = 0; i < NUM_DENS; i++){
		if(!game.Dens[i].visited){
			mvwhline(game.gameWin,game.Dens[i].area.topLeft.y,game.Dens[i].area.topLeft.x,ACS_HLINE,WIDTH_DENS);
			mvwvline(game.gameWin,game.Dens[i].area.topLeft.y,game.Dens[i].area.topLeft.x+WIDTH_DENS-1,ACS_VLINE,3);
			mvwhline(game.gameWin,game.Dens[i].area.topLeft.y+2,game.Dens[i].area.topLeft.x,ACS_HLINE,WIDTH_DENS);
			mvwvline(game.gameWin,game.Dens[i].area.topLeft.y,game.Dens[i].area.topLeft.x,ACS_VLINE,3);		
			mvwaddch(game.gameWin,game.Dens[i].area.topLeft.y,game.Dens[i].area.topLeft.x,ACS_ULCORNER);
		    mvwaddch(game.gameWin,game.Dens[i].area.topLeft.y,game.Dens[i].area.topLeft.x+WIDTH_DENS-1,ACS_URCORNER);
		    mvwaddch(game.gameWin,game.Dens[i].area.topLeft.y+2,game.Dens[i].area.topLeft.x,ACS_LLCORNER);
		    mvwaddch(game.gameWin,game.Dens[i].area.botRight.y,game.Dens[i].area.botRight.x,ACS_LRCORNER);		
		}
	}

	box(game.statWin,0,0);

	wattroff(game.gameWin,COLOR_PAIR(SAFE_ZONE)); 
	wattroff(game.statWin,COLOR_PAIR(ROAD));  
 
}

void translateDirection(Direction dir)
{
	switch(dir){
	    case N:
	    	printw("north");
		break;

	    case S:
	    	printw("south");
		break;

		case W:
			printw("west ");
		break;

		case E:
			printw("east ");
		break;
	}
}

int denCollisions()
{
	int i;
	for(i = 0; i < NUM_DENS; i++){
		if(verifyHitbox(game.player.box,game.Dens[i].area)){
			game.Dens[i].visited = true;
			return i;
		}
	}

	return NUM_DENS;
}