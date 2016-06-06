cc = gcc
src = src/lex.c src/parse.c src/eval.c src/core.c
obj = ${src:%.c=%.o}
test = test/test.c test/lex_test.c test/parse_test.c test/eval_test.c

all: repl runtest 

src/%.o: src/%.c
	$cc -c $CFLAGS src/$stem.c -o src/$stem.o

repl: $obj src/repl.c
	$cc $obj src/repl.c -o repl
	
test: $obj $test
	$cc $obj $test -o test/test
	
runtest: test
	test/test
	
clean:
	rm -rf src/*.o test/test repl
