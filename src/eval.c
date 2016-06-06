#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lisp.h"

/* TODO: env must support: 
  - deriving new env 
  - binding names to atoms
  - looking up names in env
*/

env_entry *env_entry_make(char *k, atom *v, env_entry *next) {
  env_entry *ent = (env_entry *)malloc(sizeof(env_entry));
  ent->k = strdup(k);
  ent->v = v;
  ent->next = next;
  return ent;
}

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

atom *eval(env *e, atom *exp) {

  switch (exp->typ) {
  case A_NUMBER:
  case A_STRING:
    return exp;
  case A_SYMBOL:
    return env_lookup(e, exp->val);
  default:
    return atom_make(A_ERROR, "what the hello");
  }


  /*  TODO:
    if number or string
      return it
    if symbol
      look it up in env
    if list
      apply first arg to rest
  */

}

#define car(a) (a->pair.car)
#define cdr(a) (a->pair.cdr)

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
  return a;
}

atom *apply(atom *op, atom *args) {

  int res;
  env *e = env_make(car(op)->env, 8);
  atom *param_names = car(cdr(op));
  atom *body = car(cdr(cdr(op)));
  
  if ((res = env_bind_args(e, param_names, args)) != 0) {
    return atom_make(A_ERROR, "number of args does not match fn arity"); 
  }
  
  return eval(e, body); 
  
  /* TODO:
    create new env with names bound to args
    eval body in new env
  */
}
