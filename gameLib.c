#include "gameLib.h"

Gamestate game;

// munzione per il movimento della rana
void phrog(int pipewrite)
{
    char input;
    Entity player;

    player.box.topLeft.x = 1;
    player.box.topLeft.y = MAXY-3;

    player.box.botRight.x = player.box.topLeft.x + PHROG_SIZE-1;
    player.box.botRight.y = player.box.topLeft.y + PHROG_SIZE-1;

    player.lives = 3;
    player.dir = FIXED;
    player.et = PHROG;
    player.pid = getpid();
    player.col = RED;

    while(true) {
        input=getch();
        switch(input) {
            case UP:
                if(player.box.topLeft.y > 1){
                    player.box.topLeft.y -= PHROG_SIZE;
                    player.dir = N;
                }
            break;

            case DOWN:
                if(player.box.botRight.y < MAXY-1){
                    player.box.topLeft.y += PHROG_SIZE;
                    player.dir = S;
                }
            break;

        	case LEFT:
        		if(player.box.topLeft.x > 1){
                    player.box.topLeft.x -= PHROG_SIZE;
                    player.dir = W;
                }
        	break;


        	case RIGHT:
        		if(player.box.botRight.x < MAXX-1){
                    player.box.topLeft.x += PHROG_SIZE;
                    player.dir = E;
                }
        	break;      
        }

        player.box.botRight.x = player.box.topLeft.x + PHROG_SIZE-1;
        player.box.botRight.y = player.box.topLeft.y + PHROG_SIZE-1;

        updateEntity(player,pipewrite);
        usleep(20000);
    }
}

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

void roadsAndPonds(int piperead, int pipewrite)
{
    initializeData();
    Entity tempEntity;

    drawMap();
    
    while(game.running)
    {   
        drawMap();
        read(piperead, &tempEntity, sizeof(Entity));

        switch (tempEntity.et)
        {
            case PHROG:
                bodyClearing(game.player);
                game.player.box.topLeft.x = tempEntity.box.topLeft.x;
                game.player.box.topLeft.y = tempEntity.box.topLeft.y;
                game.player.box.botRight.x = tempEntity.box.botRight.x;
                game.player.box.botRight.y = tempEntity.box.botRight.y;
                game.player.pid = tempEntity.pid;
                if(game.player.lives > 0){
                    printer(tempEntity);
                }
            break;

            case LOG:
                //bodyClearing(game.aliensLVL1[tempEntity.id]);
                //game.aliensLVL1[tempEntity.id].box.topLeft.x = tempEntity.box.topLeft.x;
                //game.aliensLVL1[tempEntity.id].box.topLeft.y = tempEntity.box.topLeft.y;
                //game.aliensLVL1[tempEntity.id].box.botRight.x = tempEntity.box.botRight.x;
                //game.aliensLVL1[tempEntity.id].box.botRight.y = tempEntity.box.botRight.y;
                //game.aliensLVL1[tempEntity.id].d = tempEntity.d;
                //game.aliensLVL1[tempEntity.id].col = tempEntity.col;
                //game.aliensLVL1[tempEntity.id].pid = tempEntity.pid;                              
                //shipCollisions(game.aliensLVL1[tempEntity.id]);
                //checkBorderProximity(game.aliensLVL1[tempEntity.id].box.topLeft.x);               
                //if(game.aliensLVL1[tempEntity.id].lives > 0){
                //    printer(game.aliensLVL1[tempEntity.id]);
                //}
            break;
            
            case CAR:
                //bodyClearing(game.aliensLVL2[tempEntity.id]); 
                //game.aliensLVL2[tempEntity.id].box.topLeft.x = tempEntity.box.topLeft.x;
                //game.aliensLVL2[tempEntity.id].box.topLeft.y = tempEntity.box.topLeft.y;
                //game.aliensLVL2[tempEntity.id].box.botRight.x = tempEntity.box.botRight.x;
                //game.aliensLVL2[tempEntity.id].box.botRight.y = tempEntity.box.botRight.y;
                //game.aliensLVL2[tempEntity.id].d = tempEntity.d;
                //game.aliensLVL2[tempEntity.id].col = tempEntity.col;
                //game.aliensLVL2[tempEntity.id].pid = tempEntity.pid;                
                //shipCollisions(game.aliensLVL2[tempEntity.id]);
                //checkBorderProximity(game.aliensLVL2[tempEntity.id].box.topLeft.x);
                //if(game.aliensLVL2[tempEntity.id].lives > 0){
                //    printer(game.aliensLVL2[tempEntity.id]);
                //}
            break;
        
            case SPIDER:
                //bodyClearing(tempEntity);
                //tempBomba.box.topLeft.x = tempBomba.box.botRight.x = tempEntity.box.topLeft.x;
                //tempBomba.box.topLeft.y = tempBomba.box.botRight.y = tempEntity.box.botRight.y;
                //tempBomba.pid = tempEntity.pid;
                //tempBomba.d = tempEntity.d;
                //tempBomba.id = tempEntity.id;
                //tempBomba.e = tempEntity.e;                            
                //shipCollisions(tempBomba);
                //if(tempBomba.box.topLeft.x > 1){
                //    printer(tempBomba);
                //}               
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
                //projectileCollisions(tempMissile,pipewrite);
                //if(tempMissile.lives > 0 && tempMissile.box.topLeft.x < MAXX-1 && tempMissile.box.topLeft.y > 1 && tempMissile.box.topLeft.y < MAXY-1){
                //    printer(tempMissile);
                //}              
            break;
        }
        
        refresh();  
    }
}

/*
	// funzione che regola lo spawn dei nuvoi alieni
	void alienGenerator(int pipewrite){
	    int prevmod,currentmod,lastOccupPosition,i;
	    Entity aliensLVL1[ALIENS_ALLOWED*WAVES];
	    prevmod = currentmod = 0;

	    for (int i=0; i<(ALIENS_ALLOWED * WAVES); i+=1) {
	        currentmod = rand()%3;
	        if(prevmod == currentmod) {
	            if(currentmod - 1 < 0) {
	                currentmod += 1;
	            } else {
	                currentmod -=1;
	            }
	        }

	        aliensLVL1[i].lives = 1;
	        aliensLVL1[i].e = ORFIST;
	        aliensLVL1[i].id = i;

	        switch(currentmod){
	            case 0:
	                aliensLVL1[i].d = NWEST;
	                aliensLVL1[i].box.topLeft.y = 25 + rand()%9;
	                aliensLVL1[i].box.topLeft.x = MAXX - ALIEN_SIZE;
	                aliensLVL1[i].col = YELLOW;
	            break;

	            case 1:
	                aliensLVL1[i].d = SWEST;
	                aliensLVL1[i].box.topLeft.y = 1 + rand()%9;
	                aliensLVL1[i].box.topLeft.x = MAXX - ALIEN_SIZE;
	                aliensLVL1[i].col = MAGENTA;
	            break;

	            case 2:
	                aliensLVL1[i].d = FIXED;
	                aliensLVL1[i].box.topLeft.y = MAXY/2;
	                aliensLVL1[i].box.topLeft.x = MAXX - ALIEN_SIZE;
	                aliensLVL1[i].col = GREEN;
	            break;
	        }

	        aliensLVL1[i].box.botRight.x = aliensLVL1[i].box.topLeft.x + (ALIEN_SIZE-1);
	        aliensLVL1[i].box.botRight.y = aliensLVL1[i].box.topLeft.y + (ALIEN_SIZE-1);

	        prevmod = currentmod;
	    }

	    lastOccupPosition = 0;

	    while(lastOccupPosition < WAVES*ALIENS_ALLOWED){
	        
	        for(i = lastOccupPosition;i < ALIENS_ALLOWED+lastOccupPosition; i++){
	            if(fork()== 0){
	                alien(pipewrite,aliensLVL1[i]);
	            }
	            sleep(3);
	        }
	        lastOccupPosition = i;  
	        sleep(10);
	    }

	    exit(0);
	}

	// funzione per il movimento degli alieni di lvl 1
	void alien(int pipewrite, Entity alien)
	{
	    srand(time(NULL) * alien.id);
	    alien.pid = getpid();
	    int midborder = MAXY/2;

	    while (true){
	        switch(alien.d){
	            case NWEST:
	                alien.box.topLeft.x--;
	                if(alien.box.topLeft.y <= 1 || alien.box.topLeft.y == midborder-1){
	                    alien.d = SWEST;
	                    alien.box.topLeft.y++;
	                } else {
	                    alien.box.topLeft.y--;
	                }
	                alien.box.botRight.x = alien.box.topLeft.x + ALIEN_SIZE-1;
	                alien.box.botRight.y = alien.box.topLeft.y + ALIEN_SIZE-1;
	            break;

	            case SWEST:
	                alien.box.topLeft.x--;
	                if(alien.box.botRight.y >= MAXY-1 || alien.box.botRight.y == midborder+1) {
	                    alien.d = NWEST;
	                    alien.box.topLeft.y--;
	                } else {
	                    alien.box.topLeft.y++;
	                }
	                alien.box.botRight.x = alien.box.topLeft.x + ALIEN_SIZE-1;
	                alien.box.botRight.y = alien.box.topLeft.y + ALIEN_SIZE-1;
	            break;

	            case FIXED:
	                alien.box.topLeft.x--;
	                alien.box.botRight.x = alien.box.topLeft.x + ALIEN_SIZE-1;
	            break;
	        }
	        attack(pipewrite, ATK_CHANCE, alien.box);
	        updateEntity(alien,pipewrite); 
	        usleep(600000); 
	    }
	}

	// funzione per il movimento degli alieni di lvl 2
	void alien2(int pipewrite, Entity alien)
	{
	    srand(time(NULL) * alien.id);
	    alien.pid = getpid();
	    int midborder = MAXY/2-2;

	    while (true){ 
	        switch(alien.d) {
	            case NWEST:
	                alien.box.topLeft.x--;
	                if(alien.box.topLeft.y <= 1 || alien.box.topLeft.y == midborder+2) {
	                    alien.d = SWEST;
	                    alien.box.topLeft.y++;
	                } else {
	                    alien.box.topLeft.y--;
	                }
	                alien.box.botRight.y = alien.box.topLeft.y + (ALIEN_SIZE*2)-1;
	                alien.box.botRight.x = alien.box.topLeft.x + (ALIEN_SIZE*2)+1;
	            break;

	            case SWEST:
	                alien.box.topLeft.x--;
	                if(alien.box.botRight.y >= MAXY-1 || alien.box.topLeft.y == midborder-2) {
	                    alien.d = NWEST;
	                    alien.box.topLeft.y--;
	                } else {
	                    alien.box.topLeft.y++;
	                }
	                alien.box.botRight.y = alien.box.topLeft.y + (ALIEN_SIZE*2)-1;
	                alien.box.botRight.x = alien.box.topLeft.x + (ALIEN_SIZE*2)+1;
	            break;

	            case FIXED:
	                alien.box.topLeft.x--;
	                alien.box.botRight.x = alien.box.topLeft.x + (ALIEN_SIZE*2)+1;
	            break;
	        }
	        attack(pipewrite, ATK_CHANCE, alien.box);
	        updateEntity(alien,pipewrite);             
	        usleep(600000); 
	    }
	}

	// funzione che rappresenta lo spazio, stampa tutti gli elementi a schermo non appena vengono letti in pipe
	

	// verifica delle collisioni della bomba
	void projectileCollisions(Entity currentMissile, int pipewrite)
	{
	    for (int i = 0; i < ALIENS_ALLOWED*WAVES ; i++) {
	        if (game.aliensLVL1[i].lives > 0) {
	            if (verifyHitbox(game.aliensLVL1[i].box,currentMissile.box)) {

	                pid_t alien2p;

	                game.aliensLVL2[i].col = game.aliensLVL1[i].col;
	                game.aliensLVL2[i].d = game.aliensLVL1[i].d;
	                game.aliensLVL2[i].box.topLeft.x = game.aliensLVL1[i].box.topLeft.x-2;
	                game.aliensLVL2[i].box.topLeft.y = game.aliensLVL1[i].box.topLeft.y-1;
	                game.aliensLVL2[i].box.botRight.x = game.aliensLVL1[i].box.botRight.x+2;
	                game.aliensLVL2[i].box.botRight.y = game.aliensLVL1[i].box.botRight.y+2;
	                game.aliensLvl2Alive++;

	                game.aliensLvl1Alive--;
	                game.aliensLVL1[i].lives = 0;
	                bodyClearing(currentMissile);
	                bodyClearing(game.aliensLVL1[i]);
	                kill(currentMissile.pid, 1);
	                kill(game.aliensLVL1[i].pid, 1);

	                if((alien2p = fork()) == 0){
	                    alien2(pipewrite, game.aliensLVL2[i]);
	                }
	                return;
	            }
	        }
	    }

	    for (int i = 0; i < ALIENS_ALLOWED*WAVES; i++) {
	        if (game.aliensLVL2[i].lives > 0) {
	            if (verifyHitbox(game.aliensLVL2[i].box, currentMissile.box)) {

	                game.aliensLVL2[i].lives-=1;
	                bodyClearing(currentMissile);
	                kill(currentMissile.pid, 1);
	                
	                if (game.aliensLVL2[i].lives <= 0) {
	                    game.aliensLvl2Alive--;
	                    bodyClearing(game.aliensLVL2[i]);                    
	                    kill(game.aliensLVL2[i].pid, 1);                    
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

	// controlla se le hitbox di due entità compenetrano o meno
	_Bool verifyHitbox(Hitbox a, Hitbox b)
	{

	    if(((a.topLeft.x >= b.topLeft.x && a.topLeft.x <= b.botRight.x) || (a.botRight.x >= b.topLeft.x && a.botRight.x <= b.botRight.x)) &&
	       ((a.topLeft.y >= b.topLeft.y && a.topLeft.y <= b.botRight.y) || (a.botRight.y >= b.topLeft.y && a.botRight.y <= b.botRight.y))) {
	        return true;
	    } else {
	        return false;
	    }
	}
*/

// aggiornamento e scrittura di un entità in pipe
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

// inizializzazione dei dati di gioco
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
	game.player.lives = 3;
	game.player.color = RED;
	game.player.row = game.player.col = 0;
	mvprintw(MAXY+3,MAXX+1,"player stats initialized");


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
		game.visitedDens[i].coords.x = 1 + (i*WIDTH_DENS*2);
		game.visitedDens[i].coords.y = 1;
		game.visitedDens[i].visited = false;
	}
}
    
// funzioni per la stampa e la pulizia
void drawMap()
{
    int i,j,k;

    // drawing border
    for (j= 0; j <= MAXY; j++) {
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

void printer(Entity ent)
{
    // matrice giocatore...
    char phrogBody [3][3] = {
        ",_,",
        "(0)",
        "' '"
    };

    // matrice alieno lvl 1
    char ORFISTBody [3][3] = {
        "(*)",
        "]M[",
        "^v^"
    };

    // matrice alieno lvl 2
    char GRAVIDIABody [2][3] = {
        "<A>",
        "(T)"
    };

    // asterisco per i missili,zero per la bomba
    char CRITICALBody = 'O';
    char METEORBody = '0';

    
    // individuazione entità...
    switch(ent.et) {
    // giocatore
    case PHROG:
    attron(COLOR_PAIR(ent.color));
    for(int i = 0; i<PHROG_SIZE; i++) {
        for(int j = 0; j<PHROG_SIZE; j++) {
            mvaddch(ent.box.topLeft.y+i,ent.box.topLeft.x+j, phrogBody[i][j]);
        }
    }
    attroff(COLOR_PAIR(ent.color));
    break;

    // alieno lvl 1
    case LOG:

    break;

    // alieno lvl2
    case CAR:
    break;

    // bomba
    case SPIDER:
    break;

    // missili
    case SPITBALL:
    break;
    }
}

void bodyClearing(Entity ent){
    int size,x,y,adjy,adjx;

    y = ent.box.topLeft.y;
    x = ent.box.topLeft.x;
    adjy = adjx = 0;

    switch(ent.et) {
        case PHROG:            
            size = PHROG_SIZE;
        break;

        case LOG:

        break;

        case CAR:

        break;

        case SPIDER:
  
        break;

        case SPITBALL:

        break;
    }

    for(int i = 0; i < size; i++){
        for(int j = 0 ; j < size ; j++){
            mvaddch(y+i+adjy,x+j+adjx,' ');
        }
    }
}




