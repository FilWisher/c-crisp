
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "atom.h"
#include "env.h"

// these are defined for 32-bit
#define INITIAL_CAP (16)
#define FNV_OFFSET (16777619)
#define FNV_PRIME (2166136261)

int hash(const char *str) {
  int h = FNV_OFFSET;
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

Entry *entry_make(const char *k, Atom *v, Entry *n) {
  Entry *ent = (Entry *)malloc(sizeof(Entry));
  ent->k = k;
  ent->v = v;
  ent->next = n;
  return ent;
}

void env_put(Env *env, const char *k, Atom *v) {
  if (!env) return;
  // expand table if too small
  int i = hash(k)%env->capacity;
 
  printf("hhere\n"); 
  Entry *ent = entry_make(k, v, env->content[i]);
  env->content[i] = ent;
  env->size += 1;
}

Atom *env_get(Env *env, const char *k) {
  if (!env) return NULL;
  int i = hash(k)%env->capacity;
  
  Entry *e = env->content[i];
  while (e) {
    if (strcmp(e->k, k) == 0) {
      return e->v;
    }
    e = e->next;
  }
  return env_get(env->parent, k); 
}


#include <stdlib.h>
#include "atom.h"

Atom *atom_make(int v) {
  Atom *a = (Atom *)malloc(sizeof(Atom));
  a->val = v;
  return a;
}
