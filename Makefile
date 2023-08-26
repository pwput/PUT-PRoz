SOURCES=$(wildcard *.c)
HEADERS=$(SOURCES:.c=.h)
#FLAGS=-DDEBUG -g
FLAGS=-g

all:
	clear
	make clean
	make main
	make run

main:
	mpic++ ./main.cpp -o main -g

clear: clean

clean:
	rm -f main

run: main
	mpirun -oversubscribe -np 6 ./main



