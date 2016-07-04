#include <stdio.h>
#include "../src/lisp.h"

#include "test.h"

atom *atom_number(int n) {
  atom *a = atom_make(A_NUMBER, "");
  a->number = n;
  return a;
}

void test_int(env *e, char *p, int val) {
  char *msg[128];
  atom *a = eval(e, parse(p));
  sprintf(msg, "%s evaluates to number", p);
  check(a->typ == A_NUMBER, msg);
  sprintf(msg, "%s evaluates to %d", p, val);
  check(a->number == val, msg);
}

env *prepare_env() {
  env *e = env_make(NULL, 8);
  load_core(e);
  return e;
}

void core_add() {
  env *e = prepare_env();
  test_int(e, "(+ 1 1)", 2);
  test_int(e, "(+ 1 1 1)", 3);
  test_int(e, "(+ 1 (+ 2 3))", 6);
  test_int(e, "(+ (+ 2 3) (+ 4 5)))", 14);
}

void core_sub() {
  env *e = prepare_env();
  test_int(e, "(- 1 1)", 0);
  test_int(e, "(- 4 (- 2 1))", 3);
  test_int(e, "(- (- 1 4) (- 4 2))", -5);
}

void core_mul() {
  env *e = prepare_env();
  test_int(e, "(* 2 3)", 6);
  test_int(e, "(* 2 3 4)", 24);
  test_int(e, "(* (* 2 1) (* 1 3))", 6);
  test_int(e, "(* (* 2 1) (* 1 3) (* 2 2))", 24);
}

void core_div() {
  env *e = prepare_env();
  test_int(e, "(/ 3 1)", 3);
  test_int(e, "(/ 10 2)", 5);
  test_int(e, "(/ (/ 20 2) (/ 9 3))", 3);
  
  atom *exp = eval(e, parse("(/ 2 0)"));
  check(exp->typ == A_ERROR, "divide by 0 returns error");
}

void core_defun() {
  env *e = prepare_env();
  atom *exp = eval(e, parse("(defun hello (x) (+ 2 2))"));
  test_int(e, "(hello 8)", 4);
  exp = eval(e, parse("(defun cool () 10)"));
  test_int(e, "(cool)", 10);
  exp = eval(e, parse("(defun cool (x y z) (+ x y z))"));
  test_int(e, "(cool 1 2 3)", 6);
}

void core_test() {
  core_add();
  core_sub();
  core_mul();
  core_div();
  core_defun();  
}
