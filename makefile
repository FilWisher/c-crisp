all: main lisp

env.o: env.c env.h

atom.o: atom.c atom.h

main: main.c env.o atom.o
	gcc main.c env.o atom.o -o main
