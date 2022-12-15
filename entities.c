#include "entities.h"

char phrogBody[PHROG_SIZE][PHROG_SIZE] = {
    ",_,",
    "(0)",
    "' '"
};

char spiderBody[PHROG_SIZE][PHROG_SIZE] = {
	"\\ /",
	"[O]",
	"/ \\"
};

char carLeftFour[PHROG_SIZE][4] = {
    "o--o",
    "|CK|",
    "o--o"
};

char carRightFour[PHROG_SIZE][4] = {
    "o--o",
    "|HD|",
    "o--o"
};

char carLeftFive[PHROG_SIZE][5] = {
    "o---o",
    "|C K|",
    "o---o"
};

char carRightFive[PHROG_SIZE][5] = {
    "o---o",
    "|H D|",
    "o---o"
};

char carLeftSix[PHROG_SIZE][6] = {
	"o----o",
    "|C  K|",
    "o----o"
};

char carRightSix[PHROG_SIZE][6] = {
    "o----o",
    "|H  D|",
    "o----o"
};

char carLeftSeven[PHROG_SIZE][7] = {
    "o-----o",
    "|C   K|",
    "o-----o"
};

char carRightSeven[PHROG_SIZE][7] = {
    "o-----o",
    "|H   D|",
    "o-----o"
};

char carLeftEight[PHROG_SIZE][8] = {
	"o------o",
    "|C    K|",
    "o------o"
};

char carRightEight[PHROG_SIZE][8] = {
    "o------o",
    "|H    D|",
    "o------o"
};

char carLeftNine[PHROG_SIZE][9] = {
    "o-------o",
    "|C     K|",
    "o-------o"
};

char carRightNine[PHROG_SIZE][9] = {
    "o-------o",
    "|H     D|",
    "o-------o"
};

char carLeftTen[PHROG_SIZE][10] = {
    "o--------o",
    "|C      K|",
    "o--------o"
};

char carRightTen[PHROG_SIZE][10] = {
    "o--------o",
    "|H      D|",
    "o--------o"
};

char logNine[PHROG_SIZE][9] = {
    "o-------o",
    "(=======)",
    "o-------o"
};

char logSix[PHROG_SIZE][6] = {
    "o----o",
    "(====)",
    "o----o"
};

void printerCars(Entity ent)
{
	int i,j,y,x;

	y = ent.box.topLeft.y;
    x = ent.box.topLeft.x;

    attron(COLOR_PAIR(ent.color));
    if(ent.dir == W){
    	switch(ent.length){
    		case 4:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carLeftFour[i][j]);
			        }
			    }
    		break;

    		case 5:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carLeftFive[i][j]);
			        }
			    }
    		break;

    		case 6:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carLeftSix[i][j]);
			        }
			    }
    		break;

    		case 7:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carLeftSeven[i][j]);
			        }
			    }
    		break;

    		case 8:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carLeftEight[i][j]);
			        }
			    }
    		break;

    		case 9:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carLeftNine[i][j]);
			        }
			    }
    		break;

    		case 10:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carLeftTen[i][j]);
			        }
			    }
    		break;
    	}
    }else{
    	switch(ent.length){
    		case 4:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carRightFour[i][j]);
			        }
			    }
    		break;

    		case 5:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carRightFive[i][j]);
			        }
			    }
    		break;

    		case 6:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carRightSix[i][j]);
			        }
			    }
    		break;

    		case 7:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carRightSeven[i][j]);
			        }
			    }
    		break;

    		case 8:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carRightEight[i][j]);
			        }
			    }
    		break;

    		case 9:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carRightNine[i][j]);
			        }
			    }
    		break;

    		case 10:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvaddch(y+i,x+j,carRightTen[i][j]);
			        }
			    }
    		break;
    	}
    }
    attroff(COLOR_PAIR(ent.color));
}

void printerLogs(Entity ent){

	int i,j,y,x;

	y = ent.box.topLeft.y;
    x = ent.box.topLeft.x;

    attron(COLOR_PAIR(ent.color));
	if(ent.length == 6){
		for(i = 0; i < PHROG_SIZE; i++) {
	        for(j = 0; j < ent.length; j++) {
	            mvaddch(y+i,x+j,logSix[i][j]);
	        }
	    }
	}else{
		for(i = 0; i < PHROG_SIZE; i++) {
	        for(j = 0; j < ent.length; j++) {
	            mvaddch(y+i,x+j,logNine[i][j]);
	        }
	    }
	}
	attroff(COLOR_PAIR(ent.color));
}

void printerSingleEntities(Entity ent)
{
	int i,j,y,x;

    y = ent.box.topLeft.y;
    x = ent.box.topLeft.x;

    // individuazione entità...
    attron(COLOR_PAIR(ent.color));
    switch(ent.et) {
	    // giocatore
	    case PHROG:
		    for(i = 0; i < PHROG_SIZE; i++) {
		        for(j = 0; j < PHROG_SIZE; j++) {
		            mvaddch(y+i,x+j, phrogBody[i][j]);
		        }
		    }
	    break;

	    case SPIDER:
	    break;

	    case SPITBALL:
	    break;
    }
    attroff(COLOR_PAIR(ent.color));
}

void bodyClearing(Entity ent)
{
    int i,j,size,x,y;

    y = ent.box.topLeft.y;
    x = ent.box.topLeft.x;

    if(ent.et == LOG || ent.et == CAR){
    	size = ent.length;
    }else if(ent.et == SPIDER || ent.et == PHROG){
    	size = PHROG_SIZE;
    }else{
    	size = 1;
    }

    for(i = 0; i < PHROG_SIZE; i++){
        for(j = 0 ; j < size ; j++){
            mvaddch(y+i,x+j,' ');
        }
    }
}