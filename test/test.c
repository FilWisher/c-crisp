#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../src/lisp.h"
#include "test.h"

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
  case A_ENV:
    printf("<#env>");
    break;
  default:
    printf("wat");
  }
}

int check(int p, char *msg) {
  if (p) printf("ok: %s\n", msg);
  else printf("FAIL: %s\n", msg);
  return p;
}

int equal(char *a, char *b) {
  return strcmp(a, b) == 0;
}

int equal_list(atom *a, atom *b) {  
  // TODO: compare lists
}

int main() {
  simple_token_test();
  tokenize_test();
  
  simple_atom_test();
  parse_test();

  simple_env_test();
  simple_eval_test();
  eval_test();
}
