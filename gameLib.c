#include "gameLib.h"

Gamestate game;

// SEZIONE GENERAZIONE E MOVIMENTO ENTITA

// funzione per il movimento della rana
void phrog(int lives,int pipewrite)
{
    char input;
    Entity player;

    player.pid = getpid();
    player.dir = FIXED;
    player.lives = lives;
	player.color = RED;
	player.et = PHROG;

	player.box.topLeft.x = 1;
    player.box.topLeft.y = MAXY-3;
    player.box.botRight.x = player.box.topLeft.x + PHROG_SIZE-1;
    player.box.botRight.y = player.box.topLeft.y + PHROG_SIZE-1;

    player.col = 1;
	player.row = 9;

    while(true) {
        input=getch();
        switch(input) {
            case UP:
				if(player.box.topLeft.y > 1){
					player.box.topLeft.y -= 3;
					player.dir = N;		
					player.row--;
				}	
			break;

			case DOWN:
				if(player.box.botRight.y < MAXY-1){
					player.box.topLeft.y += 3;
					player.dir = S;
					player.row++;
				}
			break;

			case LEFT:
				if(player.box.topLeft.x > 1){
					player.box.topLeft.x -= 3;	
					player.dir = W;
					player.col--;
				}
			break;

			case RIGHT:
				if(player.box.botRight.x < MAXX-1){
					player.box.topLeft.x += 3;	
					player.dir = E;
					player.col++;	
				}
			break;
        }

        player.box.botRight.x = player.box.topLeft.x + PHROG_SIZE-1;
        player.box.botRight.y = player.box.topLeft.y + PHROG_SIZE-1;

        updateEntity(player,pipewrite);
    }
}

void resetPhrog(int pipewrite)
{
	game.player.lives -= 1;
	if(game.player.lives <= 0){
		game.running = false;
		erase();
		curs_set(1);
		endwin();
		printf("game over hoe");
		exit(0);
	}
	kill(game.player.pid,0);
	phrog(game.player.lives,pipewrite);
}

void carGenerator(int pipewrite)
{
	int i,j;
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
				car[i][j].color = BLUE;
			}else {
				car[i][j].color = CYAN;
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

void carCollisions(Entity currentCar, int pipewrite)
{
	int i;

    if(verifyHitbox(currentCar.box,game.player.box)){
    	resetPhrog(pipewrite);
    }

    for(i = 0; i < NUM_CARS; i++){
    	if(verifyHitbox(currentCar.box,game.carTable[currentCar.row][i].box) && currentCar.pid != game.carTable[currentCar.row][i].pid){
    		haltCar(currentCar.col,currentCar.row);
    	}
    }
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
	int i,j;
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
		logs[i].color = YELLOW;

		// deciding characteristics
		if(rand()%2 == 0){
			logs[i].length = 6;
		}else{
			logs[i].length = 9;
		}

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

void logCollisions(Entity phrog, Entity currentLog, int pipewrite)
{
	if(!verifyHitbox(phrog.box,currentLog.box)){
		resetPhrog(pipewrite);
	}
}

// SEZIONE GESTIONE GIOCO E GRAFICA
void initializeData()
{
	int i,j;

	game.running = true;
    game.loss = false;
    game.win = false;

	// init colors
	init_pair(WHITE,COLOR_WHITE,COLOR_BLACK);
    init_pair(RED,COLOR_RED,COLOR_BLACK);
    init_pair(YELLOW,COLOR_YELLOW,COLOR_BLACK);
    init_pair(GREEN,COLOR_GREEN,COLOR_BLACK);
    init_pair(BLUE,COLOR_BLUE,COLOR_BLACK);
    init_pair(MAGENTA,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(CYAN,COLOR_CYAN,COLOR_BLACK);

    //delimitations calculations
    game.zoneLimitY[0] = 4;
	game.zoneLimitY[1] = 4 + (NUM_RIVERS*PHROG_SIZE);
	game.zoneLimitY[2] = (MAXY-3) - (NUM_LANES*PHROG_SIZE);
	game.zoneLimitY[3] = MAXY-3;

	// initPlayer
	game.player.lives = PHROG_STARTING LIVES;
	game.player.color = RED;
	game.player.row = 9;
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
	}

	// init dens
	for(i = 0; i < NUM_DENS; i++){
		game.visitedDens[i].coords.x = 1 + (i*WIDTH_DENS*2);
		game.visitedDens[i].coords.y = 1;
		game.visitedDens[i].visited = false;
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

void roadsAndPonds(int piperead, int pipewrite)
{
    initializeData();
    Entity tempEntity;

    drawMap();
    
    while(game.running)
    {   
        drawMap();
        read(piperead, &tempEntity, sizeof(Entity));
        mvprintw(0,MAXX+1,"entity read:%d",tempEntity.et);

        switch (tempEntity.et)
        {
            case PHROG:
                bodyClearing(game.player);                
                     
                game.player.pid = tempEntity.pid;
                game.player.dir = tempEntity.dir;

                game.player.box.topLeft.x = tempEntity.box.topLeft.x;
                game.player.box.topLeft.y = tempEntity.box.topLeft.y;
                game.player.box.botRight.x = tempEntity.box.botRight.x;
                game.player.box.botRight.y = tempEntity.box.botRight.y;
                game.player.row = tempEntity.row;

                if(game.player.box.topLeft.y >= MIN_ROW_LOG && game.player.box.topLeft.y <= MAX_ROW_LOG){
                	logCollisions(game.player,game.logs[calcRow(game.player.row)],pipewrite);
                }
                
                if(game.player.lives > 0){
                    printerSingleEntities(game.player);
                }
                
                mvprintw(MAXY+1,0,"Phrog position x:%d, y:%d, dir:",
                game.player.box.topLeft.x,game.player.box.topLeft.y);
               	translateDirection(game.player.dir);
            break;

            case SPIDER:
                //bodyClearing(tempEntity);
                //tempBomba.box.topLeft.x = tempBomba.box.botRight.x = tempEntity.box.topLeft.x;
                //tempBomba.box.topLeft.y = tempBomba.box.botRight.y = tempEntity.box.botRight.y;
                //tempBomba.pid = tempEntity.pid;
                //tempBomba.d = tempEntity.d;
                //tempBomba.id = tempEntity.id;
                //tempBomba.e = tempEntity.e;                            
                
            	// check player collision

                //if(tempBomba.box.topLeft.x > 1){
                //    printer(tempBomba);
                //}               
            break;

            case CAR:             
                bodyClearing(game.carTable[tempEntity.row][tempEntity.col]); 

                game.carTable[tempEntity.row][tempEntity.col].length = tempEntity.length;  
                game.carTable[tempEntity.row][tempEntity.col].color = tempEntity.color;
                game.carTable[tempEntity.row][tempEntity.col].dir = tempEntity.dir;
                
                game.carTable[tempEntity.row][tempEntity.col].box.topLeft.x = tempEntity.box.topLeft.x;
                game.carTable[tempEntity.row][tempEntity.col].box.topLeft.y = tempEntity.box.topLeft.y;
                game.carTable[tempEntity.row][tempEntity.col].box.botRight.x = tempEntity.box.botRight.x;
                game.carTable[tempEntity.row][tempEntity.col].box.botRight.y = tempEntity.box.botRight.y;
                game.carTable[tempEntity.row][tempEntity.col].pid = tempEntity.pid;  

                carCollisions(game.carTable[tempEntity.row][tempEntity.col],pipewrite);   

                printerCars(game.carTable[tempEntity.row][tempEntity.col]);
            break;

            case LOG:
                bodyClearing(game.logs[tempEntity.row]); 

                game.logs[tempEntity.row].length = tempEntity.length;  
                game.logs[tempEntity.row].color = tempEntity.color;
                game.logs[tempEntity.row].dir = tempEntity.dir;
                
                game.logs[tempEntity.row].box.topLeft.x = tempEntity.box.topLeft.x;
                game.logs[tempEntity.row].box.topLeft.y = tempEntity.box.topLeft.y;
                game.logs[tempEntity.row].box.botRight.x = tempEntity.box.botRight.x;
                game.logs[tempEntity.row].box.botRight.y = tempEntity.box.botRight.y;
                game.logs[tempEntity.row].pid = tempEntity.pid;        

                printerLogs(game.logs[tempEntity.row]);
            break;
            
            case SPITBALL:
                //bodyClearing(tempEntity);
                //tempMissile.box.topLeft.x = tempMissile.box.botRight.x = tempEntity.box.topLeft.x;
                //tempMissile.box.topLeft.y = tempMissile.box.botRight.y = tempEntity.box.botRight.y;
                //tempMissile.pid = tempEntity.pid;
                //tempMissile.col = tempEntity.col;
                //tempMissile.d = tempEntity.d;
                //tempMissile.id = tempEntity.id;
                //tempMissile.e = tempEntity.e;

                // check player collision
            
                //if(tempMissile.lives > 0 && tempMissile.box.topLeft.x < MAXX-1 && tempMissile.box.topLeft.y > 1 && tempMissile.box.topLeft.y < MAXY-1){
                //    printer(tempMissile);
                //}              
            break;
        }
        
        refresh();  
    }
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

// funzioni per la stampa e la pulizia
void drawMap()
{
    int i,j,k;

    // drawing border
    for (j = 0; j <= MAXY; j++) {
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

    // limiting zones
    for(j = 0; j<ZONES; j++){
    	mvaddch(game.zoneLimitY[j],0,ACS_LTEE);	    
	    for(i = 1;i<MAXX;i++){
	    	mvaddch(game.zoneLimitY[j],i,ACS_HLINE);
	    }
	    mvaddch(game.zoneLimitY[j],MAXX,ACS_RTEE);
    }

    // drawing dens
    for(i = 0; i < NUM_DENS; i++){
    	switch(i){
	    	case 0:
	    		mvaddch(0,game.visitedDens[i].coords.x+WIDTH_DENS,ACS_TTEE);
	    		for(j=1;j<PHROG_SIZE+1;j++){
	    			mvaddch(j,game.visitedDens[i].coords.x+WIDTH_DENS,ACS_VLINE);
	    		}
	    		mvaddch(4,game.visitedDens[i].coords.x+WIDTH_DENS,ACS_BTEE);
	    	break;

	    	case NUM_DENS-1:
	    		mvaddch(0,game.visitedDens[i].coords.x,ACS_TTEE);
	    		for(j=1;j<PHROG_SIZE+1;j++){
	    			mvaddch(j,game.visitedDens[i].coords.x,ACS_VLINE);
	    		}
	    		mvaddch(4,game.visitedDens[i].coords.x,ACS_BTEE);
	    	break;

	    	default:
	    		mvaddch(0,game.visitedDens[i].coords.x,ACS_TTEE);
	    		for(j=1;j<PHROG_SIZE+1;j++){
	    			mvaddch(j,game.visitedDens[i].coords.x,ACS_VLINE);
	    		}
	    		mvaddch(4,game.visitedDens[i].coords.x,ACS_BTEE);
	    		
	    		mvaddch(0,game.visitedDens[i].coords.x+WIDTH_DENS,ACS_TTEE);
	    		for(j=1;j<PHROG_SIZE+1;j++){
	    			mvaddch(j,game.visitedDens[i].coords.x+WIDTH_DENS,ACS_VLINE);
	    		}
	    		mvaddch(4,game.visitedDens[i].coords.x+WIDTH_DENS,ACS_BTEE);	    		
	    	break;
    	}
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

// SEZIONE NON IMPLEMENTATI

// void spider(Entity log, int pipewrite){}

// funzione per la generazione degli sputi del ragno
void spit(int pipewrite, Hitbox pH)
{
    Entity spitball;

    spitball.lives = 1;
    spitball.box.topLeft.x = spitball.box.botRight.x = pH.botRight.x-1;
    spitball.box.topLeft.y = spitball.box.botRight.y = pH.botRight.y+1;
    spitball.color = MAGENTA;
    spitball.dir = S;
    spitball.et = SPITBALL;

    if(fork() == 0){
        moveSpitBall(pipewrite, spitball);
    }
}

// funzione per il movimento degli sputi
void moveSpitBall(int pipewrite, Entity projectile)
{
    
    projectile.pid = getpid();
    while (true) {
        projectile.box.topLeft.y = projectile.box.botRight.y -=1;

        if (projectile.box.topLeft.x <= 1) {
            projectile.lives = 0;
            updateEntity(projectile,pipewrite);
            kill(projectile.pid, 1);
            return;
        }

        updateEntity(projectile,pipewrite);

        usleep(25000);         
    }
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





