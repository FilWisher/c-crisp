#include <stdio.h>
#include <string.h>

#include "../src/lisp.h"
#include "test.h"

void simple_atom_test() {
  atom *a = atom_make(A_SYMBOL, "hello");
  check(strcmp(a->val, "hello") == 0, "correct val");
  check(a->typ == A_SYMBOL, "correct typ");
  atom_free(a);
}
