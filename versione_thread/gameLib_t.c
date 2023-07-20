#include "gameLib_t.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
Gamestate game;


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
                spit(game.player);
            break;

            default:
                pthread_mutex_lock(&mutex);
                game.player.dir = FIXED;
                pthread_mutex_unlock(&mutex);
            break;
        }
    }

    while(game.gameReady){
        usleep(1000);
    }

    return NULL;
}

// cars ---------------------------------

void* carGenerator()
{
    int i,j,carlength;
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
        carlength = (4 +rand()%6);
        for(j = 0; j < NUM_CARS; j++){

            // deciding map position and direction
            game.carTable[i][j].row = i;
            game.carTable[i][j].col = j;
            game.carTable[i][j].dir = dirLanes[i];
            game.carTable[i][j].et = CAR;

            // deciding characteristics
            game.carTable[i][j].length = carlength;
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

    while(game.player.lives > 0){
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

    return NULL;
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
    pthread_t spiderThread;

    int i = tempLog->row;

    game.logs[i].id = pthread_self();

    while(game.player.lives > 0){
        switch(game.logs[i].dir){
            case W:
                pthread_mutex_lock(&mutex);
                if(game.logs[i].box.topLeft.x == 1){
                    game.logs[i].dir = E;
                }else{
                    game.logs[i].box.topLeft.x -=1;
                    game.logs[i].box.botRight.x = game.logs[i].box.topLeft.x + game.logs[i].length-1;
                }

                if(game.logs[i].isOnLog && logCollisions()){
                    game.player.box.topLeft.x--;
                    game.player.box.botRight.x--;
                    game.player.dir = W;
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

                if(game.logs[i].isOnLog && logCollisions()){
                    game.player.box.topLeft.x++;
                    game.player.box.botRight.x++;
                    game.player.dir = E;
                }

                pthread_mutex_unlock(&mutex);
            break;
        }

        // Se un tronco raggiunge il bordo, viene generato un ragno
        if((game.logs[i].box.topLeft.x == 1 && game.logs[i].dir == W) || (game.logs[i].box.botRight.x == MAXX-1 && game.logs[i].dir == E)){
            if(rand()%ENEMY_CHANCE == 0 && !game.logs[i].isOnLog && !game.logs[i].hasSpider){
                game.logs[i].hasSpider = true;
                pthread_create(&spiderThread, NULL, &spider, (void*)&game.logs[i]);
            }
        }

        usleep(100000);
    }

    return NULL;
}

_Bool logCollisions()
{
    int i,misses=0;

    for(i = 0; i < NUM_LOGS; i++){
        if(verifyHitbox(game.player.box,game.logs[i].box) && !game.logs[i].hasSpider){
            game.player.dir = E;
            game.logs[i].isOnLog = true;
            game.player.isOnLog = true;

            mvprintw(5,MAXX+1,"log: %d",i);
            return true;
        }else{
            misses +=1;
            game.logs[i].isOnLog = false;
            game.player.isOnLog = false;
        }
    }

    if(misses == NUM_LOGS){
        bodyClearing(game.player,game.gameWin);
        return false;
    }
}

void* spider(void* param)
{

    Entity *log = (Entity*)param;

    int i = log->row;
    game.spiders[i].id = pthread_self();

    game.spiders[i].lives = 1;
    game.spiders[i].color = SPIDER_COLOR;
    game.spiders[i].dir = log->dir;
    game.spiders[i].row = log->row;
    game.spiders[i].et = SPIDER;

    //col in the spiders will be used to signal in what section of the log they'll be located
    //0: leftmost
    //1: rightmost
    //2: center
    if(log->length == 6){
        game.spiders[i].col = rand()%2;
    }else{
        game.spiders[i].col = rand()%3;
    }
    //mvprintw(4,MAXX+1,"col :%d",game.spiders[i].col);

    switch(game.spiders[i].col){
        case 0:
            game.spiders[i].box.topLeft.y = game.spiders[i].box.botRight.y = log->box.topLeft.y+1;
            game.spiders[i].box.topLeft.x = log->box.topLeft.x;
            game.spiders[i].box.botRight.x = log->box.topLeft.x+2;
            break;

        case 1:
            game.spiders[i].box.topLeft.y = game.spiders[i].box.botRight.y = log->box.topLeft.y+1;
            game.spiders[i].box.topLeft.x = log->box.botRight.x-2;
            game.spiders[i].box.botRight.x = log->box.botRight.x;
            break;

        case 2:
            game.spiders[i].box.topLeft.y = game.spiders[i].box.botRight.y = log->box.topLeft.y+1;
            game.spiders[i].box.topLeft.x = log->box.topLeft.x+3;
            game.spiders[i].box.botRight.x = log->box.botRight.x-3;
        break;
    }

    while (game.spiders[i].lives > 0) {
        switch(game.spiders[i].col){
            //left
            case 0:
                switch(game.spiders[i].dir){
                    case W:
                        pthread_mutex_lock(&mutex);
                        if(game.spiders[i].box.topLeft.x == 1){
                            game.spiders[i].dir = E;
                        }else{
                            game.spiders[i].box.topLeft.x--;
                            game.spiders[i].box.botRight.x--;
                        }
                        pthread_mutex_unlock(&mutex);
                        break;

                    case E:
                        pthread_mutex_lock(&mutex);
                        if(game.spiders[i].box.botRight.x == MAXX-(log->length-2)){
                            game.spiders[i].dir = W;
                        }else{
                            game.spiders[i].box.topLeft.x++;
                            game.spiders[i].box.botRight.x++;
                        }
                        pthread_mutex_unlock(&mutex);
                        break;
                }
                break;

                //right
            case 1:
                switch(game.spiders[i].dir){
                    case W:
                        pthread_mutex_lock(&mutex);
                        if(game.spiders[i].box.topLeft.x == (log->length-2)){
                            game.spiders[i].dir = E;
                        }else{
                            game.spiders[i].box.topLeft.x--;
                            game.spiders[i].box.botRight.x--;
                        }
                        pthread_mutex_unlock(&mutex);
                        break;

                    case E:
                        pthread_mutex_lock(&mutex);
                        if(game.spiders[i].box.botRight.x == MAXX-1){
                            game.spiders[i].dir = W;
                        }else{
                            game.spiders[i].box.topLeft.x++;
                            game.spiders[i].box.botRight.x++;
                        }
                        pthread_mutex_unlock(&mutex);
                        break;
                }
                break;

                //central
            case 2:
                switch(game.spiders[i].dir){
                    case W:
                        pthread_mutex_lock(&mutex);
                        if(game.spiders[i].box.topLeft.x == (PHROG_SIZE+1)){
                            game.spiders[i].dir = E;
                            game.spiders[i].box.topLeft.x++;
                            game.spiders[i].box.botRight.x++;
                        }else{
                            game.spiders[i].box.topLeft.x--;
                            game.spiders[i].box.botRight.x--;
                        }
                        pthread_mutex_unlock(&mutex);
                        break;

                    case E:
                        pthread_mutex_lock(&mutex);
                        if(game.spiders[i].box.botRight.x == MAXX-(PHROG_SIZE)-1){
                            game.spiders[i].dir = W;
                            game.spiders[i].box.topLeft.x--;
                            game.spiders[i].box.botRight.x--;
                        }else{
                            game.spiders[i].box.topLeft.x++;
                            game.spiders[i].box.botRight.x++;
                        }
                        pthread_mutex_unlock(&mutex);
                        break;
                }
                break;

        }

        if(rand()%ATTACK_CHANCE == 0){
        	spit(game.spiders[i]);
        }

        //updateEntity(spider,pipewrite);
        mvprintw(2,MAXX+1,"enemy x: %d, y: %d",game.spiders[i].box.botRight.x, game.spiders[i].row);
        usleep(100000);
    }

    return NULL;
}

void spit(Entity shooter)
{
    pthread_t spitThread;

    Entity spitball;

    spitball.lives = 1;
    spitball.box.topLeft.x = spitball.box.botRight.x = shooter.box.topLeft.x+1;
    spitball.box.topLeft.y = spitball.box.botRight.y = shooter.box.topLeft.y-1;
    spitball.et = SPITBALL;
    spitball.isOnLog = true;

    if(shooter.et == PHROG){
        spitball.color = PHROG_ON_ROAD;
        spitball.dir = N;
    }else{
        spitball.color = SPIDER_COLOR;
        spitball.dir = S;
    }

    entityNode* spitNode = insert(spitball,game.projectilePtr);

    pthread_create(&spitThread,NULL,&moveSpitBall,(void*)spitNode);
}

void* moveSpitBall(void* param)
{
     entityNode* projectile = (entityNode*)param;

    projectile->data.id = pthread_self();

    while(projectile->data.isOnLog){
        usleep(1000);
    }

    while (projectile->data.lives > 0){
        pthread_mutex_lock(&mutex);  
        switch(projectile->data.dir){
           
            case N:              
                projectile->data.box.topLeft.y--;
                projectile->data.box.botRight.y--;
                projectile->data.dir = N;
            break;
            case S:
                projectile->data.box.topLeft.y++;
                projectile->data.box.botRight.y++;
                projectile->data.dir = S;                          
            break;
        }
        if(projectile->data.box.topLeft.y <= 1 || projectile->data.box.topLeft.y >= MAXY-1){
            projectile->data.lives = 0;
        }
        pthread_mutex_unlock(&mutex);   
        usleep(50000);
    }

    return NULL;
}

int spitballCollisions(Entity spit)
{
    int i,j;

    for(i = 0; i < NUM_LANES; i++){
        for(j = 0; j < NUM_CARS; j++){
            if(verifyHitbox(spit.box, game.carTable[i][j].box)){
                return 1;
            }
        }
    }

    for(i = 0; i < NUM_LOGS; i++){
        if(verifyHitbox(spit.box, game.spiders[i].box) && spit.dir == N){
            game.spiders[i].lives = 0;
            return 2;
        }
    }

    if(verifyHitbox(spit.box,game.player.box) && spit.dir == S){
        return 3;
    }

    return 0;
}

// game --------------------------------

void initializeData(_Bool dRegister[])
{
    int i,j;

    game.gameWin = newwin(MAXY,MAXX,0,0);
    game.statWin = newwin(5,MAXX,MAXY,0);
    game.projectilePtr = initEntityList();

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

    wattrset(game.gameWin,COLOR_PAIR(SAFE_ZONE));
    wattrset(game.gameWin,COLOR_PAIR(ROAD));
    wattrset(game.gameWin,COLOR_PAIR(WATER));
    wattrset(game.gameWin,COLOR_PAIR(LOGS));
    wattrset(game.gameWin,COLOR_PAIR(TRUCKS));
    wattrset(game.gameWin,COLOR_PAIR(CARS));
    wattrset(game.gameWin,COLOR_PAIR(PHROG_ON_ROAD));
    wattrset(game.gameWin,COLOR_PAIR(PHROG_ON_GRASS));
    wattrset(game.gameWin,COLOR_PAIR(PHROG_ON_LOG));
    wattrset(game.gameWin,COLOR_PAIR(SPIT_ON_ROAD));
    wattrset(game.gameWin,COLOR_PAIR(SPIT_ON_GRASS));
    wattrset(game.gameWin,COLOR_PAIR(SPIT_ON_LOG));
    wattrset(game.gameWin,COLOR_PAIR(SPIDER));

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
    entityNode* iter;

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

        for(i = 0; i < NUM_LOGS; i++){
            bodyClearing(game.logs[i],game.gameWin);
            printerLogs(game.logs[i],game.gameWin);
            if(game.logs[i].hasSpider){
                bodyClearing(game.spiders[i],game.gameWin);
                printerSingleEntities(game.spiders[i],game.gameWin);
            }
            if(game.player.row-1 == game.logs[i].row){
                bodyClearing(game.player,game.gameWin);
                printerSingleEntities(game.player,game.gameWin);
            }
        }

        if(game.player.row == 0){
            denId = denCollisions();
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

            if(!playerIsDry)
            {
                playerHit = true;
            }
            else
            {
                for(i = 0; i<NUM_LOGS; i++){
                    if(game.logs[i].row != game.player.row-1){                      
                        game.logs[i].isOnLog = false;
                    }
                }
            }
        }

        if(playerHit)
        {
            result = OUCH;
            break;
        }
        else
        {

            // ciclo stampa macchine
            for(i = 0; i < NUM_LANES; i++){
                for(j = 0; j < NUM_CARS; j++){
                    // controllo rane investite
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

            // ciclo stampa proiettili
            if(game.projectilePtr != NULL && !playerHit){
                iter = game.projectilePtr->head;
                while(iter != NULL){
                    bodyClearing(iter->data,game.gameWin);
                    if(iter->data.lives <= 0)
                    {
                        game.projectilePtr = eraseEntity(iter,game.projectilePtr);
                    }
                    
                    else if(iter->data.isOnLog)
                    {
                        iter->data.isOnLog = false; //assicura che quando i proiettili siano pronti, non interferiscano con il lock preesistente
                    }
                    
                    else
                    {
                        // controllo sputi, quattro opzioni:
                        // 0: nessuna collisione
                        // 1: collide con una macchina
                        // 2: collide con un ragno, solo proiettili rossi
                        // 3: collide con la rana, solo proiettili gialli-viola
                        resultOfSpitCollision = spitballCollisions(iter->data);
                        switch(resultOfSpitCollision){
                            // no collision
                            case 0:
                                printerSingleEntities(iter->data,game.gameWin);
                            break;
                            // collision w/ car
                            case 1:
                                fprintf(debugLog,"spitball hit the car \n");
                                iter->data.lives = 0;                         
                            break;
                            //collision w/ spider
                            case 2:
                                fprintf(debugLog,"spitball killed a spider \n");
                                iter->data.lives = 0;
                                for(i = 0; i<NUM_LOGS; i++) {
                                    if (game.spiders[i].lives <= 0) {
                                        for (j = 0; j < NUM_LOGS; j++) {
                                            if (game.spiders[i].row == game.logs[j].row) {
                                                game.logs[i].hasSpider = false;
                                                bodyClearing(game.spiders[i], game.gameWin);
                                                fprintf(debugLog, "for real \n");
                                                break;
                                            }
                                        }
                                    }
                                }
                            break;
                            // collision w/ player
                            case 3:
                                fprintf(debugLog,"spitball killed the player \n");
                                iter->data.lives = 0;
                                bodyClearing(game.player,game.gameWin);
                                playerHit = true;
                            break;
                        }
                    }
                    
                    iter = iter->next;                    
                }

                if(playerHit)
                {
                    result = OUCH;
                    break;
                }
            }
        }

        //mvprintw(1,MAXX+1,"player x: %d, y: %d, dir:",game.player.box.topLeft.x,game.player.box.topLeft.y);
        //translateDirection(game.player.dir);
        printerSingleEntities(game.player,game.gameWin);

        mvwprintw(game.statWin,1,1,"[·phrog lives:%d·]\n", game.player.lives);
        mvwprintw(game.statWin,2,1,"[·time left:%d·]\n", timeRemaining);
        screenRefresh();

        pthread_mutex_unlock(&mutex);
    }while(!playerHit && !denReached && timeSpent <= TIMER);


    if(playerHit || timeSpent >= TIMER){
        result = OUCH;
    }

    if(denReached){
        result = denId;
    }
    
    werase(game.gameWin);
    werase(game.statWin);
    screenRefresh();
    clearMemory();
    fclose(debugLog);

    return result;
}

void drawMap()
{
    int i,j;
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

void swap(Entity *a, Entity *b) 
{ 
  Entity temp = *a;
  *a = *b;
  *b = temp;
}

int gameStart(int startingLives, _Bool dRegister[])
{

    int* lives = malloc(sizeof(startingLives));
    *lives = startingLives;
    int result = 0;

    pthread_t plThread, logThread, carThread;

    pthread_mutex_init(&mutex,NULL);

    mvprintw(2,MAXX+1,"starting x: %d, lives: %d, dir:",startingLives,*lives);

    pthread_create(&plThread,NULL,&phrog,(void*)lives);

    if(game.alreadyStarted != true){	// Se e' la prima manche avvia i threads dei log e macchine, altrimenti no
        pthread_create(&logThread,NULL,&logGenerator,NULL);
        pthread_create(&carThread,NULL,&carGenerator,NULL);
        game.alreadyStarted = true;
    }

    result = roadsAndPonds(dRegister);

    if(game.player.lives <= 0){
        game.alreadyStarted = false;
    }

    pthread_mutex_destroy(&mutex);
    return result;
}

void clearMemory()
{
    int i;
    entityNode* iter;

    for(i = 0; i<NUM_LOGS; i++){
        game.spiders[i].lives = 0;
    }

    if(game.projectilePtr != NULL){
        iter = game.projectilePtr->head;
        while(iter != NULL){
            bodyClearing(iter->data,game.gameWin);
            iter->data.lives = 0;  
            iter = iter->next;   
        }
    }

    game.gameReady = false;
}

entityList* initEntityList()
{
    entityList *list = malloc(sizeof(entityList));
    list->head = NULL;
    list->tail = NULL;
    list-> len = 0;
    return list;
}

entityNode *insert(Entity data, entityList *list)
{
    entityNode *ent;
    ent = malloc(sizeof(entityNode));

    ent->next = NULL;
    ent->prev = NULL;

    ent->data = data;

    if (list->head == NULL) 
    {
        list->head = ent;
        list->tail = ent;
        ent->prev = NULL;
        ent->next = NULL;
    }
    else
    {
        // Setting ent data
        ent->prev= list->tail;
        ent->next = NULL;

        // Setting list data
        list->tail->next = ent;
        list->tail = ent;
    }

    list->len +=1;

    return ent;
}

entityList* eraseEntity(entityNode *forDeletion, entityList *list)
{

    entityNode *ent;

    if (list->head != NULL) // If the list is not empty
    {
        ent = list->head;
        while (ent != NULL)
        {
            if (ent == forDeletion)
            {
                entityNode *predecessor, *successor;
                predecessor = ent->prev;
                successor = ent->next;
                
                if (list->head != list->tail)
                {
                    if (ent != list->head)
                    {
                        if (ent != list->tail)
                        {
                            predecessor->next = successor;
                            successor->prev = predecessor;
                        }
                        else
                        {
                            predecessor->next = NULL;
                            list->tail = predecessor;
                        }
                    }
                    else
                    {
                        list->head = successor;
                        successor->prev = NULL;
                    }
                }
                else
                {  
                    list->head = NULL;
                    list->tail = NULL;
                }
            } 
            ent = ent->next;         
        }
    }

    list->len -=1;

    return list;
}
