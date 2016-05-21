MKSHELL=$PLAN9/bin/rc

cc = gcc
src = src/lex.c src/parse.c
obj = ${src:%.c=%.o}
test = test/test.c test/lex_test.c test/parse_test.c

all: repl runtest 

src/%.o: src/%.c
	$cc -c $CFLAGS src/$stem.c -o src/$stem.o

repl: $obj
	$cc $obj src/repl.c -o repl
	
test: $obj $test
	$cc $obj $test -o test/test
	
runtest: test
	test/test
	
clean:
	rm -rf src/*.o test/test 
