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

atom *fn_mul(env *e, atom *args) {
  int mul = 1;
  atom *num;
  while (args->typ == A_PAIR) {
    num = eval(e, car(args));
    if (num->typ != A_NUMBER)
      return atom_make(A_ERROR, "arg not number?");
    else
      mul *= num->number;
    args = cdr(args);
  }
  atom *a = atom_make(A_NUMBER, "");
  a->number = mul;
  return a;
}

// TODO: handle with fractions
atom *fn_div(env *e, atom *args) {
  int dividend;
  atom *num = eval(e, car(args));
  
  if (num->typ != A_NUMBER)
    return atom_make(A_ERROR, "arg not number?");
  else
    dividend = num->number;
   
  if (dividend != 0) {
    args = cdr(args);
    while (args->typ == A_PAIR) {
      num = eval(e, car(args));
      if (num->typ != A_NUMBER)
        return atom_make(A_ERROR, "arg not number?");
      else if (num->number == 0)
        return atom_make(A_ERROR, "cannot divide by 0");
      else
        dividend /= num->number;
      args = cdr(args);
    }
  }  
  
  atom *a = atom_make(A_NUMBER, "");
  a->number = dividend;
  return a;
}

atom *fn_define(env *e, atom *args) {
  if (atom_len(args) != 2)
    return atom_make(A_ERROR, "wrong number of args passed to define");
  
  if (car(args)->typ != A_SYMBOL)
    return atom_make(A_ERROR, "first arg must be symbol");
 
  env_bind(e, car(args)->val, eval(e, car(cdr(args))));
  return car(args); 
}
// TODO: defun not acting same as (define x (lambda ...))
atom *fn_defun(env *e, atom *args) {
  atom *name = car(args);
  atom *fn = fn_lambda(e, cdr(args));

  if (atom_len(args) != 3)
    return atom_make(A_ERROR, "wrong number of args passed to defun");
  
  if (car(args)->typ != A_SYMBOL)
    return atom_make(A_ERROR, "first arg must be symbol");

  env_bind(e, name->val, fn);
  return name; 
}

atom *fn_lambda(env *e, atom *args) {
 
  if (atom_len(args) != 2) 
    return atom_make(A_ERROR, "wrong number of args passed to lambda");
  
  if (car(args)->typ != A_PAIR && car(args)->typ != A_NIL)
    return atom_make(A_ERROR, "first arg must be arg list");

  atom *fn = make_fn(e, car(args), car(cdr(args)));
  return fn;
}

static struct core_fn fns[] = {
  { "+", fn_add },
  { "-", fn_sub },
  { "*", fn_mul },
  { "/", fn_div },
  { "define", fn_define },
  { "lambda", fn_lambda },
  { "defun", fn_defun }
};

void load_core(env *e) {
  atom *a;
  struct core_fn *fp = fns;
  for (int i = 0; i < sizeof(fns)/sizeof(struct core_fn); fp++, i++) {
    a = atom_make(A_FN, fp->name);
    a->fn = fp->fn; 
    env_bind(e, fp->name, a);
  }
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
  case A_FN:
    printf("<fn :%s>", a->val);
    break;
  default:
    printf("wat");
  }
}

