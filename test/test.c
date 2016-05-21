#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "../lisp.h"

#define NUM_TESTS (20)

int asserts = 0;
int tests = 0;

void check(int t) {
  asserts += 1;
  assert(t);
}

int test_tokenize() {
  tests += 1;
  Stack *s = tokenize("cool wow pow");
  stack_print(s);
  fputs("\n", stdout);
  s = tokenize("(cool wow pow)");
  stack_print(s);
  fputs("\n", stdout);
  s = tokenize("(cool (wow (pow)))");
  stack_print(s);
}

int test_parse() {
  tests += 1;
   
  fputs("\nTEST_PARSE\n", stdout);
  Cell *ast = parse("(hello my name (is wil))");
  check(ast != NULL);
  cell_print(ast);
 
  fputs("\n", stdout);
 
  ast = parse("(add 22 22)");
  check(ast != NULL);
  cell_print(ast);
  
  fputs("\n", stdout);
  
  ast = parse("(add \"22\" '22')");
  check(ast != NULL);
  cell_print(ast);
}

int main() {
  test_tokenize();
  test_parse();
  printf("\n\nall %d assertions passed in %d tests\n", asserts, tests);
}
