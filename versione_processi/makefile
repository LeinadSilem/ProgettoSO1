processP: main_p.o gameLib_p.o entities_p.o
	gcc main_p.o gameLib_p.o entities_p.o -o succ -lncurses

main_p.o: main_p.c 
	gcc -c main_p.c

gameLib_p.o: gameLib_p.c gameLib_p.h 
	gcc -c gameLib_p.c -lncurses

entities_p.o: entities_p.c entities_p.h
	gcc -c entities_p.c -lncurses

clean:
	rm -r *o