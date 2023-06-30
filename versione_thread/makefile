threadP: main.o gameLib.o entities.o
	gcc main.o gameLib.o entities.o -o succ -lncurses -lpthread

main.o: main.c 
	gcc -c main.c

gameLib.o: gameLib.c gameLib.h 
	gcc -c gameLib.c -lncurses

entities.o: entities.c entities.h
	gcc -c entities.c -lncurses

clean:
	rm -r *o