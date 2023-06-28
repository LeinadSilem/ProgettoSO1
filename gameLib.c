#include "gameLib.h"

Gamestate game;

// SEZIONE GENERAZIONE E MOVIMENTO ENTITA

// funzione per il movimento della rana
void phrog(int lives,int pipewrite)
{
    char input;
    //int updated_life;
    Entity tempPlayer;
    Hitbox localCoords; 

    tempPlayer.pid = getpid();
    tempPlayer.lives = lives;
	tempPlayer.color =  PHROG_ON_GRASS;
	tempPlayer.row =  9;
	tempPlayer.col = 1;
	tempPlayer.et = PHROG;
	localCoords.topLeft.x = 1;
    localCoords.topLeft.y = MAXY-3;

    localCoords.botRight.x = localCoords.topLeft.x + PHROG_SIZE-1;
    localCoords.botRight.y = localCoords.topLeft.y + PHROG_SIZE-1;

    while(true) {

    	input=getchar();
	        switch(input) {
	            case 'w':
					if(localCoords.topLeft.y > 1){
						localCoords.topLeft.y -= 3;
						localCoords.botRight.y -= 3;
						tempPlayer.dir = N;	
					}	
				break;

				case 's':
					if(localCoords.botRight.y < MAXY-1){
						localCoords.topLeft.y += 3;
						localCoords.botRight.y += 3;
						tempPlayer.dir = S;	
					}
				break;

				case 'a':
					if(localCoords.topLeft.x > 1){
						localCoords.topLeft.x -= 3;	
						localCoords.botRight.x -= 3;
						tempPlayer.dir = W;
					}
				break;

				case 'd':
					if(localCoords.botRight.x < MAXX-1){
						localCoords.topLeft.x += 3;	
						localCoords.botRight.x +=3;  
						tempPlayer.dir = E;				
					}
				break;

				case ' ':
					tempPlayer.dir = FIXED;
					spit(pipewrite,localCoords,tempPlayer.et);
				break;

#if TESTING == 1
				//suicide button only for debug
				case 'q':
					tempPlayer.lives = 0;
				break;
#endif
				default:
					tempPlayer.dir = FIXED;
				break;
		    }

		tempPlayer.length = input;

        write(pipewrite, &tempPlayer, sizeof(Entity));   
        usleep(3000);
    }
}

void carGenerator(int pipewrite)
{
	int i,j;
	pid_t myPid;

	myPid = getpid();
	Entity car[NUM_LANES][NUM_CARS];
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
			car[i][j].row = i;
			car[i][j].col = j;
			car[i][j].dir = dirLanes[i];
			car[i][j].et = CAR;

			// deciding characteristics
			car[i][j].length = (4 +rand()%6);
			if(car[i][j].length > 6){
				car[i][j].color = TRUCKS;
			}else {
				car[i][j].color = CARS;
			}

			// deciding starting methods and hitbox measurements
			if(car[i][j].dir == W){
				car[i][j].box.topLeft.x = MAXX+1;
			}else{				
				car[i][j].box.topLeft.x = 0 - car[i][j].length;
			}

			car[i][j].box.topLeft.y = MIN_ROW_CAR + (i*PHROG_SIZE);
			
			car[i][j].box.botRight.x = car[i][j].box.topLeft.x + car[i][j].length-1;
			car[i][j].box.botRight.y = car[i][j].box.topLeft.y + PHROG_SIZE-1;
		}
	}

	for(j = 0; j < NUM_CARS; j++){
		for(i = 0; i < NUM_LANES; i++){
			if(fork()==0){
				moveCar(car[i][j],pipewrite);
			}			
		}
		sleep(6);
	}
}

void moveCar(Entity car,int pipewrite)
{
	car.pid = getpid();

	while(true){
		switch(car.dir){
			case W:
				if(car.box.botRight.x == 0){
					car.box.topLeft.x = MAXX+1;
				}else{
					car.box.topLeft.x -=1;
				}
			break;

			case E:
				if(car.box.topLeft.x == MAXX){
					car.box.topLeft.x = 0-car.length;
				}else{
					car.box.topLeft.x +=1;
				}		
			break;
		}

		car.box.botRight.x = car.box.topLeft.x + car.length-1;

		updateEntity(car,pipewrite);
		usleep(100000);
	}
}

_Bool carCollisions(Entity currentCar, Entity phrog)
{
	int i;

	for(i = 0; i < NUM_CARS; i++){
    	if(verifyHitbox(currentCar.box,game.carTable[currentCar.row][i].box) && currentCar.pid != game.carTable[currentCar.row][i].pid){
    		haltCar(currentCar.col,currentCar.row);
    	}
    }

    if(verifyHitbox(currentCar.box,phrog.box)){
    	bodyClearingSingleEntities(game.player,game.gameWin);
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

void logGenerator(int pipewrite)
{
	int i;
	pid_t myPid;

	myPid = getpid();
	Entity logs[NUM_LOGS];
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
		logs[i].row = i;
		logs[i].dir = dirRivers[i];
		logs[i].et = LOG;
		logs[i].color = LOGS;
		logs[i].hasSpider = false;

		
		// deciding characteristics
#if TESTING == 0
		if(rand()%2 == 0){
			logs[i].length = 6;
		}else{
			logs[i].length = 9;
		}
#else 
		logs[i].length = 18;
#endif

		// deciding starting methods and hitbox measurements
		if(logs[i].dir == W){
			logs[i].box.topLeft.x = MAXX+1;
		}else{				
			logs[i].box.topLeft.x = 0 - logs[i].length;
		}

		logs[i].box.topLeft.y = MIN_ROW_LOG + (i*PHROG_SIZE);
		logs[i].box.botRight.x = logs[i].box.topLeft.x + logs[i].length-1;
		logs[i].box.botRight.y = logs[i].box.topLeft.y + PHROG_SIZE-1;
	}

	for(i = 0; i < NUM_LOGS; i++){
		if(fork()==0){
			moveLog(logs[i],pipewrite);
		}			
	}
}

void moveLog(Entity log,int pipewrite)
{
	//spitter random generation and forking

	log.pid = getpid();

	while(true){
		switch(log.dir){
			case W:
				if(log.box.topLeft.x == 1){
					log.dir = E;
				}else{
					log.box.topLeft.x -=1;
					log.box.botRight.x = log.box.topLeft.x + log.length-1;
				}
			break;

			case E:
				if(log.box.botRight.x == MAXX-1){
					log.dir = W;
				}else{
					log.box.topLeft.x +=1;
					log.box.botRight.x = log.box.topLeft.x + log.length-1;
				}		
			break;
		}
		updateEntity(log,pipewrite);
		usleep(100000);
	}
}

_Bool logCollisions(Entity phrog, Entity currentLog)
{
	if(verifyHitbox(phrog.box,currentLog.box) && !currentLog.hasSpider){
		return true;
	}else{
		bodyClearingSingleEntities(game.player,game.gameWin);
		return false;
	}
}

void spider(Entity log, int pipewrite)
{
	Entity spider;

	spider.lives = 1;
	spider.color = SPIDER_COLOR;
	spider.dir = log.dir;
	spider.row = log.row;
	spider.et = SPIDER;
	spider.pid = getpid();

	//col in the spiders will be used to signal in what section of the log they'll be located
	//0: leftmost
	//1: rightmost
	//2: center
	if(log.length == 6){
		spider.col = rand()%2;
	}else{
		spider.col = rand()%3;
	}

	switch(spider.col){
		case 0:
			spider.box.topLeft.y = spider.box.botRight.y = log.box.topLeft.y+1;
			spider.box.topLeft.x = log.box.topLeft.x;
			spider.box.botRight.x = log.box.topLeft.x+2;
		break;

		case 1:
			spider.box.topLeft.y = spider.box.botRight.y = log.box.topLeft.y+1;
			spider.box.topLeft.x = log.box.botRight.x-2;
			spider.box.botRight.x = log.box.botRight.x;			
		break;

		case 2:
			spider.box.topLeft.y = spider.box.botRight.y = log.box.topLeft.y+1;
			spider.box.topLeft.x = log.box.topLeft.x+3;
			spider.box.botRight.x = log.box.botRight.x-3;		
		break; 
	}

	while (true) {       
    	switch(spider.col){
    		//left
			case 0:
				switch(spider.dir){
		    		case W:
		    			if(spider.box.topLeft.x == 1){
		    				spider.dir = E;
		    				spider.box.topLeft.x++;
							spider.box.botRight.x++;
		    			}else{
		    				spider.box.topLeft.x--;
							spider.box.botRight.x--;	
		    			}    			
		    		break;
		    		
		    		case E:
		    			if(spider.box.botRight.x == MAXX-(log.length-2)){
		    				spider.dir = W;
		    				spider.box.topLeft.x--;
							spider.box.botRight.x--;	
		    			}else{
		    				spider.box.topLeft.x++;
							spider.box.botRight.x++;
		    			}		    			
		    		break;
		    	}
			break;

			//right
			case 1:
				switch(spider.dir){
		    		case W:
		    			if(spider.box.topLeft.x == (log.length-2)){
		    				spider.dir = E;
		    				spider.box.topLeft.x++;
							spider.box.botRight.x++;
		    			}else{
		    				spider.box.topLeft.x--;
							spider.box.botRight.x--;	
		    			}    			
		    		break;
		    		
		    		case E:
		    			if(spider.box.botRight.x == MAXX-1){
		    				spider.dir = W;
		    				spider.box.topLeft.x--;
							spider.box.botRight.x--;
		    			}else{
		    				spider.box.topLeft.x++;
							spider.box.botRight.x++;
		    			}		    			
		    		break;
		    	}
			break;

			//central
			case 2:
				switch(spider.dir){
		    		case W:
		    			if(spider.box.topLeft.x == (PHROG_SIZE+1)){
		    				spider.dir = E;
		    				spider.box.topLeft.x++;
							spider.box.botRight.x++;
		    			}else{
		    				spider.box.topLeft.x--;
							spider.box.botRight.x--;	
		    			}    			
		    		break;
		    		
		    		case E:
		    			if(spider.box.botRight.x == MAXX-(PHROG_SIZE)-1){
		    				spider.dir = W;
		    				spider.box.topLeft.x--;
							spider.box.botRight.x--;
		    			}else{
		    				spider.box.topLeft.x++;
							spider.box.botRight.x++;
		    			}		    			
		    		break;
		    	}
			break; 
		}

		if(rand()%ATTACK_CHANCE == 0){
			spit(pipewrite,spider.box,spider.et);
		}

        updateEntity(spider,pipewrite);

        usleep(100000);         
    }
}

// funzione per la generazione degli sputi del ragno
void spit(int pipewrite, Hitbox pH, EntityType et)
{
    Entity spitball;

    spitball.lives = 1;
    spitball.box.topLeft.x = spitball.box.botRight.x = pH.topLeft.x+1;
    spitball.box.topLeft.y = spitball.box.botRight.y = pH.topLeft.y-1;
    spitball.color = COLOR_MAGENTA;
    spitball.dir = N;
    spitball.et = SPITBALL;

    if(fork() == 0){
        moveSpitBall(pipewrite,spitball,et);
    }
}

// funzione per il movimento degli sputi
void moveSpitBall(int pipewrite, Entity projectile, EntityType type)
{
    
    projectile.pid = getpid();
    while (true) {

        switch(type){
    		case PHROG:
    			projectile.box.topLeft.y--; 
        		projectile.box.botRight.y--;
        		projectile.dir = N;
			break;
			case SPIDER:
				projectile.box.topLeft.y++; 
	        	projectile.box.botRight.y++;
	        	projectile.dir = S;
			break;
    	}

        if(projectile.box.topLeft.y <= 1 || projectile.box.topLeft.y >= MAXY-1){
            projectile.lives = 0;
            updateEntity(projectile,pipewrite);
            kill(projectile.pid, 1);
        }

        updateEntity(projectile,pipewrite);

        usleep(25000);         
    }
}

int spitballCollisions(Entity spit)
{
	int i,j;

	for(i = 0; i < NUM_LANES; i++){
		for(j = 0; j < NUM_CARS; j++){
			if(verifyHitbox(spit.box, game.carTable[i][j].box)){
				bodyClearingSingleEntities(spit,game.gameWin);
				return 1;
			}
		}
	}

	for(i = 0; i < NUM_LOGS; i++){
		if(verifyHitbox(spit.box, game.spiders[i].box)){
			game.spiders[i].lives = 0;
			bodyClearingSingleEntities(spit,game.gameWin);
			return 2;
		}
	}

	if(verifyHitbox(spit.box,game.player.box)){
		bodyClearing(spit,game.gameWin);
		return 3;
	}

	return 0;
}

// SEZIONE GESTIONE GIOCO E GRAFICA
void initializeData(_Bool dRegister[], int nDens)
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
	game.player.lives = PHROG_STARTING_LIVES;
	game.player.color = PHROG_ON_GRASS;
	game.player.row = 8;
	game.player.col = 1;
	game.player.et = PHROG;

	game.player.box.topLeft.x = 1;
    game.player.box.topLeft.y = MAXY-3;

    game.player.box.botRight.x = game.player.box.topLeft.x + PHROG_SIZE-1;
    game.player.box.botRight.y = game.player.box.topLeft.y + PHROG_SIZE-1;

	// init cars
	// row in the case of cars is the lane in which they are found
	// col is just car number 0,1,2 etc..
	for(i = 0; i < NUM_LANES; i++){
		for(j = 0; j < NUM_CARS; j++){
			game.carTable[i][j].row = i;
			game.carTable[i][j].col = j;
			game.carTable[i][j].et = CAR;
		}
	}

	// init logs
	for(i = 0; i < NUM_LOGS; i++){
		game.logs[i].row = i;
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

void updateEntity(Entity temp, int pipewrite)
{
    Entity current;

    if(temp.et == SPITBALL) {
        current.box.topLeft.x = current.box.botRight.x = temp.box.topLeft.x;
        current.box.topLeft.y = current.box.botRight.y = temp.box.topLeft.y;
    } else {
        current.box.topLeft.x = temp.box.topLeft.x;
        current.box.topLeft.y = temp.box.topLeft.y;
        current.box.botRight.x = temp.box.botRight.x;
        current.box.botRight.y = temp.box.botRight.y;
    }

    current.lives = temp.lives;
    current.et = temp.et;
    current.color = temp.color;
    current.dir = temp.dir;
    current.pid = temp.pid;
 	
 	current.row = temp.row;
 	current.col = temp.col;
 	current.length = temp.length;

    write(pipewrite, &current, sizeof(Entity));
}

void screenRefresh(){
	wrefresh(game.gameWin);
	wrefresh(game.statWin);
}

int roadsAndPonds(int piperead, int pipewrite, _Bool dRegister[], int nDens)
{
    initializeData(dRegister,NUM_DENS);
    Entity tempEntity, tempProjectile;

    _Bool playerHit = false;
    _Bool denReached = false;
    _Bool playerIsDry;
    int result, denId, resultOfSpitCollision;
    int countdown = 0;
    time_t currentTime, startTime;
    FILE *debugLog;

    debugLog = fopen("debugLog.txt","a");
    fprintf(debugLog,"----start of manche----\n");
    drawMap();
    
    countdown = DELAY;
    time(&startTime);
    fclose(debugLog);
    while(!playerHit && !denReached)
    {   
    	time(&currentTime);
    	debugLog = fopen("debugLog.txt","a");
    	if(difftime(currentTime,startTime) >= DELAY){
    		playerHit = true;
    		fprintf(debugLog, "time is up, gg");
    	}else{    
        	mvwprintw(game.statWin,2,1,"[·time:%d·]",countdown);
	       
	        drawMap();
	        read(piperead, &tempEntity, sizeof(Entity));

	        switch (tempEntity.et)
	        {
	            case PHROG:
	            	fprintf(debugLog, "reading phrog\n");
	            	// se il player è sul tronco, muovilo col
	            	mvwprintw(game.statWin,1,1,"[·phrog lives:%d·]\n",tempEntity.lives);
	                bodyClearing(game.player,game.gameWin);  
	                
	                game.player.pid = tempEntity.pid;
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
	            	fprintf(debugLog, "reading spider\n");
	            	bodyClearingSingleEntities(game.spiders[tempEntity.row],game.gameWin);

	            	game.spiders[tempEntity.row].color = tempEntity.color;
	            	game.spiders[tempEntity.row].dir = tempEntity.dir;
	            	game.spiders[tempEntity.row].et = tempEntity.et;
	            	game.spiders[tempEntity.row].col = tempEntity.col;

	            	game.spiders[tempEntity.row].box.topLeft.x = tempEntity.box.topLeft.x;
	                game.spiders[tempEntity.row].box.topLeft.y = tempEntity.box.topLeft.y;
	                game.spiders[tempEntity.row].box.botRight.x = tempEntity.box.botRight.x;
	                game.spiders[tempEntity.row].box.botRight.y = tempEntity.box.botRight.y;
	                game.spiders[tempEntity.row].pid = tempEntity.pid;

	            	if(tempEntity.lives > 0){
	            		printerSingleEntities(game.spiders[tempEntity.row],game.gameWin);
	            	}
	            break;

	            case CAR:
	            	fprintf(debugLog, "reading car\n");
	                bodyClearing(game.carTable[tempEntity.row][tempEntity.col],game.gameWin); 

	                game.carTable[tempEntity.row][tempEntity.col].length = tempEntity.length;  
	                game.carTable[tempEntity.row][tempEntity.col].color = tempEntity.color;
	                game.carTable[tempEntity.row][tempEntity.col].dir = tempEntity.dir;
	                
	                game.carTable[tempEntity.row][tempEntity.col].box.topLeft.x = tempEntity.box.topLeft.x;
	                game.carTable[tempEntity.row][tempEntity.col].box.topLeft.y = tempEntity.box.topLeft.y;
	                game.carTable[tempEntity.row][tempEntity.col].box.botRight.x = tempEntity.box.botRight.x;
	                game.carTable[tempEntity.row][tempEntity.col].box.botRight.y = tempEntity.box.botRight.y;
	                game.carTable[tempEntity.row][tempEntity.col].pid = tempEntity.pid;  

	                playerHit = carCollisions(game.carTable[tempEntity.row][tempEntity.col],game.player); 

	                if(playerHit){
	                	fprintf(debugLog, "player got run over by cars\n");
	                }  

	                printerCars(game.carTable[tempEntity.row][tempEntity.col],game.gameWin);
	            break;

	            case LOG:
	            	fprintf(debugLog, "reading log\n");
	                bodyClearing(game.logs[tempEntity.row],game.gameWin); 

	                game.logs[tempEntity.row].length = tempEntity.length;  
	                game.logs[tempEntity.row].color = tempEntity.color;
	                game.logs[tempEntity.row].dir = tempEntity.dir;
	                
	                game.logs[tempEntity.row].box.topLeft.x = tempEntity.box.topLeft.x;
	                game.logs[tempEntity.row].box.topLeft.y = tempEntity.box.topLeft.y;
	                game.logs[tempEntity.row].box.botRight.x = tempEntity.box.botRight.x;
	                game.logs[tempEntity.row].box.botRight.y = tempEntity.box.botRight.y;
	                game.logs[tempEntity.row].pid = tempEntity.pid;

	                printerLogs(game.logs[tempEntity.row],game.gameWin);


	                if(game.logs[tempEntity.row].box.topLeft.x == 1 || game.logs[tempEntity.row].box.botRight.x == MAXX-1){ 	
						if(rand()%ENEMY_CHANCE == 0 && !game.player.isOnLog && !game.logs[tempEntity.row].hasSpider){
							game.logs[tempEntity.row].hasSpider = true;
							fprintf(debugLog, "spooder generated\n");
							if(fork()==0) spider(game.logs[tempEntity.row],pipewrite);
							usleep(3000);
						}
	                }	
					
	                if(game.logs[tempEntity.row].hasSpider){
	                	bodyClearing(game.spiders[tempEntity.row],game.gameWin);
	                	printerSingleEntities(game.player,game.gameWin);
	                }
	                
	               	
	                if(game.logs[tempEntity.row].isOnLog && game.player.isOnLog){
	                	bodyClearing(game.player,game.gameWin);
	                	switch(game.logs[tempEntity.row].dir){
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
	                	mvprintw(2,MAXX+1,"current player position x:%d, y:%d, row:%d dir:",game.player.box.topLeft.x,game.player.box.topLeft.y,game.player.row);          	
	                }
	                
	            break;
	            
	            case SPITBALL:
	            	fprintf(debugLog, "reading spitball\n");
	            	bodyClearingSingleEntities(tempEntity,game.gameWin);

	            	tempProjectile.box.topLeft.x = tempEntity.box.topLeft.x;
	                tempProjectile.box.topLeft.y = tempEntity.box.topLeft.y;
	                tempProjectile.box.botRight.x = tempEntity.box.botRight.x;
	                tempProjectile.box.botRight.y = tempEntity.box.botRight.y;

	                tempProjectile.et = tempEntity.et;
	                tempProjectile.color = tempEntity.color;
	                tempProjectile.dir = tempEntity.dir;
	                tempProjectile.pid = tempEntity.pid;

	            	resultOfSpitCollision = spitballCollisions(tempProjectile);

	            	switch(resultOfSpitCollision){
	            		// no collision
	            		case 0:
	            			printerSingleEntities(tempProjectile,game.gameWin);
	            		break;
	            		// collision w/ car
	            		case 1:
	            			fprintf(debugLog,"spitball hit the car \n");
	            			tempProjectile.lives = 0;
	            			kill(tempProjectile.pid,1);
	            			
	            		break;
	            		//collision w/ spider
	            		case 2:
	            			fprintf(debugLog,"spitball killed a spider \n");
	            			tempProjectile.lives = 0;
	            			kill(tempProjectile.pid,1);
	            			for(int i = 0; i<NUM_LOGS; i++){
	            				if(game.spiders[i].lives <= 0){
	       							for(int j=0; j < NUM_LOGS; j++){
	       								if(game.spiders[i].row == game.logs[j].row){
	       									game.logs[i].hasSpider = false;
	       									bodyClearingSingleEntities(game.spiders[i],game.gameWin);
	    			            			kill(game.spiders[i].pid,1); //sighup here
			            					fprintf(debugLog,"for real \n");
	       								}
	       							}
	            					
	            				}
	            			}

	            		break;
	            		// collision w/ player
	            		case 3:
	            			fprintf(debugLog,"spitball killed the player \n");
	            			tempProjectile.lives = 0;
	            			kill(tempProjectile.pid,1);
        					bodyClearingSingleEntities(game.player,game.gameWin);
	            			playerHit = true;
	            		break;
	            	}          
	            break;
	        }
	        
	        screenRefresh();
	    }       
	    countdown--; 
	    fclose(debugLog);
    }

    erase();
    screenRefresh();

    if(playerHit){
    	result = OUCH;
    }

    if(denReached){
    	result = denId;
    }
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

void drawGridNums()
{
	int i;

	for(i = 0; i <= MAXX; i++){
		mvprintw(MAXY+1,i,"%d",i%10);
	}

	for(i = 0; i <= MAXY; i++){
		mvprintw(i,MAXX+1,"%d",i);
	}
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