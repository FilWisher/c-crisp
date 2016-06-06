#include <stdio.h>
#include <string.h>

#include "lisp.h"

#define MAX_INPUT (1024)

int main() {

  char input[MAX_INPUT];
  atom *ast;
  
  env *global = env_make(NULL, 8);
  load_core(global);

  for (;;) {
  
    fputs("\n> ", stdout);
    fgets(input, MAX_INPUT, stdin);
   
    if (strlen(input) > 0) input[strlen(input)-1] = '\0';

    ast = eval(global, parse(input));
   
    atom_print(ast);
  
  }



  printf("repl\n");
}
