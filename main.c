#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "atom.h"
#include "env.h"

int main() {
  
  Env *e = env_make(NULL);
  srand(time(NULL));
  
  env_put(e, "lol", atom_make(rand()));
  env_put(e, "cool", atom_make(rand()));
  env_put(e, "poo", atom_make(rand()));
  env_put(e, "barf", atom_make(rand()));
  //env_put(e, "gog", atom_make(rand()));
  //env_put(e, "plock", atom_make(rand()));
  //env_put(e, "slarty", atom_make(rand()));
  
  printf("env->size = %d\n", e->size); 
 
  
  printf("hehe\n");
}
