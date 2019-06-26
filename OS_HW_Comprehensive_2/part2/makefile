
main: main.o util.o
	gcc -o main main.o util.o -lm

main.o: main.c
	gcc -std=c99 -c main.c
util.o: util.c
	gcc -std=c99 -c util.c
clean: 
	rm main.o util.o