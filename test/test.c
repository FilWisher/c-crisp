#include <stdio.h>
#include <assert.h>

#include "test.h"

int check(int p, char *msg) {
  if (p) printf("ok: %s\n", msg);
  else printf("FAIL: %s\n", msg);
  return p;
}

int main() {
  simple_token_test();
  tokenize_test();
  
  simple_atom_test();
}
