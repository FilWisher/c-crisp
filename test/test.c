#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../src/lisp.h"
#include "test.h"

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
}
