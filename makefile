processP: main.c gameLib.c
	gcc -g -o succ main.c gameLib.c -lncurses -I. 

clean:
	rm *o succ