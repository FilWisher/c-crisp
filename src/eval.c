#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lisp.h"

env_entry *env_entry_make(char *k, atom *v, env_entry *next) {
  env_entry *ent = (env_entry *)malloc(sizeof(env_entry));
  ent->k = strdup(k);
  ent->v = v;
  ent->next = next;
  return ent;
}

// TODO: increase size of env
env *env_make(env *parent, int cap) {
  env *e = (env *)malloc(sizeof(env));
  e->parent = parent;
  e->table = (env_entry **)malloc(sizeof(env_entry *) * cap);
  e->n = 0;
  e->cap = cap;
  return e;
}

unsigned int hash(char *k) {
  // TODO: implement fnv-1 algorithm
  unsigned int i = 0;
  for (char *cp = k; *cp != '\0'; cp++) {
    i += (unsigned int)*cp;
  }
  return i;
}

void *env_bind(env *e, char *k, atom *v) {
  // TODO: if load balance >= 1, copy table over
  unsigned int i = hash(k) % e->cap;
  *(e->table + i) = env_entry_make(k, v, *(e->table + i));
}
atom *env_lookup(env *e, char *k) {
  unsigned int i = hash(k);
  env_entry *ent = NULL;
  while (e) {
    ent = *(e->table + (i%e->cap));
    while (ent != NULL) {
      if (strcmp(ent->k, k) == 0) return ent->v;
      else ent = ent->next;
    }
    e = e->parent; 
  }
  return atom_make(A_ERROR, "Couldn't find it");
}

int equal (char *a, char *b) { return strcmp(a, b) == 0; }

atom *eval(env *e, atom *exp) {

  atom *op;
  switch (exp->typ) {
  case A_NUMBER:
  case A_STRING:
    return exp;
  case A_SYMBOL:
    return env_lookup(e, exp->val);
  case A_PAIR:
    op = eval(e, car(exp));
    if (op->typ == A_FN) {
      // call builtin
      return op->fn(e, cdr(exp)); 
    } else {
      return apply(op, cdr(exp));
    }
  default:
    return atom_make(A_ERROR, "what the hello");
  }
}

int atom_len(atom *a) {
  int len = 0;
  while (a->typ == A_PAIR) {
    len += 1;
    a = a->pair.cdr;
  }  
  return len;
}

// returns difference between len(names) and len(args)
int env_bind_args(env *e, atom *names, atom *args) {

  if (atom_len(names) != atom_len(args)) {
    printf("len(names): %d\n", atom_len(names));
    printf("len(args): %d\n", atom_len(args));
    return atom_len(names) - atom_len(args);
  }

  atom *np = names, *ap = args;
  while (np->typ != A_NIL && ap->typ != A_NIL) {
    env_bind(e, car(np)->val, car(ap));
    np = cdr(np);
    ap = cdr(ap);
  }
  return 0;
}

atom *make_fn(env *e, atom *params, atom *body) {
  atom *a = atom_make(A_PAIR, "");
  car(a) = atom_make(A_ENV, "");
  car(a)->env = e;
  cdr(a) = atom_make(A_PAIR, "");
  car(cdr(a)) = params;
  cdr(cdr(a)) = atom_make(A_PAIR, "");
  car(cdr(cdr(a))) = body;
  cdr(cdr(cdr(a))) = atom_make(A_NIL, "");
  return a;
}

atom *apply(atom *op, atom *args) {

  int res;
  if (op->typ != A_PAIR) {
    return atom_make(A_ERROR, "This isn't an operation\n"); 
  }
  env *e = env_make(car(op)->env, 8);
  atom *param_names = car(cdr(op));
  atom *body = car(cdr(cdr(op)));
  
  if ((res = env_bind_args(e, param_names, args)) != 0) {
    return atom_make(A_ERROR, "number of args does not match fn arity"); 
  }
  
  return eval(e, body); 
}
