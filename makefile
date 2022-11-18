SDT: main.o gameLib.o
	gcc main.o gameLib.o -o SDP -lncurses

main.o: main.c
	gcc -c main.c

gamelib.o: gameLib.c gameLib.h
	gcc -c gameLib.c -lncurses

clean:
	rm *o SDP