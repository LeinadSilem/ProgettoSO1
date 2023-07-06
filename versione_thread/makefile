threadP: main_t.o gameLib_t.o entities_t.o
	gcc main_t.o gameLib_t.o entities_t.o -o succ -lncurses -lpthread

main_t.o: main_t.c 
	gcc -c main_t.c

gameLib_t.o: gameLib_t.c gameLib_t.h 
	gcc -c gameLib_t.c -lncurses

entities_t.o: entities_t.c entities_t.h
	gcc -c entities_t.c -lncurses

clean:
	rm -r *o