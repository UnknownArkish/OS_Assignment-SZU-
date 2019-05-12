main: main.o Tool.o
	gcc -o main main.o Tool.o

main.o: main.c Tool.h Config.h
	gcc -std=c99 -c main.c
Tool.o: Tool.c
	gcc -c Tool.c
clean: 
	rm main.o Tool.o