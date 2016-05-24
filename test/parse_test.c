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
  check(a->typ == A_PAIR, "returns pair");
  
   
}
