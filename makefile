processP: main.c gameLib.c
	gcc -g -o processP main.c gameLib.c -lncurses -I. 

clean:
	rm *o processP