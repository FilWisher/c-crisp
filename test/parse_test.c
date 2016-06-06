#include <stdio.h>
#include <string.h>

#include "../src/lisp.h"
#include "test.h"

void simple_atom_test() {
  atom *a = atom_make(A_SYMBOL, "hello");
  check(equal(a->val, "hello"), "correct val");
  check(a->typ == A_SYMBOL, "correct typ");
  atom_free(a);
}

void atom_print(atom *a) {

  switch(a->typ) {
  case A_SYMBOL:
    printf("%s ", a->val);
    break;
  case A_NUMBER:
    printf("<%d> ", a->number);
    break;
  case A_PAIR:
    printf("(");
    atom_print(a->pair.car);
    printf(" . ");
    atom_print(a->pair.cdr);
    printf(")");
    break;
  case A_NIL:
    printf("nil");
    break;
  }
}

void parse_test() {
  atom *a = parse("");
  check(a->typ == A_ERROR, "returns error");
  
  a = parse("hello");
  check(a->typ == A_SYMBOL, "returns symbol");
  check(equal(a->val, "hello"), "correct symbol");
  
  a = parse("84251");
  check(a->typ == A_NUMBER, "returns number");
  check(a->number == 84251, "correct number");
  
  a = parse("(hello)");
  check(a->typ == A_PAIR, "(hello) returns pair");
  check(equal(a->pair.car->val, "hello"), "correct symbol");
  
  a = parse("(hello there)");
  check(a->typ == A_PAIR, "(hello there) returns pair");
  check(equal(a->pair.car->val, "hello"), "correct symbol");
  
  a = parse("(hello there (goodbye 88 friend))");
  check(a->typ == A_PAIR, "(hello there (goodbye friend)) returns pair");
  check(equal(a->pair.car->val, "hello"), "correct symbol");
  
  atom_print(a);
}
