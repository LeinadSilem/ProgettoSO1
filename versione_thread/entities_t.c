#include "entities_t.h"

char phrogBody[PHROG_SIZE][PHROG_SIZE] = {
    "\\_/",
    "(0)",
    "/ \\"
};

char spiderBody[PHROG_SIZE] = {")O("};

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

char logTEST[PHROG_SIZE][18] = {
    "o----------------o",
    "(================)",
    "o----------------o"
};

void printerCars(Entity ent, WINDOW* win)
{
	int i,j,y,x;

	y = ent.box.topLeft.y;
    x = ent.box.topLeft.x;

    wattron(win,COLOR_PAIR(ent.color));
    if(ent.dir == W){
    	switch(ent.length){
    		case 4:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carLeftFour[i][j]);
			        }
			    }
    		break;

    		case 5:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carLeftFive[i][j]);
			        }
			    }
    		break;

    		case 6:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carLeftSix[i][j]);
			        }
			    }
    		break;

    		case 7:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carLeftSeven[i][j]);
			        }
			    }
    		break;

    		case 8:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carLeftEight[i][j]);
			        }
			    }
    		break;

    		case 9:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carLeftNine[i][j]);
			        }
			    }
    		break;

    		case 10:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carLeftTen[i][j]);
			        }
			    }
    		break;
    	}
    }else{
    	switch(ent.length){
    		case 4:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carRightFour[i][j]);
			        }
			    }
    		break;

    		case 5:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carRightFive[i][j]);
			        }
			    }
    		break;

    		case 6:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carRightSix[i][j]);
			        }
			    }
    		break;

    		case 7:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carRightSeven[i][j]);
			        }
			    }
    		break;

    		case 8:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carRightEight[i][j]);
			        }
			    }
    		break;

    		case 9:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carRightNine[i][j]);
			        }
			    }
    		break;

    		case 10:
    			for(i = 0; i < PHROG_SIZE; i++) {
			        for(j = 0; j < ent.length; j++) {
			            mvwaddch(win,y+i,x+j,carRightTen[i][j]);
			        }
			    }
    		break;
    	}
    }
    wattroff(win,COLOR_PAIR(ent.color));
}

void printerLogs(Entity ent, WINDOW* win)
{
	int i,j,y,x;

	y = ent.box.topLeft.y;
    x = ent.box.topLeft.x;

    wattron(win,COLOR_PAIR(ent.color));
	if(ent.length == 6){
		for(i = 0; i < PHROG_SIZE; i++) {
	        for(j = 0; j < ent.length; j++) {
#if TESTING == 0
	            mvwaddch(win,y+i,x+j,logSix[i][j]);
#else
	            mvwaddch(win,y+i,x+j,logTEST[i][j]);
#endif
	        }
	    }
	}else{
		for(i = 0; i < PHROG_SIZE; i++) {
	        for(j = 0; j < ent.length; j++) {
#if TESTING == 0
	            mvwaddch(win,y+i,x+j,logNine[i][j]);
#else
	            mvwaddch(win,y+i,x+j,logTEST[i][j]);
#endif
	        }
	    }
	}
	wattroff(win,COLOR_PAIR(ent.color));
}

void printerSingleEntities(Entity ent, WINDOW* win)
{
	int i,j,y,x;

    y = ent.box.topLeft.y;
    x = ent.box.topLeft.x;

    // individuazione entità...
    wattron(win,COLOR_PAIR(ent.color));
    switch(ent.et) {
	    // giocatore
	    case PHROG:
		    for(i = 0; i < PHROG_SIZE; i++) {
		        for(j = 0; j < PHROG_SIZE; j++) {
		            mvwaddch(win,y+i,x+j, phrogBody[i][j]);
		        }
		    }
	    break;

	    case SPIDER:
	    	for(j = 0; j < PHROG_SIZE; j++) {
	            mvwaddch(win,y,x+j, spiderBody[j]);
	        }
	    break;

	    case SPITBALL:
	    	mvwaddch(win,y,x,'*');
	    break;
    }
    wattroff(win,COLOR_PAIR(ent.color));
}

void bodyClearing(Entity ent, WINDOW* win)
{
    int i,j,x,y;

    y = ent.box.topLeft.y;
    x = ent.box.topLeft.x;

    for(i = 0; i < (ent.box.botRight.y - ent.box.topLeft.y +1); i++){
        for(j = 0 ; j < (ent.box.botRight.x - ent.box.topLeft.x +1) ; j++){
            mvwaddch(win,y+i,x+j,' ');
        }
    }
}

void bodyClearingSingleEntities(Entity ent, WINDOW* win)
{
    int i,j,x,y,adjx,adjy;

    y = ent.box.topLeft.y;
    x = ent.box.topLeft.x;

    switch(ent.dir){
    	case N:
    		adjy = 1;
    	break;

    	case S:
    		adjy = -1;
    	break;

    	case W:
    		adjx = 1;
    	break;

    	case E:
    		adjx = -1;
    	break;
    }

    if(ent.et == SPITBALL){
    	 mvwaddch(win,y+adjy,x,' ');
	}else {
		for(i = 0; i <(ent.box.botRight.y - ent.box.topLeft.y + 1); i++){
	        for(j = 0 ; j < (ent.box.botRight.x - ent.box.topLeft.x + 1); j++){
	            mvwaddch(win,y+i+adjy,x+j+adjx,' ');
	        }
	    }
	}  
}

_Bool verifyHitbox(Hitbox a, Hitbox b)
{
    if(((a.topLeft.x >= b.topLeft.x && a.topLeft.x <= b.botRight.x) || (a.botRight.x >= b.topLeft.x && a.botRight.x <= b.botRight.x)) &&
       ((a.topLeft.y >= b.topLeft.y && a.topLeft.y <= b.botRight.y) || (a.botRight.y >= b.topLeft.y && a.botRight.y <= b.botRight.y))) {
        return true;
    } else {
        return false;
    }
}