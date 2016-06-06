#include <stdio.h>

#include "../src/lisp.h"
#include "test.h"


void simple_env_test() {
  
  printf("\n");
  env *e = env_make(NULL, 8);
  env_bind(e, "hello", atom_make(A_SYMBOL, "hi"));
  
  atom *a = env_lookup(e, "hello");
  check(equal(a->val, "hi"), "correct atom retrieved");
  
  env *e1 = env_make(e, 8);
  env_bind(e1, "bye", atom_make(A_SYMBOL, "cool"));
  a = env_lookup(e1, "bye");
  check(equal(a->val, "cool"), "correct atom retrieved");
  a = env_lookup(e1, "hello");
  check(equal(a->val, "hi"), "correct atom retrieved from parent");
  
  a = env_lookup(e1, "squigglyjoe");
  check(a->typ == A_ERROR, "error returned");
}

void simple_eval_test() {
  
  env *e = env_make(NULL, 8);
  atom *a = eval(e, atom_make(A_NUMBER, "89"));
  check(a->typ == A_NUMBER, "number evals to itself");
  check(equal(a->val, "89"), "number has correct value");
  
  a = eval(e, atom_make(A_STRING, "'foo'"));
  check(a->typ == A_STRING, "string evals to itself");
  check(equal(a->val, "'foo'"), "string has correct value");
 
  env_bind(e, "egg", atom_make(A_NUMBER, "99"));
  a = eval(e, atom_make(A_SYMBOL, "egg"));
  check(a->typ == A_NUMBER, "symbol looked up in env");
  check(equal(a->val, "99"), "correct atom retrieved");
}

void eval_test() {
  
  env *e = env_make(NULL, 8);
  atom *fn = make_fn(e, parse("(hello)"), parse("hello"));
  atom *a = apply(fn, parse("(101)"));  
  check(a->typ == A_NUMBER, "atom has correct typ");
  check(equal(a->val, "101"), "atom has correct val");
  
  fn = make_fn(e, parse("(hello friend)"), parse("friend"));
  a = apply(fn, parse("(101 777)"));  
  check(a->typ == A_NUMBER, "atom has correct typ");
  check(equal(a->val, "777"), "atom has correct val");
  
  
  printf("val: %s\n", a->val);

  check(1==1, "true");
}
