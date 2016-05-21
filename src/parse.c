#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lisp.h"

atom *atom_make(enum a_typ typ, char *val) {
  atom *a = (atom *)malloc(sizeof(atom));
  a->typ = typ;
  a->val = strdup(val);
  return a;
}

void atom_free(atom *a) {
  free(a->val);
  free(a);
}

atom *parse() {}
