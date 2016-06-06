#include <stdio.h>

#include "lisp.h"

struct core_fn {
  char *name;
  atom *(*fn)(env *e, atom *);
};

atom *fn_add(env *e, atom *args) {
  int sum = 0;
  atom *num;
  while (args->typ == A_PAIR) {
    num = eval(e, car(args));
    if (num->typ != A_NUMBER)
      return atom_make(A_ERROR, "arg not number?");
    else
      sum += num->number;
    args = cdr(args);
  }
  atom *a = atom_make(A_NUMBER, "");
  a->number = sum;
  return a;
}

atom *fn_sub(env *e, atom *args) {
  int minuend;
  atom *num = eval(e, car(args));
  
  if (num->typ != A_NUMBER)
    return atom_make(A_ERROR, "arg not number?");
  else
    minuend = num->number;
    
  args = cdr(args);
  while (args->typ == A_PAIR) {
    num = eval(e, car(args));
    if (num->typ != A_NUMBER)
      return atom_make(A_ERROR, "arg not number?");
    else
      minuend -= num->number;
    args = cdr(args);
  }
  
  atom *a = atom_make(A_NUMBER, "");
  a->number = minuend;
  return a;
}

static struct core_fn fns[] = {
  { "+", fn_add },
  { "-", fn_sub }
};

void load_core(env *e) {

  atom *a;
  struct core_fn *fp = fns;
  for (int i = 0; i < sizeof(fns)/sizeof(struct core_fn); fp++, i++) {
    a = atom_make(A_FN, fp->name);
    a->fn = fp->fn; 
    env_bind(e, fp->name, a);
  }

  //// TODO: add other built in fns
  //atom *a = atom_make(A_FN, "");
  //a->fn = fn_add;
  //env_bind(e, "+", a);
}

void atom_print(atom *a) {

  switch(a->typ) {
  case A_SYMBOL:
    printf("'%s", a->val);
    break;
  case A_NUMBER:
    printf("%d", a->number);
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
  case A_ERROR:
    printf("err: %s", a->val);
    break;
  default:
    printf("wat");
  }
}
