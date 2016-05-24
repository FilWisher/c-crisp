#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

atom *atom_err(char *e) {
  return atom_make(A_ERROR, e);
}

atom *atom_num(char *n) {
  atom *a = atom_make(A_NUMBER, n);
  a->number = atoi(n);
  return a;
}

atom *parse_atom(token *t) {
  // TODO: parse all types of atoms
  enum a_typ typ = A_NUMBER;
  for (char *cp = t->val; *cp; cp++) {
    if (!isdigit(*cp)) {
      return atom_make(A_SYMBOL, t->val);
    }
  }
  return atom_num(t->val);
}

atom *parse_list(token *t) {
  return atom_make(A_PAIR, "fairgroupnd");
}

atom *parse(char *input) {
  token *t = tokenize(input);
  if (!t) return atom_err("Parse error");
  
  while (t) { 
    switch(t->typ) {
    case TK_OPEN:
      return parse_list(t);
    case TK_SYMBOL:
      return parse_atom(t); 
    default:
      return atom_err("unexpected token");
    }
  }
}
