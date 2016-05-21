#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lisp.h"

// these are defined for 32-bit
#define INITIAL_CAP (16)
#define MAX_INPUT (1024)
#define FNV_OFFSET (2166136261)
#define FNV_PRIME (16777619)

#define car(c) ((c)->pair->car)
#define cdr(c) ((c)->pair->cdr)

// env
unsigned int hash(const char *str) {
  unsigned int h = FNV_OFFSET;
  while (*str) {
    h ^= *(str++); 
    h *= FNV_PRIME;
  }
  return h;
}

Env *env_make(Env *p) {
  Env *e = (Env *)malloc(sizeof(Env));
  e->parent = p;
  e->size = 0;
  e->capacity = INITIAL_CAP;
  e->content = malloc(sizeof(Entry)*INITIAL_CAP);
  return e;
}

Entry *entry_make(const char *k, Cell *v, Entry *n) {
  Entry *ent = (Entry *)malloc(sizeof(Entry));
  ent->k = k;
  ent->v = v;
  ent->next = n;
  return ent;
}

void env_put(Env *env, const char *k, Cell *v) {
  if (!env) return;
  // expand table if too small
  unsigned int i = hash(k)%env->capacity;
  Entry *ent = entry_make(k, v, env->content[i]);
  env->content[i] = ent;
  env->size += 1;
}

Cell *env_get(Env *env, const char *k) {
  if (!env) return NULL;
  unsigned int i = hash(k)%env->capacity;
  Entry *e = env->content[i];
  while (e) {
    if (strcmp(e->k, k) == 0) {
      return e->v;
    }
    e = e->next;
  }
  return env_get(env->parent, k); 
}

// core
Cell *cell_new() { return (Cell *)malloc(sizeof(Cell)); }
Cell *cell_nil() { Cell *c = cell_new(); c->typ = T_NIL; }
Cell *cell_pair(Cell *car, Cell *cdr) {

  Cell *c = cell_new();
  c->typ = T_PAIR;
  c->pair = (struct pair *)malloc(sizeof(struct pair));
  c->pair->car = car; 
  c->pair->cdr = cdr;
  return c;
}

Cell *cell_error(char *msg) {
  Cell *c = cell_new();
  c->typ = T_ERROR;
  c->val = strdup(msg);
  return c;
}

Cell *cell_make(enum type t, char *input) {
  Cell *c = cell_new();
  c->typ = t;
  c->val = strdup(input);
  return c;
}

void cell_print(Cell *c) {
	switch (c->typ) {
	case T_SYMBOL:
		printf("'%s ", c->val);
		break;
	case T_NIL:
		printf("'()");
		break;
	case T_ERROR:
		printf("ERR(%s) ", c->val);
		break;
	case T_PAIR:
		printf("(");
    cell_print(car(c)); 
    cell_print(cdr(c)); 
    printf(")");
		break;
  case T_NUMBER:
		printf("%d ", c->number);
		break;
  case T_STRING:
		printf("\"%s\" ", c->val);
		break;
	default:
		printf("unrecognized: %d\n", c->typ);
	}
}

Token *token_make_n(enum tok typ, char *val, int n) {
  Token *t = (Token *)malloc(sizeof(Token));
  t->typ = typ;
  t->val = strndup(val, n);
  t->next = t->prev = NULL;
  return t;
}

Token *token_make(enum tok typ, char *val) {
  return token_make_n(typ, val, strlen(val));
}

Stack *stack_make() { return (Stack *)malloc(sizeof(Stack)); }

void stack_push(Stack *s, Token *t) {
  t->next = s->start;
  if (s->start) s->start->prev = t;
  if (!s->end) s->end = t;
  s->start = t;
}

Token *stack_pop(Stack *s) {
  Token *t = s->end;
  dbg("stack_pop");
  if (t) {
    dbg("stack_pop2");
    s->end = t->prev;
    dbg("stack_pop3");
    if (s->end == NULL) printf("null\n");
    else printf("not null\n");
    
    if (s->end != NULL) {
      dbg("pretok");
      printf("tok: %s\n", s->end->val);
      s->end->next = NULL;
    }
    dbg("stack_pop4");
  }
  return t;
}

void token_print(Token *t) {
  switch(t->typ) {
  case TK_OPEN:
    printf("<<OPEN>> "); 
    break;
  case TK_CLOSE:
    printf("<<CLOSE>> "); 
    break;
  case TK_NIL:
    printf("<<NIL>> "); 
    break;
  case TK_SYMBOL:
    printf("<<SYMBOL:%s>> ", t->val); 
    break;
  default:
    printf("<<UNRECOGNIZED: %s>>", t->val);
  }
}

void stack_print(Stack *s) {
  for (Token *t = s->end; t; t = t->prev) {
    token_print(t);  
  }
}

int issymbol(char c) {
  return (isalnum(c) || (ispunct(c) && c != '(' && c != ')'));
}

Stack *tokenize(char *input) {
  Stack *s = stack_make();
  char *ip = input;
  int len = 0;
  while (*ip) {
    while (isspace(*ip)) ip++;
    switch (*ip) {
    case '(':
      stack_push(s, token_make(TK_OPEN, "("));
      ip++;
      break;
    case ')':
      stack_push(s, token_make(TK_CLOSE, ")"));
      ip++;
      break;
    case '\'':
    case '\"':
    default:
      while (issymbol(*(ip+len))) len++;
      stack_push(s, token_make_n(TK_SYMBOL, ip, len));
      ip += len;
      len = 0;
    }  
  }
  return s;
}

Cell *parse_list(Stack *s, Token *t) {
  t = stack_pop(s);
  Cell *car;
  switch (t->typ) {
  case TK_OPEN:
    car = parse_list(s, t);
    break;
  case TK_SYMBOL:
    car = parse_cell(t);
    break;
  case TK_CLOSE:
    return cell_nil();
  }
  return cell_pair(car, parse_list(s, t));
}

Cell *parse_number(Token *t) {
  char *ip = t->val;
  while (isdigit(*ip)) ip++;
  if (*ip != '\0') return cell_make(T_SYMBOL, t->val);
  Cell *c = cell_make(T_NUMBER, t->val);
  c->number = atoi(t->val);
  return c;
}
Cell *parse_string(Token *t) {
  char end = *t->val;
  // TODO: check valid string format
  t->val++;
  *(t->val+strlen(t->val)-1) = '\0';
  return cell_make(T_STRING, t->val);
}
Cell *parse_symbol(Token *t) {
  return cell_make(T_SYMBOL, t->val);
}

Cell *parse_cell(Token *t) {
  if (isdigit(*t->val)) return parse_number(t);
  else if (*t->val == '\'' || *t->val == '\"') return parse_string(t);
  else return parse_symbol(t);
}

Cell *parse(char *input) {
  Stack *s = tokenize(input);
  Token *t; 
  dbg("parse");
  while (t = stack_pop(s)) {
    dbg("parsin");
    switch (t->typ) {
    case TK_OPEN:
      return parse_list(s, t);
      break;
    default:
      return parse_cell(t); 
    } 
    token_print(t); 
  }
  dbg("parsin");
  return NULL; 
}

Cell *cell_read(FILE *f) {
  dbg("cell_read");
  char input[MAX_INPUT];
  fgets(input, MAX_INPUT, f);
  return parse(input);
}

Cell *apply(Cell *fn, Cell *args) { return fn; }

int is(char *a, char *b) {
  return strcmp(a, b) == 0;
}

void dbg(char *msg) {
  static int i = 0;
  printf("%d: %s - called \n", i++, msg);
}

Cell *eval(Cell *expr, Env *env) { 
  dbg("eval");
  Cell *res;
  switch(expr->typ) {
  case T_NUMBER:
  case T_STRING:
    res =  expr;
    break;
  case T_SYMBOL:
    if (is(expr->val, "define")) {
      printf("define!");
    }
    res = env_get(env, expr->val);
    break;
  } 
  if (!res) res = cell_error("not working");
  return res; 
}
