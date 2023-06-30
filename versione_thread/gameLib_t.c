#include "gameLib_t.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
Gamestate game;

// SEZIONE GENERAZIONE E MOVIMENTO ENTITA

int gameStart(int startingLives, _Bool dRegister[])
{

	int result = 0;
	pthread_t plThread, logThread, carThread, fieldThread;

	pthread_mutex_init(&mutex,NULL);

	//initializeData()

	pthread_create(&plThread,NULL,&phrog,(void*)startingLives);
    //pthread_create(&logThread,NULL,&logGenerator,NULL);
    //pthread_create(&carThread,NULL,&carGenerator,NULL);
    pthread_create(&fieldThread,NULL,&roadsAndPonds,(void*)dRegister);


	if(result > -1){
		pthread_join(plThread,NULL);
		//pthread_join(logThread,NULL);
		//pthread_join(carThread,NULL);
		pthread_join(fieldThread,NULL);
	}

	return result;
}

// funzione per il movimento della rana
void* phrog(void* param)
{
	char input;

	game.player.lives = (int)param; 
	game.player.id = pthread_self();
	   
    while(true) {

    	input=getchar();
        switch(input) {
            case 'w':
            	pthread_mutex_lock(&mutex);
				if(game.player.box.topLeft.y > 1){
					game.player.box.topLeft.y -= 3;
					game.player.box.botRight.y -= 3;
					game.player.dir = N;	
				}	
				pthread_mutex_unlock(&mutex);
			break;

			case 's':
				pthread_mutex_lock(&mutex);
				if(game.player.box.botRight.y < MAXY-1){
					game.player.box.topLeft.y += 3;
					game.player.box.botRight.y += 3;
					game.player.dir = S;	
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

			case ' ':
				pthread_mutex_lock(&mutex);
				game.player.dir = FIXED;
				//spit(pipewrite,game.player.box,game.player.et);
				pthread_mutex_unlock(&mutex);
			break;

#if TESTING == 1
			//suicide button only for debug
			case 'q':
				pthread_mutex_lock(&mutex);
				game.player.lives = 0;
				pthread_mutex_unlock(&mutex);
			break;
#endif
			default:
				pthread_mutex_lock(&mutex);
				game.player.dir = FIXED;
				pthread_mutex_unlock(&mutex);
			break;
	    }

        usleep(3000);
    }
}

// SEZIONE GESTIONE GIOCO E GRAFICA
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

void* roadsAndPonds(void* param)
{
	_Bool dRegister = (_Bool*)param;
    initializeData(&dRegister);
    Entity tempEntity, tempProjectile;

    _Bool playerHit = false;
    _Bool denReached = false;
    _Bool playerIsDry;
    int result, denId, currentRow;
    int countdown = 0;
    time_t currentTime, startTime;
    FILE *debugLog;

    debugLog = fopen("debugLog.txt","a");
    fprintf(debugLog,"----start of manche----\n");
    drawMap();
    
    countdown = DELAY;
    time(&startTime);
    while(!playerHit && !denReached)
    {   
    	time(&currentTime);

    	if(difftime(currentTime,startTime) >= DELAY){
    		playerHit = true;
    		fprintf(debugLog, "time is up, gg");
    	}else{    
        	mvwprintw(game.statWin,2,1,"[·time:%d·]",countdown);
	       
	        drawMap();
	        read(piperead, &tempEntity, sizeof(Entity));
	        mvprintw(0,MAXX+1,"entity read:%d",tempEntity.et);

	        switch (tempEntity.et)
	        {
	            case PHROG:
	            	// se il player è sul tronco, muovilo col
	            	mvwprintw(game.statWin,1,1,"[·phrog lives:%d·]\n",tempEntity.lives);
	                bodyClearing(game.player,game.gameWin);  
	                
	                game.player.id = tempEntity.id;
	                game.player.col = tempEntity.col;
	                game.player.color = tempEntity.color;
	                game.player.length = tempEntity.length;
	                game.player.dir = tempEntity.dir;

	                switch(game.player.dir){
	                	case N:
							if(game.player.box.topLeft.y > 1){
								game.player.box.topLeft.y -= 3;
								game.player.box.botRight.y -= 3;
								game.player.row--; 	
							}	
						break;

						case S:
							if(game.player.box.botRight.y < MAXY-1){
								game.player.box.topLeft.y += 3;
								game.player.box.botRight.y += 3;
								game.player.row++;  									
							}
						break;

						case W:
							if(game.player.box.topLeft.x > 1){
								game.player.box.topLeft.x -= 3;	
								game.player.box.botRight.x -= 3;
								
							}
						break;

						case E:
							if(game.player.box.botRight.x < MAXX-1){
								game.player.box.topLeft.x += 3;	
								game.player.box.botRight.x += 3;  			
							}
						break;
	                }         

	                fprintf(debugLog, "input recieved from the child : %d\n", game.player.length);

	                mvprintw(1,MAXX+1,"is player on log in game: %d, is player on log fr: %d",game.player.isOnLog,tempEntity.isOnLog);
	                mvprintw(2,MAXX+1,"current player position x:%d, y:%d, row:%d dir:",game.player.box.topLeft.x,game.player.box.topLeft.y,game.player.row);
	               	translateDirection(game.player.dir);       
					
	                if(game.player.box.topLeft.y >= MIN_ROW_LOG && game.player.box.topLeft.y <= MAX_ROW_LOG){
	                	playerIsDry = logCollisions(game.player,game.logs[game.player.row-1]);
	                	
	                	if(!playerIsDry && game.logs[game.player.row-1].hasSpider){
	                		fprintf(debugLog, "attempted to jump on log with a spider in row %d\n", game.player.row);
	                		playerHit = true;
	                	}else if(!playerIsDry && !game.logs[game.player.row-1].hasSpider){
	                		fprintf(debugLog, "missed the log in row %d\n", game.player.row);
	                		playerHit = true;
	                	}else if(playerIsDry){
	                		fprintf(debugLog, "player is dry and on log in position x:%d y:%d \n", game.player.box.topLeft.x, game.player.box.topLeft.y);
	                		game.logs[game.player.row-1].isOnLog = true;
	                		game.player.isOnLog = true;
	                		for(int i = 0; i<NUM_LOGS; i++){
	                			if(game.logs[i].row != game.player.row) game.logs[i].isOnLog = false;
	                		}
	                	}
	                }
   
	                denId = denCollisions();

	                if(game.player.box.topLeft.y == 1){
		                if(denId < NUM_DENS){
		                	denReached = true;
		                }else{
		                	playerHit = true;
		                }
	                }     
	                	               	
	               	mvwprintw(game.statWin,3,1,"visited dens: ");
	               	for(int i = 0; i < NUM_DENS; i++){
	               		wprintw(game.statWin,"%d[%d]\t", i+1, game.Dens[i].visited);
	               	}           
	              
	                if(game.player.lives > 0){
	                    printerSingleEntities(game.player,game.gameWin);
	                }else{
	                	playerHit = true;
	                }    
	            break;

	            case SPIDER:
	            break;

	            case CAR:
	            break;

	            case LOG:
	            break;
	            
	            case SPITBALL:
	            break;
	        }
	        
	        screenRefresh();
	    }       
	    countdown--; 
    }

    erase();
    screenRefresh();

    if(playerHit){
    	result = OUCH;
    }

    if(denReached){
    	result = denId;
    }

    fclose(debugLog);
    return result;
}

int calcRow(int playerRow)
{
	// 2,3,4 rivers 
	// 6,7,8 roads
	if(playerRow > 1 && playerRow <= 4){
		return playerRow - (NUM_RIVERS-1);
	}else if(playerRow > 5 && playerRow <= 8){
		return playerRow - (NUM_LANES*2);
	}
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


/*
	// verifica delle collisioni della bomba
	void projectileCollisions(Entity currentMissile, int pipewrite)
	{
	    for (int i = 0; i < ALIENS_ALLOWED*WAVES ; i++) {
	        if (game.aliensLVL1[i].lives > 0) {
	            if (verifyHitbox(game.aliensLVL1[i].box,currentMissile.box)) {

	                pid_t alien2p;

	                game.carTable[i].col = game.aliensLVL1[i].col;
	                game.carTable[i].d = game.aliensLVL1[i].d;
	                game.carTable[i].box.topLeft.x = game.aliensLVL1[i].box.topLeft.x-2;
	                game.carTable[i].box.topLeft.y = game.aliensLVL1[i].box.topLeft.y-1;
	                game.carTable[i].box.botRight.x = game.aliensLVL1[i].box.botRight.x+2;
	                game.carTable[i].box.botRight.y = game.aliensLVL1[i].box.botRight.y+2;
	                game.aliensLvl2Alive++;

	                game.aliensLvl1Alive--;
	                game.aliensLVL1[i].lives = 0;
	                bodyClearing(currentMissile);
	                bodyClearing(game.aliensLVL1[i]);
	                kill(currentMissile.pid, 1);
	                kill(game.aliensLVL1[i].pid, 1);

	                if((alien2p = fork()) == 0){
	                    alien2(pipewrite, game.carTable[i]);
	                }
	                return;
	            }
	        }
	    }

	    for (int i = 0; i < ALIENS_ALLOWED*WAVES; i++) {
	        if (game.carTable[i].lives > 0) {
	            if (verifyHitbox(game.carTable[i].box, currentMissile.box)) {

	                game.carTable[i].lives-=1;
	                bodyClearing(currentMissile);
	                kill(currentMissile.pid, 1);
	                
	                if (game.carTable[i].lives <= 0) {
	                    game.aliensLvl2Alive--;
	                    bodyClearing(game.carTable[i]);                    
	                    kill(game.carTable[i].pid, 1);                    
	                }
	                return;               
	            }
	        }
	    }
	}

	// verifica delle collisioni con la nave
	void shipCollisions(Entity temp)
	{

	    if (verifyHitbox(game.player.box, temp.box)){
	        game.player.lives = 0;
	        bodyClearing(game.player);
	        bodyClearing(temp);        
	        kill(game.player.pid, 1);
	        kill(temp.pid, 1);      
	        game.running = false;  
	        refresh();
	    }
	}

	// controllo per verificare se gli alieni hanno oltrepassato il confine sinistro della mappa
	void checkBorderProximity(int topLeftx)
	{
	    if (topLeftx <= 1) { //Ufo has reached the end of the area, the player has lost
	        game.running = false;
	        refresh();
	    }
	}
*/