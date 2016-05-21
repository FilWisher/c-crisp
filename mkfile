all: test runtest

lisp.o: lisp.c lisp.h
	gcc -c lisp.c -o lisp.o

test: test/test.c lisp.o
	gcc test/test.c lisp.o -o test/test

main: main.c lisp.o
	gcc main.c lisp.o -o main

runtest:
	test/test

clean: main lisp.o test/test
	rm -rf main lisp.o test/test
