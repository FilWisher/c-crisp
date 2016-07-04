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

atom *pair_make(atom *car, atom *cdr) {
  atom *a = atom_make(A_PAIR, "");
  car(a) = car;
  cdr(a) = atom_make(A_PAIR, "");
  car(cdr(a)) = cdr;
  cdr(cdr(a)) = atom_make(A_NIL, "");
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

parser *parser_make() {
  parser *p = (parser *)malloc(sizeof(parser));
  p->tks = p->tp = NULL;
  return p;
}

atom *parse_symbol(parser *p) {
  // TODO: parse all types of atoms
  enum a_typ typ = A_NUMBER;
  for (char *cp = p->tp->val; *cp; cp++) {
    if (!isdigit(*cp)) {
      return atom_make(A_SYMBOL, p->tp->val);
    }
  }
  return atom_num(p->tp->val);
}

atom *parse_list(parser *p) {
  p->tp = p->tp->next;
  if (p->tp->typ == TK_CLOSE) return atom_make(A_NIL, "");
  atom *h = atom_make(A_PAIR, "");
  atom **ap = &h;
  for (; p->tp->typ != TK_CLOSE; p->tp = p->tp->next) {
    switch (p->tp->typ) {
    case TK_OPEN:
      (*ap)->pair.car = parse_list(p);
      break;
    case TK_SYMBOL:
      (*ap)->pair.car = parse_symbol(p);
      break;
    }
    ap = &(*ap)->pair.cdr;
    if (p->tp->next->typ != TK_CLOSE) (*ap) = atom_make(A_PAIR, "");
  }
  (*ap) = atom_make(A_NIL, "");
  return h;
}

atom *parse_switch(parser *p) {
  switch(p->tp->typ) {
  case TK_OPEN:
    return parse_list(p);
  case TK_SYMBOL:
    return parse_symbol(p); 
  case TK_CLOSE:
    return atom_make(A_NIL, "");
  case TK_STRING:
    return atom_make(A_STRING, p->tp->val);
  default:
    return atom_err("unexpected token");
  }
}

atom *parse(char *input) {
  parser *p = parser_make();
  p->tks = p->tp = tokenize(input);
  if (!p->tp) return atom_err("Parse error");
 
  return parse_switch(p);
}
